#ifndef RUSH00_H
#define RUSH00_H
#include <avr/io.h>

enum event
{
	CHANGE_GAME_STATUS = 1,
	BE_A_SLAVE,
	TIMER,
	IM_READY,
	IM_NOT_READY_YET,
	PRESSED_BUTTON,

};

typedef enum
{
	IDLE = 0,
	NOT_READY,
	INGAME,
	WIN,
	LOOSE,
} game_state_t;

void light_led(uint8_t index);
void master_loop(void);
void slave_loop(void);
#endif
