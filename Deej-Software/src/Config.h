#ifndef CONFIG_H
#define CONFIG_H

#include "ButtonStates.h"

#define DEFAULT_OUTPUT OutputSelectionButtonState::HEADSET
#define BAUD_RATE 115200

#define BRIGHTNESS    200      // Range: 0 (off) to 255 (full bright)
#define LED_TYPE      WS2812B
#define COLOR_ORDER   GRB

#endif