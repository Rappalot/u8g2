#ifndef U8G2CB_H
#define U8G2CB_H

#include "u8g2.h"
#include <stdint.h>

uint8_t u8x8_gpio_and_delay_stm32f1(u8x8_t *u8x8, uint8_t msg,
                uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_byte_stm32f1_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg,
                uint8_t arg_int, void *arg_ptr);


#endif /* U8G2CB_H */