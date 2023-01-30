#include <FastLED.h>

#ifndef LED_PROGRAM
#define LED_PROGRAM

class LEDProgram {
public:
  virtual void update(const unsigned long& tick, CRGB leds[]);
};

#endif
