#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

extern
uint32_t clocks_per_microsecond;

void delay_setup(void);
void delay_us (uint32_t us);
void delay_ms (uint32_t ms);

#endif /* DELAY_H */
