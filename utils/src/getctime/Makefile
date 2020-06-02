CC = gcc
CFLAGS = -O3 -Wall -Wextra -DNDEBUG

BIN_DIR = ../../bin

SRC = getctime.c
BIN = getctime

all:: $(BIN)

$(BIN) : $(SRC)
	$(CC) $(CFLAGS) $< -o $(BIN_DIR)/$(BIN)

.PHONY: clean

clean:
	rm $(BIN_DIR)/$(BIN)
