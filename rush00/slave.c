#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <i2c.h>
#include <uart.h>
#include "rush00.h"

extern volatile game_state_t game_state;
extern volatile uint8_t slave_button_state;

void slave_loop()
{

	static game_state_t old_game_state = IDLE;
	if (old_game_state == game_state)
		return;
	if (game_state == NOT_READY)
	{
	}
	else if (game_state == INGAME)
	{
		while (game_state == INGAME)
		{
			// Send to master when button pressed
		}
	}
	else if (game_state == WIN)
	{
		// met vert
	}
	else if (game_state == LOOSE)
	{
		// met rouge
	}
	old_game_state = game_state;
}
