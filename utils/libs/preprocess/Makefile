CC = g++
AR = ar crs

IFLAGS = -I../
CFLAGS = -std=c++17 -O3 -Wall -Wextra -pedantic-errors $(IFLAGS)

LIB = libpreprocess.a

all: $(LIB)

SRCS = preprocess.cpp
OBJS = $(SRCS:%.cpp=%.o)

preprocess.o : preprocess.cpp preprocess.hpp

$(OBJS):
	$(CC) $(CFLAGS) -c $<

$(LIB): $(OBJS)
	$(AR) $@ $(OBJS)

.PHONY: clean test

clean:
	rm *.a *.o *.out
	#(cd tests && make clean)

#test: all
#	(cd tests && make && make run) || exit 1
