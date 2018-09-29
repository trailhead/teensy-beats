#include "keypad.h"
#include "beats.h"

MultiplexedKeypad::MultiplexedKeypad(uint8_t row0, uint8_t row1, uint8_t row2, uint8_t row3, uint8_t row4, uint8_t col0, uint8_t col1, uint8_t col2, uint8_t col3, uint8_t col4) {
  col_pins_[0] = col0;
  col_pins_[1] = col1;
  col_pins_[2] = col2;
  col_pins_[3] = col3;
  col_pins_[4] = col4;
  row_pins_[0] = row0;
  row_pins_[1] = row1;
  row_pins_[2] = row2;
  row_pins_[3] = row3;
  row_pins_[4] = row4;
}

void MultiplexedKeypad::scan(void) 
{
  int x, y;
  
  for(x = 0; x < NUM_BUTTON_COLS; x++) {
    pinMode(col_pins_[x], INPUT_PULLUP);
  }

  for(y = 0; y < NUM_BUTTON_ROWS; y++) {
    pinMode(row_pins_[y], INPUT_PULLUP);

  }

  long t = millis();

  delayMicroseconds(16);
  delayMicroseconds(16);
  delayMicroseconds(16);

  for(y = 0; y < NUM_BUTTON_ROWS; y++) {
    pinMode(row_pins_[y], OUTPUT);
    digitalWrite(row_pins_[y], LOW);

    for(x = 0; x < NUM_BUTTON_COLS; x++) {

      pinMode(col_pins_[x], INPUT_PULLUP);
      if (last_press_[y][x] < t) {
        last_press_[y][x] = 0;
      }
  
      delayMicroseconds(16);
      delayMicroseconds(16);
      delayMicroseconds(16);

      if (digitalRead(col_pins_[x]) == LOW && last_press_[y][x] == 0) {
        last_press_[y][x] = t + BUTTON_DEBOUNCE;
        // Do callback here
        if (callbacks_[y][x] != nullptr) {
          (callbacks_[y][x])(x, y);
        }
      }
      pinMode(col_pins_[x], INPUT_PULLUP);

    }
    pinMode(row_pins_[y], INPUT_PULLUP);
  } 
}

void MultiplexedKeypad::setCallback(uint8_t x, uint8_t y, void (*callback)(uint8_t, uint8_t)) {
  callbacks_[y][x] = callback;
}

void MultiplexedKeypad::clearCallback(uint8_t x, uint8_t y) {
  callbacks_[y][x] = nullptr;
}

void MultiplexedKeypad::clearAllCallbacks(void) {
  for (uint8_t x = 0; x < NUMCOLS; x++) {
    for (uint8_t y = 0; y < NUMROWS; y++) {
      clearCallback(x, y);
    }
  }
}

