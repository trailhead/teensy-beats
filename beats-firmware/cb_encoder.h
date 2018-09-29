#ifndef __CB_ENCODER_H__
#define __CB_ENCODER_H__

#include<Encoder.h>

class CallbackEncoder : public Encoder {
private:
  void (*callback_)(boolean, long);
  long last_value_ = 0;

public:
  CallbackEncoder(uint8_t pin1, uint8_t pin2) : Encoder(pin1, pin2), callback_(nullptr) {}
  CallbackEncoder(uint8_t pin1, uint8_t pin2, void (*cb)(boolean, long)) : Encoder(pin1, pin2), callback_(cb) {}

  void update(void) {
    long value_new = read() / 2;
    boolean clockwise = false;

    if (value_new == last_value_) {
      return;
    }

    if (value_new < last_value_) {
      clockwise = true;
    } else {
      clockwise = false;
    }
    last_value_ = value_new;

    // Call the callback if there is one
    if (callback_ != nullptr) {
      (callback_)(clockwise, value_new);
    }
  }

  void setCallback(void (*cb)(boolean, long)) {
    callback_ = cb;
  }
  
  void clearCallback(void) {
    callback_ = nullptr;
  }
};

#endif // __CB_ENCODER_H__
