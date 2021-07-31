CC=gcc
FLAGS= -O3 -static -fopenmp -std=gnu99 -D_GNU_SOURCE -DNUM_SIM_CORES=16 -pipe -mmmx -msse -msse2 -msse3 -mpopcnt -mno-ssse3 -mno-sse4.1 -mno-sse4.2 -mno-sse4 -mno-avx

all: mmul mvmul dft brightness rgb2gray manhattan hamming wordcount exactmatch fuzzymatch cnn pagerank
clean:
	rm -rf *.exe
	rm -rf *~

mmul: ./mmul.cpp
	$(CC) $(FLAGS) -o ../bin/mmul.exe ./mmul.cpp

mvmul: ./mvmul.cpp
	$(CC) $(FLAGS) -o ../bin/mvmul.exe ./mvmul.cpp

dft: ./dft.cpp
	$(CC) $(FLAGS) -o ../bin/dft.exe ./dft.cpp

brightness: ./brightness.cpp
	$(CC) $(FLAGS) -o ../bin/brightness.exe ./brightness.cpp

rgb2gray: ./rgb2gray.cpp
	$(CC) $(FLAGS) -o ../bin/rgb2gray.exe ./rgb2gray.cpp

manhattan: ./manhattan.cpp
	$(CC) $(FLAGS) -o ../bin/manhattan.exe ./manhattan.cpp

hamming: ./hamming.cpp
	$(CC) $(FLAGS) -o ../bin/hamming.exe ./hamming.cpp

wordcount: ./wordcount.cpp
	$(CC) $(FLAGS) -o ../bin/grep.exe ./wordcount.cpp

exactmatch: ./exactmatch.cpp
	$(CC) $(FLAGS) -o ../bin/exactMatch.exe ./exactmatch.cpp

fuzzymatch: ./fuzzymatch.cpp
	$(CC) $(FLAGS) -o ../bin/fuzzyMatch.exe ./fuzzymatch.cpp

cnn: ./lenetFull.cpp
	$(CC) $(FLAGS) -o ../bin/lenet.exe ./lenetFull.cpp

pagerank: ./pagerank.cpp
	$(CC) $(FLAGS) -o ../bin/pagerank.exe ./pagerank.cpp
