CFLAGS=-std=c++11
all: input 1943to48 test48

o3: CFLAGS+=-O3
o3: all

input: input.cpp 
	g++ input.cpp $(CFLAGS) -o input
1943to48: 1943to48.cpp trans.o
	g++ 1943to48.cpp trans.o $(CFLAGS) -o 1943to48
test48: test48.cpp trans.o
	g++ test48.cpp trans.o $(CFLAGS) -o test48
trans.o: trans.cpp
	g++ trans.cpp $(CFLAGS) -c
clean:
	rm input 1943to48 test48
