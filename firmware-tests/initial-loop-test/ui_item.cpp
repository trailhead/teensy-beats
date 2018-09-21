#include "ui_item.h"

bool UiItem::HitTest(uint16_t touch_x, uint16_t touch_y) {
  if (touch_x >= x_ && touch_x <= x_ + w_ && touch_y >= y_ && touch_y <= y_ + h_) {
    return true;
  }
  return false;
}

void UiItem::Draw(void) {
  this->DrawBackground();
  DrawText();
}

void UiItem::DrawBackground(void) {
  tft.fillRoundRect(x_, y_, w_, h_, radius_, bg_color_);
  if (border_ == true) {
    tft.drawRoundRect(x_, y_, w_, h_, radius_, border_color_);
  }
}

void UiItem::DrawText(void) {
  if (text_ != nullptr) {
    tft.setCursor((float)x_ + ((float)w_ / 2.0f) - ((float)tft.strPixelLen(text_) / 2.0f), (float)y_ + ((float)h_ / 2.0f) - ((float)tft.getTextSize() * 4.0f));
    tft.print(text_);
  }
}

bool UiScreen::OnTouch(uint16_t touch_x, uint16_t touch_y) {
  return false;
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
  for (unsigned i = 0; i < 16; i++) {
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
  }

  dtostrf(bpm, 6, 1, bpm_buf_);
  play_button_.text_ = bpm_buf_;

  sprintf(chan_buf_, "Ch %d", current_inst + 1);
  channel_button_.text_ = chan_buf_;

  sprintf(pat_buf_, "Pat   %d", 1);
  pattern_button_.text_ = pat_buf_;

  for (unsigned i = 0; i < sizeof(p_children_) / sizeof(UiItem*); i++) {
    p_children_[i]->Draw();
  }
}

bool HomeScreen::OnTouch(uint16_t touch_x, uint16_t touch_y) {
  for (unsigned i = 0; i < sizeof(p_children_) / sizeof(UiItem*); i++) {
    if (p_children_[i]->HitTest(touch_x, touch_y) == true) {

      if (i < 16) {
        if (notes[current_inst][i / 4][i % 4] > 0) {
          notes[current_inst][i / 4][i % 4] = 0;
        } else {
          notes[current_inst][i / 4][i % 4] = DEFAULT_NOTE;
        }
      }
      
      Serial.print("Touch on child item ["); 
      Serial.print(i); 
      Serial.println("]"); 
    }
  }
}

void PlayButton::Draw(void) {
  UiItem::DrawBackground();
  if (playing == true) {
    for (int i = 0; i < w_ / 4; i++) {
      tft.drawFastVLine(x_ + w_ / 2 - w_ / 8 - i, y_ + (float)h_ / 2.0f - (float)i / 2.0f, i, ILI9341_WHITE);
    }
  } else {
    tft.fillRect(x_ + (float)w_ / 8.0f, y_ + h_ / 4, w_ / 8, h_ / 2, ILI9341_WHITE);
    tft.fillRect(x_ + (float)w_ / 8.0f * 2.5f, y_ + h_ / 4, w_ / 8, h_ / 2, ILI9341_WHITE);
  }

  tft.setFont(Arial_9);
  tft.setCursor(x_ + (float)w_ / 2.0f, y_ + (float)h_ / 2.0f - tft.getTextSize() * 4);
  tft.print(text_);

}


