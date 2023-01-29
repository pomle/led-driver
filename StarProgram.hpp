#include <FastLED.h>
#include "LEDProgram.hpp"
#include "config.hpp"

class StarProgram : public LEDProgram {
private:
  int state[NUM_LEDS];

public:
  StarProgram();

  void update(int tick, CRGB leds[]);
};