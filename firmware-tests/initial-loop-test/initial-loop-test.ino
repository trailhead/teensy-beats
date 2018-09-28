#include <JC_Button.h>
#include <Encoder.h>
#include <Audio.h>
#include <Wire.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <XPT2046_Touchscreen.h>
#include "ui_item.h"
#include "instrument.h"
#include "beats.h"

Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);

XPT2046_Touchscreen ts(TOUCH_CS);

uint8_t rows[NUMROWS] = { 28, 29, 30, 31 };
uint8_t cols[NUMCOLS] = { 35, 36, 37, 38 };

bool leds[NUMROWS][NUMCOLS];
long buttons[NUMROWS][NUMCOLS];
uint8_t notes[NUMINST][NUMCOLS][NUMROWS];

// Frequencies of a scale in octave 1
float tones[] = {
  32.70320,   // C1
  34.64783,
  36.70810,
  38.89087,
  41.20344,
  43.65353,
  46.24930,
  51.91309,
  55.00000,
  58.27047,
  61.73541
};

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform2;      //xy=354.8333396911621,846.1666584014893
AudioSynthWaveform       waveform3;      //xy=354.8333396911621,904.6111555099487
AudioSynthWaveform       waveform1;      //xy=359.2777976989746,782.7222785949707
AudioSynthWaveform       waveform4;      //xy=359.2777900695801,961.3888683319092
AudioPlaySdWav           sdWav4;     //xy=524.5833511352539,617.0833683013916
AudioPlaySdWav           sdWav1;     //xy=525.2500152587891,471.75001525878906
AudioPlaySdWav           sdWav2;     //xy=525.2500152587891,520.7500152587891
AudioPlaySdWav           sdWav3;     //xy=525.2500152587891,566.7500152587891
AudioEffectEnvelope      env_w1;      //xy=565.8888854980469,774.8611755371094
AudioEffectEnvelope      env_w2;      //xy=566.0000114440918,840.9721965789795
AudioEffectEnvelope      env_w4;      //xy=567.7777786254883,974.8610982894897
AudioEffectEnvelope      env_w3;      //xy=568.3333282470703,907.5277557373047
AudioMixer4              mix_sdwav_r;         //xy=748.5833511352539,569.3055782318115
AudioMixer4              mix_sdwav_l;         //xy=749.8055381774902,495.0833568572998
AudioMixer4              mixer_w_l;         //xy=806.4443130493164,832.2220659255981
AudioMixer4              mixer_w_r;         //xy=813.3333104451498,942.2222157584296
AudioMixer4              mix_out_l;         //xy=1011.4443969726562,595.7777404785156
AudioMixer4              mix_out_r;         //xy=1012.888843536377,678.3333406448364
AudioOutputUSB           usb1;           //xy=1189.8888854980469,573.1110877990723
AudioOutputI2S           i2s1;           //xy=1190.0000076293945,691.2222490310669
AudioConnection          patchCord1(waveform2, env_w2);
AudioConnection          patchCord2(waveform3, env_w3);
AudioConnection          patchCord3(waveform1, env_w1);
AudioConnection          patchCord4(waveform4, env_w4);
AudioConnection          patchCord5(sdWav4, 0, mix_sdwav_l, 3);
AudioConnection          patchCord6(sdWav4, 1, mix_sdwav_r, 3);
AudioConnection          patchCord7(sdWav1, 0, mix_sdwav_l, 0);
AudioConnection          patchCord8(sdWav1, 1, mix_sdwav_r, 0);
AudioConnection          patchCord9(sdWav2, 0, mix_sdwav_l, 1);
AudioConnection          patchCord10(sdWav2, 1, mix_sdwav_r, 1);
AudioConnection          patchCord11(sdWav3, 0, mix_sdwav_l, 2);
AudioConnection          patchCord12(sdWav3, 1, mix_sdwav_r, 2);
AudioConnection          patchCord13(env_w1, 0, mixer_w_l, 0);
AudioConnection          patchCord14(env_w1, 0, mixer_w_r, 0);
AudioConnection          patchCord15(env_w2, 0, mixer_w_l, 1);
AudioConnection          patchCord16(env_w2, 0, mixer_w_r, 1);
AudioConnection          patchCord17(env_w4, 0, mixer_w_l, 3);
AudioConnection          patchCord18(env_w4, 0, mixer_w_r, 3);
AudioConnection          patchCord19(env_w3, 0, mixer_w_l, 2);
AudioConnection          patchCord20(env_w3, 0, mixer_w_r, 2);
AudioConnection          patchCord21(mix_sdwav_r, 0, mix_out_r, 0);
AudioConnection          patchCord22(mix_sdwav_l, 0, mix_out_l, 0);
AudioConnection          patchCord23(mixer_w_l, 0, mix_out_l, 1);
AudioConnection          patchCord24(mixer_w_r, 0, mix_out_r, 1);
AudioConnection          patchCord25(mix_out_l, 0, usb1, 0);
AudioConnection          patchCord26(mix_out_l, 0, i2s1, 0);
AudioConnection          patchCord27(mix_out_r, 0, usb1, 1);
AudioConnection          patchCord28(mix_out_r, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=911,298
// GUItool: end automatically generated code

WaveformInstrument wave1  = WaveformInstrument("Waveform1", &waveform1, &env_w1, &mixer_w_l, &mixer_w_r, 0, 0);
WaveformInstrument wave2  = WaveformInstrument("Waveform2", &waveform2, &env_w2, &mixer_w_l, &mixer_w_r, 1, 1);

Encoder enc1(26, 25);
Encoder enc2(34, 17);

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
volatile boolean batt_stat1 = LOW;
volatile boolean batt_stat2 = LOW;

volatile unsigned long batt_stat1_last_change = 0;
volatile unsigned long batt_stat2_last_change = 0;
volatile uint16_t      batt_stat1_hiz_count = 0;
volatile uint16_t      batt_stat2_hiz_count = 0;

volatile boolean       batt_stat1_hiz = false;
volatile boolean       batt_stat2_hiz = false;

boolean wastouched = false;

void setup() {

  Serial.begin(115200);
  
  blank();

  pinMode(PCTL_PIN, OUTPUT);
  digitalWrite(PCTL_PIN, HIGH);

  pinMode(BATT_STAT1_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BATT_STAT1_PIN), batt_stat1_changed, CHANGE);
  
  pinMode(BATT_STAT2_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BATT_STAT2_PIN), batt_stat2_changed, CHANGE);

  pinMode(4, OUTPUT);
  
  enc1.write(0);
  enc2.write(0);

  but1.begin();

  AudioMemory(15);

  AudioNoInterrupts();

  // Triangle
  env_w1.hold(0);
  env_w1.attack(10);
  env_w1.hold(0);
  env_w1.decay(1000);
  env_w1.sustain(0.0f);
  env_w1.release(1); 
  env_w1.releaseNoteOn(0);

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

  strip.begin();
  for (int x = 0; x < NUMPIXELS; x++) {
    strip.setPixelColor(x, 0x000000);
  }
  strip.show();  // Turn all LEDs off ASAP
}

void batt_stat1_changed(void) {
  if (millis() < batt_stat1_last_change + HIZ_TIMEOUT) {
    batt_stat1_hiz_count++;
  } else {
    batt_stat1_hiz_count = 0;
  }
  if (batt_stat1_hiz_count > HIZ_COUNT) {
    batt_stat1_hiz = true;
  } else {
    batt_stat1_hiz = false;
  }
  batt_stat1_last_change = millis();
}

void batt_stat2_changed(void) {
  if (millis() < batt_stat2_last_change + HIZ_TIMEOUT) {
    batt_stat2_hiz_count++;
  } else {
    batt_stat2_hiz_count = 0;
  }
  if (batt_stat2_hiz_count > HIZ_COUNT) {
    batt_stat2_hiz = true;
  } else {
    batt_stat2_hiz = false;
  }
  batt_stat2_last_change = millis();
}

void loop() {
  int x, y, inst;
  long ms = 0;
  static long cycles = 0;
  static long last_status = 0;

  analogWrite(4, sin(((float)millis() / 1000.0f)) * 255.0f); // 


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
        }
        if (inst == 1) {
        }
        if (inst == 2) {
          waveform1.begin(1.0f, tone_to_freq(notes[inst][tick / NUMCOLS][tick % NUMCOLS], 3), WAVEFORM_TRIANGLE);
          env_w1.decay((60000.0f / bpm / 4.0f));
          env_w1.noteOn();
        }
        if (inst == 3) {
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
      current_screen->OnTouch(p.x, p.y);

    }
  }
  wastouched = istouched;

  but1.read();
  if (but1.wasPressed()) {
    playing = !playing;
    current_screen->Draw(true);
  }

  int knob = analogRead(A13 );
  float vol = 1.0f - (float)knob / 1280.0f - 0.25f;
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

    current_screen->Draw();
  }

  for(y = 0; y < NUMROWS; y++) {
    for(x = 0; x < NUMCOLS; x++) {
      if (leds[y][x] == true) {
        strip.setPixelColor(row_col_to_led(y, x), 0x002030);
      } else {
        if (tick == y * NUMCOLS + x) {
          strip.setPixelColor(row_col_to_led(y, x), 0x000008);
        } else {
          strip.setPixelColor(row_col_to_led(y, x), 0x000000);
        }
      }
    }
  }
  strip.show();
  
  scan_buttons();
 
  if (millis() >= last_status + STATUS_INTERVAL) {

    vbat = (float)analogRead(VBAT_PIN) / 1024.0f * 3.3f * VBAT_DIVIDER;

    batt_stat1 = digitalRead(BATT_STAT1_PIN);
    batt_stat2 = digitalRead(BATT_STAT2_PIN);

    if (millis() > batt_stat1_last_change + HIZ_TIMEOUT) {
      batt_stat1_hiz_count = 0;
      batt_stat1_hiz = false; 
    }
    if (millis() > batt_stat2_last_change + HIZ_TIMEOUT) {
      batt_stat2_hiz_count = 0;
      batt_stat2_hiz = false; 
    }

    if (batt_stat1 == LOW && batt_stat2 == HIGH) {
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

uint16_t tick_to_led(uint16_t t) {
  uint16_t row = t / NUMCOLS;
  uint16_t col = t % NUMCOLS;
  if (row % 2) {
    return row * NUM_LED_COLS + NUM_LED_COLS - col;
  } else {
    return row * NUM_LED_COLS + col;
  }
}

uint16_t row_col_to_led(uint16_t row, uint16_t col) {
  if (row % 2) {
    return row * NUM_LED_COLS + NUM_LED_COLS - 1 - col;
    return 0;
  } else {
    return row * NUM_LED_COLS + col;
  }
}

void play_note(uint16_t which_note) {  
  Serial.print("Playing note");
  Serial.println(which_note);

  waveform1.begin(1.0f, tone_to_freq(which_note, 3), WAVEFORM_TRIANGLE);
  env_w1.decay((60000.0f / bpm / 4.0f));
  env_w1.noteOn();
}

float tone_to_freq(uint16_t note, uint16_t octave) {
  if (note >= 12) return 0.0f;
  if (octave < 1 || octave > 5) return 0.0f;

  return tones[note] * pow(2, octave);
}

