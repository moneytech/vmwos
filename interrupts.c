#include <stdint.h>
#include "bcm2835_periph.h"
#include "mmio.h"
#include "led.h"

/* global variable */
int blinking_enabled=1;

void __attribute__((interrupt("IRQ"))) interrupt_handler(void) {

	static int lit = 0;

	/* Clear the ARM Timer interrupt		*/
	/* Since it's the only one we have enabled,	*/
	/* Assume it is what caused the interrupt.	*/
	/* FIXME: do proper detection of IRQ src	*/

	mmio_write(TIMER_IRQ_CLEAR,0x1);

	if (!blinking_enabled) return;

	/* Flip the LED */
	if( lit ) {
		led_off();
		lit = 0;
	}
	else {
		led_on();
		lit = 1;
	}

}

