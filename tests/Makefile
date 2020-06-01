CC = g++

LFLAGS = -L../ -lqtc
IFLAGS = -I../../

CFLAGS = -std=c++17 -O3 -Wall -Wextra -pedantic -fsanitize=address $(IFLAGS) $(LFLAGS)

SRC = test_qtc.cpp
BIN = test_qtc.out

LIBS = ../libqtc.a

$(BIN): $(SRC) $(LIBS)
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean run

clean:
	rm $(BIN)

run: $(BIN)
	./$(BIN)
