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

#define BATT_STAT1_PIN  15
#define BATT_STAT2_PIN  16

// Time interval for serial status in ms
#define STATUS_INTERVAL 1000

#define NUMPIXELS 1 // Number of LEDs in strip

// RGB LED pins
#define DATAPIN    0
#define CLOCKPIN   1

// For optimized ILI9341_t3 library
#define TFT_DC      20
#define TFT_CS      21
#define TFT_RST    255  // 255 = unused, connect to 3.3V
#define TFT_MOSI     7
#define TFT_SCLK    14
#define TFT_MISO    12

#define TOUCH_CS    33

extern ILI9341_t3 tft;

#define TFT_INTERVAL 100

extern int tick;
extern int current_inst;
extern bool leds[NUMROWS][NUMCOLS];
extern long buttons[NUMROWS][NUMCOLS];
extern uint8_t notes[NUMINST][NUMROWS][NUMCOLS];
extern boolean playing;
extern float bpm;
extern uint16_t tones[];

#define DEFAULT_NOTE  1

#endif // __BEATS_H___