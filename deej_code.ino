/*
  Complete 8-Channel Audio Controller with WS2812B LED Chain
  
  LED Index Mapping:
    - LED [0..7]  : Channel Mute Status (Ch 1 = LED 0, Ch 8 = LED 7)
    - LED [8]     : Headset / Speaker Status
    - LED [9]     : Profile Indicator
    - LED [10]    : Panic Status
*/

#include <FastLED.h>

// --- HARDWARE DEFINITIONS ---
#define LED_PIN       13
#define NUM_LEDS      11
#define BRIGHTNESS    40     // Range: 0 (off) to 255 (full bright)
#define LED_TYPE      WS2812B
#define COLOR_ORDER   GRB

CRGB leds[NUM_LEDS];

// Sliders (A0 - A7)
const int NUM_SLIDERS = 8;
const int sliderPins[NUM_SLIDERS] = {A0, A1, A2, A3, A4, A5, A6, A7};
int sliderValues[NUM_SLIDERS];

// Channel Mute Buttons (D2 - D9)
const int mutePins[NUM_SLIDERS] = {2, 3, 4, 5, 6, 7, 8, 9};
bool channelMutes[NUM_SLIDERS]  = {false};

// Top Utility Buttons (D10, D11, D12)
const int BTN_HEADSET = 10;
const int BTN_PROFILE = 11;
const int BTN_PANIC   = 12;

// Utility States
bool headsetOutput = true;   // true = Headset (Green), false = Speaker (Blue)
int activeProfile   = 0;      // 0 = Default (Cyan), 1 = Gaming (Magenta), 2 = Stream (Yellow)
bool panicActive    = false;  // Panic Mute All

// Non-blocking Debounce Structure
const unsigned long DEBOUNCE_DELAY = 45;

struct Button {
  int pin;
  bool lastReading;
  bool stableState;
  unsigned long lastDebounceTime;
};

Button muteBtns[NUM_SLIDERS];
Button btnHeadset = {BTN_HEADSET, HIGH, HIGH, 0};
Button btnProfile = {BTN_PROFILE, HIGH, HIGH, 0};
Button btnPanic   = {BTN_PANIC,   HIGH, HIGH, 0};

// Helper: Read debounced button press (falling edge: HIGH -> LOW)
bool isPressed(Button &btn) {
  bool reading = digitalRead(btn.pin);

  if (reading != btn.lastReading) {
    btn.lastDebounceTime = millis();
  }
  btn.lastReading = reading;

  if ((millis() - btn.lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != btn.stableState) {
      btn.stableState = reading;
      if (btn.stableState == LOW) {
        return true;
      }
    }
  }
  return false;
}

void setup() {
  Serial.begin(9600); // deej baud rate

  // Initialize FastLED
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  // Configure Channel Mute Buttons
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(mutePins[i], INPUT_PULLUP);
    muteBtns[i] = {mutePins[i], HIGH, HIGH, 0};
  }

  // Configure Utility Buttons
  pinMode(BTN_HEADSET, INPUT_PULLUP);
  pinMode(BTN_PROFILE, INPUT_PULLUP);
  pinMode(BTN_PANIC,   INPUT_PULLUP);

  updateLEDs();
}

void loop() {
  // --- 1. HANDLE CHANNEL MUTE BUTTONS ---
  for (int i = 0; i < NUM_SLIDERS; i++) {
    if (isPressed(muteBtns[i])) {
      channelMutes[i] = !channelMutes[i];
      updateLEDs();
    }
  }

  // --- 2. HANDLE UTILITY BUTTONS ---
  // Headset / Speaker Toggle (D10)
  if (isPressed(btnHeadset)) {
    headsetOutput = !headsetOutput;
    updateLEDs();
  }

  // Profile Switch (D11) - Cycles through 3 profiles
  if (isPressed(btnProfile)) {
    activeProfile = (activeProfile + 1) % 3;
    updateLEDs();
  }

  // Panic Button (D12) - Toggles Master Panic Mute
  if (isPressed(btnPanic)) {
    panicActive = !panicActive;
    updateLEDs();
  }

  // --- 3. READ SLIDERS & BUILD DEEJ SERIAL STREAM ---
  String serialOutput = "";
  for (int i = 0; i < NUM_SLIDERS; i++) {
    int potValue = analogRead(sliderPins[i]);

    // Force output to 0 if channel is individually muted or Panic is active
    if (channelMutes[i] || panicActive) {
      potValue = 0;
    }

    serialOutput += String(potValue);
    if (i < NUM_SLIDERS - 1) {
      serialOutput += "|";
    }
  }

  // Send formatted values: "1023|512|0|1023|0|250|1023|1023"
  Serial.println(serialOutput);

  delay(10); // Sampling interval
}

// --- LED UPDATER FUNCTION ---
void updateLEDs() {
  // 1. Channel LEDs [0..7]
  for (int i = 0; i < NUM_SLIDERS; i++) {
    if (panicActive) {
      leds[i] = CRGB::Red; // Blink/Solid Red under Panic
    } else if (channelMutes[i]) {
      leds[i] = CRGB::OrangeRed; // Channel Muted
    } else {
      leds[i] = CRGB::Green;     // Active Channel
    }
  }

  // 2. Headset/Speaker LED [8]
  leds[8] = headsetOutput ? CRGB::ForestGreen : CRGB::DeepSkyBlue;

  // 3. Profile LED [9]
  switch (activeProfile) {
    case 0: leds[9] = CRGB::Cyan;    break; // Profile 1
    case 1: leds[9] = CRGB::Magenta; break; // Profile 2
    case 2: leds[9] = CRGB::Yellow;  break; // Profile 3
  }

  // 4. Panic LED [10]
  leds[10] = panicActive ? CRGB::Red : CRGB::DimGray;

  FastLED.show();
}