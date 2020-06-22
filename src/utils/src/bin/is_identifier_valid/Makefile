include ../../Mk/general.mk

IFLAGS = -I../../libs
CXXFLAGS += $(IFLAGS)

NAME = is_identifier_valid
SRC = $(NAME).cpp
BIN = ../../../bin/$(NAME)

.PHONY: all clean

all: $(BIN)

$(BIN): $(SRC) ../../libs/quicktest_identifier/quicktest_identifier.hpp
	$(CXX) $(CXXFLAGS) $< -o $@
	cp $@ $(NAME).out

clean:
	rm $(BIN) *.out
