#ifndef __UI_ITEM_H__
#define __UI_ITEM_H__

#include "beats.h"

#define BUTTON_ROWS     5
#define BUTTON_HEIGHT   ((SCREEN_HEIGHT - STATUS_HEIGHT) / BUTTON_ROWS)
#define BUTTON_SPACING  2
#define STATUS_HEIGHT   15

class UiItem {

public:
  uint16_t x_ = 0;
  uint16_t y_ = 0;
  uint16_t w_ = 0;
  uint16_t h_ = 0;
  uint16_t radius_ = 0;
  uint16_t bg_color_ = ILI9341_RED;
  uint16_t border_color_ = ILI9341_WHITE;
  boolean border_ = false;
  uint16_t text_color_ = 0;
  void (*callback_)(uint16_t);
  uint16_t callback_param_ = 0;
  char *text_;

  boolean dirty_ = true;

  void (*on_touch_func_)(void);

public:
  UiItem(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t radius, uint16_t bg_color, uint16_t text_color, char *text, void (*cb)(uint16_t) = nullptr, uint16_t cb_param = 0) :
  x_(x),
  y_(y),
  w_(w),
  h_(h),
  radius_(radius),
  bg_color_(bg_color),
  text_color_(text_color),
  text_(text),
  callback_(cb),
  callback_param_(cb_param) {
  }

  virtual bool HitTest(uint16_t touch_x, uint16_t touch_y);
  virtual void Draw(boolean force_refresh = false);
  virtual void DrawBackground(void);
  virtual void DrawText (void);

  void SetBorder(boolean new_w);
  void SetBackground(uint16_t new_bg);
  void SetText(char *new_text);
  void SetDirty(void);

};

class UiScreen {
protected:
  UiItem *children_[];
  UiScreen() {};

public:
  virtual void Draw(boolean force_refresh = false) = 0;
  virtual void Setup(void);
  virtual bool OnTouch(uint16_t touch_x, uint16_t touch_y);
};

class PlayButton : public UiItem {
private:

public:
  PlayButton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t radius, uint16_t bg_color, uint16_t text_color, char *text, void (*cb)(uint16_t) = nullptr, uint16_t cb_param = 0) :
  UiItem(x, y, w, h, radius, bg_color, text_color, text, cb, cb_param) {    
  }
  void Draw(boolean force_refresh = false);
};

class HomeScreen : public UiScreen {
protected:

  char bpm_buf_[7];
  char chan_buf_[7];
  char pat_buf_[7];
  
  UiItem note_buttons_[16] = {
    UiItem(0, 0, 40, 40, 4, ILI9341_RED, 0, "1"),
    UiItem(42, 0, 40, 40, 4, ILI9341_RED, 0, "2"),
    UiItem(84, 0, 40, 40, 4, ILI9341_RED, 0, "3"),
    UiItem(126, 0, 40, 40, 4, ILI9341_RED, 0, "4"),
    UiItem(0, 42, 40, 40, 4, ILI9341_RED, 0, "5"),
    UiItem(42, 42, 40, 40, 4, ILI9341_RED, 0, "6"),
    UiItem(84, 42, 40, 40, 4, ILI9341_RED, 0, "7"),
    UiItem(126, 42, 40, 40, 4, ILI9341_RED, 0, "8"),
    UiItem(0, 84, 40, 40, 4, ILI9341_RED, 0, "9"),
    UiItem(42, 84, 40, 40, 4, ILI9341_RED, 0, "10"),
    UiItem(84, 84, 40, 40, 4, ILI9341_RED, 0, "11"),
    UiItem(126, 84, 40, 40, 4, ILI9341_RED, 0, "12"),
    UiItem(0, 126, 40, 40, 4, ILI9341_RED, 0, "13"),
    UiItem(42, 126, 40, 40, 4, ILI9341_RED, 0, "14"),
    UiItem(84, 126, 40, 40, 4, ILI9341_RED, 0, "15"),
    UiItem(126, 126, 40, 40, 4, ILI9341_RED, 0, "16")
  };
  PlayButton play_button_ = PlayButton(1, STATUS_HEIGHT + BUTTON_SPACING / 2, 78, BUTTON_HEIGHT - BUTTON_SPACING, 4, ILI9341_RED, 0, nullptr);
  UiItem channel_button_ = UiItem(81, STATUS_HEIGHT + BUTTON_SPACING / 2, 78, BUTTON_HEIGHT - BUTTON_SPACING, 4, ILI9341_RED, 0, nullptr);
  UiItem pattern_button_ = UiItem(161, STATUS_HEIGHT + BUTTON_SPACING / 2, 78, BUTTON_HEIGHT - BUTTON_SPACING, 4, ILI9341_RED, 0, nullptr);
  UiItem settings_button_ = UiItem(241, STATUS_HEIGHT + BUTTON_SPACING / 2, 78, BUTTON_HEIGHT - BUTTON_SPACING, 4, ILI9341_RED, 0, "Settings");
  UiItem keyboard_button_ = UiItem(241, STATUS_HEIGHT + BUTTON_HEIGHT + BUTTON_SPACING, 78, BUTTON_HEIGHT - BUTTON_SPACING, 4, ILI9341_RED, 0, "Keyboard");
  UiItem instrument_button_ = UiItem(241, STATUS_HEIGHT + BUTTON_HEIGHT * 2 + BUTTON_SPACING , 78, BUTTON_HEIGHT - BUTTON_SPACING, 4, ILI9341_RED, 0, "Inst.");
  UiItem clear_button_ = UiItem(241, STATUS_HEIGHT + BUTTON_HEIGHT * 4 + BUTTON_SPACING , 78, BUTTON_HEIGHT - BUTTON_SPACING, 4, ILI9341_RED, 0, "Clear");

  UiItem *p_children_[23] = {
    &note_buttons_[0],
    &note_buttons_[1],
    &note_buttons_[2],
    &note_buttons_[3],
    &note_buttons_[4],
    &note_buttons_[5],
    &note_buttons_[6],
    &note_buttons_[7],
    &note_buttons_[8],
    &note_buttons_[9],
    &note_buttons_[10],
    &note_buttons_[11],
    &note_buttons_[12],
    &note_buttons_[13],
    &note_buttons_[14],
    &note_buttons_[15],
    &play_button_,
    &channel_button_,
    &pattern_button_,
    &settings_button_,
    &keyboard_button_,
    &instrument_button_,
    &clear_button_
  };

public:
  HomeScreen(void);
  void Setup(void);
  void Draw(boolean force_refresh = false);
  bool OnTouch(uint16_t touch_x, uint16_t touch_y);

};

class KeysScreen : public UiScreen {
protected:

  // These should get optimized to inline constants.  TODO: check this.
  const uint16_t num_children_ = sizeof(p_children_) / sizeof(UiItem*);
  const uint16_t keys_spacing_ = SCREEN_WIDTH / 7;
  const uint16_t keys_width_ = keys_spacing_ - BUTTON_SPACING;
  const uint16_t black_keys_height_ = (SCREEN_HEIGHT - STATUS_HEIGHT - BUTTON_HEIGHT - BUTTON_SPACING) * 0.5;
  const uint16_t black_keys_y_ = STATUS_HEIGHT + BUTTON_SPACING / 2;
  const uint16_t last_row_y_ = BUTTON_HEIGHT * 4 + STATUS_HEIGHT + BUTTON_SPACING / 2;

  UiItem key_buttons_[12] = {
    UiItem(0, 0, keys_width_, 40, 4, ILI9341_WHITE, 0, "1", &play_note, 0),
    UiItem(42, 0, keys_width_, 40, 4, ILI9341_WHITE, 0, "2", &play_note, 2),
    UiItem(84, 0, keys_width_, 40, 4, ILI9341_WHITE, 0, "3", &play_note, 4),
    UiItem(126, 0, keys_width_, 40, 4, ILI9341_WHITE, 0, "4", &play_note, 5),
    UiItem(0, 42, keys_width_, 40, 4, ILI9341_WHITE, 0, "5", &play_note, 7),
    UiItem(42, 42, keys_width_, 40, 4, ILI9341_WHITE, 0, "6", &play_note, 9),
    UiItem(84, 42, keys_width_, 40, 4, ILI9341_WHITE, 0, "7", &play_note, 11),
    UiItem(1, black_keys_y_, keys_width_, black_keys_height_, 4, ILI9341_BLACK, 0, "8", &play_note, 1),
    UiItem(1, black_keys_y_, keys_width_, black_keys_height_, 4, ILI9341_BLACK, 0, "9", &play_note, 3),
    UiItem(1, black_keys_y_, keys_width_, black_keys_height_, 4, ILI9341_BLACK, 0, "10", &play_note, 6),
    UiItem(1, black_keys_y_, keys_width_, black_keys_height_, 4, ILI9341_BLACK, 0, "11", &play_note, 8),
    UiItem(1, black_keys_y_, keys_width_, black_keys_height_, 4, ILI9341_BLACK, 0, "12", &play_note, 10)
  };
  
  UiItem home_button_ = UiItem(1, last_row_y_, 78, BUTTON_HEIGHT - BUTTON_SPACING, 4, ILI9341_RED, 0, "Home");

  UiItem *p_children_[13] = {
    &key_buttons_[0],
    &key_buttons_[1],
    &key_buttons_[2],
    &key_buttons_[3],
    &key_buttons_[4],
    &key_buttons_[5],
    &key_buttons_[6],
    &key_buttons_[7],
    &key_buttons_[8],
    &key_buttons_[9],
    &key_buttons_[10],
    &key_buttons_[11],
    &home_button_
  };

public:
  KeysScreen(void);
  void Setup(void);
  void Draw(boolean force_refresh = false);
  bool OnTouch(uint16_t touch_x, uint16_t touch_y);

};

class WaveformInstrumentSettings : public UiScreen {
protected:
  WaveformInstrument *p_inst_ = nullptr;

  char attack_buf_[10];
  char decay_buf_[10];
  char sustain_buf_[10];
  char release_buf_[10];

  UiItem attack_button_ = PlayButton(1, STATUS_HEIGHT +  BUTTON_SPACING / 2, 78, BUTTON_HEIGHT - BUTTON_SPACING, 4, ILI9341_RED, 0, attack_buf_);
  UiItem decay_button_ = UiItem(81, STATUS_HEIGHT + BUTTON_SPACING / 2, 78, BUTTON_HEIGHT - BUTTON_SPACING, 4, ILI9341_RED, 0, decay_buf_);
  UiItem sustain_button_ = UiItem(161, STATUS_HEIGHT + BUTTON_SPACING / 2, 78, BUTTON_HEIGHT - BUTTON_SPACING, 4, ILI9341_RED, 0, sustain_buf_);
  UiItem release_button_ = UiItem(241, STATUS_HEIGHT + BUTTON_SPACING / 2, 78, BUTTON_HEIGHT - BUTTON_SPACING, 4, ILI9341_RED, 0, release_buf_);  
  UiItem waveform_button_ = UiItem(241, STATUS_HEIGHT + BUTTON_SPACING + BUTTON_HEIGHT, 78, BUTTON_HEIGHT - BUTTON_SPACING, 4, ILI9341_BLUE, 0, nullptr);  
  UiItem pan_button_ = UiItem(241, STATUS_HEIGHT + BUTTON_SPACING + BUTTON_HEIGHT * 2, 78, BUTTON_HEIGHT - BUTTON_SPACING, 4, ILI9341_BLUE, 0, nullptr);  

  UiItem *p_children_[6] = {
    &attack_button_,
    &decay_button_,
    &sustain_button_,
    &release_button_,
    &waveform_button_,
    &pan_button_
  };

public:
  WaveformInstrumentSettings(void) {};
  void Setup(void);
  void Draw(boolean force_refresh);
  bool OnTouch(uint16_t touch_x, uint16_t touch_y);
  void SetInstrument(WaveformInstrument *p_inst) { p_inst_ = p_inst; };
  WaveformInstrument *GetInstrument(void) { return p_inst_; };

};

void change_screen(UiScreen *p_new);

extern WaveformInstrumentSettings wf_set;
extern KeysScreen k;
extern HomeScreen h;
extern UiScreen *current_screen;

#endif // __UI_ITEM_H__
