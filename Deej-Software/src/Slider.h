#ifndef SLIDER_H
#define SLIDER_H

#include "DebouncedButton.h"
#include "ButtonStates.h"

class Slider {
  DebouncedSliderButton button;
  unsigned int pin;

  public:
  Slider(unsigned int sliderPin, unsigned int buttonPin, CRGB* led): button(buttonPin, led), pin(sliderPin) {
    pinMode(pin, INPUT);
  }

  void update(bool isPanicActive) {
    button.update(isPanicActive);
  }

  int read() {
    return button.state == MUTED || button.state == PANIC ? 0 : analogRead(pin);
  }
};

#endif