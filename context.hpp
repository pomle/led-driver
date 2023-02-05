#ifndef PLAY_CONTEXT
#define PLAY_CONTEXT

struct PlayContext {
  bool playback = false;
  int speed = 0;  
  char direction = 1;
  unsigned long tick = 0;
};

#endif