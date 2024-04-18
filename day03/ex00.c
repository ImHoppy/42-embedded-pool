#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define UART_BAUDRATE 115200
#define BAUD ((F_CPU / 16.0 / UART_BAUDRATE) + 0.5) - 1

// Initialization USART 20.5
void uart_init(uint16_t ubrr)
{
	UBRR0 = 0;

	// Set Baud Rate (20.11.5)
	UBRR0 = ubrr;

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

int main(void)
{
	uart_init(BAUD);

	while (1)
	{
		uart_tx('Z');
		_delay_ms(1000);
	}
}
