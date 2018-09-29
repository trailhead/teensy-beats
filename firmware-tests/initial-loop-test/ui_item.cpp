#include "beats.h"
#include "ui_item.h"

KeysScreen k = KeysScreen();
HomeScreen h = HomeScreen();
WaveformInstrumentSettings wf_set = WaveformInstrumentSettings();

UiScreen *current_screen = &h;

void tempo_encoder_onchange(boolean clockwise, long value);
void channel_encoder_onchange(boolean clockwise, long value);

bool UiItem::HitTest(uint16_t touch_x, uint16_t touch_y) {
  if (touch_x >= x_ && touch_x <= x_ + w_ && touch_y >= y_ && touch_y <= y_ + h_) {
    if (callback_ != nullptr) {
      (callback_)(callback_param_);
    }
    return true;
  }
  return false;
}

void UiItem::Draw(boolean force_refresh) {
  if (dirty_ || force_refresh) {
    this->DrawBackground();
    DrawText();
  }
  dirty_ = false;
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

void UiItem::SetBorder(boolean new_border) {
  if (new_border != border_) {
    dirty_ = true;
  }
  border_ = new_border;
}

void UiItem::SetBackground(uint16_t new_bg) {
  if (new_bg != bg_color_) {
    dirty_ = true;
  }
  bg_color_ = new_bg;
}

void UiItem::SetText(char *new_text) {
  if (new_text != text_ || ((new_text != nullptr && text_ != nullptr) && strcmp(const_cast<const char*>(new_text),const_cast<const char*>(text_)) != 0)) {
    dirty_ = true;
  }
  text_ = new_text;
}
void UiItem::SetDirty(void) {
  dirty_ = true;
}

bool UiScreen::OnTouch(uint16_t touch_x, uint16_t touch_y) {
  return false;
}

HomeScreen::HomeScreen(void) {
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      uint8_t index = row * 4 + col;
      note_buttons_[index].w_ = 46;
      note_buttons_[index].h_ = BUTTON_HEIGHT - BUTTON_SPACING;
      note_buttons_[index].x_ = BUTTON_SPACING / 2 + col * 48;
      note_buttons_[index].y_ = (STATUS_HEIGHT + BUTTON_SPACING / 2) + row * BUTTON_HEIGHT;
    }
  }
};

void HomeScreen::Draw(boolean force_refresh) {
  static int last_bpm = 0;
  static int last_channel = 0;
  static int last_pattern = 0;

  if (force_refresh) {
    encoder1.setCallback(&tempo_encoder_onchange);
    encoder2.setCallback(&channel_encoder_onchange);
  }
  
  for (unsigned i = 0; i < 16; i++) {
    if (tick == i) {
      note_buttons_[i].SetBorder(true);
    } else {
      note_buttons_[i].SetBorder(false);
    }

    if (notes[current_inst][i / NUMROWS][i % NUMCOLS] > 0) {
      note_buttons_[i].SetBackground(ILI9341_BLUE);
    } else {
      note_buttons_[i].SetBackground(ILI9341_RED);
    }
  }

  dtostrf(bpm, 6, 1, bpm_buf_);
  play_button_.SetText(bpm_buf_);
  if (last_bpm != bpm) {
    play_button_.SetDirty();
  }

  sprintf(chan_buf_, "Ch %d", current_inst + 1);
  channel_button_.SetText(chan_buf_);  
  if (last_channel != current_inst) {
    channel_button_.SetDirty();
  }

  sprintf(pat_buf_, "Pat   %d", 1);
  pattern_button_.SetText(pat_buf_);
  if (last_pattern != 1) {
    pattern_button_.SetDirty();
  }

  for (unsigned i = 0; i < sizeof(p_children_) / sizeof(UiItem*); i++) {
    p_children_[i]->Draw(force_refresh);
  }

  last_bpm = bpm;
  last_channel = current_inst;
  last_pattern = 1;
}

bool HomeScreen::OnTouch(uint16_t touch_x, uint16_t touch_y) {
  for (int i = 0; i < sizeof(p_children_) / sizeof(UiItem*); i++) {
    if (p_children_[i]->HitTest(touch_x, touch_y) == true) {
      if (i < 16) {
        if (notes[current_inst][i / NUMROWS][i % NUMCOLS] > 0) {
          notes[current_inst][i / NUMROWS][i % NUMCOLS] = 0;
        } else {
          notes[current_inst][i / NUMROWS][i % NUMCOLS] = DEFAULT_NOTE;
        }
      }
    }
  }
  if (keyboard_button_.HitTest(touch_x, touch_y) == true) {
    tft.fillScreen(ILI9341_BLACK);
    k.Draw(true);
    current_screen = &k;
  }
  if (play_button_.HitTest(touch_x, touch_y) == true) {
    playing = !playing;
    play_button_.Draw(true);
  }
  if (instrument_button_.HitTest(touch_x, touch_y) == true) {
    tft.fillScreen(ILI9341_BLACK);
    wf_set.Draw(true);
    current_screen = &wf_set;
    return true;
  }
}

void PlayButton::Draw(boolean force_refresh) {
  if (dirty_ || force_refresh) {
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
  dirty_ = false;
}

KeysScreen::KeysScreen(void) {  
  for (int key = 0; key < 7; key++) {
    key_buttons_[key].w_ = keys_width_;
    key_buttons_[key].h_ = SCREEN_HEIGHT - STATUS_HEIGHT - BUTTON_HEIGHT - BUTTON_SPACING;
    key_buttons_[key].x_ = keys_spacing_ * key + BUTTON_SPACING / 2;
    key_buttons_[key].y_ = STATUS_HEIGHT + BUTTON_SPACING / 2;
  }
  key_buttons_[7].x_ = keys_spacing_ - keys_spacing_ / 2 - BUTTON_SPACING / 2;
  key_buttons_[8].x_ = keys_spacing_ * 2 - keys_spacing_ / 2 - BUTTON_SPACING / 2;
  key_buttons_[9].x_ = keys_spacing_ * 4 - keys_spacing_ / 2 - BUTTON_SPACING / 2;
  key_buttons_[10].x_ = keys_spacing_ * 5 - keys_spacing_ / 2 - BUTTON_SPACING / 2;
  key_buttons_[11].x_ = keys_spacing_ * 6 - keys_spacing_ / 2 - BUTTON_SPACING / 2;
};

void KeysScreen::Draw(boolean force_refresh) {
  for (int i = 0; i < sizeof(p_children_) / sizeof(UiItem*); i++) {
    p_children_[i]->Draw(force_refresh);
  }
}

bool KeysScreen::OnTouch(uint16_t touch_x, uint16_t touch_y) {
  // Iterate the keys in reverse order to get top-most touches first.
  for (int i = 11; i >= 0; --i) {
    if (p_children_[i]->HitTest(touch_x, touch_y) == true) {
      // Return on the first successful hit.
      return true;
    }
  }
  if (home_button_.HitTest(touch_x, touch_y) == true) {
    tft.fillScreen(ILI9341_BLACK);
    h.Draw(true);
    current_screen = &h;
    return true;
  }
  
  return false;
}

void WaveformInstrumentSettings::Draw(boolean force_refresh) {
  for (int i = 0; i < sizeof(p_children_) / sizeof(UiItem*); i++) {
    p_children_[i]->Draw(force_refresh);
  }
}

bool WaveformInstrumentSettings::OnTouch(uint16_t touch_x, uint16_t touch_y) {
  for (int i = 0; i < sizeof(p_children_) / sizeof(UiItem*); i++) {
    if (p_children_[i]->HitTest(touch_x, touch_y) == true) {
      Serial.println("touched");
    }
  }
}


void tempo_encoder_onchange(boolean clockwise, long value) {
  if (clockwise) {
    if (playing) {
      bpm -= 5;
      if (bpm < 50) {
        bpm = 50;
      }
    } else {
      tick--;      
      if (tick < 0) {
        tick = NUMTICKS - 1;
      }
    }
  } else {
    if (playing) {
      bpm += 5;
      if (bpm > 250) {
        bpm = 250;
      }
    } else {
      tick++;
      if (tick >= NUMTICKS) {
        tick = 0;
      }
    }
  }
}

void channel_encoder_onchange(boolean clockwise, long value) {
  if (clockwise) {
    if (playing) {
      current_inst--;
      if (current_inst < 0) {
        current_inst = NUMINST - 1;
      }
    } else {
      notes[current_inst][tick / NUMCOLS][tick % NUMCOLS]--;      
      if (notes[current_inst][tick / NUMCOLS][tick % NUMCOLS] <= 0) {
        notes[current_inst][tick / NUMCOLS][tick % NUMCOLS] = sizeof(tones) / sizeof(uint16_t) - 1;
      }
      draw_freq();
    }
  } else {
    if (playing) {
      current_inst++;
      if (current_inst >= NUMINST) {
        current_inst = 0;
      }
    } else {
      notes[current_inst][tick / NUMCOLS][tick % NUMCOLS]++;      
      if (notes[current_inst][tick / NUMCOLS][tick % NUMCOLS] >= sizeof(tones) / sizeof(uint16_t)) {
        notes[current_inst][tick / NUMCOLS][tick % NUMCOLS] = 1;
      }
      draw_freq();
    }
  }
}


