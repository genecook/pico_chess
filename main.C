// dude...

#include <stdio.h>
#include <iostream>
#include <string>
#include <queue>

#include "pico/stdlib.h"
#include "pico/multicore.h"

#include <pico_chess.h>

#define FLAG_VALUE 123

//********************************************************************
// entry point, core 1...
//********************************************************************

void core1_main() {
  printf("Core 1 active...\n");
  
  // let other core know this core is active...
  
  multicore_fifo_push_blocking(FLAG_VALUE);

  // now wait for other cores response...
  
  uint32_t g = multicore_fifo_pop_blocking();
  if (g == FLAG_VALUE) {
    // have response from other core...
    //multicore_fifo_push_blocking(FLAG_VALUE);
  } else {
    printf("MULTICORE PROBLEMS DETECTED - CORE 1 ???\n");
  }

  printf("Core 0 active, detected from core 1...\n");
  
  // both cores active and okay to proceed...

  while(1) {
    uint32_t c = multicore_fifo_pop_blocking();
    printf("c: '%c'\n",c);
  }
  
}

//********************************************************************
// (main) entry point - core 0...
//********************************************************************

namespace PicoStreamPlayer {
  int Play(PicoChess::Engine *the_engine);
}

int main() {
  stdio_init_all();
  printf("Starting, core 0 active...\n");

  // launch other core, wait for it to respond...
  
  multicore_launch_core1(core1_main);
  uint32_t g = multicore_fifo_pop_blocking();

  // now send other core this cores 'response'...
  
  if (g == FLAG_VALUE) {
    multicore_fifo_push_blocking(FLAG_VALUE);
  } else {
    printf("MULTICORE PROBLEMS DETECTED - CORE 0 ???\n");
  }
  
  printf("Core 1 active, detected from core 0...\n");
  
  // both cores active and okay to proceed...

/*
  printf(">>>");
  
  std::string tbuf;
  std::cin >> tbuf;
  std::cout << "'" << tbuf << "'"<< std::endl;
  
  while(true) {
    printf("Dude, dude, dude!\n");
    int c = getchar();
    multicore_fifo_push_blocking(c); 
    sleep_ms(1000);
  }
*/

  PicoChess::Engine my_little_engine;
  
  PicoStreamPlayer::Play(&my_little_engine);
  
  return 0;
}
