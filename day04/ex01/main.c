#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include <i2c.h>
#include <uart.h>
#include <aht20.h>

#define AHT20_ADDR 0x38
#define AHT20_CMD 0xAC

int main(void)
{
	uart_init(UART_TX);

	i2c_init();
	// Wait for AHT20 to wake up
	_delay_ms(50);

	aht20_init();

	while (1)
	{
		// Read AHT20 status
		aht20_mesure();
		uart_printstr("\n\r");
		_delay_ms(500);
	}

	return 0;
}
