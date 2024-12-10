#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/watchdog.h"
// #include "hardware/regs/glitch_detector.h"
#include "hardware/regs/powman.h"
#include "hardware/structs/powman.h"
#include "hardware/structs/otp.h"
#include "rp2350_playground.h"

#include "display/display.h"

#include <stdio.h>

#pragma GCC push_options
#pragma GCC optimize("O0")
#define OUTER_LOOP_CNT 200
#define INNER_LOOP_CNT 200


void __not_in_flash_func(glitch_target)(void)
{
	// volatile register uint32_t i, j;
	// volatile register uint32_t cnt;
	uint32_t blink_status = 1;
	bool glitch_detector_triggered = (powman_hw->chip_reset & 0x04000000) != 0;

	// uart_putc_raw(TARGET_UART_INSTANCE, 'Q');

	if (glitch_detector_triggered)
	{
		uart_putc_raw(TARGET_UART_INSTANCE, 'G');
	}
	else
	{
		uart_putc_raw(TARGET_UART_INSTANCE, 'R');
	}

	// if (!glitch_detector_armed())
	// {
	// 	uart_putc_raw(TARGET_UART_INSTANCE, 'S');
	// }
	// else
	// {
	// 	uart_putc_raw(TARGET_UART_INSTANCE, 'F');
	// }

	watchdog_update();
	// cnt = 0;
	uint32_t cnt = 0, i ,j;
	// float features[2] = {0.0f, 1.0f};
	// int label = -1;

	watchdog_update();
	gpio_put(TRIGGER_PIN, 1);
	sleep_ms(10);
	watchdog_update();
	gpio_put(TRIGGER_PIN, 0);

	
	gpio_put(TRIGGER2_PIN, 1);
	for (i = 0; i < OUTER_LOOP_CNT; i++)
	{
		for (j = 0; j < INNER_LOOP_CNT; j++)
		{
			cnt++;
		}
	}
	gpio_put(TRIGGER2_PIN, 0);

	// const bool glitch_detected = i != OUTER_LOOP_CNT || j != INNER_LOOP_CNT || cnt != (OUTER_LOOP_CNT * INNER_LOOP_CNT);
	// char buffer[32];
	// snprintf(buffer, sizeof(buffer), "i=%d, j=%d, cnt=%d", i, j, cnt);
	// uart_puts(uart0, buffer);


	// Check for glitch
	if (i != OUTER_LOOP_CNT || j != INNER_LOOP_CNT || cnt != (OUTER_LOOP_CNT * INNER_LOOP_CNT))
	{
		// Q indicates successful glitch
		watchdog_update();
		uart_putc_raw(uart0, 'Q');
	}
	else
	{
		// N indicates regular execution
		uart_putc_raw(uart0, 'N');
	}
	watchdog_update();

	// while (1) {}
	
	
	/*
	gpio_put(TRIGGER2_PIN, 1);
	if (features[0] < 0.5f) 
	{
        if (features[1] < 0.5f) {
            label = 0;
        } else {
            label = 1;
        }
    } else {
        if (features[1] < 0.5f) {
            label = 2;
        } else {
            label = 3;
        }
    }
	gpio_put(TRIGGER2_PIN, 0);

	const bool tree_glitch_detected = label != 1;

	// Check for glitch
	if (tree_glitch_detected)
	{
		// X indicates successful glitch
		uart_putc_raw(uart0, 'X');
	}
	else
	{
		// N indicates regular execution
		uart_putc_raw(uart0, 'N');
	}
	watchdog_update();
	*/
	
}
#pragma GCC pop_options

int main()
{

	init_uart();
	// while (true) {
    //     uart_puts(TARGET_UART_INSTANCE, "Hello from RP2350!\r\n");
    //     sleep_ms(1000);
    // }
	watchdog_enable(100, 1);
	gpio_init(TRIGGER_PIN);
	gpio_set_dir(TRIGGER_PIN, GPIO_OUT);
	gpio_put(TRIGGER_PIN, 0);

	gpio_init(TRIGGER2_PIN);
	gpio_set_dir(TRIGGER2_PIN, GPIO_OUT);
	gpio_put(TRIGGER2_PIN, 0);

	for(int i=0; i < 5; i++) {
		gpio_init(joy_io[i]);
		gpio_set_dir(joy_io[i], GPIO_IN);
		gpio_pull_up(joy_io[i]);
	}

	glitch_target();

	for (;;) { watchdog_update(); }
}
