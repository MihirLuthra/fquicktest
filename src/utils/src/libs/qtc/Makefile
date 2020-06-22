include ../../Mk/general.mk

IFLAGS = -I../
CXXFLAGS += $(IFLAGS)

LIB_DIR = ../../../libs
LIB = $(LIB_DIR)/libqtc.so

.PHONY: all clean test

all: $(LIB)

SRCS = qtc.cpp
OBJS = $(SRCS:%.cpp=%.o)

qtc.o : qtc.cpp qtc.hpp ../quicktest_identifier/quicktest_identifier.hpp

$(OBJS): 
	$(CXX) $(CXXFLAGS) -fPIC -c $<

$(LIB): $(OBJS)
	$(CXX) $(CXXFLAGS) -fPIC -shared -o $@ $(OBJS)

clean:
	rm *.o $(LIB)
