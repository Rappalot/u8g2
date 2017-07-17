#ifndef _DRAW_DISC_SECTOR_H
#define _DRAW_DISC_SECTOR_H


#include "u8g2.h"

void u8g2_DrawDisc_sector(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rad,
                        u8g2_int_t sect_start, u8g2_int_t sect_end);

int_fast16_t cos_256(int_fast16_t phi);
int_fast16_t sin_256(int_fast16_t phi);
int_fast16_t sin_512(int_fast16_t phi);
int_fast16_t cos_512(int_fast16_t phi);

#endif /* _DRAW_DISC_SECTOR_H */
