#include <JC_Button.h>
#include <Encoder.h>
#include <Audio.h>
#include <Wire.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <XPT2046_Touchscreen.h>
#include "tft.h"
#include "ui_item.h"
#include "instrument.h"

#define NUMROWS       4
#define NUMCOLS       4
#define NUMINST       4
#define NUMTICKS  (NUMROWS*NUMCOLS)
#define BUTTON_DEBOUNCE 250
#define VBAT_PIN      A22
#define VBAT_DIVIDER  2.0f

#define BATT_STAT1_PIN  15
#define BATT_STAT2_PIN  16

// Time interval for serial status in ms
#define STATUS_INTERVAL 1000

#define NUMPIXELS 1 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define DATAPIN    0
#define CLOCKPIN   1
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);

XPT2046_Touchscreen ts(TOUCH_CS);

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

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform2;      //xy=74,765
AudioSynthWaveform       waveform1;      //xy=77,679
AudioSynthWaveform       waveform3;      //xy=84,831
AudioSynthWaveform       waveform4;      //xy=107,896
AudioSynthNoiseWhite     noise1;         //xy=207,341
AudioEffectEnvelope      env_w_1;      //xy=233,685
AudioEffectEnvelope      env_w_2;      //xy=235,771
AudioEffectEnvelope      env_w_3;      //xy=243,833
AudioEffectEnvelope      env_w_4;      //xy=268,885
AudioSynthKarplusStrong  string1;        //xy=366,988
AudioEffectReverb        reverb_w_1;        //xy=386,681
AudioFilterStateVariable filter_n_1;        //xy=394,352
AudioEffectReverb        reverb_w_2;        //xy=395,755
AudioEffectReverb        reverb_w_3;        //xy=406,808
AudioSynthSimpleDrum     drum1;          //xy=409,574
AudioEffectReverb        reverb_w_4;        //xy=443,860
AudioEffectEnvelope      env_st_1;      //xy=527,962
AudioEffectEnvelope      env_n_1;      //xy=556,391
AudioMixer4              mixer_w;         //xy=611,699
AudioMixer4              mixer1;         //xy=847,588
AudioOutputI2S           i2s1;           //xy=1001,613
AudioConnection          patchCord1(waveform2, env_w_2);
AudioConnection          patchCord2(waveform1, env_w_1);
AudioConnection          patchCord3(waveform3, env_w_3);
AudioConnection          patchCord4(waveform4, env_w_4);
AudioConnection          patchCord5(noise1, 0, filter_n_1, 0);
AudioConnection          patchCord6(env_w_1, reverb_w_1);
AudioConnection          patchCord7(env_w_2, reverb_w_2);
AudioConnection          patchCord8(env_w_3, reverb_w_3);
AudioConnection          patchCord9(env_w_4, reverb_w_4);
AudioConnection          patchCord10(string1, env_st_1);
AudioConnection          patchCord11(reverb_w_1, 0, mixer_w, 0);
AudioConnection          patchCord12(filter_n_1, 2, env_n_1, 0);
AudioConnection          patchCord13(reverb_w_2, 0, mixer_w, 1);
AudioConnection          patchCord14(reverb_w_3, 0, mixer_w, 2);
AudioConnection          patchCord15(drum1, 0, mixer1, 1);
AudioConnection          patchCord16(reverb_w_4, 0, mixer_w, 3);
AudioConnection          patchCord17(env_st_1, 0, mixer1, 3);
AudioConnection          patchCord18(env_n_1, 0, mixer1, 0);
AudioConnection          patchCord19(mixer_w, 0, mixer1, 2);
AudioConnection          patchCord20(mixer1, 0, i2s1, 1);
AudioConnection          patchCord21(mixer1, 0, i2s1, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=911,298
// GUItool: end automatically generated code

WaveformInstrument wave1  = WaveformInstrument("Waveform1", &waveform1, &env_w_1, &reverb_w_1, &mixer_w, 0);
WaveformInstrument wave2  = WaveformInstrument("Waveform2", &waveform2, &env_w_2, &reverb_w_2, &mixer_w, 1);
WaveformInstrument wave3  = WaveformInstrument("Waveform3", &waveform3, &env_w_3, &reverb_w_3, &mixer_w, 2);
WaveformInstrument wave4  = WaveformInstrument("Waveform4", &waveform4, &env_w_4, &reverb_w_4, &mixer_w, 3);
NoiseInstrument    nois1 = NoiseInstrument("Noise1", &noise1, &filter_n_1, &env_n_1, &mixer1, 1);

Encoder enc1(25, 26);
Encoder enc2(24, 10);

Button but1 = Button(3, 100, true, false);

long enc1_pos    = 0;
long enc2_pos    = 0;

float bpm        = 80.0;
int tick         = 0;
int current_inst = 0;
long next_tick   = 0;
long next_tft    = 0;
bool playing     = true;
float vbat = 0;
bool bat_stat1 = LOW;
bool bat_stat2 = LOW;
boolean wastouched = false;

HomeScreen h = HomeScreen();
UiScreen *p_screen = &h;

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

  // Noise
  env_n_1.hold(0);
  env_n_1.attack(0);
  env_n_1.hold(0);
  env_n_1.decay(100);
  env_n_1.sustain(0.0f);
  env_n_1.release(1); 
  env_n_1.releaseNoteOn(0);
  filter_n_1.frequency(8000);

  // Triangle
  env_w_1.hold(0);
  env_w_1.attack(10);
  env_w_1.hold(0);
  env_w_1.decay(1000);
  env_w_1.sustain(0.0f);
  env_w_1.release(1); 
  env_w_1.releaseNoteOn(0);

  // String
  env_st_1.hold(0);
  env_st_1.attack(20);
  env_st_1.hold(0);
  env_st_1.decay(1000);
  env_st_1.sustain(0.0f);
  env_st_1.release(1); 
  env_st_1.releaseNoteOn(0);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.25);

  AudioInterrupts();

  tft.begin();
  tft.setRotation( 3 );
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setFont(Arial_9);

  ts.begin();
  ts.setRotation(3);

  next_tick = millis() + (long)(60000.0f / bpm / 4.0f);  
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
    next_tick = ms + (long)(60000.0f / bpm / 4.0f);

    leds[tick / NUMCOLS][tick % NUMCOLS] = false;

    tick++;
    if (tick >= NUMTICKS) {
      tick = 0;
    }

    for (inst = 0; inst < NUMINST; inst++) {
      if (notes[inst][tick / NUMCOLS][tick % NUMCOLS]) {
        if (inst == 0) {
          env_n_1.noteOn();
        }
        if (inst == 1) {
          drum1.frequency(50);
          drum1.noteOn();
        }
        if (inst == 2) {
          waveform1.begin(1.0f, tones[notes[inst][tick / NUMCOLS][tick % NUMCOLS]], WAVEFORM_TRIANGLE);
          env_w_1.decay((60000.0f / bpm / 4.0f));
          env_w_1.noteOn();
        }
        if (inst == 3) {
          string1.noteOn(261.6f, 1.0f);
          env_st_1.noteOn();
        }
        if (inst == current_inst) {
          leds[tick / NUMCOLS][tick % NUMCOLS] = true;
        }
      }
    }
  }

  boolean istouched = ts.touched();
  if (istouched) {
    TS_Point p = ts.getPoint();
    p.x = map(p.x, 290, 3700, 320, 0);
    p.y = map(p.y, 480, 3700, 240, 0);
    if (!wastouched) {
      Serial.print("New touch: ");
      Serial.print(p.x);
      Serial.print(", ");
      Serial.println(p.y);
      p_screen->OnTouch(p.x, p.y);

    }
  }
  wastouched = istouched;

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
      if (bpm < 50) {
        bpm = 50;
      }
    } else {
      tick--;      
      if (tick < 0) {
        tick = NUMTICKS - 1;
      }
    }
  } else if (enc1_new > enc1_pos) {
    if (playing) {
      bpm += 5;
      if (bpm > 250) {
        bpm = 250;
      }
    } else {
      tick++;
      if (tick >= NUMTICKS) {
        tick = 0;
      }
    }
  }
  enc1_pos = enc1_new;

  long enc2_new = enc2.read() / 4;
  if (enc2_new < enc2_pos) {
    if (playing) {
      current_inst--;
      if (current_inst < 0) {
        current_inst = NUMINST - 1;
      }
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

    p_screen->Draw();
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

