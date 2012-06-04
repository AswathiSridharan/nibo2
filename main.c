/*
  Mit diesem Programm weicht der NIBO 2 Hindernissen aus, er fährt jedoch nur wenn er auf dem Boden 
  steht. Hebt man ihn auf, drehen sich die Räder nicht mehr.

*/

#include "nibo/niboconfig.h"
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
#include "proximitysensors.c"

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) ((hi*0x100)+lo)


/*
  Hier werden die Zustände definiert, in denen sich der NIBO 2 befinden kann.
*/
int16_t tspeed_l = 0;
int16_t tspeed_r = 0;

uint8_t dist_l;
uint8_t dist_fl;
uint8_t dist_f;
uint8_t dist_fr;
uint8_t dist_r;

uint8_t dist_max_ls; // max leftside
uint8_t dist_max_fs; // max frontside
uint8_t dist_max_rs; // max rightside

uint8_t headlight_target;
uint8_t headlight_act;

float volt_flt = 10.0;

uint16_t stress;

void print_hex (uint8_t val) {
  char c1=val/16;
  char c2=val%16;
  if (c1<10) c1+='0'; else c1+='a'-10;
  if (c2<10) c2+='0'; else c2+='a'-10;
  gfx_print_char(c1);
  gfx_print_char(c2);
}

uint8_t back_cnt;

int main(void) {
  sei(); // enable interrupts
  leds_init();
  pwm_init();
  bot_init();
  spi_init();
  floor_init();
  floor_readPersistent();

  if (display_init()!=0) {
    leds_set_displaylight(50);
    if (display_type==2) {
      gfx_init();
    }
  }

  copro_ir_startMeasure();
  delay(10);
  //motco_setSpeedParameters(5, 4, 6); // ki, kp, kd
  copro_setSpeedParameters(15, 20, 10); // ki, kp, kd

  stress = 0;
  
  while (1) {
    delay(50);
    leds_set_displaylight(1024);

    // Spannung
    bot_update();
    
    // Analog values:
    floor_update();
    int16_t value_fl = floor_l;
    int16_t value_fr = floor_r;
    int16_t value_ll = line_l;
    int16_t value_lr = line_r;

    if (value_fl<0) value_fl=0;
    if (value_fr<0) value_fr=0;
    if (value_ll<0) value_ll=0;
    if (value_lr<0) value_lr=0;
    if (value_fl>255) value_fl=255;
    if (value_fr>255) value_fr=255;
    if (value_ll>255) value_ll=255;
    if (value_lr>255) value_lr=255;

    gfx_move(30, 10);
    print_hex(value_fr);
    gfx_print_char(' ');
    print_hex(value_lr);
    gfx_print_char(' ');
    print_hex(value_ll);
    gfx_print_char(' ');
    print_hex(value_fl);

    // Distance

    int16_t speed_l = 0;
    int16_t speed_r = 0;

    // Request distance data
    if (!copro_update()) {
      gfx_move(10, 10);
      gfx_set_proportional(1);
      gfx_print_text("COPRO Error   ");
      gfx_set_proportional(0);
      continue;
    }

    dist_l  = copro_distance[4]/128;
    dist_fl = copro_distance[3]/128;
    dist_f  = copro_distance[2]/128;
    dist_fr = copro_distance[1]/128;
    dist_r  = copro_distance[0]/128;

    dist_l = (dist_l<250)? (dist_l+5):255;
    dist_r = (dist_r<250)? (dist_r+5):255;
    dist_f = (dist_f>5)? (dist_f-5):0;


    dist_max_ls = (dist_l>dist_fl)?dist_l:dist_fl;
    dist_max_rs = (dist_r>dist_fr)?dist_r:dist_fr;
    dist_max_fs = (dist_fr>dist_fl)?dist_fr:dist_fl;
    if (dist_f>dist_max_fs) dist_max_fs = dist_f;

    gfx_move(25, 20);
    print_hex(dist_r);
    gfx_print_char(' ');
    print_hex(dist_fr);
    gfx_print_char(' ');
    print_hex(dist_f);
    gfx_print_char(' ');
    print_hex(dist_fl);
    gfx_print_char(' ');
    print_hex(dist_l);

    uint8_t dist[5];
    dist[0] = dist_r;
    dist[1] = dist_fr;
    dist[2] = dist_f;
    dist[3] = dist_fl;
    dist[4] = dist_l;
    
    proximity_check(dist);
    
    uint16_t sum = dist_r + dist_fr
                 + dist_f + dist_fl
                 + dist_l;
    sum /= 5;
    
    uint8_t dmax = (sum<0xe0)?(sum+0x20):0xff;
    uint8_t dmin = (sum>0x20)?(sum-0x20):0x00;

   
    stress = (stress>10)?(stress-10):(0);

    stress += copro_current_l;
    stress += copro_current_r;
  }
}
