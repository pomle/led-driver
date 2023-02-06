#include <FastLED.h>
#include <EEPROM.h>
#import "context.hpp"
#include "config.hpp"
#import "LEDProgram.hpp"
#include "StarProgram.hpp"
#include "PaletteProgram.hpp"

CRGB leds[NUM_LEDS];


// This example shows several ways to set up and use 'palettes' of colors
// with FastLED.
//
// These compact palettes provide an easy way to re-colorize your
// animation on the fly, quickly, easily, and with low overhead.
//
// USING palettes is MUCH simpler in practice than in theory, so first just
// run this sketch, and watch the pretty lights as you then read through
// the code.  Although this sketch has eight (or more) different color schemes,
// the entire sketch compiles down to about 6.5K on AVR.
//
// FastLED provides a few pre-configured color palettes, and makes it
// extremely easy to make up your own color schemes with palettes.
//
// Some notes on the more abstract 'theory and practice' of
// FastLED compact palettes are at the bottom of this file.


CRGBPalette16 makeRandomPalette() {
  CRGBPalette16 p;
  for (int i = 0; i < 16; i++) {
    p[i] = CHSV(random8(), 255, 255);
  }
  return p;
}

CRGBPalette16 makeWhiteFlashPalette() {
  CRGBPalette16 palette;
  fill_solid(palette, 16, CRGB::Black);
  palette[15] = CRGB::White;
  return palette;
}

CRGBPalette16 makeBlackAndWhiteStripedPalette() {
  CRGBPalette16 palette;
  // 'black out' all 16 palette entries...
  fill_solid(palette, 16, CRGB::Black);
  // and set every fourth one to white.
  palette[0] = CRGB::White;
  palette[4] = CRGB::White;
  palette[8] = CRGB::White;
  palette[12] = CRGB::White;

  return palette;
}

auto randoms = PaletteProgram(makeRandomPalette());
auto rainbow = PaletteProgram(RainbowColors_p);
auto rainbowStripe = PaletteProgram(RainbowStripeColors_p);
auto cloud = PaletteProgram(CloudColors_p);
auto party = PaletteProgram(PartyColors_p);
auto ocean = PaletteProgram(OceanColors_p);
auto lava = PaletteProgram(LavaColors_p);
auto forest = PaletteProgram(ForestColors_p);
auto sweden = PaletteProgram({
  CRGB::Yellow, CRGB::Yellow, CRGB::Yellow, CRGB::Black,
  CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Black,
  CRGB::Yellow, CRGB::Yellow, CRGB::Yellow, CRGB::Black,
  CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Black,
});
auto black_white = PaletteProgram(makeBlackAndWhiteStripedPalette());
auto flash = PaletteProgram(makeWhiteFlashPalette());

auto star = StarProgram();

LEDProgram* programs[] = {
  &randoms,
  &rainbow,
  &rainbowStripe,
  &cloud,
  &party,
  &ocean,
  &lava,
  &forest,
  &sweden,
  &black_white,
  &flash,
  &star,
};

byte programIndex = 0;

PlayContext context;

LEDProgram* program;

void setup() {
  pinMode(PLAY_BUTTON, INPUT);
  pinMode(COLOR_BUTTON, INPUT);

  EEPROM.get(PROGRAM_INDEX_ADDRESS, programIndex);
  EEPROM.get(DIRECTION_ADDRESS, context.direction);

  delay(2000);  // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  Serial.begin(9600);

  program = programs[programIndex];
}

void loop() {
  if (!PlayRoutine()) {
    FastLED.clear(true);
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
    return;
  }
  
  ToggleRoutine();

  HandleBrightness();

  context.speed = constrain(analogRead(SPEED_PIN) - 50, 0, 1000);
  context.tick += 1;
  program->update(context, leds);

  FastLED.show();

  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void HandleBrightness() {
  int brightness = 0;
  for (int i=0; i< 16; i++) brightness += analogRead(BRIGHTNESS_PIN);
  brightness /= 16;

  brightness = map(
    brightness - 10,
    0, 1000,
    4, MAX_BRIGHTNESS);
  
  FastLED.setBrightness(brightness);
}

void ToggleRoutine() {
  static unsigned int toggleHits = 0;
  if (digitalRead(COLOR_BUTTON) == HIGH) {
    toggleHits++;
    if (toggleHits == 50) {
      ToggleBlending();
    }
    if (toggleHits == 100) {
      // Toggle back blending if we reached 100 ticks
      ToggleBlending();
      context.direction *= -1;
      EEPROM.put(DIRECTION_ADDRESS, context.direction);
    }
  } else {
    if (toggleHits > 0) {
      if (toggleHits < 50) {
        ToggleProgram();
      }
    }
    toggleHits = 0;
  }
}

bool PlayRoutine() {
  static unsigned int playHits = 0;

  if (digitalRead(PLAY_BUTTON) == HIGH) {
    playHits++;
  } else {
    playHits = 0;
  }

  if (playHits == 1) {
    context.playback = !context.playback;
  }

  return context.playback;
}

void ToggleProgram() {
  int len = sizeof(programs) / sizeof(programs[0]);
  programIndex = (programIndex + 1) % len;
  program = programs[programIndex];
  EEPROM.put(PROGRAM_INDEX_ADDRESS, programIndex);
}

void ToggleBlending() {
  if (context.blending == LINEARBLEND) {
    context.blending = NOBLEND;
  } else {
    context.blending = LINEARBLEND;
  }
}
