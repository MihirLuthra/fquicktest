CC = g++

LFLAGS = -L../../../libs/qtc -lqtc
IFLAGS = -I../../../libs
CFLAGS = -std=c++17 -O3 -Wall -Wextra -pedantic -DNDEBUG $(LFLAGS) $(IFLAGS)

BIN_DIR = ../../../../bin

NAME = remove_key
SRC = $(NAME).cpp
BIN = $(NAME)

.PHONY: all clean libs

all:: $(BIN)

$(BIN) : $(SRC) libs
	$(CC) $(CFLAGS) $< -o $(BIN_DIR)/$(BIN)
	cp $(BIN_DIR)/$(BIN) $(NAME).out


clean:
	rm $(BIN_DIR)/$(BIN) *.out

libs:
	@(cd ../../../libs/qtc && make) || exit 1
