#include <FastLED.h>
#include "LEDProgram.hpp"
#include "config.hpp"

class KITTProgram : public LEDProgram {
private:
  int position = 0;
  int velocity = 0;
  char direction = 1;

public:
  KITTProgram();

  void update(const PlayContext& context, CRGB leds[]);
};