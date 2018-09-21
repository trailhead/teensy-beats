#ifndef __TFT_H__
#define __TFT_H__

#include <stdint.h>
#include "ILI9341_t3.h"
#include "font_Arial.h"

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

#endif // __TFT_H__
