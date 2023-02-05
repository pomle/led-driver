#include <FastLED.h>
#include "LEDProgram.hpp"
#include "config.hpp"

class PaletteProgram : public LEDProgram {
private:
  CRGBPalette16 palette;
  TBlendType blending;
  word offset = 0;

public:
  PaletteProgram(CRGBPalette16 p);

  void update(const PlayContext& context, CRGB leds[]);
};