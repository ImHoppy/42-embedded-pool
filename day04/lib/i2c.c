#include <avr/io.h>
#include <util/twi.h>
#include <uart.h>

#define SCL_CLOCK 100000L

// TWDR; // TWI Data Register (22.5.3)
void i2c_init(void)
{
	// TWI Bit Rate Generator Register (22.5.2)
	TWBR = ((F_CPU / SCL_CLOCK) - 16) / 2;
	TWSR = 0; // TWI Status Register (22.5.2) Contains Prescaler bits
	// Enable 2-wire Serial Interface (22.7.1)
	TWCR = (1 << TWEN); // TWI Control Register (22.5.3) (22.9.2)
	TWAR = (0x38 << 1);
}

#define WAIT_DATA \
	while (!(TWCR & (1 << TWINT)))

uint8_t i2c_start(void)
{
	// Send START Condition (22.7.1)
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	// Wait for TWINT Flag set (22.7.1)
	WAIT_DATA;

	uart_printhex(TW_STATUS);
	uart_printstr(" ");
	// if START has been transmitted.
	if (TW_STATUS != TW_START)
		return (0);

	// Set AHT20 Slave Address (AHT20 7.3)
	TWDR = TWAR;
	TWCR = (1 << TWINT) | (1 << TWEN);
	// Wait end of transmission
	WAIT_DATA;

	// if SLA+W has been transmitted and ACK has been received.
	if (TW_STATUS != TW_MT_SLA_ACK)
		return (0);
	uart_printhex(TW_STATUS);
	uart_printstr("\n\r");
	return 1;
}
void i2c_stop(void)
{
	// Send STOP Condition
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	uart_printhex(TW_STATUS);
	uart_printstr("\n\r");
}
