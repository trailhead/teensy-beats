void draw_note_grid(uint8_t data[NUMROWS][NUMCOLS], uint16_t x, uint16_t y)
{
  for (uint8_t row = 0; row < NUMROWS; row++) {
    for (uint8_t col = 0; col < NUMCOLS; col++) {
      if (data[row][col] > 0) {
        tft.fillRect(x + col * 42, y + row * 42, 40, 40, ILI9341_BLUE);
      } else {
        tft.fillRect(x + col * 42, y + row * 42, 40, 40, ILI9341_RED);
      }
      if (tick == row * NUMCOLS + col) {
        tft.drawRect(x + col * 42, y + row * 42, 40, 40, ILI9341_WHITE);
      }
    }
  }
}

void draw_note_menu(void) {
  tft.fillRoundRect(231, 1, 86, 46, 4, ILI9341_RED);
  tft.setCursor(250,14);
  tft.print("BPM");
  tft.fillRoundRect(231, 49, 86, 46, 4, ILI9341_RED);
  tft.setCursor(250,62);
  tft.print("NOTE");
  tft.fillRoundRect(231, 97, 86, 46, 4, ILI9341_RED);
  tft.setCursor(250,110);
  tft.print("LEN");
  tft.fillRoundRect(231, 145, 86, 46, 4, ILI9341_RED);
  tft.fillRoundRect(231, 193, 86, 46, 4, ILI9341_RED);
}

void draw_bpm(void) {
  tft.fillRect(0, 0, 100, 15, ILI9341_BLACK);
  tft.setCursor(0,0);
  tft.print(bpm,1);
  tft.print(" bpm  ");  
}

void draw_channel(void) {
  tft.fillRect(0, 15, 100, 15, ILI9341_BLACK);
  tft.setCursor(0,15);
  tft.print("channel ");
  tft.print(current_inst);
}

void draw_voltage(void) {
  tft.fillRect(0, 225, 100, 15, ILI9341_BLACK);
  tft.setCursor(0,225);
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

