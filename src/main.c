// #include <stdio.h>
#include "../include/float8.h"

int main() {
  const float floats[] = {
      0.001f, -20, 55.8f,         // 0 000 0000, 1 111 0000, 0 111 0000
      15.5f, 15.49f, 14.9f,       // 0 110 1111, 0 110 1110, 0 110 1101
      0.25f, -0.296875, 0.304688, // 0 001 0000, 1 001 0011, 0 001 0011
      // Denormalized
      0.234375f, 0.015625f, 0.187500f // 0 000 1111, 0 000 0001, 0 000 1100
  };
  int length = sizeof(floats) / sizeof(floats[0]);
  process_floats(floats, length);

  return 0;
}