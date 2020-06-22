include ../../../Mk/general.mk

LFLAGS = -L../../../../libs -lqtc
IFLAGS = -I../../../libs
CXXFLAGS += $(IFLAGS)

BIN_DIR = ../../../../bin

NAME = set_value_for_key
SRC = $(NAME).cpp
BIN = $(BIN_DIR)/$(NAME)

.PHONY: all clean

all: $(BIN)

$(BIN) : $(SRC) ../../../libs/error/error.h
	$(CXX) $(CXXFLAGS) $< -o $(BIN) $(LFLAGS)
	cp $(BIN) $(NAME).out

clean:
	rm $(BIN) *.out
