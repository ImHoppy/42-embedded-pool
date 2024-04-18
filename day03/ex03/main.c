#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <uart.h>

ON_RX
{
	char c = uart_rx();
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		uart_tx(c ^ 32);
	else
		uart_tx(c);
}

int main(void)
{
	uart_init(UART_ALL);

	// Enable RX Complete Interrupt (20.7.3)
	UCSR0B |= (1 << RXCIE0);
	sei();

	while (1)
	{
	}
}
