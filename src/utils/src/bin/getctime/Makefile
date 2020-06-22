include ../../Mk/general.mk

IFLAGS = -I../../libs/
CFLAGS += $(IFLAGS)

BIN_DIR = ../../../bin

NAME = getctime
SRC = $(NAME).c
BIN = $(NAME)

.PHONY: all clean

all: $(BIN_DIR)/$(BIN)

$(BIN_DIR)/$(BIN) : $(SRC) ../../libs/error/error.h
	$(CC) $(CFLAGS) $< -o $@
	cp $@ $(BIN).out


clean:
	rm $(BIN_DIR)/$(BIN) *.out
