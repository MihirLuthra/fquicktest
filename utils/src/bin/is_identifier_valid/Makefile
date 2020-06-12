CC = g++

IFLAGS = -I../../libs
CFLAGS = -std=c++17 -O3 -Wall -Wextra -pedantic $(IFLAGS)

NAME = is_identifier_valid
SRC = $(NAME).cpp
BIN = ../../../bin/$(NAME)

.PHONY: all clean

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $< -o $@
	cp $@ $(NAME).out

clean:
	rm $(BIN) *.out
