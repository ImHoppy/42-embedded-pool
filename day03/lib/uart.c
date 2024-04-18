#include <avr/interrupt.h>
#include "uart.h"

// Initialization USART 20.5
void uart_init(void)
{
	// Set Baud Rate (20.11.5)
	UBRR0 = BAUD;

	// Enable transmitter (20.11.3)
	UCSR0B = (1 << TXEN0);

	// Set frame format: 8 data bit, N, 1 stop bit (20.11.4)
	UCSR0C = (0b11 << UCSZ00);
}

// Data Transmission (20.6)
void uart_tx(char c)
{
	// Clear TXCn Flag (20.5)
	// TXC0 = 0;
	// Wait for empty transmit buffer
	while (!(UCSR0A & (1 << UDRE0)))
		;
	// Put data into buffer, sends the data
	UDR0 = c;
}

void uart_printstr(const char *str)
{
	for (char c = *str; c != 0; ++str, c = *str)
		uart_tx(c);
}
