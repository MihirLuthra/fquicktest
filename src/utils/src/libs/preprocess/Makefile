include ../../Mk/general.mk

IFLAGS = -I../
CXXFLAGS += $(IFLAGS)

LIB_DIR = ../../../libs
LIB = $(LIB_DIR)/libpreprocess.so

.PHONY: all clean test

all: $(LIB)

SRCS = preprocess.cpp
OBJS = $(SRCS:%.cpp=%.o)

preprocess.o : preprocess.cpp preprocess.hpp ../quicktest_identifier/quicktest_identifier.hpp

$(OBJS):
	$(CXX) $(CXXFLAGS) -fPIC -c $<

$(LIB): $(OBJS)
	$(CXX) $(CXXFLAGS) -fPIC -shared -o $@ $(OBJS)

clean:
	rm *.o $(LIB)
	#(cd tests && make clean)

#test: all
#	(cd tests && make && make run) || exit 1
