#include <FastLED.h>
#include "config.hpp"
#include "LEDProgram.hpp"
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



CRGBPalette16 currentPalette;
TBlendType currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

// This function fills the palette with totally random colors.
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
char direction = 1;
unsigned long tick = 0;

LEDProgram* program;

void setup() {
  pinMode(PLAY_BUTTON, INPUT);
  pinMode(COLOR_BUTTON, INPUT);

  delay(2000);  // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(128);

  Serial.begin(9600);

  program = programs[0];

  currentBlending = LINEARBLEND;
}

void loop() {
  ToggleRoutine();

  HandleBrightness();

  int speed = 100;
  tick += speed * 2 * direction;
  program->update(tick, leds);

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
      direction *= -1;
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

int clamp(int value, int min, int max) {
  if (value > max) {
    return max;
  }
  if (value < min) {
    return min;
  }
  return value;
}

bool PlayRoutine() {
  static bool playState = false;
  static unsigned int playHits = 0;

  if (digitalRead(PLAY_BUTTON) == HIGH) {
    playHits++;
  } else {
    playHits = 0;
  }

  if (playHits == 1) {
    playState = !playState;
  }

  return playState;
}

void ToggleProgram() {
  int len = sizeof(programs) / sizeof(programs[0]);
  programIndex = (programIndex + 1) % len;
  program = programs[programIndex];
}

void ToggleBlending() {
  if (currentBlending == LINEARBLEND) {
    currentBlending = NOBLEND;
  } else {
    currentBlending = LINEARBLEND;
  }
}

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM = {
  CRGB::Red,
  CRGB::Gray,  // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};



// Additionl notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.