#include <FastLED.h>
#include "KITTProgram.hpp"
#import "config.hpp"

KITTProgram::KITTProgram() {
  
};

void KITTProgram::update(const PlayContext& context, CRGB leds[]) {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (position == i) {
      leds[i][0] = 255; 
      leds[i][1] = 0; 
      leds[i][2] = 0; 
    } else if (leds[i][0] > 0) {
      leds[i][0] = constrain(leds[i][0] - 8, 0, 255);
    }
  }

  if (velocity > 500) {
    position += direction;
    velocity = 0;
  }

  velocity += context.intensity;

  if (position <= 0 && direction < 0) {
    direction = 1;
  } else if (position >= NUM_LEDS - 1 && direction > 0) {
    direction = -1;
  }
};