#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <i2c.h>
#include <uart.h>
#include "rush00.h"

extern volatile game_state_t game_state;

void send_light_update(uint8_t sec)
{
	i2c_start(0x10, I2C_WRITE);
	i2c_write(TIMER | (sec << 4));
	i2c_stop();
	light_led(sec);
}

void master_loop()
{
	static game_state_t old_game_state = IDLE;
	if (old_game_state == game_state)
		return;
	old_game_state = game_state;
	if (game_state == NOT_READY)
	{
		light_rgb(game_state);
		uint8_t is_ready = 0;
		i2c_start(0x10, I2C_READ);
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
		_delay_ms(10);
		if (is_ready)
		{
			game_state = INGAME;
			i2c_start(0x10, I2C_WRITE);
			i2c_write(CHANGE_GAME_STATUS | (INGAME << 4));
			i2c_stop();
		}
	}
	else if (game_state == INGAME)
	{
		_delay_ms(10);
		uart_printstr("GAME STARTED \n\r");
		uint8_t sec = 4;
		uint8_t pressed = 0;
		uint8_t millisec = 0;
		while (sec > 0 && game_state == INGAME)
		{
			sec -= 1;
			send_light_update(sec);
			millisec = 100;
			while (millisec > 0)
			{
				_delay_ms(10);
				millisec -= 1;

				i2c_start(0x10, I2C_READ);
				i2c_read(NACK);
				i2c_stop();
				pressed = TWDR;
				if (pressed == 1)
				{
					game_state = WIN;
				}
				if (game_state != INGAME)
					goto checkWinCondition;
			}
		}
		while (sec == 0 && game_state == INGAME)
		{
			i2c_start(0x10, I2C_READ);
			i2c_read(NACK);
			i2c_stop();
			pressed = TWDR;
			if (pressed == 1)
			{
				goto checkWinCondition;
			}
		}

	checkWinCondition:
		// Turn off last leds
		send_light_update(4);

		uart_printstr("\n\rEnd of game\n\r");
		uart_printhex(pressed);
		uart_printstr(" ");
		uart_printhex(game_state);
		uart_printstr(" ");
		uart_printhex(sec);
		uart_printstr(" ");
		uart_printhex(millisec);
		uart_printstr("\n\r");
		// Checking if slave pressed button
		if ((pressed == 1 && sec == 0 && millisec == 0) || (pressed == 0 && (sec != 0 || millisec != 0)))
		{
			game_state = LOOSE;
			i2c_start(0x10, I2C_WRITE);
			i2c_write(CHANGE_GAME_STATUS | (WIN << 4));
			i2c_stop();
			light_rgb(LOOSE);
		}
		else
		{
			i2c_start(0x10, I2C_WRITE);
			i2c_write(CHANGE_GAME_STATUS | (LOOSE << 4));
			i2c_stop();
			light_rgb(WIN);
		}
		game_state = IDLE;
	}
}
