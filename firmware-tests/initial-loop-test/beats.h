#ifndef __BEATS_H__
#define __BEATS_H__

#include <stdint.h>
#include "ILI9341_t3.h"
#include "font_Arial.h"

#define NUMROWS       4
#define NUMCOLS       4
#define NUMINST       4
#define NUMTICKS  (NUMROWS*NUMCOLS)
#define BUTTON_DEBOUNCE 250
#define VBAT_PIN      A22
#define VBAT_DIVIDER  2.0f

#define SCREEN_HEIGHT   240
#define SCREEN_WIDTH    320

#define BATT_STAT1_PIN  4
#define BATT_STAT2_PIN  5
#define HIZ_TIMEOUT     200 // ms of stable state to reset high-z state
#define HIZ_COUNT       5   // minimum pin changes under HIZ_TIMEOUT to indicate we're in high-z state

// Time interval for serial status in ms
#define STATUS_INTERVAL 2000

#define NUMPIXELS 20 // Number of LEDs in strip

// RGB LED pins
#define DATAPIN       1
#define CLOCKPIN      0

#define NUM_LED_COLS  5
#define NUM_LED_ROWS  4

// For optimized ILI9341_t3 library
#define TFT_DC      20
#define TFT_CS      21
#define TFT_RST    255  // 255 = unused, connect to 3.3V
#define TFT_MOSI     7
#define TFT_SCLK    14
#define TFT_MISO    12

#define TOUCH_CS    33

extern ILI9341_t3 tft;

#define TFT_INTERVAL 50

extern int tick;
extern int current_inst;
extern bool leds[NUMROWS][NUMCOLS];
extern long buttons[NUMROWS][NUMCOLS];
extern uint8_t notes[NUMINST][NUMROWS][NUMCOLS];
extern boolean playing;
extern float bpm;
extern uint16_t tones[];

#define DEFAULT_NOTE  12

void play_note(uint16_t which_note);

#endif // __BEATS_H___
