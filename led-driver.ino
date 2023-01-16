#include <FastLED.h>

#define PLAY_BUTTON 2
#define COLOR_BUTTON 3

#define LED_PIN 5
#define NUM_LEDS 64
#define BRIGHTNESS 64
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

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

bool playState = false;

void setup() {
  Serial.begin(9600);

  pinMode(PLAY_BUTTON, INPUT);
  pinMode(COLOR_BUTTON, INPUT);

  delay(3000);  // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}

void loop() {
  static unsigned int toggleHits = 0;
  if (digitalRead(COLOR_BUTTON) == HIGH) {
    toggleHits++;
    if (toggleHits == 50) {
      ToggleBlending();
    }
  } else {
    if (toggleHits > 0) {
      if (toggleHits < 50) {
        TogglePalette();
      }
    }
    toggleHits = 0;
  }

  static unsigned int playHits = 0;
  if (digitalRead(PLAY_BUTTON) == HIGH) {
    playHits++;
  } else {
    playHits = 0;
  }

  if (playHits == 1) {
    playState = !playState;
  }

  if (!playState) {
    FastLED.clear(true);
    delay(1000 / UPDATES_PER_SECOND);
    return;
  }

  int brightness = analogRead(A0);
  FastLED.setBrightness(max(8, brightness / 4));

  static word moveOffset = 0;
  moveOffset += max(0, analogRead(A1) - 40);  // 0-1023
  Serial.println(max(0, analogRead(A1) - 40));

  static uint8_t startIndex = 0;
  startIndex = moveOffset / 256;
  FillLEDsFromPaletteColors(startIndex);  // 0-255

  FastLED.show();

  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors(uint8_t colorIndex) {
  uint8_t brightness = 255;

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}

int paletteIndex = 0;

CRGBPalette16 palettes[] = {
  RainbowColors_p,
  RainbowStripeColors_p,
  CloudColors_p,
  PartyColors_p,
  OceanColors_p,
  LavaColors_p,
  ForestColors_p,
  SetupSwedenPalette(),
  SetupBlackAndWhiteStripedPalette(),
  SetupTotallyRandomPalette(),
  SetupPurpleAndGreenPalette(),
  SetupWhiteFlashPalette(),
};

void TogglePalette() {
  paletteIndex++;
  int len = sizeof(palettes) / sizeof(palettes[0]);
  int p = paletteIndex % len;
  currentPalette = palettes[p];
}

void ToggleBlending() {
  if (currentBlending == LINEARBLEND) {
    currentBlending = NOBLEND;
  } else {
    currentBlending = LINEARBLEND;
  }
}

// This function fills the palette with totally random colors.
CRGBPalette16 SetupTotallyRandomPalette() {
  CRGBPalette16 p;
  for (int i = 0; i < 16; i++) {
    p[i] = CHSV(random8(), 255, 255);
  }
  return p;
}

CRGBPalette16 SetupWhiteFlashPalette() {
  CRGBPalette16 palette;
  fill_solid(palette, 16, CRGB::Black);
  palette[0] = CRGB::White;
  return palette;
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
CRGBPalette16 SetupBlackAndWhiteStripedPalette() {
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


// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
CRGBPalette16 SetupSwedenPalette() {
  CRGBPalette16 palette;
  for (int i = 0; i < 16; i++) {
    palette[i] = i % 2 == 0 ? CRGB::Yellow : CRGB::Blue;
  }
  return palette;
}

// This function sets up a palette of purple and green stripes.
CRGBPalette16 SetupPurpleAndGreenPalette() {
  CRGB purple = CHSV(HUE_PURPLE, 255, 255);
  CRGB green = CHSV(HUE_GREEN, 255, 255);
  CRGB black = CRGB::Black;

  CRGBPalette16 p = CRGBPalette16(
    green, green, black, black,
    purple, purple, black, black,
    green, green, black, black,
    purple, purple, black, black);
  return p;
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