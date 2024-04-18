F_CPU = 16000000UL
CC = avr-gcc
CFLAGS = -Wall -Werror -Wextra -mmcu=atmega328 -DF_CPU=$(F_CPU) -Os
INC = -Iinclude

TARGET_DIR = target/

BIN_PATH = $(TARGET_DIR)bin/
OBJ_PATH = $(TARGET_DIR)obj/
HEX_PATH = $(TARGET_DIR)hex/

ifdef SRC
SRC_PATH = $(dir $(SRC))
SRC_NAME = $(lastword $(subst /, ,$(SRC)))
else
SRC_PATH = src/
SRC_NAME = main.c
endif


OBJ_NAME = $(SRC_NAME:.c=.o)
HEX_NAME = $(SRC_NAME:.c=.hex)
BIN_NAME = $(HEX_NAME:.hex=.bin)

SRC ?= $(addprefix $(SRC_PATH), $(SRC_NAME))
BIN = $(addprefix $(BIN_PATH), $(BIN_NAME))
OBJ = $(addprefix $(OBJ_PATH), $(OBJ_NAME))
HEX = $(addprefix $(HEX_PATH), $(HEX_NAME))

BAUD_RATE = 115200
PORT = /dev/ttyUSB0
DUDE_FLAGS = -b $(BAUD_RATE) -c arduino -p m328p -P $(PORT) -e -U flash:w:$(HEX)

all: hex flash

$(BIN): $(OBJ)
	mkdir -p $(@D)
	$(CC) $(OBJ) $(CFLAGS) $(INC) -o $(BIN)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

hex: $(HEX)

$(HEX): $(BIN)
	mkdir -p $(@D)
	avr-objcopy $(BIN) -O ihex $(HEX)

flash:
	avrdude $(DUDE_FLAGS)

clean:
	rm -r $(TARGET_DIR)

.PHONY: all hex flash clean
