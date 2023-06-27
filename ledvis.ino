#include <FastLED.h>

// send debug messages over serial
//#define DEBUG

#define LED_PIN 2
#define N_LEDS 180
//#define N_LEDS_TEST 10
#define BAUDRATE 115200

CRGB leds[N_LEDS];
uint8_t incomingBytes[3*N_LEDS];
static int incomingCounter = 0;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, N_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 300);
  FastLED.clear();
  FastLED.show();
  Serial.begin(BAUDRATE);
}

void loop() {

  /*
   * while there is serial data and the incoming data belongs
   * to the current packet (counter < 3*NUM_LEDS), read serial
   * into incomingBytes
   */
  while (Serial.available() > 0 && incomingCounter < 3*N_LEDS) {
    incomingBytes[incomingCounter] = Serial.read();
    incomingCounter++;
  }

  /*
   * if we read the full incomingBytes array, set new colors
   * for the led strip
   */
  if (incomingCounter == 3*N_LEDS) {

    #ifdef DEBUG
      for (int i=0; i<3*N_LEDS; i++) {
        Serial.print(incomingBytes[i]);
        Serial.print(",");
      }
      Serial.print("; counter: ");
      Serial.println(incomingCounter);
    #endif

    // reset index
    incomingCounter = 0;
    // set colors of LEDs
    for (int i=0; i<N_LEDS; i++) {
      leds[i] = CRGB(incomingBytes[i*3], incomingBytes[i*3+1], incomingBytes[i*3+2]);
    }
    
    // this must be placed *inside* the if-block, otherwise
    // serial reading is messed up
    FastLED.show();
  }
}
