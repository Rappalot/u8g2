#include "delay.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/dwt.h>
#include <stdint.h>

uint32_t clocks_per_microsecond;

void delay_setup(void)
{
    /* DWT cycle counter used for µs delays */
    dwt_enable_cycle_counter();

    clocks_per_microsecond = rcc_ahb_frequency / 1000000;
}


void delay_us (uint32_t us)   // max. delay at 72 MHz is 910 µs and 8 ms at 8 MHz
{
    uint32_t delaycount = DWT_CYCCNT + (clocks_per_microsecond * us);

    while ((DWT_CYCCNT - delaycount) & 0x80000000)
        ;
}

#if 0
void delay_us (uint32_t us)
{
    DWT_CYCCNT = 0;
    uint32_t delaycount = clocks_per_microsecond * us;
    while (DWT_CYCCNT < delaycount)
        ;
}
#endif

void delay_ms (uint32_t ms)   // 2 * ms to be save at 72 MHz (2 * 500 µs = 1 ms)
{
    uint32_t i;
    uint32_t j = 2 * ms;

    for (i = 0; i < j; i++) {
        delay_us(500);
    }
}
