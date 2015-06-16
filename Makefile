CFLAGS=-std=c++11
all: input viterbi

o3: CFLAGS+=-O3
o3: all

input: src/input.cpp 
	g++ src/input.cpp $(CFLAGS) -o bin/input
viterbi: src/viterbi.cpp trans.o
	g++ src/viterbi.cpp trans.o $(CFLAGS) -o bin/viterbi
trans.o: src/trans.cpp
	g++ src/trans.cpp $(CFLAGS) -c
clean:
	rm bin/input bin/viterbi trans.o
