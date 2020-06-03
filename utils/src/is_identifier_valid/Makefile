CC = g++

IFLAGS = -I../
CFLAGS = -std=c++17 -O3 -Wall -Wextra -pedantic $(IFLAGS)

NAME = is_identifier_valid
SRC = $(NAME).cpp
BIN = ../../bin/$(NAME)

all: $(BIN)

.PHONY: clean

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $< -o $@
	cp $@ $(NAME).out

clean:
	rm $(BIN) *.out
