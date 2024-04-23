#include <avr/io.h>
#include <util/delay.h>
#include <rgb.h>
#include <uart.h>

int main()
{
	init_rgb();
	init_uart();

	while (1)
	{
	}
}
