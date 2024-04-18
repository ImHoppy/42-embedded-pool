#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <uart.h>

int main(void)
{
	uart_init(UART_ALL);

	while (1)
	{
		char c = uart_rx();
		if (c == 0x7F)
			uart_printstr("\b \b");
		else
			uart_tx(c);
	}
}
