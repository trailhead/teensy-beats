#include <JC_Button.h>
#include <Encoder.h>
#include <Audio.h>
#include <Wire.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <XPT2046_Touchscreen.h>
#include "beats.h"
#include "keypad.h"
#include "cb_encoder.h"
#include "ui_item.h"
#include "instrument.h"

AudioControlSGTL5000     sgtl5000_1;     //xy=911,298

MultiplexedKeypad keypad = MultiplexedKeypad( KEYPAD_ROW_0, KEYPAD_ROW_1, KEYPAD_ROW_2, KEYPAD_ROW_3, KEYPAD_ROW_4,
                           KEYPAD_COL_0, KEYPAD_COL_1, KEYPAD_COL_2, KEYPAD_COL_3, KEYPAD_COL_4);

Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);

XPT2046_Touchscreen ts(TOUCH_CS);

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

WaveformInstrument wf_inst1 = WaveformInstrument();
WaveformInstrument wf_inst2 = WaveformInstrument();
WaveformInstrument wf_inst3 = WaveformInstrument();
PCMInstrument pcm_inst1 = PCMInstrument();

CallbackEncoder encoder1(26, 25);
CallbackEncoder encoder2(34, 17);
CallbackEncoder encoder3(10, 24);
CallbackEncoder encoder4(3, 2);

float bpm        = 80.0;
int tick         = 0;
int current_inst = 0;
unsigned long next_tick   = 0;
unsigned long next_tft    = 0;
boolean playing     = false ;
boolean sd_inserted = false;
float vbat = 0;
volatile boolean batt_stat1 = LOW;
volatile boolean batt_stat2 = LOW;

volatile unsigned long batt_stat1_last_change = 0;
volatile unsigned long batt_stat2_last_change = 0;
volatile uint16_t      batt_stat1_hiz_count = 0;
volatile uint16_t      batt_stat2_hiz_count = 0;

volatile boolean       batt_stat1_hiz = false;
volatile boolean       batt_stat2_hiz = false;

unsigned long last_pwr_btn_press = 0;
unsigned long last_pwr_btn_release = 0;
boolean pwr_btn_down             = false;

boolean wastouched = false;

void setup() {

  Serial.begin(115200);

  pinMode(PWR_BTN_PIN, INPUT);

  pinMode(PCTL_PIN, OUTPUT);
  digitalWrite(PCTL_PIN, HIGH);

  pinMode(BATT_STAT1_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BATT_STAT1_PIN), batt_stat1_changed, CHANGE);

  pinMode(BATT_STAT2_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BATT_STAT2_PIN), batt_stat2_changed, CHANGE);

  pinMode(4, OUTPUT);

  for (uint8_t x = 0; x < NUMCOLS; x++) {
    for (uint8_t y = 0; y < NUMROWS; y++) {
      keypad.setCallback(x, y, &set_note);
    }
  }

  keypad.setCallback(0, 4, &toggle_playing);

  if (SD.begin(SDCARD_CS_PIN)) {

    Serial.println("Found SD card");
    sd_inserted = true;
  } else {
    sd_inserted = false;
  }

  AudioMemory(30);

  AudioNoInterrupts();

  wf_inst1.setToneType(WAVEFORM_SINE);
  wf_inst1.setAttack(10.0f);
  wf_inst1.setHold(0);
  wf_inst1.setDecay(1000.0f);
  wf_inst1.setRelease(1.0f);
  wf_inst1.setSustain(0);

  wf_inst2.setToneType(WAVEFORM_TRIANGLE);
  wf_inst2.setAttack(10.0f);
  wf_inst2.setHold(0);
  wf_inst2.setDecay(1000.0f);
  wf_inst2.setRelease(1.0f);
  wf_inst2.setSustain(0);

  wf_inst2.setToneType(WAVEFORM_SQUARE);
  wf_inst3.setAttack(10.0f);
  wf_inst3.setHold(0);
  wf_inst3.setDecay(1000.0f);
  wf_inst3.setRelease(1.0f);
  wf_inst3.setSustain(0);

  pcm_inst1.setPath(0, "13750_~1.WAV");
  pcm_inst1.setPath(1, "106290~1.WAV");
  pcm_inst1.setPath(2, "132279~1.WAV");
  pcm_inst1.setPath(3, "147597~1.WAV");
  pcm_inst1.setPath(4, "154713~1.WAV");
  pcm_inst1.setPath(5, "244194~1.WAV");
  pcm_inst1.setPath(6, "316874~1.WAV");
  pcm_inst1.setPath(7, "349501~1.WAV");
  pcm_inst1.setPath(8, "COSTCO~1.WAV");

  wf_set.SetInstrument(&wf_inst1);

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

  next_tick = millis() + (unsigned long)(60000.0f / bpm / 4.0f);
  next_tft = 0;
  /*
    notes[0][0][0] = 1;
    notes[0][0][1] = 1;
    notes[0][0][2] = 1;
    notes[0][0][3] = 1;
    notes[0][2][0] = 1;
  */

  vbat = (float)analogRead(VBAT_PIN) / 1024.0f * 3.3f * VBAT_DIVIDER;

  strip.begin();
  for (int x = 0; x < NUMPIXELS; x++) {
    strip.setPixelColor(x, 0x000000);
  }
  strip.show();  // Turn all LEDs off ASAP

  current_screen->Draw(true);
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
  unsigned long ms = 0;
  static unsigned long cycles = 0;
  static unsigned long last_status = 0;

  analogWrite(4, sin(((float)millis() / 1000.0f)) * 255.0f); //

  ms = millis();
  if (playing && ms >= next_tick) {
    next_tick = ms + (unsigned long)(60000.0f / bpm / 4.0f);

    tick++;
    if (tick >= NUMTICKS) {
      tick = 0;
    }

    wf_inst1.setDecay((60000.0f / bpm / 4.0f));

    for (inst = 0; inst < NUMINST; inst++) {
      if (notes[inst][tick / NUMCOLS][tick % NUMCOLS]) {
        if (inst == 0) {
          pcm_inst1.playNote(notes[inst][tick / NUMCOLS][tick % NUMCOLS] - 1, 3, 1.0f);
        }
        if (inst == 1) {
          wf_inst1.playNote(notes[inst][tick / NUMCOLS][tick % NUMCOLS] - 1, 2, 1.0f);
        }
        if (inst == 2) {
          wf_inst2.playNote(notes[inst][tick / NUMCOLS][tick % NUMCOLS] - 1, 4, 1.0f);
        }
        if (inst == 3) {
          wf_inst3.playNote(notes[inst][tick / NUMCOLS][tick % NUMCOLS] - 1, 5, 1.0f);
        }
      }
    }
  }

  boolean istouched = ts.touched();
  if (istouched) {
    TS_Point p = ts.getPoint();
    p.x = map(p.x, 290, 3700, 320, 0);
    p.y = map(p.y, 3700, 480, 240, 0);
    if (!wastouched) {
      /*
        Serial.print("New touch: ");
        Serial.print(p.x);
        Serial.print(", ");
        Serial.println(p.y);
      */
      current_screen->OnTouch(p.x, p.y);

    }
  }
  wastouched = istouched;

  int pot_val = analogRead(A13 );
  float vol = 1.0f - (float)pot_val / 1280.0f - 0.25f;
  sgtl5000_1.volume(vol);

  encoder1.update();
  encoder2.update();
  encoder3.update();
  encoder4.update();

  if (ms >= next_tft) {
    next_tft = ms + TFT_INTERVAL;

    current_screen->Draw();
  }

  for (y = 0; y < NUMROWS; y++) {
    for (x = 0; x < NUMCOLS; x++) {
      if (tick == y * NUMCOLS + x) {
        if (notes[current_inst][y][x]) {
          strip.setPixelColor(row_col_to_led(y, x), 0x002030);
        } else {
          strip.setPixelColor(row_col_to_led(y, x), 0x000008);
        }
      } else {
        strip.setPixelColor(row_col_to_led(y, x), 0x000000);
      }
    }
  }
  strip.show();

  keypad.scan();

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

    Serial.print("Memory: ");
    Serial.print(AudioMemoryUsage());
    Serial.print(",");
    Serial.println(AudioMemoryUsageMax());

    if (batt_stat1 == LOW && batt_stat2 == HIGH) {
      //Serial.println("Charging");
    } else {
      //Serial.print("Battery voltage: ");
      //Serial.println(vbat);
    }
    draw_voltage();

    //Serial.print((float)cycles / (millis() / 1000.0f));
    //Serial.println(" cycles per sec");

    last_status = millis();
  }

  handle_pwr_btn();

  cycles++;
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
  pcm_inst1.playNote(which_note, 2, 1.0f);
}

float mix_l_gain_from_pan(float pan) {
  return 1.0f;
}

float mix_r_gain_from_pan(float pan) {
  return 1.0f;
}

float tone_to_freq(uint16_t note, uint16_t octave) {
  if (note >= 12) return 0.0f;
  if (octave < 1 || octave > 5) return 0.0f;

  return tones[note] * pow(2, octave);
}

void toggle_note(uint8_t x, uint8_t y) {
  if (x < NUMCOLS && y < NUMROWS) {
    if (notes[current_inst][y][x] > 0) {
      notes[current_inst][y][x] = 0;
    } else {
      notes[current_inst][y][x] = DEFAULT_NOTE;
    }
  }
}

void set_note(uint8_t x, uint8_t y) {

  notes[current_inst][tick / NUMROWS][tick % NUMROWS] = (y * NUMROWS + x) % 12;

  switch (current_inst) {
    case 0:
      pcm_inst1.playNote((y * NUMROWS + x) % 12 - 1, 3, 1.0f);
      break;
    case 1:
      wf_inst1.playNote((y * NUMROWS + x) % 12 - 1, 3, 1.0f);
      break;
    case 2:
      wf_inst2.playNote((y * NUMROWS + x) % 12 - 1, 3, 1.0f);
      break;
    case 3:
      wf_inst3.playNote((y * NUMROWS + x) % 12 - 1, 3, 1.0f);
      break;
  }
}

void toggle_playing(uint8_t x, uint8_t y) {
  playing = !playing;
  current_screen->Draw(true);
}

void handle_pwr_btn(void) {
  if (analogRead(PWR_BTN_PIN) > 512) {
    if (pwr_btn_down) {
      // Button was already down
      if (last_pwr_btn_press + SHUTDOWN_HOLD_TIME < millis()) {
        Serial.println("Power button down for > SHUTDOWN_HOLD_TIME");
        // Drop the enable line on the power circuit to shutdown
        digitalWrite(PCTL_PIN, LOW);
      }
    } else {
      // This must be a new press
      pwr_btn_down       = true;
      last_pwr_btn_press = millis();
    }
  } else {
    if (pwr_btn_down == true && last_pwr_btn_release + BUTTON_DEBOUNCE < millis()) {
      last_pwr_btn_release = millis();
      pwr_btn_down       = false;
      Serial.println("Power button released");
      change_screen(&h);
    }
    pwr_btn_down = false;
  }
}

void clear_channel(uint8_t channel) {
  for (uint8_t y = 0; y < NUMROWS; y++) {
    for (uint8_t x = 0; x < NUMCOLS; x++) {
      notes[channel][y][x] = 0;
    }
  }
}

