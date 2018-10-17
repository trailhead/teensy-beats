#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include "beats.h"

class MultiplexedKeypad {
  private:
    uint8_t row_pins_[NUM_BUTTON_ROWS] = { 28, 29, 30, 31, 27 };
    uint8_t col_pins_[NUM_BUTTON_COLS] = { 35, 36, 37, 38, 39 };

    unsigned long last_press_[NUM_BUTTON_ROWS][NUM_BUTTON_COLS];

    void (*callbacks_[NUM_BUTTON_ROWS][NUM_BUTTON_COLS])(uint8_t, uint8_t);

  public:
    MultiplexedKeypad(uint8_t row0, uint8_t row1, uint8_t row2, uint8_t row3, uint8_t row4, uint8_t col0, uint8_t col1, uint8_t col2, uint8_t col3, uint8_t col4);
    void scan(void);
    void setCallback(uint8_t x, uint8_t y, void (*callback)(uint8_t, uint8_t));
    void clearCallback(uint8_t x, uint8_t y);
    void clearAllCallbacks(void);

};
#endif // __KEYPAD_H__
