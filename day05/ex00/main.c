#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#define is_pressed(reg, pos) ((reg & (1 << pos)) == 0)

#define EEPROM_ADDRESS (uint8_t *)0

void update_led(uint8_t nb)
{
	PORTB &= (0xFF << 3) & ~(1 << 4);			  // 0b1111 1000 & 0x1110 1111
	PORTB |= (nb & 0b111) | ((nb & 0b1000) << 1); // 0b0000 0111 | 0b0001 0000
}

uint8_t on_press(uint8_t state, uint8_t reg, uint8_t pos, uint8_t action)
{
	if (state == 0 && is_pressed(reg, pos))
	{
		// Read the value from the eeprom at address 0
		uint8_t nb = eeprom_read_byte(EEPROM_ADDRESS);
		nb += action;
		nb %= 16;
		update_led(nb);
		// Write the value to the eeprom at address 0
		eeprom_write_byte(EEPROM_ADDRESS, nb);
		_delay_ms(20);
		state = 1;
	}
	if (state == 1 && !is_pressed(reg, pos))
	{
		_delay_ms(20);
		state = 0;
	}
	return state;
}
int main()
{
	uint8_t sw1_pressed = 0;

	DDRB |= 0b10111;

	update_led(eeprom_read_byte(EEPROM_ADDRESS));
	while (1)
	{
		sw1_pressed = on_press(sw1_pressed, PIND, PIND2, 1);
	}
}
