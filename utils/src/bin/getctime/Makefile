CC = gcc

IFLAGS = -I../../libs/
CFLAGS = -O3 -Wall -Wextra -DNDEBUG -pedantic $(IFLAGS)

BIN_DIR = ../../../bin

NAME = getctime
SRC = $(NAME).c
BIN = $(NAME)

all: $(BIN_DIR)/$(BIN)

$(BIN_DIR)/$(BIN) : $(SRC)
	$(CC) $(CFLAGS) $< -o $@
	cp $@ $(BIN).out

.PHONY: clean

clean:
	rm $(BIN_DIR)/$(BIN) *.out
