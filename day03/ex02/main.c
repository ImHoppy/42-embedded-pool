#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <uart.h>

int main(void)
{
	uart_init(UART_ALL);

	while (1)
	{
		uart_tx(uart_rx());
	}
}
