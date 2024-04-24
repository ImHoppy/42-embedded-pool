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
		// Read internal temperature sensor and convert to Celsius
		uart_printdec(adc_read(8) >> 2);
		uart_printnl();
		_delay_ms(20);
	}
}
