#include <avr/io.h>
#include <util/delay.h>

#define is_pressed(reg, pos) ((reg & (1 << pos)) == 0)

void set_bit(volatile uint8_t *reg, uint8_t pos, uint8_t value) {
	if (value == 0) return;
	*reg |= (1 << pos);
}

uint8_t on_press(uint8_t state, uint8_t reg, uint8_t pos, uint8_t action) {
	static uint8_t nb;

	if (state == 0 && is_pressed(reg, pos)) {
		_delay_ms(20);
		state = 1;
		nb += action;
		nb %= 16;
		// Clear all led
		PORTB &= (0xFF << 3) & ~(1 << 4); // 0b1111 1000 & 0x1110 1111
		set_bit(&PORTB, PIN0, nb & 1);
		set_bit(&PORTB, PIN1, nb & 2);
		set_bit(&PORTB, PIN2, nb & 4);
		set_bit(&PORTB, PIN4, nb & 8);
	}
	if (state == 1 && !is_pressed(reg, pos)) {
		_delay_ms(20);
		state = 0;
	}
	return state;
}
int main() {

	uint8_t sw1_pressed = 0;
	uint8_t sw2_pressed = 0;

	DDRB = 0xFF | (1 << PIN4);

	while (1) {
		sw1_pressed = on_press(sw1_pressed, PIND, 2, 1);
		sw2_pressed = on_press(sw2_pressed, PIND, 4, -1);
	}
}
