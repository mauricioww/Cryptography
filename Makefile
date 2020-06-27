.PHONY: clean	

all: main

main: AES.o main.o
	g++  -std=c++11  AES.o main.o -o main

AES.o: AES.cpp AES.h
	g++ -c AES.cpp

main.o: main.cpp
	g++ -c main.cpp 

.PRECIOUS: %.o

clean:
	rm -f *.o