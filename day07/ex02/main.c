#include <avr/io.h>
#include <util/delay.h>
#include <uart.h>

void print_adc_value(uint8_t channel)
{
	// Clear previous channel selection
	ADMUX &= ~(0b1111 << MUX0);
	// Select ADC channel
	ADMUX |= (channel << MUX0);
	// Start conversion
	ADCSRA |= (1 << ADSC);
	// Wait for conversion to complete
	while (ADCSRA & (1 << ADSC))
		;
	// Read 10 bits of ADC value
	uint16_t adc = ADC & 0x3FF;
	// Send ADC value to UART
	uart_printdec(adc);
}

int main()
{
	// Set voltage reference to AVCC
	ADMUX = (0b01 << REFS0);
	// Set prescaler to 128
	ADCSRA = (0b111 << ADPS0);
	// Enable ADC
	ADCSRA |= (1 << ADEN);
	// // Enable ADC interrupt
	// ADCSRA |= (1 << ADIE);
	// sei();

	// Read in 10 bits so don't touch ADLAR
	// ADMUX |= (1 << ADLAR);

	uart_init(UART_TX);

	while (1)
	{
		// Select ADC0 Potentiometer
		print_adc_value(0);
		uart_printstr(", ");
		// Select ADC1 LDR Sensor
		print_adc_value(1);
		uart_printstr(", ");
		// Select ADC2 Thermistor
		print_adc_value(2);
		uart_printnl();
		_delay_ms(20);
	}
}
