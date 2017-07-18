/*

  draw_disc_sector.c
  Copyright (c) 2017, Leo C. <erbl259-lmu@yahoo.de>

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

*/

#include "draw_disc_sector.h"
#include "u8g2.h"

const uint16_t sintab_256[91] = {
      0,   4,   9,  13,  18,  22,  27,  31,
     36,  40,  44,  49,  53,  58,  62,  66,
     71,  75,  79,  83,  88,  92,  96, 100,
    104, 108, 112, 116, 120, 124, 128, 132,
    136, 139, 143, 147, 150, 154, 158, 161,
    165, 168, 171, 175, 178, 181, 184, 187,
    190, 193, 196, 199, 202, 204, 207, 210,
    212, 215, 217, 219, 222, 224, 226, 228,
    230, 232, 234, 236, 237, 239, 241, 242,
    243, 245, 246, 247, 248, 249, 250, 251,
    252, 253, 254, 254, 255, 255, 255, 256,
    256, 256, 256
};

const uint16_t sintab_512[91] = {
      0,   9,  18,  27,  36,  45,  54,  62,  71,  80,
     89,  98, 106, 115, 124, 133, 141, 150, 158, 167,
    175, 183, 192, 200, 208, 216, 224, 232, 240, 248,
    256, 264, 271, 279, 286, 294, 301, 308, 315, 322,
    329, 336, 343, 349, 356, 362, 368, 374, 380, 386,
    392, 398, 403, 409, 414, 419, 424, 429, 434, 439,
    443, 448, 452, 456, 460, 464, 468, 471, 475, 478,
    481, 484, 487, 490, 492, 495, 497, 499, 501, 503,
    504, 506, 507, 508, 509, 510, 511, 511, 512, 512,
    512,
};


int_fast16_t sin_256(int_fast16_t phi)
{
    uint_fast8_t neg = 0;
    int_fast16_t s;

    while (phi >= 360)
        phi -= 360;
    while (phi < 0)
        phi += 360;
    if (phi >= 180) {
        neg = 1;
        phi -= 180;
    }
    if (phi > 90)
        phi = 180 - phi;

    s = sintab_256[phi];

    if (neg)
        return -s;
    else
        return s;
}

int_fast16_t cos_256(int_fast16_t phi)
{
    return sin_256(90 - phi);
}

int_fast16_t sin_512(int_fast16_t phi)
{
    uint_fast8_t neg = 0;
    int_fast16_t s;

    while (phi >= 360)
        phi -= 360;
    while (phi < 0)
        phi += 360;
    if (phi >= 180) {
        neg = 1;
        phi -= 180;
    }
    if (phi > 90)
        phi = 180 - phi;

    s = sintab_512[phi];

    if (neg)
        return -s;
    else
        return s;
}

int_fast16_t cos_512(int_fast16_t phi)
{
    return sin_512(90 - phi);
}



/*==============================================*/
/* Disk sector */

static uint_fast8_t are_clockwise(u8g2_int_t x1, u8g2_int_t y1, u8g2_int_t x2, u8g2_int_t y2)
{
    return -x1 * y2 + y1 * x2 > 0;
}

static uint_fast8_t is_inside_sector(u8g2_int_t x, u8g2_int_t y,
                    u8g2_int_t x_start, u8g2_int_t y_start,
                    u8g2_int_t x_end, u8g2_int_t y_end)
{
    return !are_clockwise(x_start, y_start, x, y) &&
                    are_clockwise(x_end, y_end, x, y);
}

static void drawVLine_sections(u8g2_t *u8g2, u8g2_int_t xs, u8g2_int_t ys,
                              u8g2_int_t h, u8g2_uint_t x0, u8g2_uint_t y0,
                              u8g2_int_t x_start, u8g2_int_t y_start,
                              u8g2_int_t x_end, u8g2_int_t y_end, uint_fast8_t reverse)
{
    u8g2_int_t y;
#if 0
    for (y = ys; y <= ys+h; y++)
        if (reverse ^ is_inside_sector(xs, y, x_start, y_start, x_end, y_end))
            u8g2_DrawPixel(u8g2, x0+xs, y0+y);
#else
    u8g2_int_t seg_start;
    uint_fast8_t in_sector;

    seg_start = ys;
    in_sector = reverse ^ is_inside_sector(xs, ys, x_start, y_start, x_end, y_end);
    for (y = ys+1; y <= ys+h; y++) {
        if (((reverse ^ is_inside_sector(xs, y, x_start, y_start, x_end, y_end)) ^ in_sector) != 0) {
            if (in_sector) {
                u8g2_DrawVLine(u8g2, x0+xs, y0+seg_start, y - seg_start);
            }
            seg_start = y;
            in_sector ^= 1;
        }
    }
    if (in_sector) {
        u8g2_DrawVLine(u8g2, x0+xs, y0+seg_start, y - seg_start);
    }
#endif
}

static void u8g2_draw_disc_sector_section(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y,
                        u8g2_uint_t x0, u8g2_uint_t y0,
                        u8g2_int_t x_start, u8g2_int_t y_start,
                    u8g2_int_t x_end, u8g2_int_t y_end, uint_fast8_t reverse)
{
    /* rightmost: 315 - 45 */
    drawVLine_sections(u8g2, y, -x, 2*x+1, x0, y0, x_start, y_start, x_end, y_end, reverse);
    /* middle right: 45-90, 270 - 315 */
    drawVLine_sections(u8g2, x, -y, 2*y+1, x0, y0, x_start, y_start, x_end, y_end, reverse);
    /* middle left: 90 - 135, 225 - 270 */
    drawVLine_sections(u8g2, -x, -y, 2*y+1, x0, y0, x_start, y_start, x_end, y_end, reverse);
    /* leftmost: 135 - 180, 180 - 225 */
    drawVLine_sections(u8g2, -y, -x, 2*x+1, x0, y0, x_start, y_start, x_end, y_end, reverse);
}

static void u8g2_draw_disc_sector(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0,
                        u8g2_uint_t rad,
                        u8g2_int_t sect_start, u8g2_int_t sect_end)
{
  u8g2_int_t x_start;
  u8g2_int_t y_start;
  u8g2_int_t x_end;
  u8g2_int_t y_end;
  uint_fast8_t reverse;

  u8g2_int_t f;
  u8g2_int_t ddF_x;
  u8g2_int_t ddF_y;
  u8g2_uint_t x;
  u8g2_uint_t y;

  f = 1;
  f -= rad;
  ddF_x = 1;
  ddF_y = 0;
  ddF_y -= rad;
  ddF_y *= 2;
  x = 0;
  y = rad;

  reverse = 0;
  if (sect_start > sect_end)
      sect_start -= 360;
  if (sect_end - sect_start > 180) {
      u8g2_int_t v;
      v = sect_start;
      sect_start = sect_end;
      sect_end = v;
      reverse = 1;
  }

  x_start = cos_512(sect_start);
  y_start = sin_512(sect_start);
  x_end   = cos_512(sect_end);
  y_end   = sin_512(sect_end);

  u8g2_draw_disc_sector_section(u8g2, x, y, x0, y0, x_start, y_start, x_end, y_end, reverse);

  while ( x < y )
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    u8g2_draw_disc_sector_section(u8g2, x, y, x0, y0, x_start, y_start, x_end, y_end, reverse);
  }
}


void u8g2_DrawDisc_sector(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rad,
                        u8g2_int_t sect_start, u8g2_int_t sect_end)
{
  /* check for bounding box */
#ifdef U8G2_WITH_INTERSECTION
  {
    if ( u8g2_IsIntersection(u8g2, x0-rad, y0-rad, x0+rad+1, y0+rad+1) == 0 )
      return;
  }
#endif /* U8G2_WITH_INTERSECTION */

  /* draw disc */
  u8g2_draw_disc_sector(u8g2, x0, y0, rad, sect_start, sect_end);
}
