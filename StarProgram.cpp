#include <Arduino.h>
#include <FastLED.h>
#include "StarProgram.hpp"
#include "config.hpp"

StarProgram::StarProgram() {
  for (int i = 0; i < NUM_LEDS; i++) {
    state[i] = 0; 
  }
};

void StarProgram::update(const unsigned long& tick, CRGB leds[]) {
  uint8_t index = 0;
  index = NUM_LEDS + 1;

  if (tick % 10000 == 0) {
    index = rand() % NUM_LEDS;
  }

  if (index < NUM_LEDS && state[index] == 0) {
    state[index] = 1;
  }
  
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t intensity = state[i];
    if (intensity > 0) {
      intensity += intensity % 2 == 0 ? -2 : 2;
      if (intensity > 253) {
        intensity -= 1;
      }

      leds[i][0] = intensity;
      leds[i][1] = intensity;
      leds[i][2] = intensity;
    }
    state[i] = intensity;
  }
};