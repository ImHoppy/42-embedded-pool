#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define check_bit(reg, pin) ((reg & (1 << pin)) >> pin)

volatile uint8_t number = 0;

void update_led() { PORTB = (number & 0b111) | ((number & 0b1000) << 1); }

// Interrupt Service Routine
// EIFR = Interrupt Flag Register
// INTF0 = Position in IFR
ISR(INT0_vect)
{
	static uint8_t sw_pressed = 0;

	if (!sw_pressed)
	{
		number = ((number + 1) % 16);
		// Set Sense Control to Rising Edge
		// TODO: Test 01 for any change
		EICRA |= (0b11 << ISC00);
		sw_pressed = 1;
	}
	else
	{
		// Reset Sense Control to Falling Edge
		EICRA |= (0b10 << ISC00);
		sw_pressed = 0;
	}
	update_led();
}

// PCIFR = Interrupt Flag Register
// PCIF2 = Position in IFR
ISR(PCINT2_vect)
{
	static uint8_t sw_pressed = 0;

	if (!sw_pressed)
	{
		number = ((number - 1) % 16);
		// Set Sense Control to Rising Edge
		sw_pressed = 1;
	}
	else
	{
		// Reset Sense Control to Falling Edge
		sw_pressed = 0;
	}
	update_led();
}

// External interrupt --> INT0 et/ou INT1 + PCINT23...0
// --> Must be configured as OUTPUT
//  PCINT[7:0]      --> PCI0
//  PCINT[14:8]     --> PCI1
//  PCINT[23:16]    --> PCI2
// EICRA – External Interrupt Control Register A (PAGE 80)
// EIMSK – External Interrupt Mask Register (PAGE 81)
// SREG - Status Register
// EIFR – External Interrupt Flag Register

int main()
{
	cli();

	// Put LED 1 as output
	DDRB = (1 << PINB0) | (1 << PINB1) | (1 << PINB2) | (1 << PINB4);
	EICRA |= (0b00 << ISC00); // Low level --> interrupt request for INT0
	PCIFR |= (0b00 << PCIF2); // Falling Edge -> interrupt request for
	//   PCINT20
	EIMSK |= (0b1 << INT0);		// Set INTO (SW1) to be interrupted
	PCMSK2 |= (0b1 << PCINT20); // Set PCINT20 (SW2) to be interrupted
	PCICR |= (0b1 << PCIE2);	// Enable interrupt for PCINT[23:16] to PCINT2_vect
	// Activate interrupt routine
	sei();
	update_led();

	while (1)
	{
	}
}
