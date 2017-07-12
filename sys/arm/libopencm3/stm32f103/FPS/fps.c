/*

  fps.c

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
  U8glib

  ST7920_192X32, SPI:    FPS: Box=7.6   @=9.8                iFPS: Box=11.4  @=14.7
  ST7920_192X32, 8Bit:   FPS: Box=6.2   @=7.5                iFPS: Box=9.3 @=11.2
  DOGM128 SW SPI:         FPS: Box=5.1   @=5.9  Pix=2.6               iFPS: Box=10.2 @=11.8 Pix=5.2
  DOGM128 HW SPI:         FPS: Box=5.5   @=6.3               iFPS: Box=11.0 @=12.6
  DOGXL160 SW SPI:         FPS: Box=1.7   @=1.9               iFPS: Box=6.9 @=7.7
  DOGXL160 HW SPI:         FPS: Box=1.8   @=2.1

  NHD27OLED_BW, SW SPI: FPS: Box=3.0  @=3.7
  NHD27OLED_BW, HW SPI: FPS: Box=3.5  @=4.5
  NHD27OLED_2X_BW, SW SPI: FPS: Box=3.8  @=4.9
  NHD27OLED_2X_BW, HW SPI: FPS: Box=4.6  @=6.4

  30 Sep 2012
  NHD27OLED_BW, SW SPI: FPS: Clip=9.2 Box=3.9  @=4.4		NEW_CODE
  NHD27OLED_BW, SW SPI: FPS: Clip=9.2 Box=3.6  @=4.5
  NHD27OLED_BW, HW SPI: FPS: Clip=16.3 Box=4.7  @=5.6
  NHD27OLED_2X_BW, SW SPI: FPS: Clip=9.7 Box=4.5  @=5.8
  NHD27OLED_2X_BW, SW SPI: FPS: Clip=18.0 Box=5.8  @=7.9

  1 Oct 2012
  ST7920_192X32, 8Bit:   FPS: Box=7.2   @=10.0
  DOGM128 SW SPI:         FPS: Box=5.2   @=6.6  Pix=2.6
  DOGM128 HW SPI:         FPS: Clip=33.2 Box=5.5   @=7.1
  DOGXL160 SW SPI:         FPS: Box=1.7   @=2.0
  DOGXL160 HW SPI:         FPS: Box=1.8   @=2.2

  DOGXL160 GR SW SPI:         FPS: Box=1.1   @=1.3

  1 Mar 2013
  ST7920_192X32_1X, SPI:    FPS: Clip=10.3 Box=5.5  @=7.2 Pix=3.9
  ST7920_192X32_4X, SPI:    FPS: Clip=10.9 Box=6.7  @=8.8 Pix=7.4
  ST7920_192X32_1X, 8Bit:    FPS: Clip=14.2 Box=6.1  @=8.4 Pix=4.2
  ST7920_192X32_4X, 8Bit:    FPS: Clip=14.2 Box=7.8  @=10.7 Pix=8.7
  ST7920_192X32_1X, HW SPI:    FPS: Clip=14.2 Box=6.3  @=8.7 Pix=4.3
  ST7920_192X32_4X, HW SPI:    FPS: Clip=15.3 Box=8.0  @=11.2 Pix=9.0

  2 Jun 2013
  U8GLIB_DOGM128 SW SPI:		FPS: Clip=23.9 Box=4.5  @=6.6 Pix=2.1
  U8GLIB_DOGM128_2X SW SPI:	FPS: Clip=28.5 Box=6.6  @=9.7 Pix=3.9
  U8GLIB_DOGM128_2X HW SPI:	FPS: Clip=40.8 Box=7.1  @=10.8 Pix=4.1

  3 Jun 2013
  U8GLIB_ST7920_192X32_1X -Os	SW SPI	FPS: Clip=11.0 Box=5.4  @=7.1 Pix=3.9	Size=11828
  U8GLIB_ST7920_192X32_1X -O3	SW SPI	FPS: Clip=10.9 Box=5.6  @=7.5 Pix=4.0	Size=13800
  U8GLIB_ST7920_192X32_1X -Os	SW SPI	FPS: Clip=16.8 Box=6.7  @=9.6 Pix=4.5	Size=11858	(new seq data output)
  U8GLIB_ST7920_192X32_1X -Os	HW SPI	FPS: Clip=25.7 Box=7.5  @=11.3 Pix=4.8	(new seq data output)

  6 Jun 2013
  U8GLIB_DOGS102 u8g(13, 11, 10, 9);    STD SW SPI	FPS: Clip=9.5 Box=7.6  @=8.2 Pix=6.2	Size=15652
  U8GLIB_DOGS102 u8g(13, 11, 10, 9);    SW SPI	        FPS: Clip=19.1 Box=12.8  @=14.0 Pix=9.2	Size=15532


  12 Jun 2013
  SSD1351_128X128_332	SW SPI Clip=1.3 Box=0.7  @=0.9 Pix=0.4
  SSD1351_128X128_332	HW SPI Clip=3.6 Box=1.1  @=1.5 Pix=0.5

  24 Jun 2013
  Uno SSD1351_128X128_332				SW SPI Clip=1.4 Box=0.8  @=0.9 Pix=0.4

  Uno SSD1351_128X128_332					HW SPI Clip=4.4 Box=1.2  @=1.6 Pix=0.5
  Uno SSD1351_128X128_HICOLOR			HW SPI Clip=3.7 Box=0.8  @=1.0 Pix=0.3

  Mega2560 SSD1351_128X128_332			HW SPI Clip=4.4 Box=1.2  @=1.6 Pix=0.5
  Mega2560 SSD1351_128X128_4X_332			HW SPI Clip=4.6 Box=2.3  @=2.8 Pix=1.5
  Mega2560 SSD1351_128X128_HICOLOR		HW SPI Clip=3.6 Box=0.8  @=1.0 Pix=0.3
  Mega2560 SSD1351_128X128_4X_HICOLOR	HW SPI Clip=4.2 Box=1.7  @=2.1 Pix=1.0

  Due SSD1351_128X128_332					HW SPI Clip=24.6 Box=6.3  @=7.8 Pix=2.8
  Due SSD1351_128X128_4X_332 				HW SPI Clip=28.1 Box=13.0  @=15.1 Pix=8.5
  Due SSD1351_128X128_HICOLOR			HW SPI Clip=20.8 Box=3.4  @=4.5 Pix=1.4
  Due SSD1351_128X128_4X_HICOLOR			HW SPI Clip=26.3 Box=8.9  @=11.1 Pix=4.8

  Due SSD1351_128X128_4X_HICOLOR			SW SPI Clip=0.4 Box=0.4  @=0.4 Pix=0.4

  Due DOGS102 u8g(13, 11, 10, 9);    			SW SPI	        FPS: Clip=19.1 Box=13.1  @=14.3 Pix=9.4
  Due DOGS102 u8g(10, 9);    					HW SPI	        FPS: Clip=128.9 Box=30.7  @=40.6 Pix=15.4

  Due NHD27OLED_BW u8g(10, 9)				HW SPI		FPS: Clip=53.0 Box=19.6  @=23.8 Pix=10.6
  Due NHD27OLED_2X_BW u8g(10, 9)			HW SPI		FPS: Clip=57.0 Box=25.3  @=31.7 Pix=18.1
  Due NHD27OLED_GR u8g(10, 9)				HW SPI		FPS: Clip=34.1 Box=11.7  @=13.7 Pix=5.6
  Due NHD27OLED_2X_GR u8g(10, 9)			HW SPI		FPS: Clip=38.1 Box=15.5  @=20.0 Pix=8.8


  28. Apr 2016
  Uno DOGS102 u8g(13, 11, 10, 9);    			SW SPI	        FPS: Clip=26.6 Box=6.1  @=8.5 Pix=2.8		11548 Bytes
  Uno DOGS102 u8g(10, 9);    					HW SPI	        FPS: Clip=33.1 Box=6.4  @=9.1 Pix=2.9		11366 Bytes

==============
  U8g2

  28. Apr 2016
  Uno DOGS102 u8g(13, 11, 10, 9);    			SW SPI	        FPS: Clip=3.0 Box=3.3  @=2.1 Pix=2.4		11472 Bytes
  Uno DOGS102 u8g(10, 9);    					HW SPI	        FPS: Clip=24.5 Box=87.4  @=5.0 Pix=7.3	11554 Bytes

  15. Mai 2016
  Uno U8G2_UC1701_DOGS102_1_4W_SW_SPI		SW SPI	        FPS: Clip=3.5 Box=3.8  @=2.3 Pix=2.7		Arduino Flash Size: 13262 Bytes
  Uno U8G2_UC1701_DOGS102_1_4W_HW_SPI    	HW SPI	        FPS: Clip=26.3 Box=90.5  @=5.1 Pix=8.5	Arduino Flash Size: 13370 Bytes
  Uno U8G2_UC1701_DOGS102_F_4W_HW_SPI    	HW SPI	        FPS: Clip=50.6 Box=119.6  @=9.4 Pix=15.7	Arduino Flash Size: 13370 Bytes

  16. Mai 2016 (improved SW 4-Wire SPI)
  Uno U8G2_UC1701_DOGS102_1_4W_SW_SPI		SW SPI	        FPS: Clip=18.6 Box=42.2  @=4.7 Pix=7.7

  24. Mai 2016
  Uno U8G2_ST7565_DOGM128_1_4W_SW_SPI		SW SPI	        FPS: Clip=18.1 Box=35.8  @=3.9 Pix=6.2
  Uno U8G2_ST7565_DOGM128_1_4W_HW_SPI		HW SPI	        FPS: Clip=25.4 Box=78.7  @=4.2 Pix=6.9

  25. June 2016
  U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI	HW SPI		FPS: Clip=56.4 Box=114.1  @=8.6 Pix=12.9
  U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI	HW SPI		FPS: Clip=27.7 Box=87.7  @=4.3 Pix=7.1

  18. Aug 2016
  U8G2_ST7920_128X64_1_HW_SPI				Uno			FPS: Clip=13.2 Box=11.5  @=3.7 Pix=5.9
  U8G2_ST7920_128X64_1_SW_SPI				Uno			FPS: Clip=1.2 Box=1.2  @=0.9 Pix=1.1

  U8G2_ST7920_128X64_1_SW_SPI				Due			FPS: Clip=3.1 Box=3.1  @=2.8 Pix=2.9
  U8G2_ST7920_128X64_1_HW_SPI				Due			FPS: Clip=26.2 Box=25.8  @=13.9 Pix=15.3


  U8G2_SSD1325_NHD_128X64_1_4W_HW_SPI		Uno			Clip=12.6 Box=19.1  @=3.6 Pix=5.8
  U8G2_SSD1325_NHD_128X64_1_4W_HW_SPI		Uno			Clip=15.1 Box=23.3  @=3.6 Pix=5.8		with graphics acceleration

  Issue #79:
  U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C Atmega32U8     Clip=15.7 Box=26.6  @=5.3 Pix=8.4

  26.Nov 2016
  U8G2_ST7920_128X64_1_8080				Uno			Clip=13.5 Box=12.1  @=3.6 Pix=5.9		atmega&arduino optimized
  U8G2_ST7920_128X64_1_SW_SPI 				Uno			Clip=13.2 Box=11.4  @=3.5 Pix=5.6			issue 87
  U8G2_SSD1306_128X64_NONAME_1_4W_SW_SPI	Uno			Clip=21.7 Box=46.8  @=4.0 Pix=7.1
  U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI	Uno			Clip=25.8 Box=84.2  @=4.2 Pix=7.8
  U8G2_SSD1306_128X64_NONAME_1_8080		Uno			Clip=16.1 Box=29.1  @=3.9 Pix=6.6
  U8G2_SSD1306_128X64_NONAME_1_6800		Uno			Clip=6.8 Box=8.4  @=2.9 Pix=4.3			6800 mode is not yet optimized for Atmega

  7 Dec 2016
  U8G2_KS0108_128X64_1 					Uno			Clip=6.7 Box=8.2  @=2.9 Pix=4.3

  11 Dec 2016
  U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI	Uno			Clip=25.8 Box=84.2  @=4.2 Pix=7.8		Old: no pixel optimization
  U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI	Uno			Clip=25.9 Box=84.1  @=4.3 Pix=8.3		Pixel (len=1) optimization

  14 Dec 2016
  U8G2_KS0108_128X64_1 					Uno			Clip=15.9 Box=28.0  @=3.9 Pix=6.9

  31 Dec 2016
  U8G2_LC7981_160X80_1_6800 				Uno			Clip=3.3 Box=3.0  @=1.5 Pix=2.1

  14 Jan 2017
  U8G2_ST7920_128X64_1_HW_SPI 				Uno			Clip=16.3 Box=20.2  @=4.1 Pix=7.2
  U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI	Uno			Clip=31.7 Box=77.2  @=4.7 Pix=8.9

  11 Mar 2017
  before
  U8G2_ST7920_128X64_1_SW_SPI 				Uno			Clip=16.0 Box=18.5  @=4.0 Pix=6.8		issue 200
  U8G2_ST7920_128X64_1_HW_SPI 				Uno			Clip=16.3 Box=20.2  @=4.1 Pix=7.2		issue 200

  after
  U8G2_ST7920_128X64_1_SW_SPI 				Uno			Clip=17.2 Box=20.2  @=4.1 Pix=7.0		issue 200
  U8G2_ST7920_128X64_1_HW_SPI 				Uno			Clip=17.4 Box=21.8  @=4.2 Pix=7.4		issue 200

  14 May 2017, Arduino 1.8
  U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI	Uno	4MHz	Clip=31.7 Box=77.2  @=4.7 Pix=8.9		issue 215
  U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI	Uno	8MHz	Clip=32.9 Box=84.1  @=4.7 Pix=9.0		issue 215

  U8G2_ST7565_EA_DOGM128_1_4W_HW_SPI 		Uno	4MHz	Clip=32.0 Box=78.5  @=4.7 Pix=8.9		issue 215

  U8G2_SSD1322_NHD_256X64_1_4W_HW_SPI		Uno	8MHz 16 Bit	Clip=9.0 Box=10.7  @=2.0 Pix=3.0		issue 302 before optimization
  U8G2_SSD1322_NHD_256X64_1_4W_HW_SPI		Uno	10MHz 16 Bit	Clip=10.9 Box=13.5  @=2.0 Pix=3.2	issue 302 after optimization (runs with 8MHz)

  U8G2_SSD1327_SEEED_96X96_1_HW_I2C		Uno			Clip=1.2 Box=1.2  @=0.9 Pix=1.0		issue 302 before optimization
  U8G2_SSD1327_SEEED_96X96_1_HW_I2C		Uno			Clip=1.4 Box=1.4  @=1.0 Pix=1.2		issue 302 after optimization

  U8G2_SSD1325_NHD_128X64_1_4W_HW_SPI		Uno			Clip=18.2 Box=25.1  @=4.0 Pix=6.7		issue 302 before optimization
  U8G2_SSD1325_NHD_128X64_1_4W_HW_SPI		Uno			Clip=19.1 Box=28.3  @=4.1 Pix=7.2		issue 302 after optimization

==============
  12 Jul 2017, Ported to C API for STM32F103 @ 72 MHz with libopencm3
  u8g2_Setup_ssd1322_nhd_256x64_1  4W-HW-SPI  Clip=42.2  Box=47.4  @=10.9  Pix=12.7		issue 302 before optimization
  u8g2_Setup_ssd1322_nhd_256x64_1  4W-HW-SPI  Clip=48.6  Box=55.7  @=11.3  Pix=13.1		issue 302 after optimization
  u8g2_Setup_ssd1322_nhd_256x64_2  4W-HW-SPI  Cpip=45.7  Box=48.3  @=14.3  Pix=17.8		issue 302 before optimization
  u8g2_Setup_ssd1322_nhd_256x64_2  4W-HW-SPI  Cpip=53.4  Box=57.0  @=14.9  Pix=18.7		issue 302 after optimization    

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
  //u8g2_Setup_ssd1322_nhd_256x64_2(&u8g2, U8G2_R0, u8x8_byte_stm32f1_4wire_hw_spi, u8x8_gpio_and_delay_stm32f1);  // init u8g2 structure
  u8g2_Setup_ssd1322_nhd_256x64_1(&u8g2, U8G2_R0, u8x8_byte_stm32f1_4wire_hw_spi, u8x8_gpio_and_delay_stm32f1);  // init u8g2 structure

  u8g2_InitDisplay(&u8g2);      // send init sequence to the display, display is in sleep mode after this,
  u8g2_SetPowerSave(&u8g2, 0);  // wake up display

  // assign default color value
  draw_color = 1;         // pixel on
}

/**
 * Do all initialisations, that are not done by a specific module here.
 */
static void setup_clock_and_gpios(void)
{
    /* Clock setup */
    /* Default clock is 8MHz HSI */
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    /* GPIO setup */
    /* Only the on board led is configured here */

    //rcc_periph_clock_enable();

}

int main(void)
{
  setup_clock_and_gpios();
  systick_setup();
  delay_setup();
  setup_u8g2();

  for (;;) {
    uint16_t fps;
#if 0
#endif
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
  }
}