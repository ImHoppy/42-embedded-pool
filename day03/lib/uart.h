#pragma once

#define UART_BAUDRATE 115200
#define BAUD ((F_CPU / 16.0 / UART_BAUDRATE) + 0.5) - 1

typedef enum
{
	UART_TX = 1 << TXEN0,
	UART_RX = 1 << RXEN0,
	UART_ALL = UART_TX | UART_RX
} uart_data_type;

/// @brief Initialize UART
/// @param  What uart will be used for (transmit, receive, or both) via UART_x macros
void uart_init(uart_data_type data_type);

// Transmit functions
void uart_tx(char c);
void uart_printstr(const char *str);
void uart_printhex(uint8_t n);

// Receive functions
char uart_rx(void);
