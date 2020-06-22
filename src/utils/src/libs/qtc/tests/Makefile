include ../../Mk/general.mk

LFLAGS = -L../../../../libs  -lqtc
IFLAGS = -I../../

CXXFLAGS += $(IFLAGS)

SRC = test_qtc.cpp
BIN = test_qtc.out

$(BIN): $(SRC) ../../../libs/error/error.h
	$(CXX) $(CXXFLAGS) $< -o $@ $(LFLAGS)

.PHONY: clean run

clean:
	rm $(BIN)

run: $(BIN)
	./$(BIN)
