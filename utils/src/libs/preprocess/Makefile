CC = g++

IFLAGS = -I../
CFLAGS = -std=c++17 -O3 -Wall -Wextra -pedantic-errors $(IFLAGS)

LIB_DIR = ../../../libs
LIB = libpreprocess.so

.PHONY: all clean test

all: $(LIB)

SRCS = preprocess.cpp
OBJS = $(SRCS:%.cpp=%.o)

preprocess.o : preprocess.cpp preprocess.hpp

$(OBJS):
	$(CC) $(CFLAGS) -c $<

$(LIB): $(OBJS)
	$(CC) $(CFLAGS) -fPIC -shared -o $(LIB_DIR)/$@ $(OBJS)

clean:
	rm *.o *.so $(LIB_DIR)/$(LIB)
	#(cd tests && make clean)

#test: all
#	(cd tests && make && make run) || exit 1
