#include "eeprom.h"

bool safe_eeprom_read(void *buffer, size_t offset, size_t length)
{
	if ((length + offset + 1) > EEPROM_MAX_ADDRESS)
		return false;
	if (eeprom_read_byte((uint8_t *)offset) != EEPROM_MAGIC_NUMBER)
		return false;
	eeprom_read_block(buffer, (void *)(offset + 1), length);
	return true;
}

bool safe_eeprom_write(void *buffer, size_t offset, size_t length)
{
	if (length + offset > EEPROM_MAX_ADDRESS)
		return false;
	eeprom_write_byte((uint8_t *)offset, EEPROM_MAGIC_NUMBER);
	// eeprom_write_block(buffer, (void *)(offset + 1), length);
	for (size_t i = 0; i < length; i++)
	{
		if (eeprom_read_byte((uint8_t *)(offset + 1 + i)) != ((uint8_t *)buffer)[i])
			eeprom_write_byte((uint8_t *)(offset + 1 + i), ((uint8_t *)buffer)[i]);
	}
	return true;
}
