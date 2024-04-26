#include <avr/io.h>
#include <util/delay.h>
#include <i2c.h>
#include <stdbool.h>
#include <uart.h>

#define SW3 (1 << 0)

static const uint8_t slave_address = (0b0100 << 3) /* Fixed Address*/ | 0b000 /* A2 A1 A0 */;

void seg7_display(uint8_t pos, uint8_t n)
{
	static const uint8_t seg7[11] = {
		0b00111111, // 0
		0b00000110, // 1
		0b01011011, // 2
		0b01001111, // 3
		0b01100110, // 4
		0b01101101, // 5
		0b01111101, // 6
		0b00000111, // 7
		0b01111111, // 8
		0b01101111, // 9
		0b00000000, // Clear
	};

	// Clear old data register before writing new data
	i2c_start(slave_address, I2C_WRITE);
	i2c_write(0x02);
	i2c_write(0xff);
	i2c_write(0);
	i2c_stop();

	i2c_start(slave_address, I2C_WRITE);
	i2c_write(0x02);
	i2c_write(~(0x80 >> pos));
	i2c_write(seg7[n]);
	i2c_stop();
}

int main()
{
	i2c_init();
	uart_init(UART_TX);

	// Set as output
	i2c_start(slave_address, I2C_WRITE);
	i2c_write(0x06);
	i2c_write(0b00001111); // Set output digit selector
	i2c_write(0x00);	   // Set output all segments
	i2c_stop();

	while (1)
	{
		seg7_display(0, 2);
		seg7_display(1, 4);
	}
}
