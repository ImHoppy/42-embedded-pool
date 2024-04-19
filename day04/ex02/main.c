#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include <i2c.h>
#include <uart.h>
#include <aht20.h>
#include <stdlib.h>
int main(void)
{
	uart_init(UART_TX);

	i2c_init();
	// Wait for AHT20 to wake up
	_delay_ms(50);

	aht20_init();

	aht20_data last_data[4] = {0};
	uint8_t index = 0;
	while (1)
	{
		index %= 3;
		// Read AHT20 status
		last_data[index] = aht20_mesure();
		if (last_data[0].temp && last_data[1].temp && last_data[2].temp)
		{
			print_temp((last_data[0].temp + last_data[1].temp + last_data[2].temp) / 3);
			uart_printstr(", ");
			print_humi((last_data[0].humidity + last_data[1].humidity + last_data[2].humidity) / 3);
			uart_printstr("\n\r");
		}
		index++;
		_delay_ms(500);
	}

	return 0;
}
