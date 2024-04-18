#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <uart.h>

int main(void)
{
	uart_init();

	while (1)
	{
		uart_tx('Z');
		_delay_ms(1000);
	}
}
