#include <Arduino.h>
#include <FastLED.h>
#include "LEDProgram.hpp"
#include "config.hpp"

void LEDProgram::update(const PlayContext& context, CRGB leds[]) {
  Serial.println("LEDProgram");
};