/*

  fps-pie.c

  Frames Per Second: Display Performance Test

  Ported from FPS.ino, C++/Arduino to C API
  Leo C. <erbl259-lmu@yahoo.de>

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice, this
    list of conditions and the following disclaimer in the documentation and/or other
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.



==============
  STM32F103 @ 72 MHz with libopencm3, u8g2_Setup_ssd1322_nhd_256x64_1  4W-HW-SPI, 16 bit

    text     --- fps ----
    size     disc    sect
    ----------------------------------------------
    9712     40.8    14.2    "orig"
    9692     40.4    16.5    "pixel y=0"
    9668     40.6    18.2    "pixel y=ys"
    9724     40.6    21.7    "vline"
    9668     40.6    18.2    clean up, pixel y=ys
    9724     40.6    21.7    clean up, vline
    9720     40.8    12.7    10-50,10-330 orig
    9676     40.6    15.6    10-50,10-330 pixel
    9732     40.6    21.1    10-50,10-330 vline

*/


#include "u8g2.h"
#include "u8x8cb.h"
#include "draw_disc_sector.h"
#include <libopencm3/stm32/rcc.h>
#include <string.h>
#include "timer.h"
#include "delay.h"

#define millis()  get_timer(0)
#define delay(x)  delay_ms(x)

typedef u8g2_uint_t u8g_uint_t;

#define SECONDS 10
uint8_t flip_color = 0;
uint8_t draw_color = 1;
u8g2_t u8g2;

#if 0
void draw_set_screen(void) {
  // graphic commands to redraw the complete screen should be placed here
  u8g2_SetDrawColor(&u8g2, flip_color);
  u8g2_DrawBox(&u8g2,  0, 0, u8g2_GetDisplayWidth(&u8g2), u8g2_GetDisplayHeight(&u8g2) );
}

void draw_clip_test(void) {
  u8g_uint_t i, j, k;
  char buf[3] = "AB";
  k = 0;
  u8g2_SetDrawColor(&u8g2, draw_color);
  u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);

  for( i = 0; i  < 6; i++ ) {
    for( j = 1; j  < 8; j++ ) {
      u8g2_DrawHLine(&u8g2, i-3, k, j);
      u8g2_DrawHLine(&u8g2, i-3+10, k, j);

      u8g2_DrawVLine(&u8g2, k+20, i-3, j);
      u8g2_DrawVLine(&u8g2, k+20, i-3+10, j);

      k++;
    }
  }
  u8g2_SetFontDirection(&u8g2, 0);
  u8g2_DrawStr(&u8g2, 0-3, 50, buf);
  u8g2_SetFontDirection(&u8g2, 2);
  u8g2_DrawStr(&u8g2, 0+3, 50, buf);

  u8g2_SetFontDirection(&u8g2, 0);
  u8g2_DrawStr(&u8g2, u8g2_GetDisplayWidth(&u8g2)-3, 40, buf);
  u8g2_SetFontDirection(&u8g2, 2);
  u8g2_DrawStr(&u8g2, u8g2_GetDisplayWidth(&u8g2)+3, 40, buf);

  u8g2_SetFontDirection(&u8g2, 1);
  u8g2_DrawStr(&u8g2, u8g2_GetDisplayWidth(&u8g2)-10, 0-3, buf);
  u8g2_SetFontDirection(&u8g2, 3);
  u8g2_DrawStr(&u8g2, u8g2_GetDisplayWidth(&u8g2)-10, 3, buf);

  u8g2_SetFontDirection(&u8g2, 1);
  u8g2_DrawStr(&u8g2, u8g2_GetDisplayWidth(&u8g2)-20, u8g2_GetDisplayHeight(&u8g2)-3, buf);
  u8g2_SetFontDirection(&u8g2, 3);
  u8g2_DrawStr(&u8g2, u8g2_GetDisplayWidth(&u8g2)-20, u8g2_GetDisplayHeight(&u8g2)+3, buf);

  u8g2_SetFontDirection(&u8g2, 0);

}

void draw_char(void) {
  char buf[2] = "@";
  u8g_uint_t i, j;
  // graphic commands to redraw the complete screen should be placed here
  u8g2_SetDrawColor(&u8g2, draw_color);
  u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);
  j = 8;
  for(;;) {
    i = 0;
    for(;;) {
      u8g2_DrawStr(&u8g2,  i, j, buf);
      i += 8;
      if ( i > u8g2_GetDisplayWidth(&u8g2) )
        break;
    }
    j += 8;
    if ( j > u8g2_GetDisplayHeight(&u8g2) )
      break;
  }

}

void draw_pixel(void) {
  u8g_uint_t x, y, w2, h2;
  u8g2_SetDrawColor(&u8g2, draw_color);
  w2 = u8g2_GetDisplayWidth(&u8g2);
  h2 = u8g2_GetDisplayHeight(&u8g2);
  w2 /= 2;
  h2 /= 2;
  for( y = 0; y < h2; y++ ) {
    for( x = 0; x < w2; x++ ) {
      if ( (x + y) & 1 ) {
        u8g2_DrawPixel(&u8g2, x,y);
        u8g2_DrawPixel(&u8g2, x,y+h2);
        u8g2_DrawPixel(&u8g2, x+w2,y);
        u8g2_DrawPixel(&u8g2, x+w2,y+h2);
      }
    }
  }
}
#endif

void draw_disc(void) {
  u8g_uint_t x0_left, x0_right, y0;
  u8g_uint_t r, w, h;

  u8g2_SetDrawColor(&u8g2, draw_color);
  w = u8g2_GetDisplayWidth(&u8g2);
  h = u8g2_GetDisplayHeight(&u8g2);
  r = h/2;
  if (w/4 < r)
    r = w/4;
  r -= 2;
  x0_left = w/4;
  x0_right = w - w/4;
  y0 = h/2;

  u8g2_DrawDisc(&u8g2, x0_left, y0, r, U8G2_DRAW_ALL);
  u8g2_DrawDisc(&u8g2, x0_right, y0, r, U8G2_DRAW_ALL);
}

void draw_pie(void) {
  u8g_uint_t x0_left, x0_right, y0;
  u8g_uint_t r, w, h;

  u8g2_SetDrawColor(&u8g2, draw_color);
  w = u8g2_GetDisplayWidth(&u8g2);
  h = u8g2_GetDisplayHeight(&u8g2);
  r = h/2;
  if (w/4 < r)
    r = w/4;
  r -= 2;
  x0_left = w/4;
  x0_right = w - w/4;
  y0 = h/2;

  u8g2_DrawDisc_sector(&u8g2, x0_left, y0, r, 10, 50);
  u8g2_DrawDisc_sector(&u8g2, x0_right, y0, r, 10, 330);
}

// returns unadjusted FPS
uint16_t picture_loop_with_fps(void (*draw_fn)(void)) {
  uint16_t FPS10 = 0;
  uint32_t time;

  time = millis() + SECONDS*1000;

  // picture loop
  do {
    u8g2_FirstPage(&u8g2);
    do {
      draw_fn();
    } while( u8g2_NextPage(&u8g2) );
    FPS10++;
    flip_color = flip_color ^ 1;
  } while( millis() < time );
  return FPS10;
}

const char *convert_FPS(uint16_t fps) {
  static char buf[6];
  strcpy(buf, u8g2_u8toa( (uint8_t)(fps/10), 3));
  buf[3] =  '.';
  buf[4] = (fps % 10) + '0';
  buf[5] = '\0';
  return buf;
}

void show_result(const char *s, uint16_t fps) {
  // assign default color value
  u8g2_SetDrawColor(&u8g2, draw_color);
  u8g2_SetFont(&u8g2, u8g2_font_8x13B_tf);
  u8g2_FirstPage(&u8g2);
  do {
    u8g2_DrawStr(&u8g2, 0,12, s);
    u8g2_DrawStr(&u8g2, 0,24, convert_FPS(fps));
  } while( u8g2_NextPage(&u8g2) );
}

void setup_u8g2(void)
{
  // The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupc
  u8g2_Setup_ssd1322_nhd_256x64_1(&u8g2, U8G2_R0, u8x8_byte_stm32f1_4wire_hw_spi, u8x8_gpio_and_delay_stm32f1);  // init u8g2 structure
  //u8g2_Setup_ssd1322_nhd_256x64_2(&u8g2, U8G2_R0, u8x8_byte_stm32f1_4wire_hw_spi, u8x8_gpio_and_delay_stm32f1);  // init u8g2 structure

  u8g2_InitDisplay(&u8g2);      // send init sequence to the display, display is in sleep mode after this,
  u8g2_SetPowerSave(&u8g2, 0);  // wake up display

  // assign default color value
  draw_color = 1;         // pixel on
}

static void setup_mcu(void)
{
  /* Clock setup */
  /* Default clock is 8MHz HSI */
  rcc_clock_setup_in_hse_8mhz_out_72mhz();

  systick_setup();
  delay_setup();
}

int main(void)
{
  setup_mcu();
  setup_u8g2();

  for (;;) {
    uint16_t fps;
    fps = picture_loop_with_fps(draw_disc);
    show_result("draw disc", fps);
    delay(5000);
    fps = picture_loop_with_fps(draw_pie);
    show_result("draw pie", fps);
    delay(5000);
    }
}
