#include <Arduino.h>
#include <FastLED.h>
#include "PaletteProgram.hpp"
#include "config.hpp"

PaletteProgram::PaletteProgram(CRGBPalette16 p) {
  blending = LINEARBLEND;
  palette = p;
};

void PaletteProgram::update(int tick, CRGB leds[]) {
  uint8_t colorIndex = tick / 256;
  uint8_t brightness = 255;

  Serial.println(brightness);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(
      palette, 
      colorIndex, 
      brightness, 
      blending);
    colorIndex += 3;
  }
};