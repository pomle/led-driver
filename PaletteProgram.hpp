#include <FastLED.h>
#include "LEDProgram.hpp"
#include "config.hpp"

class PaletteProgram : public LEDProgram {
private:
  CRGBPalette16 palette;
  TBlendType blending;

public:
  PaletteProgram(CRGBPalette16 p);

  void update(const PlayContext& context, CRGB leds[]);
};