#include <FastLED.h>

struct PlayContext {
  bool playback = false;
  word intensity = 0;
  char direction = 1;
  unsigned long tick = 0;
  TBlendType blending = LINEARBLEND;
};
