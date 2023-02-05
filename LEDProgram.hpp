#include <FastLED.h>
#include "context.hpp"

#ifndef LED_PROGRAM
#define LED_PROGRAM

class LEDProgram {
public:
  virtual void update(const PlayContext& context, CRGB leds[]);
};

#endif
