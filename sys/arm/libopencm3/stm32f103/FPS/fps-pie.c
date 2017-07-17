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

==============
  15 Jul 2017, Ported to C API for STM32F103 @ 72 MHz with libopencm3 (16 bit)
  u8g2_Setup_ssd1322_nhd_256x64_1  4W-HW-SPI  Clip=42.4  Box=48.0  @=10.4  Pix=11.7		issue 302 before optimization
  u8g2_Setup_ssd1322_nhd_256x64_1  4W-HW-SPI  Clip=49.0  Box=56.5  @=10.8  Pix=12.1		issue 302 after optimization
  u8g2_Setup_ssd1322_nhd_256x64_2  4W-HW-SPI  Cpip=46.0  Box=48.9  @=13.8  Pix=16.7		issue 302 before optimization
  u8g2_Setup_ssd1322_nhd_256x64_2  4W-HW-SPI  Cpip=53.8  Box=57.8  @=14.4  Pix=17.5		issue 302 after optimization    

*/


#include "u8g2.h"
#include "u8x8cb.h"
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

void draw_pie(void) {
  u8g_uint_t x, y, w, h;
  u8g2_SetDrawColor(&u8g2, draw_color);
  w = u8g2_GetDisplayWidth(&u8g2);
  h = u8g2_GetDisplayHeight(&u8g2);
#if 0
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
#endif
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
#if 0
    fps = picture_loop_with_fps(draw_clip_test);
    show_result("draw clip test", fps);
    delay(5000);
    fps = picture_loop_with_fps(draw_set_screen);
    show_result("clear screen", fps);
    delay(5000);
    fps = picture_loop_with_fps(draw_char);
    show_result("draw @", fps);
    delay(5000);
    fps = picture_loop_with_fps(draw_pixel);
    show_result("draw pixel", fps);
    delay(5000);
#endif
    fps = picture_loop_with_fps(draw_pie);
    show_result("draw pie", fps);
    delay(5000);
    }
}
