#compiler
CXX = g++

#compile flags
CXXFLAGS = -std=c++20 -Wall -Wextra
LDFLAGS = -lgpiod

#target executable name
TARGET = feeder_main

#src files
SRCS = feeder_main.cpp FeedDispenser.cpp FeederScheduler.cpp

#object files
OBJS = $(SRCS:.cpp=.o)

#include path (include folder is one level up)
INCLUDE_PATH = ../includes

#default target
all: $(TARGET)

#link
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

#compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_PATH) -c $< -o $@

#clean up build artifacts
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
