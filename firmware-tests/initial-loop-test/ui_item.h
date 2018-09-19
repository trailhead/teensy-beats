#ifndef __UI_ITEM_H__
#define __UI_ITEM_H__

#include <stdint.h>
#include "tft.h"

extern int tick;
extern int current_inst;
extern uint8_t notes[4][4][4];
extern boolean playing;
extern float bpm;

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
  char *text_;

  void (*on_touch_func_)(void);

public:
  UiItem(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t radius, uint16_t bg_color, uint16_t text_color, char *text) :
  x_(x),
  y_(y),
  w_(w),
  h_(h),
  radius_(radius),
  bg_color_(bg_color),
  text_color_(text_color),
  text_(text) {
  }

  virtual bool HitTest(uint16_t touch_x, uint16_t touch_y);
  virtual void Draw(void);
};

class UiScreen {
protected:
  UiItem *children_[];
  UiScreen() {};

public:
  virtual void Draw(void) = 0;
};

class PlayButton : public UiItem {
private:
  char bpm_buf_[7];

public:
  PlayButton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t radius, uint16_t bg_color, uint16_t text_color, char *text) :
  UiItem(x, y, w, h, radius, bg_color, text_color, text) {    
  }
  void Draw(void);
};

class HomeScreen : public UiScreen {
protected:
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
  PlayButton play_button_ = PlayButton(1, 193, 78, 48, 4, ILI9341_RED, 0, nullptr);

public:
  HomeScreen(void);
  void Draw(void);
};


#endif // __UI_ITEM_H__
