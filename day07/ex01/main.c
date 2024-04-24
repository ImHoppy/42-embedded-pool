#include <avr/io.h>
#include <util/delay.h>
#include <uart.h>
#include <adc.h>

int main()
{
	uart_init(UART_TX);

	adc_init(ADC_NORMAL);

	while (1)
	{
		// Select ADC0 Potentiometer
		uart_printhex(adc_read8(0));
		uart_printstr(", ");
		// Select ADC1 LDR Sensor
		uart_printhex(adc_read8(1));
		uart_printstr(", ");
		// Select ADC2 Thermistor
		uart_printhex(adc_read8(2));
		uart_printnl();
		_delay_ms(20);
	}
}
