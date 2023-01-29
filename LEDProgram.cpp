#include <Arduino.h>
#include <FastLED.h>
#include "LEDProgram.hpp"
#include "config.hpp"

void LEDProgram::update(int tick, CRGB leds[]) {
  Serial.println(tick);
};