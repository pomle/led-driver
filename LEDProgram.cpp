#include <Arduino.h>
#include <FastLED.h>
#include "LEDProgram.hpp"
#include "config.hpp"

void LEDProgram::update(const unsigned long& tick, CRGB leds[]) {
  Serial.println("LEDProgram");
};