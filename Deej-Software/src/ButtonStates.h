#ifndef BUTTON_STATES_H
#define BUTTON_STATES_H

enum SliderButtonState {
  UNMUTED,
  MUTED,
  PANIC
};

enum ProfileButtonState {
  PROFILE0,
  PROFILE1,
  PROFILE2
};

enum OutputSelectionButtonState {
  HEADSET,
  SPEAKER
};

enum PanicButtonState {
  ACTIVE,
  INACTIVE
};

#endif