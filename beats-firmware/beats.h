#ifndef __BEATS_H__
#define __BEATS_H__

#include <stdint.h>
#include <Audio.h>
#include "ILI9341_t3.h"
#include "font_Arial.h"
#include "cb_encoder.h"
#include "instrument.h"

#define NUM_BUTTON_ROWS       5
#define NUM_BUTTON_COLS       5
#define KEYPAD_ROW_0          28
#define KEYPAD_ROW_1          29
#define KEYPAD_ROW_2          30
#define KEYPAD_ROW_3          31
#define KEYPAD_ROW_4          27
#define KEYPAD_COL_0          35
#define KEYPAD_COL_1          36
#define KEYPAD_COL_2          37
#define KEYPAD_COL_3          38
#define KEYPAD_COL_4          39

#define NUMROWS               4
#define NUMCOLS               4
#define NUMCOLS               4
#define NUMINST               4
#define NUMTICKS              (NUMROWS*NUMCOLS)
#define BUTTON_DEBOUNCE       250
#define VBAT_PIN              A22
#define VBAT_DIVIDER          2.0f

#define SCREEN_HEIGHT         240
#define SCREEN_WIDTH          320

#define PCTL_PIN              5
#define PWR_BTN_PIN           A11
#define SHUTDOWN_HOLD_TIME    3000

#define BATT_STAT1_PIN        15
#define BATT_STAT2_PIN        16
#define HIZ_TIMEOUT           500 // ms of stable state to reset high-z state
#define HIZ_COUNT             3   // minimum pin changes under HIZ_TIMEOUT to indicate we're in high-z state

#define SDCARD_CS_PIN         BUILTIN_SDCARD

// Time interval for serial status in ms
#define STATUS_INTERVAL       2000

#define NUMPIXELS 20 // Number of LEDs in strip

// RGB LED pins
#define DATAPIN               1
#define CLOCKPIN              0

#define NUM_LED_COLS          5
#define NUM_LED_ROWS          4

// For optimized ILI9341_t3 library
#define TFT_DC                20
#define TFT_CS                21
#define TFT_RST               255  // 255 = unused, connect to 3.3V
#define TFT_MOSI              7
#define TFT_SCLK              14
#define TFT_MISO              12

#define TOUCH_CS              33
#define TFT_INTERVAL          50

extern ILI9341_t3 tft;

extern int tick;
extern int current_inst;
extern uint8_t notes[NUMINST][NUMROWS][NUMCOLS];
extern boolean playing;
extern boolean sd_inserted;
extern float bpm;
extern float tones[12];

extern WaveformInstrument wf_inst1;
extern WaveformInstrument wf_inst2;
extern WaveformInstrument wf_inst3;

extern CallbackEncoder encoder1;
extern CallbackEncoder encoder2;
extern CallbackEncoder encoder3;
extern CallbackEncoder encoder4;

#define DEFAULT_NOTE          1

typedef struct {
  uint8_t note;
  uint8_t octave;
  uint8_t vol;
  int8_t pan; 
} noteinfo;

float mix_l_gain_from_pan(float pan);
float mix_r_gain_from_pan(float pan);
void play_note(uint16_t which_note);
float tone_to_freq(uint16_t note, uint16_t octave);
void clear_channel(uint8_t channel);

#endif // __BEATS_H___
