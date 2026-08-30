/* #include <FastLED.h>

// --- KONFIGURATION LEDs ---
#define LED_PIN     13       // Daten-Pin für die LEDs (ggf. D10 nutzen, falls D13 flackert)
#define NUM_LEDS    8        // Anzahl der LEDs (LED 0 bis 7)
#define LED_TYPE    WS2812B  // LED-Typ
#define COLOR_ORDER GRB      // Farbreihenfolge
#define BRIGHTNESS  50       // Helligkeit (0-255)

// --- KONFIGURATION TASTER ---
const int FIRST_PIN = 2;     // D2
const int LAST_PIN  = 9;     // D9

CRGB leds[NUM_LEDS];
bool lastButtonState[NUM_LEDS];
bool ledActive[NUM_LEDS];        

void setup() {
  Serial.begin(9600);

  // FastLED Initialisierung
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  
  // Alle LEDs starten AN (Rot)
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
  }
  FastLED.show();

  // Pins D2 bis D9 konfigurieren
  for (int i = 0; i < NUM_LEDS; i++) {
    int pin = FIRST_PIN + i;
    pinMode(pin, INPUT_PULLUP);
    lastButtonState[i] = HIGH;
    ledActive[i]       = true; // Startzustand: Rot
  }

  Serial.println("==========================================");
  Serial.println("Deej-Test (Invertierte Pin-Zuordnung):");
  Serial.println("D2 -> LED 7 | D9 -> LED 0");
  Serial.println("==========================================");
}

void loop() {
  for (int i = 0; i < NUM_LEDS; i++) {
    int pin = FIRST_PIN + i;
    
    // Umgekehrte LED-Index-Berechnung:
    // Wenn i = 0 (Pin D2)  -> ledIndex = 7 - 0 = 7 (LED 7)
    // Wenn i = 7 (Pin D9)  -> ledIndex = 7 - 7 = 0 (LED 0)
    int ledIndex = (NUM_LEDS - 1) - i;

    bool currentState = digitalRead(pin);

    // Flankenerkennung: Taster wurde NEU gedrückt
    if (currentState == LOW && lastButtonState[i] == HIGH) {
      
      // Zustand für diese spezielle LED umkehren
      ledActive[ledIndex] = !ledActive[ledIndex];

      if (ledActive[ledIndex]) {
        leds[ledIndex] = CRGB::Red;   // Wieder AN (Rot)
        Serial.print("Taster D");
        Serial.print(pin);
        Serial.print(" -> LED ");
        Serial.print(ledIndex);
        Serial.println(" Rot (AN)");
      } else {
        leds[ledIndex] = CRGB::Black; // AUS
        Serial.print("Taster D");
        Serial.print(pin);
        Serial.print(" -> LED ");
        Serial.print(ledIndex);
        Serial.println(" AUS");
      }

      FastLED.show();
      delay(50); // Entprellung
    }

    lastButtonState[i] = currentState;
  }
} */