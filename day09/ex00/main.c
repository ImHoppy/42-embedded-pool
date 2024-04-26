#include <avr/io.h>
#include <util/delay.h>
#include <i2c.h>

// D9 P15
// D10 P16
// D11 P17

#define D9 (1 << 3)

int main()
{
	i2c_init();

	uint8_t slave_address = (0b0100 << 3) /* Fixed Address*/ | 0b000 /* A2 A1 A0 */;
	// Set as output
	i2c_start(slave_address, I2C_WRITE);
	i2c_write(0x06);
	// Set D9 as output (Clear bit to 0 to set as output)
	i2c_write(~D9);
	i2c_write(0xff);
	i2c_stop();
	uint8_t led_state = 0;
	while (1)
	{
		led_state = !led_state;
		i2c_start(slave_address, I2C_WRITE);
		i2c_write(0x02);
		// Set D9 to 0 (led on) or 1 (led off)
		i2c_write(led_state ? ~D9 : 0xff); // ~(1 << 1) = d11, ~(1 << 2) = d10, ~(1 << 3)= d9
		i2c_write(0xff);
		i2c_stop();
		_delay_ms(500);
	}
}
