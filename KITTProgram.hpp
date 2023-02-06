#include <FastLED.h>
#include "LEDProgram.hpp"
#include "config.hpp"

class KITTProgram : public LEDProgram {
private:
  byte position = 0;
  word velocity = 0;
  char direction = 1;

public:
  KITTProgram();

  void update(const PlayContext& context, CRGB leds[]);
};