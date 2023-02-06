#include <FastLED.h>
#include "LEDProgram.hpp"
#include "config.hpp"

class KITTProgram : public LEDProgram {
private:
  int position = 0;

public:
  KITTProgram();

  void update(const PlayContext& context, CRGB leds[]);
};