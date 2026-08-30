#ifndef LED_H
#define LED_H

#include <FastLED.h>
#include "ButtonStates.h"

class LED {
  protected:
  CRGB* led;

  LED(CRGB* led): led(led) { 
  }
};

class SliderButtonLED : public LED {
  public:
  SliderButtonLED(CRGB* led): LED(led) {
  }

  void updateColor(SliderButtonState currentState) {
    *led = colorOf(currentState);
  }

  private:
  CRGB colorOf(SliderButtonState state) {
    switch(state) {
      case UNMUTED:         return CRGB::Green;
      case MUTED:           return CRGB::Red;
      case PANIC:           return CRGB::Red;
    }

    return CRGB::Black;
  }
};

class ProfileButtonLED : public LED {
  public:
  ProfileButtonLED(CRGB* led): LED(led) { 
  }

  void updateColor(ProfileButtonState currentState) {
    *led = colorOf(currentState);
  }

  private:
  CRGB colorOf(ProfileButtonState state) {
    switch(state) {
      case PROFILE0:        return CRGB::Cyan;
      case PROFILE1:        return CRGB::Magenta;
      case PROFILE2:        return CRGB::Yellow;
    }

    return CRGB::Black;
  }
};

class OutputSelectionButtonLED : public LED {
  public:
  OutputSelectionButtonLED(CRGB* led): LED(led) { 
  }

  void updateColor(OutputSelectionButtonState currentState) {
    *led = colorOf(currentState);
  }

  private:
  CRGB colorOf(OutputSelectionButtonState state) {
    switch(state) {
      case HEADSET:  return CRGB::ForestGreen;
      case SPEAKER:  return CRGB::DimGray;
    }

    return CRGB::Black;
  }
};

class PanicButtonLED : public LED {
  public:

  PanicButtonLED(CRGB* led): LED(led) { 
  }
  
  void updateColor(PanicButtonState currentState) {
    *led = colorOf(currentState);
  }

  private:
  CRGB colorOf(PanicButtonState state) {
    switch(state) {
      case ACTIVE:  return CRGB::Red;
      case INACTIVE:  return CRGB::Green;
    }

    return CRGB::Black;
  }
};

#endif