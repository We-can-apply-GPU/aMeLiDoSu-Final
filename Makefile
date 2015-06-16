CFLAGS=-std=c++11
all: input

o3: CFLAGS+=-O3
o3: all

input: input.cpp 
	g++ input.cpp $(CFLAGS) -o bin/input
clean:
	rm bin/input
