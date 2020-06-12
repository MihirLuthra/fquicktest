CC = g++

LFLAGS = -L../../libs/preprocess -L../../libs/qtc -lpreprocess -lqtc
IFLAGS = -I../../libs
CFLAGS = -std=c++17 -O3 -Wall -Wextra -pedantic -DNDEBUG $(LFLAGS) $(IFLAGS)

BIN_DIR = ../../bin

NAME = preprocess
SRC = $(NAME).cpp
BIN = $(NAME)

all:: $(BIN)

$(BIN) : $(SRC) libs
	$(CC) $(CFLAGS) $< -o $(BIN_DIR)/$(BIN)
	cp $(BIN_DIR)/$(BIN) $(NAME).out

.PHONY: clean libs

clean:
	rm $(BIN_DIR)/$(BIN) *.out

libs:
	(cd ../../libs/preprocess && make) || exit 1
	(cd ../../libs/qtc && make) || exit 1
