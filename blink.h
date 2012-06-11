#include "nibo/iodefs.h"

void blink(void){

  IO_LEDS_RED_PORT = 0x80;
  IO_LEDS_GREEN_PORT = 0x80;
  _delay_ms(500);

  IO_LEDS_RED_PORT = 0x40;
  IO_LEDS_GREEN_PORT = 0x40;
  _delay_ms(500);
 
  IO_LEDS_RED_PORT = 0x20;
  IO_LEDS_GREEN_PORT = 0x20;
  _delay_ms(500);
 
  IO_LEDS_RED_PORT = 0x10;
  IO_LEDS_GREEN_PORT = 0x10;
  _delay_ms(500);
 
  IO_LEDS_RED_PORT = 0x08;
  IO_LEDS_GREEN_PORT = 0x08;
  _delay_ms(500); 

  IO_LEDS_RED_PORT = 0x04;
  IO_LEDS_GREEN_PORT = 0x04;
  _delay_ms(500);
 
}
