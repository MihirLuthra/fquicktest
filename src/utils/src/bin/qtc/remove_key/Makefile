CC = g++

LFLAGS = -L../../../../libs -lqtc
IFLAGS = -I../../../libs
CFLAGS = -std=c++17 -O3 -Wall -Wextra -pedantic -DNDEBUG $(IFLAGS)

BIN_DIR = ../../../../bin

NAME = remove_key
SRC = $(NAME).cpp
BIN = $(BIN_DIR)/$(NAME)

.PHONY: all clean 

all: $(BIN)

$(BIN) : $(SRC) ../../../libs/error/error.h
	$(CC) $(CFLAGS) $< -o $(BIN) $(LFLAGS)
	cp $(BIN) $(NAME).out

clean:
	rm $(BIN) *.out
