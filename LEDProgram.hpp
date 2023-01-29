#include <FastLED.h>

#ifndef LED_PROGRAM
#define LED_PROGRAM

class LEDProgram {
public:
  void update(int tick, CRGB leds[]);
};

#endif
