#include <iostream>
#include <unistd.h>

#define FPS 5

void print_leds(size_t len, uint8_t *leds) {
  for (int i = 0; i < len; i++) {
    printf("%d", leds[i]);
  }
  std::cout << "\n";
}

int main() {
  uint8_t leds[64];

  for (int i = 0; i < 64; i++) {
    leds[i] = 0; 
  }

  for (int tick = 0; tick < 10; tick++) {

    print_leds(sizeof(leds) / sizeof(*leds), leds);

    usleep(1000000 / FPS);
  }


  return 0;
}
