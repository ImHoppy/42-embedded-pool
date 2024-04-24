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
		uart_printdec(adc_read(0));
		uart_printstr(", ");
		// Select ADC1 LDR Sensor
		uart_printdec(adc_read(1));
		uart_printstr(", ");
		// Select ADC2 Thermistor
		uart_printdec(adc_read(2));
		uart_printnl();
		_delay_ms(20);
	}
}
