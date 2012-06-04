#include "nibo/niboconfig.h"
#include "nibo/iodefs.h"
#include "nibo/delay.h"
#include "nibo/display.h"
#include "nibo/gfx.h"
#include "nibo/spi.h"
#include "nibo/copro.h"
#include "nibo/adc.h"
#include "nibo/floor.h"
#include "nibo/leds.h"
#include "nibo/pwm.h"
#include "nibo/bot.h"

#include "mylog.h"

#define THRESHOLD 150

void drive_and_stay_on_line(){
  uint8_t go = 0;
  char mytext[300] = "";
  while (1) {
    sei();
    _delay_ms(1);
    floor_update();
    if (go==0) {
      if ((floor_relative[FLOOR_LEFT]>THRESHOLD) && (floor_relative[FLOOR_RIGHT]>THRESHOLD)) {
        // Boden links und rechts vorhanden -> losfahren!
        mylog("boden lnr");
    sprintf(mytext, "floor_left: %i",floor_relative[FLOOR_LEFT]);
    mylog(mytext);
    sprintf(mytext, "floor_right: %i",floor_relative[FLOOR_RIGHT]);
    mylog(mytext);
        go=1;
        copro_setSpeed(20, 20);
        IO_LEDS_RED_PORT=0x00;
        IO_LEDS_GREEN_PORT=0x84;
      }
    } else {
      if ((floor_relative[FLOOR_LEFT]<THRESHOLD) || (floor_relative[FLOOR_RIGHT]<THRESHOLD)) {
        // Boden links oder rechts verloren -> sofort stoppen!
        mylog("boden verloren");
        copro_stopImmediate();
        copro_setSpeed(-25, -25);
        go=0;
        IO_LEDS_RED_PORT=0x84;
        IO_LEDS_GREEN_PORT=0x00;
      }
    }
    
  }
}
