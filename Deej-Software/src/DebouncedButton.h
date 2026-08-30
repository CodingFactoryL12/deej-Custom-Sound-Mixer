#ifndef DEBOUNCED_BUTTON_H
#define DEBOUNCED_BUTTON_H

#include "LED.h"
#include "Arduino.h"
#include "ButtonStates.h"
#include "Config.h"
#include <FastLED.h>

// Non-blocking Debounce Structure
#define DEBOUNCE_DELAY_MS 50

class DebouncedButton {
  unsigned int pin;
  bool lastReading = HIGH; // not debounced
  bool debouncedState = HIGH;
  unsigned long lastDebounceTime = 0;
  CRGB* led;

  public:

  DebouncedButton(unsigned int pin, CRGB *led) : pin(pin), led(led) {
    pinMode(pin, INPUT_PULLUP);
  }

  bool justPressed() {
    bool wasPressed = !debouncedState;
    bool nowPressed = isPressed();
    return !wasPressed && nowPressed;
  }

  bool isPressed() {
    update();
    return !debouncedState;
  }

  private:

  void update() {
    bool reading = digitalRead(pin);

    if (reading != lastReading) {
      lastDebounceTime = millis();
      lastReading = reading;
    }

    if (millis() - lastDebounceTime > DEBOUNCE_DELAY_MS) {
      debouncedState = reading;
    }
  }
};

class DebouncedSliderButton : public DebouncedButton {
  public:
  SliderButtonState state = UNMUTED;
  SliderButtonLED led;

  DebouncedSliderButton(unsigned int pin, CRGB* led)
      : DebouncedButton(pin, led), led(led) {
  }

  void update(bool isPanicActive) {
    if(justPressed()) {
      Serial.print("Button pressed. Old State: ");
      Serial.print(state);
      switch(state) {
        case UNMUTED: state = MUTED; break;
        case MUTED: state = UNMUTED; break;
        default: break;
      }
      Serial.print(", new state: ");
      Serial.print(state);
    }

    if(isPanicActive) {
      state = PANIC;
    } else if(state == PANIC) {
      state = UNMUTED;
    }

    Serial.print(", state after panic check: ");
    Serial.println(state);

    led.updateColor(state);
  }
};

class DebouncedProfileButton: public DebouncedButton {
  public:
  ProfileButtonState state = PROFILE0;
  ProfileButtonLED led;

  DebouncedProfileButton(unsigned int pin, CRGB* led)
    : DebouncedButton(pin, led), led(led) {
  }

  void update() {
    if(justPressed()) {
      switch(state) {
        case PROFILE0: state = PROFILE1; break;
        case PROFILE1: state = PROFILE2; break;
        case PROFILE2: state = PROFILE0; break;
      }
    }

    led.updateColor(state);
  }
};

class DebouncedOutputSelectionButton: public DebouncedButton {
  public:
  OutputSelectionButtonState state = DEFAULT_OUTPUT;
  OutputSelectionButtonLED led;

  DebouncedOutputSelectionButton(unsigned int pin, CRGB* led)
    : DebouncedButton(pin, led), led(led) {
  }

  void update() {
    if(justPressed()) {
      switch(state) {
        case HEADSET: state = SPEAKER; break;
        case SPEAKER: state = HEADSET; break;
      }
    }

    led.updateColor(state);
  }
};

class DebouncedPanicButton: public DebouncedButton {
  public:
  PanicButtonState state = INACTIVE;
  PanicButtonLED led;

  DebouncedPanicButton(unsigned int pin, CRGB* led)
    : DebouncedButton(pin, led), led(led) {
  }

  void update() {
    if(justPressed()) {
      switch(state) {
        case ACTIVE: state = INACTIVE; break;
        case INACTIVE: state = ACTIVE; break;
      }
    }

    led.updateColor(state);
  }
};



#endif