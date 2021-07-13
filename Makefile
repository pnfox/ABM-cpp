SHELL = /bin/sh
CXX = g++
CFLAGS = -Wall -std=c++17

%.o: %.cpp agents.h simulation.h
	$(CXX) $(CFLAGS) -c -o $@ $<

all: simulation.o agents.o
	$(CXX) $(CFLAGS) -o simulation main.cpp simulation.o agents.o

clean:
	rm simulation simulation.o agents.o
