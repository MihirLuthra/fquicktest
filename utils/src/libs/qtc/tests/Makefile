CC = g++

LFLAGS = -L../../../../libs  -lqtc
IFLAGS = -I../../

CFLAGS = -std=c++17 -O3 -Wall -Wextra -pedantic -fsanitize=address $(IFLAGS) $(LFLAGS)

SRC = test_qtc.cpp
BIN = test_qtc.out

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean run

clean:
	rm $(BIN)

run: $(BIN)
	./$(BIN)
