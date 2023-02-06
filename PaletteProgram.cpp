#include <FastLED.h>
#include "PaletteProgram.hpp"
#include "config.hpp"

PaletteProgram::PaletteProgram(CRGBPalette16 p) {
  palette = p;
};

void PaletteProgram::update(const PlayContext& context, CRGB leds[]) {
  uint8_t colorIndex = offset / 256;
  uint8_t brightness = 255;

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(
      palette, 
      colorIndex, 
      brightness, 
      context.blending);
    colorIndex += 3;
  }

  offset += context.speed * 2 * context.direction;
};