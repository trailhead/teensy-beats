#include "beats.h"

void draw_voltage(void) {
  tft.fillRect(240, 0, 100, 15, ILI9341_BLACK);
  tft.setCursor(240,0);
  if (batt_stat1_hiz == true && batt_stat2_hiz == true) {
    tft.print("No Batt");
  } else {
    if (batt_stat1 == LOW && batt_stat2 == HIGH) {
      tft.print("Charging");
    } else {
      tft.print("VBAT: ");
      tft.print(vbat);
    }
  }
}

