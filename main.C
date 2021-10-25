// dude...

#include <stdio.h>
//#include <stdlib.h>
#include "pico/stdlib.h"

int main() {
  stdio_init_all();
  
  while(true) {
    printf("Hello Dude!\n");
    int c = getchar();
    printf("c: '%c'\n",c);
    sleep_ms(1000);
  }
  
  return 0;
}
