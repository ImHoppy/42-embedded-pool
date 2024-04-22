#include <avr/io.h>
#include <avr/eeprom.h>

#define is_pressed(reg, pos) ((reg & (1 << pos)) == 0)

#define EEPROM_ADDRESS (uint8_t *)0

void update_led(uint8_t nb)
{
	PORTB &= (0xFF << 3) & ~(1 << 4);			  // 0b1111 1000 & 0x1110 1111
	PORTB |= (nb & 0b111) | ((nb & 0b1000) << 1); // 0b0000 0111 | 0b0001 0000
}

void debounce(volatile uint8_t *reg, uint8_t pos)
{
	for (uint32_t i = 0; i < F_CPU / 12 / 8; ++i)
		;
	while (is_pressed(*reg, pos))
		;
	for (uint32_t i = 0; i < F_CPU / 12 / 8; ++i)
		;
}

int main()
{
	DDRB |= 0b10111;

	update_led(eeprom_read_byte(EEPROM_ADDRESS));
	while (1)
	{
		if (is_pressed(PIND, PIND2))
		{
			// Read the value from the eeprom at address 0
			uint8_t nb = eeprom_read_byte(EEPROM_ADDRESS);
			nb += 1;
			nb %= 16;
			update_led(nb);
			// Write the value to the eeprom at address 0
			eeprom_write_byte(EEPROM_ADDRESS, nb);
			debounce(&PIND, PIND2);
		}
	}
}
