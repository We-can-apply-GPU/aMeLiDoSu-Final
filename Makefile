CFLAGS=-std=c++11
all: DNN/train/input DNN/predict/input HMM/viterbi

o3: CFLAGS+=-O3
o3: all

DNN/train/input: DNN/train/input.cpp 
	g++ DNN/train/input.cpp $(CFLAGS) -o DNN/train/input

DNN/predict/input: DNN/predict/input.cpp 
	g++ DNN/predict/input.cpp $(CFLAGS) -o DNN/predict/input

HMM/viterbi: viterbi.o trans.o
	g++ viterbi.o trans.o $(CFLAGS) -o HMM/viterbi
	
viterbi.o: HMM/viterbi.cpp
	g++ HMM/viterbi.cpp $(CFLAGS) -c

trans.o: HMM/trans.cpp
	g++ HMM/trans.cpp $(CFLAGS) -c

clean:
	rm *.o DNN/train/input DNN/predict/input HMM/viterbi
