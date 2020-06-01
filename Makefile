CC = g++
AR = ar crs

CFLAGS = -std=c++17 -O3 -Wall -Wextra -pedantic-errors

LIB = libqtc.a

all: $(LIB)

SRCS = qtc.cpp
OBJS = $(SRCS:%.cpp=%.o)

qtc.o : qtc.cpp qtc.hpp

$(OBJS): 
	$(CC) $(CFLAGS) -c $<

$(LIB): $(OBJS)
	$(AR) $@ $(OBJS)

.PHONY: clean test

clean:
	rm *.a *.o
	(cd tests && make clean) || exit 1

test: all
	(cd tests && make && make run) || exit 1
