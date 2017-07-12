/*
 * (C) Copyright 20014 - 2017 Leo C. <erbl259-lmu@yahoo.de>
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void systick_setup(void);
uint32_t get_timer(uint32_t);

#endif /* TIMER_H */
