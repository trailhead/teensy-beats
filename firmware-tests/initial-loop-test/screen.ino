#include "tft.h"

void draw_channel(void) {
  tft.fillRect(0, 15, 100, 15, ILI9341_BLACK);
  tft.setCursor(0,15);
  tft.print("channel ");
  tft.print(current_inst);
}

void draw_voltage(void) {
  tft.fillRect(240, 0, 100, 15, ILI9341_BLACK);
  tft.setCursor(240,0);
  if (bat_stat2 == LOW) {
    tft.print("Charging");
  } else {
    tft.print("VBAT: ");
    tft.print(vbat);
  }
}

void draw_freq(void) {
  tft.fillRect(100, 15, 100, 15, ILI9341_BLACK);
  if (!playing) {
    tft.setCursor(100,15);
    tft.print("Freq ");
    tft.print(tones[notes[current_inst][tick / NUMCOLS][tick % NUMCOLS]]);
  }
}

