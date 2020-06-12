CC = g++

LFLAGS = -L../../../../libs -lqtc
IFLAGS = -I../../../libs
CFLAGS = -std=c++17 -O3 -Wall -Wextra -pedantic -DNDEBUG $(LFLAGS) $(IFLAGS)

BIN_DIR = ../../../../bin

NAME = set_value_for_key
SRC = $(NAME).cpp
BIN = $(BIN_DIR)/$(NAME)

.PHONY: all clean

all: $(BIN)

$(BIN) : $(SRC)
	$(CC) $(CFLAGS) $< -o $(BIN)
	cp $(BIN) $(NAME).out

clean:
	rm $(BIN) *.out
