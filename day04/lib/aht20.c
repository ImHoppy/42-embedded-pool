#include <util/delay.h>
#include <util/twi.h>
#include <avr/io.h>
#include <aht20.h>
#include <uart.h>

#define AHT20_ADDR 0x38

// AHT20 commands

#define AHT20_STATUS_CMD 0x71
#define AHT20_STATUS_CALBIT 3

#define AHT20_INIT_CMD 0xBE
#define AHT20_INIT_PARAM 0x08
#define AHT20_INIT_PARAM2 0x00

#define AHT20_MESURE_CMD 0xAC
#define AHT20_MESURE_PARAM 0x33
#define AHT20_MESURE_PARAM2 0x00

#define AHT20_BUSY 7
void aht20_init(void)
{
	// Wait for AHT20 to wake up
	_delay_ms(50);
	// 5.4.1
	i2c_start(AHT20_ADDR, I2C_WRITE);
	i2c_write(AHT20_STATUS_CMD);
	i2c_stop();
	i2c_start(AHT20_ADDR, I2C_READ);
	i2c_read(NACK);
	i2c_stop();
	const uint8_t status = TWDR;

	if (!(status & _BV(AHT20_STATUS_CALBIT)))
	{
		uart_printstr("\n\rCalibration bit not set\n\r");
		// 5.4.2
		i2c_start(AHT20_ADDR, I2C_WRITE);
		i2c_write(AHT20_INIT_CMD);
		i2c_write(AHT20_INIT_PARAM);
		i2c_write(AHT20_INIT_PARAM2);
		_delay_ms(10);
	}
	// Wait for AHT20 to wake up
	i2c_stop();
	_delay_ms(50);
}

aht20_data aht20_mesure(void)
{
	aht20_data data = {0};
	i2c_start(AHT20_ADDR, I2C_WRITE);
	i2c_write(AHT20_MESURE_CMD);
	i2c_write(AHT20_MESURE_PARAM);
	i2c_write(AHT20_MESURE_PARAM2);
	i2c_stop();
	_delay_ms(80);
	i2c_start(AHT20_ADDR, I2C_READ);
	i2c_read(ACK);
	if (!(TWDR & _BV(AHT20_BUSY)))
	{
		i2c_read(ACK);
		data.humidity = TWDR << 8;
		i2c_read(ACK);
		data.humidity |= TWDR;
		i2c_read(ACK);
		data.humidity_temp = TWDR;
		i2c_read(ACK);
		data.temp = TWDR << 8;
		i2c_read(ACK);
		data.temp |= TWDR;
		i2c_read(NACK);
	}
	i2c_stop();
	return data;
}
