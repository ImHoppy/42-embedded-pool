#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <i2c.h>
#include <uart.h>
#include "rush00.h"

extern volatile game_state_t game_state;

void master_loop()
{
	static game_state_t old_game_state = IDLE;
	if (old_game_state == game_state)
		return;
	old_game_state = game_state;
	uart_printstr("CHANGE_GAME_STATUS  ");
	uart_printhex(game_state);
	uart_printstr("\n\r");
	if (game_state == NOT_READY)
	{
		uint8_t is_ready = 0;
		i2c_start(0x10, I2C_READ);
		uart_printstr("AFTER START  ");
		while (is_ready == 0)
		{
			// uart_printstr("WAITING FOR SLAVE IM_READY IN LOOP    ");
			i2c_read(ACK);
			is_ready = TWDR;

			// uart_printstr("AFTER READ  ");
			// uart_printhex(TWDR);
			// uart_printstr(" \n\r");
		}
		i2c_read(NACK);
		i2c_stop();
		_delay_ms(100);
		if (is_ready)
		{
			uart_printstr("SLAVE IM_READY IN LOOP \n\r");
			game_state = INGAME;
			i2c_start(0x10, I2C_WRITE);
			i2c_write(CHANGE_GAME_STATUS | (INGAME << 4));
			i2c_stop();
		}
	}
	else if (game_state == INGAME)
	{
		uart_printstr("GAME STARTED \n\r");
		int sec = 0;
		while (sec < 4)
		{
			i2c_start(0x10, I2C_WRITE);
			i2c_write(TIMER | (sec << 4));
			i2c_stop();
			light_led(sec);
			_delay_ms(1000);
			sec += 1;
		}

		i2c_start(0x10, I2C_READ);
		uint8_t pressed = 0;
		while (is_ready == 0)
		{
			is_ready = TWDR;
		}
		i2c_read(NACK);
		i2c_stop();
	}
	else if (game_state == WIN)
	{
	}
	else if (game_state == LOOSE)
	{
	}
}
