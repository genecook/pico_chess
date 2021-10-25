// dude...

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/float.h"
#include "pico/multicore.h"

#define FLAG_VALUE 123

void core1_main() {
  // let other core know this core is active, verify
  // other core as well...
  
  multicore_fifo_push_blocking(FLAG_VALUE);
  
  uint32_t g = multicore_fifo_pop_blocking();
  if (g == FLAG_VALUE) {
    multicore_fifo_push_blocking(FLAG_VALUE);
  } else {
    printf("MULTICORE PROBLEMS DETECTED - CORE 1 ???\n");
  }

  printf("Core 1 active...\n");
  
  // both cores active and okay to proceed...

  while(1) {
    uint32_t c = multicore_fifo_pop_blocking();
    printf("c: '%c'\n",c);
  }
  
}

int main() {
  stdio_init_all();
  printf("Starting...\n");

  // launch other core, verify its active...
  
  multicore_launch_core1(core1_main);
  
  uint32_t g = multicore_fifo_pop_blocking();
  if (g == FLAG_VALUE) {
    multicore_fifo_push_blocking(FLAG_VALUE);
  } else {
    printf("MULTICORE PROBLEMS DETECTED - CORE 0 ???\n");
  }
  
  printf("Core 0 active...\n");
  
  // both cores active and okay to proceed...
  
  while(true) {
    printf("Hello Dude!\n");
    int c = getchar();
    multicore_fifo_push_blocking(c);
    sleep_ms(1000);
  }
  
  return 0;
}
