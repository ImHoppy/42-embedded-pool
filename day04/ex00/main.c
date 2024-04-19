#include <avr/io.h>
#include <util/twi.h>
#include <i2c.h>
#include <uart.h>
#include <util/delay.h>

int main(void)
{
	i2c_init();
	uart_init(UART_TX);

	i2c_start();
	i2c_stop();

	return 0;
}
