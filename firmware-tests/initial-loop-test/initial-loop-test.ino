#include <JC_Button.h>
#include <Encoder.h>
#include <Audio.h>
#include <Wire.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ILI9341_t3.h"
#include "font_Arial.h"

#define NUMROWS       4
#define NUMCOLS       4
#define NUMINST       4
#define NUMTICKS  (NUMROWS*NUMCOLS)
#define BUTTON_DEBOUNCE 250
#define DEFAULT_NOTE  1
#define VBAT_PIN      A22
#define VBAT_DIVIDER  2.0f

#define BATT_STAT1_PIN  1
#define BATT_STAT2_PIN  4

// Time interval for serial status in ms
#define STATUS_INTERVAL 1000

#define NUMPIXELS 1 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define DATAPIN    0
#define CLOCKPIN   1
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);


uint8_t rows[NUMROWS] = { 28, 29, 30, 31 };
uint8_t cols[NUMCOLS] = { 35, 36, 37, 38 };

bool leds[NUMROWS][NUMCOLS];
long buttons[NUMROWS][NUMCOLS];
uint8_t notes[NUMINST][NUMCOLS][NUMROWS];

uint16_t tones[] = {
  440 / 2,
  466 / 2,
  494 / 2,
  523 / 2,
  554 / 2,
  587 / 2,
  622 / 2,
  659 / 2,
  698 / 2,
  740 / 2,
  784 / 2,
  831 / 2,
  880 / 2,
  440,
  466,
  494,
  523,
  554,
  587,
  622,
  659,
  698,
  740,
  784,
  831,
  880
};

// GUItool: begin automatically generated code
AudioSynthNoiseWhite     noise1;         //xy=376,500
AudioSynthSimpleDrum     drum1;          //xy=404,587
AudioSynthKarplusStrong  string1;        //xy=410,745
AudioSynthWaveform       triangle1;      //xy=415,655
AudioFilterStateVariable filter1;        //xy=536,537
AudioEffectEnvelope      env3;      //xy=613,720
AudioEffectEnvelope      env2;      //xy=634,651
AudioEffectEnvelope      env1;      //xy=696,551
AudioMixer4              mixer1;         //xy=847,588
AudioOutputI2S           i2s1;           //xy=1001,613
AudioConnection          patchCord1(noise1, 0, filter1, 0);
AudioConnection          patchCord2(drum1, 0, mixer1, 1);
AudioConnection          patchCord3(string1, env3);
AudioConnection          patchCord4(triangle1, env2);
AudioConnection          patchCord5(filter1, 2, env1, 0);
AudioConnection          patchCord6(env3, 0, mixer1, 3);
AudioConnection          patchCord7(env2, 0, mixer1, 2);
AudioConnection          patchCord8(env1, 0, mixer1, 0);
AudioConnection          patchCord9(mixer1, 0, i2s1, 1);
AudioConnection          patchCord10(mixer1, 0, i2s1, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=911,298
// GUItool: end automatically generated code

Encoder enc1(25, 26);
Encoder enc2(24, 10);

Button but1 = Button(3, 100, true, false);

long enc1_pos    = 0;
long enc2_pos    = 0;

float bpm        = 200.0;
int tick         = 0;
int current_inst = 0;
long next_tick   = 0;
long next_tft    = 0;
bool playing     = true;
float vbat = 0;
bool bat_stat1 = LOW;
bool bat_stat2 = LOW;


// For optimized ILI9341_t3 library
#define TFT_DC      20
#define TFT_CS      21
#define TFT_RST    255  // 255 = unused, connect to 3.3V
#define TFT_MOSI     7
#define TFT_SCLK    14
#define TFT_MISO    12
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);

#define TFT_INTERVAL 100

void setup() {
  blank();

  pinMode(BATT_STAT1_PIN, INPUT_PULLUP);
  pinMode(BATT_STAT2_PIN, INPUT_PULLUP);
  
  enc1.write(0);
  enc2.write(0);

  but1.begin();

  AudioMemory(15);

  AudioNoInterrupts();

  noise1.amplitude(1.0f);

  env1.hold(0);
  env1.attack(0);
  env1.hold(0);
  env1.decay(100);
  env1.sustain(0.0f);
  env1.release(1); 
  env1.releaseNoteOn(0);
  filter1.frequency(8000);

  env2.hold(0);
  env2.attack(10);
  env2.hold(0);
  env2.decay(1000);
  env2.sustain(0.0f);
  env2.release(1); 
  env2.releaseNoteOn(0);
  
  env3.hold(0);
  env3.attack(20);
  env3.hold(0);
  env3.decay(1000);
  env3.sustain(0.0f);
  env3.release(1); 
  env3.releaseNoteOn(0);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  AudioInterrupts();

  tft.begin();
  tft.setRotation( 3 );
  tft.fillScreen(ILI9341_BLACK);

  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setFont(Arial_12);

  draw_note_menu();
  draw_bpm();
  draw_channel();

  next_tick = millis() + (long)(60000.0f / bpm);  
  next_tft = 0;

  notes[0][0][0] = 1;
  notes[0][0][1] = 1;
  notes[0][0][2] = 1;
  notes[0][0][3] = 1;
  notes[0][2][0] = 1;

  vbat = (float)analogRead(VBAT_PIN) / 1024.0f * 3.3f * VBAT_DIVIDER;

  strip.begin(); // Initialize pins for output
  strip.setPixelColor(0, 0x0000FF); // 'On' pixel at head
  strip.show();  // Turn all LEDs off ASAP
  
  Serial.begin(115200);
}

void loop() {
  int x, y, inst;
  long ms = 0;
  static long cycles = 0;
  static long last_status = 0;

  ms = millis();
  if (playing && ms >= next_tick) {
    next_tick = ms + (long)(60000.0f / bpm);

    leds[tick / NUMCOLS][tick % NUMCOLS] = false;

    tick++;
    if (tick >= NUMTICKS) {
      tick = 0;
    }

    for (inst = 0; inst < NUMINST; inst++) {
      if (notes[inst][tick / NUMCOLS][tick % NUMCOLS]) {
        if (inst == 0) {
          env1.noteOn();
        }
        if (inst == 1) {
          drum1.frequency(50);
          drum1.noteOn();
        }
        if (inst == 2) {
          triangle1.begin(1.0f, tones[notes[inst][tick / NUMCOLS][tick % NUMCOLS]], WAVEFORM_TRIANGLE);
          env2.decay((60000.0f / bpm));
          env2.noteOn();
        }
        if (inst == 3) {
          string1.noteOn(261.6f, 1.0f);
          env3.noteOn();
        }
        if (inst == current_inst) {
          leds[tick / NUMCOLS][tick % NUMCOLS] = true;
        }
      }
    }
  }

  but1.read();
  if (but1.wasPressed()) {
    playing = !playing;
    if (playing) {
      tft.fillRect(100, 0, 100, 15, ILI9341_BLACK);
      tft.setCursor(100,0);
      tft.print("Playing");
      draw_freq();
    } else {
      tft.fillRect(100, 0, 100, 15, ILI9341_BLACK);
      tft.setCursor(100,0);
      tft.print("Paused");
      draw_freq();
    }
  }

  int knob = analogRead(A1);
  float vol = (float)knob / 1280.0f;
  sgtl5000_1.volume(vol);
  
  long enc1_new = enc1.read() / 4;
  if (enc1_new < enc1_pos) {
    if (playing) {
      bpm -= 5;
      if (bpm < 60) {
        bpm = 60;
      }
    } else {
      tick--;      
      if (tick < 0) {
        tick = NUMTICKS - 1;
      }
    }
    draw_bpm();
  } else if (enc1_new > enc1_pos) {
    if (playing) {
      bpm += 5;
      if (bpm > 550) {
        bpm = 550;
      }
    } else {
      tick++;
      if (tick >= NUMTICKS) {
        tick = 0;
      }
    }
    draw_bpm();
  }
  enc1_pos = enc1_new;

  long enc2_new = enc2.read() / 4;
  if (enc2_new < enc2_pos) {
    if (playing) {
      current_inst--;
      if (current_inst < 0) {
        current_inst = NUMINST - 1;
      }
      draw_channel();
    } else {
      notes[current_inst][tick / NUMCOLS][tick % NUMCOLS]--;      
      if (notes[current_inst][tick / NUMCOLS][tick % NUMCOLS] <= 0) {
        notes[current_inst][tick / NUMCOLS][tick % NUMCOLS] = sizeof(tones) / sizeof(uint16_t) - 1;
      }
      draw_freq();
    }
  } else if (enc2_new > enc2_pos) {
    if (playing) {
      current_inst++;
      if (current_inst >= NUMINST) {
        current_inst = 0;
      }
      draw_channel();
    } else {
      notes[current_inst][tick / NUMCOLS][tick % NUMCOLS]++;      
      if (notes[current_inst][tick / NUMCOLS][tick % NUMCOLS] >= sizeof(tones) / sizeof(uint16_t)) {
        notes[current_inst][tick / NUMCOLS][tick % NUMCOLS] = 1;
      }
      draw_freq();
    }
  }
  enc2_pos = enc2_new;

  if (ms >= next_tft) {
    next_tft = ms + TFT_INTERVAL;

    draw_note_grid(notes[current_inst], 30, 30);
  }

  for(y = 0; y < NUMROWS; y++) {
    pinMode(rows[y], INPUT);
  }
  for(x = 0; x < NUMCOLS; x++) {
    pinMode(cols[x], INPUT);
  }

  delay(1);

  for(y = 0; y < NUMROWS; y++) {
    for(x = 0; x < NUMCOLS; x++) {
      pinMode(rows[y], OUTPUT);
      digitalWrite(rows[y], HIGH);
      pinMode(cols[x], OUTPUT);

      if (leds[y][x] == true) {
        digitalWrite(cols[x], LOW);
      } else {
        digitalWrite(cols[x], HIGH);
      }

      delayMicroseconds(16);
      delayMicroseconds(16);
      delayMicroseconds(16);

      pinMode(rows[y], INPUT);
      pinMode(cols[x], INPUT);
    }
  }
  scan_buttons();
 
  if (millis() >= last_status + STATUS_INTERVAL) {

    bat_stat1 = digitalRead(BATT_STAT1_PIN);
    bat_stat2 = digitalRead(BATT_STAT2_PIN);

    vbat = (float)analogRead(VBAT_PIN) / 1024.0f * 3.3f * VBAT_DIVIDER;
    if (bat_stat2 == LOW) {
      Serial.println("Charging");
    } else {
      Serial.print("Battery voltage: ");
      Serial.println(vbat);
    }
    draw_voltage();
    
    Serial.print((float)cycles / (millis() / 1000.0f));
    Serial.println(" cycles per sec");

    last_status = millis();
  }

  cycles++;
}

void blank(void)
{
  for(int x = 0; x < NUMCOLS; x++) {
    pinMode(cols[x], INPUT);
  }
  for(int y = 0; y < NUMROWS; y++) {
    pinMode(rows[y], INPUT);  
  }
}

void scan_buttons() 
{
  int x, y;
  
  for(x = 0; x < NUMCOLS; x++) {
    pinMode(cols[x], INPUT_PULLUP);
  }

  for(y = 0; y < NUMROWS; y++) {
    pinMode(rows[y], INPUT_PULLUP);

  }

  long t = millis();

  delayMicroseconds(16);
  delayMicroseconds(16);
  delayMicroseconds(16);

  for(y = 0; y < NUMROWS; y++) {
    pinMode(rows[y], OUTPUT);
    digitalWrite(rows[y], LOW);

    for(x = 0; x < NUMCOLS; x++) {

      pinMode(cols[x], INPUT_PULLUP);
      if (buttons[y][x] < t) {
        buttons[y][x] = 0;
      }
  
      delayMicroseconds(16);
      delayMicroseconds(16);
      delayMicroseconds(16);

      if (digitalRead(cols[x]) == LOW && buttons[y][x] == 0) {
        buttons[y][x] = t + BUTTON_DEBOUNCE;
        if (notes[current_inst][y][x] > 0) {
          notes[current_inst][y][x] = 0;
        } else {
          notes[current_inst][y][x] = DEFAULT_NOTE;
        }
      }
      pinMode(cols[x], INPUT_PULLUP);

    }
    pinMode(rows[y], INPUT_PULLUP);
    //delay(1);
  } 
}

