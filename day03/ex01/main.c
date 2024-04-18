#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <uart.h>
#include <timer.h>

ISR(TIMER1_OVF_vect)
{
	uart_printstr("Hello World!\r\n");
}

int main(void)
{
	uart_init(UART_TX);

	timer1_init(2000);
	timer1_OVF();

	while (1)
	{
	}
}
