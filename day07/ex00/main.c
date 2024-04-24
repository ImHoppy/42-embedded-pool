#include <avr/io.h>
#include <util/delay.h>
#include <uart.h>

int main()
{
	// Set voltage reference to AVCC
	ADMUX = (0b01 << REFS0);
	// Select ADC0
	ADMUX |= (0b0000 << MUX0);
	// Set prescaler to 128
	ADCSRA = (0b111 << ADPS0);
	// Enable ADC
	ADCSRA |= (1 << ADEN);
	// // Enable ADC interrupt
	// ADCSRA |= (1 << ADIE);
	// sei();
	// Start conversion
	ADCSRA |= (1 << ADSC);

	uart_init(UART_TX);

	while (1)
	{
		// Wait for conversion to complete
		while (ADCSRA & (1 << ADSC))
			;
		// Read ADC value
		uint16_t adc = ADC;
		// Send ADC value to UART
		uart_printhex(adc >> 8);
		uart_printhex(adc & 0xFF);
		uart_printstr("\n\r");
		// Start conversion
		ADCSRA |= (1 << ADSC);
		_delay_ms(20);
	}
}
