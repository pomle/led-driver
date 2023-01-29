#include <FastLED.h>
#include "LEDProgram.hpp"
#include "config.hpp"

class StarProgram : public LEDProgram {
private:
  uint8_t state[NUM_LEDS];

public:
  StarProgram();

  void update(const unsigned long& tick, CRGB leds[]);
};