CC = g++
AR = ar crs

IFLAGS = -I../
CFLAGS = -std=c++17 -O3 -Wall -Wextra -pedantic-errors $(IFLAGS)

LIB = libqtc.a

.PHONY: all clean test

all: $(LIB)

SRCS = qtc.cpp
OBJS = $(SRCS:%.cpp=%.o)

qtc.o : qtc.cpp qtc.hpp

$(OBJS): 
	$(CC) $(CFLAGS) -c $<

$(LIB): $(OBJS)
	$(AR) $@ $(OBJS)

clean:
	rm *.a *.o

test: all
	(cd tests && make && make run) || exit 1
