#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#define is_pressed(reg, pos) ((reg & (1 << pos)) == 0)

#define EEPROM_COUNTER_INDEX 0
#define EEPROM_MIN_ADDRESS 1
#define EEPROM_MAX_ADDRESS 5

void debounce(volatile uint8_t *reg, uint8_t pos)
{
	for (uint32_t i = 0; i < F_CPU / 12 / 4; ++i)
		;
	while (is_pressed(*reg, pos))
		;
	for (uint32_t i = 0; i < F_CPU / 12 / 4; ++i)
		;
}

void update_led(uint8_t nb)
{
	PORTB &= (0xFF << 3) & ~(1 << 4);			  // 0b1111 1000 & 0x1110 1111
	PORTB |= (nb & 0b111) | ((nb & 0b1000) << 1); // 0b0000 0111 | 0b0001 0000
}

int main()
{
	// Get the counter index from the eeprom
	uint8_t counter_index = eeprom_read_byte((uint8_t *)EEPROM_COUNTER_INDEX);

	DDRB |= 0b10111;

	// Protect current address from being out of bounds
	if (counter_index >= EEPROM_MAX_ADDRESS - EEPROM_MIN_ADDRESS)
	{
		counter_index = 0;
		eeprom_write_byte(EEPROM_COUNTER_INDEX, counter_index);
	}
	// Update led from previous value
	update_led(eeprom_read_byte((uint8_t *)(EEPROM_MIN_ADDRESS + counter_index)));
	while (1)
	{
		if (is_pressed(PIND, PIND2))
		{
			// Read the value from the eeprom at address + counter_index
			uint8_t nb = eeprom_read_byte((uint8_t *)(EEPROM_MIN_ADDRESS + counter_index));
			nb += 1;
			nb %= 16;
			update_led(nb);
			// Write the value to the eeprom at address + counter_index
			eeprom_write_byte((uint8_t *)(EEPROM_MIN_ADDRESS + counter_index), nb);
			debounce(&PIND, PIND2);
		}
		if (is_pressed(PIND, PIND4))
		{
			counter_index += 1;
			counter_index %= (EEPROM_MAX_ADDRESS - EEPROM_MIN_ADDRESS);
			update_led(eeprom_read_byte((uint8_t *)(EEPROM_MIN_ADDRESS + counter_index)));
			eeprom_write_byte((uint8_t *)EEPROM_COUNTER_INDEX, counter_index);
			debounce(&PIND, PIND4);
		}
	}
}
