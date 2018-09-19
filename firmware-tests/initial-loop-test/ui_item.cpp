#include "ui_item.h"

bool UiItem::HitTest(uint16_t touch_x, uint16_t touch_y) {
  return true;
}

void UiItem::Draw(void) {
  int16_t x1, y1, w, h; // To receive text bounds
  tft.fillRoundRect(x_, y_, w_, h_, radius_, bg_color_);
  if (text_ != nullptr) {
    tft.setCursor((float)x_ + ((float)w_ / 2.0f) - ((float)tft.strPixelLen(text_) / 2.0f), (float)y_ + ((float)h_ / 2.0f) - ((float)tft.getTextSize() * 4.0f));
    tft.print(text_);
  }
  if (border_ == true) {
    tft.drawRoundRect(x_, y_, w_, h_, radius_, border_color_);
  }
}

HomeScreen::HomeScreen(void) {
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      uint8_t index = row * 4 + col;
      note_buttons_[index].w_ = 46;
      note_buttons_[index].h_ = 46;
      note_buttons_[index].x_ = 1 + col * 48;
      note_buttons_[index].y_ = 1 + row * 48;
    }
  }
};

void HomeScreen::Draw(void) {
  for (int i = 0; i < 16; i++) {
    if (tick == i) {
      note_buttons_[i].border_ = true;
    } else {
      note_buttons_[i].border_ = false;
    }

    if (notes[current_inst][i / 4][i % 4] > 0) {
      note_buttons_[i].bg_color_ = ILI9341_BLUE;
    } else {
      note_buttons_[i].bg_color_ = ILI9341_RED;
    }
    note_buttons_[i].Draw();
  }
  play_button_.Draw();
}

void PlayButton::Draw(void) {
  UiItem::Draw();
  if (playing == true) {
    for (int i = 0; i < w_ / 4; i++) {
      tft.drawFastVLine(x_ + w_ / 2 - w_ / 8 - i, y_ + (float)h_ / 2.0f - (float)i / 2.0f, i, ILI9341_WHITE);
    }
  } else {
    tft.fillRect(x_ + (float)w_ / 8.0f, y_ + h_ / 4, w_ / 8, h_ / 2, ILI9341_WHITE);
    tft.fillRect(x_ + (float)w_ / 8.0f * 2.5f, y_ + h_ / 4, w_ / 8, h_ / 2, ILI9341_WHITE);
  }

  dtostrf(bpm, 6, 1, bpm_buf_);
  tft.setFont(Arial_9);
  tft.setCursor(x_ + (float)w_ / 2.0f, y_ + (float)h_ / 2.0f - tft.getTextSize() * 4);
  tft.print(bpm_buf_);

}


