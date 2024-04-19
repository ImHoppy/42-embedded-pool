#include <avr/io.h>
#include <util/twi.h>
#include <i2c.h>
#include <uart.h>
#include <util/delay.h>

#define IC2_DEBUG
#define AHT20_ADDR 0x38

int main(void)
{
	i2c_init();
	uart_init(UART_TX);

	i2c_start(AHT20_ADDR, 0);
	i2c_stop();

	return 0;
}
