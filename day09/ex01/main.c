#include <avr/io.h>
#include <util/delay.h>
#include <i2c.h>
#include <stdbool.h>
#include <uart.h>

// D9 P15
// D10 P16
// D11 P17

#define D9 (1 << 3)
#define D10 (1 << 2)
#define D11 (1 << 1)
#define SW3 (1 << 0)

static const uint8_t slave_address = (0b0100 << 3) /* Fixed Address*/ | 0b000 /* A2 A1 A0 */;

void update_leds(uint8_t n)
{
	i2c_start(slave_address, I2C_WRITE);
	i2c_write(0x02);
	i2c_write(~(n << 1));
	i2c_write(0xff);
	i2c_stop();
}

void start_read()
{
	i2c_start(slave_address, I2C_WRITE);
	i2c_write(0x00);
	i2c_stop();
	i2c_start(slave_address, I2C_READ);
}

uint8_t read_at(uint8_t pos)
{
	uint8_t data;
	i2c_read(ACK);
	data = TWDR;
	i2c_read(ACK);
	return (data & (1 << pos)) == 0;
}

int main()
{
	i2c_init();
	uart_init(UART_TX);

	// Set as output
	i2c_start(slave_address, I2C_WRITE);
	i2c_write(0x06);
	// Set D9 as output (Clear bit to 0 to set as output)
	i2c_write(~(D9 | D10 | D11));
	i2c_write(0xff);
	i2c_stop();
	uint8_t number = 0;
	update_leds(number);
	while (1)
	{
		start_read();
		uint8_t data = read_at(0);
		if (data)
		{
			i2c_stop();
			number = (number + 1) % 8;
			update_leds(number);

			start_read();
			data = read_at(0);
			while (data)
			{
				data = read_at(0);
				_delay_ms(100);
			}
			i2c_stop();
		}
		_delay_ms(25);
	}
}
