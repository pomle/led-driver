#include <iostream>
#include <unistd.h>
#include <stdlib.h> 

#define FPS 60
#define LEDS 32

void print_leds(size_t len, uint8_t *leds) {
  for (int i = 0; i < len; i++) {
    printf("%4u", leds[i]);
  }
  std::cout << "\n";
}

int main() {
  uint8_t leds[LEDS];

  for (int i = 0; i < LEDS; i++) {
    leds[i] = 0; 
  }

  uint8_t index = 0;
  for (int tick = 0; tick < 10000; tick++) {
    index = LEDS + 1;

    if (tick % 10 == 0) {
      index = rand() % 256;
    }

    if (index < LEDS && leds[index] == 0) {
      leds[index] = 2;
    }
    
    for (int i = 0; i < LEDS; i++) {
      uint8_t intensity = leds[i];
      if (intensity > 0) {
        intensity += intensity % 2 == 0 ? 2 : -2;
        if (intensity > 253) {
          intensity -= 1;
        }
      }
      leds[i] = intensity;
    }

    print_leds(sizeof(leds) / sizeof(*leds), leds);

    usleep(1000000 / FPS);
  }


  return 0;
}
