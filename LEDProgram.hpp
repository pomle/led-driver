#include <FastLED.h>
#import "context.hpp"

class LEDProgram {
public:
  virtual void update(const PlayContext& context, CRGB leds[]);
};
