#include <FastLED.h>

struct PlayContext {
  bool playback = false;
  int speed = 0;  
  char direction = 1;
  unsigned long tick = 0;
  TBlendType blending = LINEARBLEND;
};
