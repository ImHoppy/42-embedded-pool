#include "eeprom.h"

// Ex02

bool safe_eeprom_read(void *buffer, size_t offset, size_t length)
{
	if ((length + offset + 1) > EEPROM_MAX_ADDRESS)
		return false;
	if (eeprom_read_word((uint16_t *)offset) != EEPROM_MAGIC_NUMBER)
		return false;
	eeprom_read_block(buffer, (void *)(offset + 2), length);
	return true;
}

bool safe_eeprom_write(void *buffer, size_t offset, size_t length)
{
	if (length + offset > EEPROM_MAX_ADDRESS)
		return false;
	if (eeprom_read_word((uint16_t *)offset) != EEPROM_MAGIC_NUMBER)
		eeprom_write_word((uint16_t *)offset, EEPROM_MAGIC_NUMBER);
	// eeprom_write_block(buffer, (void *)(offset + 1), length);
	for (size_t i = 0; i < length; i++)
	{
		if (eeprom_read_byte((uint8_t *)(offset + 2 + i)) != ((uint8_t *)buffer)[i])
			eeprom_write_byte((uint8_t *)(offset + 2 + i), ((uint8_t *)buffer)[i]);
	}
	return true;
}

// Ex03

uint16_t eepromalloc_find_free_space(uint16_t offset)
{
	while (offset < EEPROM_MAX_ADDRESS)
	{
		if (eeprom_read_word((uint16_t *)offset) != EEPROM_MAGIC_NUMBER)
			return offset;
		offset += EEPROM_MAGIC_NUMBER_SIZE;
		offset += eeprom_read_word((uint16_t *)offset) + sizeof(uint16_t);
	}
	return EEPROM_MAX_ADDRESS;
}

/// @brief Write data to the eeprom at a specific id
/// @param id The id of the data
/// @param buffer The data to write
/// @param length The length of the data
/// @return true if the data was written successfully, false if id already exists or if the data is too big
bool eepromalloc_write(uint16_t id, void *buffer, uint16_t length)
{
	uint32_t total_size = sizeof(id) + EEPROM_MAGIC_NUMBER_SIZE + sizeof(length) + length;
	if (length == 0 || total_size > EEPROM_MAX_ADDRESS)
		return false;

	uint16_t offset = 0;
	while ((offset + total_size) < EEPROM_MAX_ADDRESS)
	{
		if (eeprom_read_word((uint16_t *)offset) == EEPROM_MAGIC_NUMBER)
		{
			offset += EEPROM_MAGIC_NUMBER_SIZE;
			if (eeprom_read_word((uint16_t *)(offset)) == id)
				return false;
			offset += sizeof(id);
			uint16_t block_length = eeprom_read_word((uint16_t *)offset);
			offset += block_length + sizeof(block_length);
		}
		else
		{
		}
		return false;
		offset += sizeof(id) + EEPROM_MAGIC_NUMBER_SIZE + sizeof(length) + length;
	}
	return true;
}
bool eepromalloc_read(uint16_t id, void *buffer, uint16_t length)
{
	return true;
}
bool eepromalloc_free(uint16_t id)
{
	return true;
}
