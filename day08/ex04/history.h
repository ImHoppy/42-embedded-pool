#ifndef HISTORY_H
#define HISTORY_H

#include <avr/io.h>
#include <stdbool.h>
#include <string.h>
#include "history.h"

#define ERROR_MSG "Invalid input (expected #RRGGBBDX)\n\r"
#define LINE_LEN 12

typedef struct
{
	char history[4][LINE_LEN];
	int8_t history_index;
	uint8_t cursor_position;
} history_data_t;

void read_line(char line[LINE_LEN]);

#endif
