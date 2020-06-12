CC = g++

LFLAGS = -L../../../libs -lpreprocess -lqtc
IFLAGS = -I../../libs
CFLAGS = -std=c++17 -O3 -Wall -Wextra -pedantic -DNDEBUG $(LFLAGS) $(IFLAGS)

BIN_DIR = ../../../bin

NAME = preprocess
SRC = $(NAME).cpp
BIN = $(NAME)

.PHONY: all clean

all: $(BIN)

$(BIN) : $(SRC)
	$(CC) $(CFLAGS) $< -o $(BIN_DIR)/$(BIN)
	cp $(BIN_DIR)/$(BIN) $(NAME).out


clean:
	rm $(BIN_DIR)/$(BIN) *.out
