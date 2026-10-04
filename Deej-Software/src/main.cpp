#include <Arduino.h>
#include <FastLED.h>
#include "DebouncedButton.h"
#include "Slider.h"
#include "Config.h"
#include "ButtonStates.h"

#define NUM_LEDS      11
#define LED_PIN       10

CRGB leds[NUM_LEDS];

#define NUM_SLIDERS 8

// Top Utility Buttons
#define OUTPUT_SELECTION_BUTTON_PIN 13
#define OUTPUT_SELECTION_BUTTON_LED_INDEX 8

#define PROFILE_BUTTON_PIN 12
#define PROFILE_BUTTON_LED_INDEX 9

#define PANIC_BUTTON_PIN 11
#define PANIC_BUTTON_LED_INDEX 10

Slider sliders[NUM_SLIDERS] = {
  Slider(A0, 2, &leds[7]),
  Slider(A1, 3, &leds[6]),
  Slider(A2, 4, &leds[5]),
  Slider(A3, 5, &leds[4]),
  Slider(A4, 6, &leds[3]),
  Slider(A5, 7, &leds[2]),
  Slider(A6, 8, &leds[1]),
  Slider(A7, 9, &leds[0]),
};

DebouncedOutputSelectionButton outputSelectionButton(OUTPUT_SELECTION_BUTTON_PIN, &leds[OUTPUT_SELECTION_BUTTON_LED_INDEX]);
DebouncedProfileButton profileButton(PROFILE_BUTTON_PIN, &leds[PROFILE_BUTTON_LED_INDEX]);
DebouncedPanicButton panicButton(PANIC_BUTTON_PIN, &leds[PANIC_BUTTON_LED_INDEX]);

void setup() {
  Serial.begin(BAUD_RATE);

  // Initialize FastLED
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  outputSelectionButton.update();
  profileButton.update();
  panicButton.update();

  for (int i = 0; i < NUM_SLIDERS; i++) {
    sliders[i].update(panicButton.state == ACTIVE);
  }

  FastLED.show();

  // --- 3. READ SLIDERS & BUILD DEEJ SERIAL STREAM ---
  for (int i = 0; i < NUM_SLIDERS; i++) {
    Serial.print(sliders[i].read());

    if (i < NUM_SLIDERS - 1) {
      Serial.print("|");
    }
  }

  Serial.println();

  if(outputSelectionButton.state == OutputSelectionButtonState::SPEAKER) {
    Serial.println("WORKS 1");
  } else {
    Serial.println("WORKS 2");
  }
}