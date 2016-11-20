PROJECT_NAME=Compass

BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/obj

SRC=$(wildcard *.c)
OBJS=$(addprefix $(OBJ_DIR)/,$(notdir $(SRC:.c=.o)))

PROJECT_OBJ=$(BUILD_DIR)/$(PROJECT_NAME).o
PROJECT_HEX=$(BUILD_DIR)/$(PROJECT_NAME).hex

CC=avr-gcc
CC_FLAGS=-mmcu=atmega88 -Wall -g

PROGRAMMER=jtag2isp
MICROCONTROLLER=m88

MKDIR=mkdir -p

all: $(PROJECT_HEX)

$(PROJECT_HEX): $(PROJECT_OBJ)
	avr-objcopy -j .text -j .data -O ihex $(PROJECT_OBJ) $(PROJECT_HEX)

$(PROJECT_OBJ): $(BUILD_DIR) $(OBJS)
	$(CC) $(CC_FLAGS) $(OBJS) -o $(PROJECT_OBJ)

$(OBJ_DIR)/%.o: %.c $(OBJ_DIR)
	$(CC) $(CC_FLAGS) -c -o $@ $<

$(BUILD_DIR):
	$(MKDIR) $@

$(OBJ_DIR):
	$(MKDIR) $@

install-isp: $(PROJECT_HEX)
	sudo avrdude -c $(PROGRAMMER) -p $(MICROCONTROLLER) -U flash:w:$(PROJECT_HEX):i

doc:
	cd ../../ && doxygen && cd src/Compass

.PHONY: clean
clean:
	@rm $(BUILD_DIR) -r
