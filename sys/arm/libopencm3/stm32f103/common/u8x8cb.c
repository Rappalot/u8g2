/*
  u8x8cb.c

  STM32F103 with libopencm3

*/

#include "u8x8cb.h"
#include "u8g2.h"
#include "delay.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>


#define OLED_PORT       GPIOA
#define OLED_RCC_GPIO   RCC_GPIOA

#define OLED_CS         GPIO4
#define OLED_SCL        GPIO5
#define OLED_A0         GPIO6
#define OLED_SI         GPIO7

#define OLED_SPI        SPI1
#define OLED_RCC_SPI    RCC_SPI1


uint8_t u8x8_gpio_and_delay_stm32f1(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    (void) arg_ptr; /* unused, suppress warning */

    switch(msg) {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:

        /* Enable clock for OLED Port */
        rcc_periph_clock_enable(OLED_RCC_GPIO);

        /* All pins are outputs */
        gpio_set_mode(OLED_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
            OLED_CS | OLED_SCL | OLED_A0 | OLED_SI);
        break;

    case U8X8_MSG_DELAY_NANO:
        /* 125ns per cycle @ 8 MHz sysclk */
        /* 14ns per cycle @ 72 MHz sysclk */
        //__asm__("nop");
        break;

    case U8X8_MSG_DELAY_100NANO:
        /* not used */
        break;

    case U8X8_MSG_DELAY_10MICRO:
        /* not used */
        break;

    case U8X8_MSG_DELAY_MILLI:
        delay_us(arg_int*1000UL);
        break;

    case U8X8_MSG_GPIO_DC:
        if (arg_int)
            GPIO_BSRR(OLED_PORT) = OLED_A0;
        else
            GPIO_BRR(OLED_PORT) = OLED_A0;
        break;

    case U8X8_MSG_GPIO_CS:
        if (arg_int)
            GPIO_BSRR(OLED_PORT) = OLED_CS;
        else
            GPIO_BRR(OLED_PORT) = OLED_CS;
        break;

    case U8X8_MSG_GPIO_SPI_CLOCK:
        if (arg_int)
            GPIO_BSRR(OLED_PORT) = OLED_SCL;
        else
            GPIO_BRR(OLED_PORT) = OLED_SCL;
        break;
    case U8X8_MSG_GPIO_SPI_DATA:
        if (arg_int)
            GPIO_BSRR(OLED_PORT) = OLED_SI;
        else
            GPIO_BRR(OLED_PORT) = OLED_SI;
        break;
/*
    case U8X8_MSG_GPIO_MENU_SELECT:
        u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_SELECT_PORT, KEY_SELECT_PIN));
        break;
    case U8X8_MSG_GPIO_MENU_NEXT:
        u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_NEXT_PORT, KEY_NEXT_PIN));
        break;
    case U8X8_MSG_GPIO_MENU_PREV:
        u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_PREV_PORT, KEY_PREV_PIN));
        break;

    case U8X8_MSG_GPIO_MENU_HOME:
        u8x8_SetGPIOResult(u8x8, Chip_GPIO_GetPinState(LPC_GPIO, KEY_HOME_PORT, KEY_HOME_PIN));
        break;
*/
    default:
        u8x8_SetGPIOResult(u8x8, 1);
    break;
    }
    return 1;
}

uint8_t u8x8_byte_stm32f1_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    static uint32_t spi_cr;     /* TODO: eliminate static variable */
    uint8_t *data;

    switch(msg) {
    case U8X8_MSG_BYTE_SEND:
        data = (uint8_t *)arg_ptr;
        //SPI_CR1(OLED_SPI) |= SPI_CR1_SPE;
        while ( arg_int-- > 0 ) {
            SPI_DR(OLED_SPI) = *data++;
            while ((SPI_SR(OLED_SPI) & SPI_SR_TXE) == 0)
                ;
        }
        break;

    case U8X8_MSG_BYTE_INIT:
        u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
        rcc_periph_clock_enable(OLED_RCC_SPI);
        gpio_set_mode(OLED_PORT, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                OLED_SCL | OLED_SI);

        spi_cr = SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE;
        if (u8x8->display_info->spi_mode & 0x1)
            spi_cr |= SPI_CR1_CPHA;
        if (u8x8->display_info->spi_mode & 0x2)
            spi_cr |= SPI_CR1_CPOL;

        uint32_t freq = (OLED_SPI == SPI1 ? rcc_apb2_frequency : rcc_apb1_frequency)/2;
        
        uint32_t br_div = 0;
        while (br_div < SPI_CR1_BAUDRATE_FPCLK_DIV_256 &&
                    u8x8->display_info->sck_clock_hz < freq) {
            freq /= 2;
            br_div += SPI_CR1_BAUDRATE_FPCLK_DIV_4;
        }
        spi_cr |= br_div;
        break;

    case U8X8_MSG_BYTE_SET_DC:
        if (arg_int)
            GPIO_BSRR(OLED_PORT) = OLED_A0;
        else
            GPIO_BRR(OLED_PORT) = OLED_A0;
        break;

    case U8X8_MSG_BYTE_START_TRANSFER:
        SPI_CR1(OLED_SPI) = spi_cr;
        GPIO_BRR(OLED_PORT) = OLED_CS;
        break;

    case U8X8_MSG_BYTE_END_TRANSFER:
        GPIO_BSRR(OLED_PORT) = OLED_CS;
        SPI_CR1(OLED_SPI) &= ~SPI_CR1_SPE;
        break;

    default:
        return 0;
    }
    return 1;
}
