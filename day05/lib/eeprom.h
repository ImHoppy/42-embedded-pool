#ifndef EEPROM_H
#define EEPROM_H

#define EEPROM_MAX_ADDRESS 1023
#define EEPROM_MAGIC_NUMBER 0xDEAD
#define EEPROM_MAGIC_NUMBER_SIZE 2

#include <avr/eeprom.h>
#include <stdbool.h>

// Ex02
bool safe_eeprom_read(void *buffer, size_t offset, size_t length);
bool safe_eeprom_write(void *buffer, size_t offset, size_t length);

// Ex03
bool eepromalloc_write(uint16_t id, void *buffer, uint16_t length);
bool eepromalloc_read(uint16_t id, void *buffer, uint16_t length);
bool eepromalloc_free(uint16_t id);

#endif
