/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>
#include <stdlib.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/systick.h>
#include "ticks.h"
#include "rng_driver.h"

static void clock_setup(void) {

  rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);

	/* Enable GPIOD clock for LED & USARTs. */
	rcc_periph_clock_enable(RCC_GPIOD);
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Enable clocks for USART2. */
	rcc_periph_clock_enable(RCC_USART2);

	/* Enable clock for RNG. */
	rcc_periph_clock_enable(RCC_RNG);
}

static void usart_setup(void) {
	/* Setup USART2 parameters. */
	usart_set_baudrate(USART2, 115200);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART2);
}

static void gpio_setup(void) {
	/* Setup GPIO pin GPIO12 on GPIO port D for LED. */
	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);

	/* Setup GPIO pins for USART2 transmit. */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

	/* Setup USART2 TX pin as alternate function. */
	gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
}

int main(void) {
  clock_setup();
  systick_setup(1000);
  gpio_setup();
  usart_setup();
	rng_driver_setup();

  //printf("\nTyche\n");
	uint32_t rnd;
	while (1) {
		/* Blink the LED (PD12) on the board with every transmitted byte. */
		gpio_toggle(GPIOD, GPIO12);	/* LED on/off */
		rnd = rng_driver_get_uint32();
	//	printf("%u - ", (unsigned int) rnd);
	//	while (rnd) {
	//		if (rnd & 1)
	//			printf("1");
	//		else
	//			printf("0");
	//		rnd >>= 8;
	//	}
			printf("R:");
			printf("%02x", (unsigned int) (rnd >> 24) & 0xFF);
			printf("%02x", (unsigned int) (rnd >> 16) & 0xFF);
			printf("%02x", (unsigned int) (rnd >> 8) & 0xFF);
			printf("%02x", (unsigned int) rnd & 0xFF);
			printf(":.\n");
		//c = (c == 9) ? 0 : c + 1;	/* Increment c. */
		//if ((j++ % 80) == 0) {		/* Newline after line full. */
		//	usart_send_blocking(USART2, '\r');
		//	usart_send_blocking(USART2, '\n');
		//}
		msleep(20);
	}
	return 0;
}
