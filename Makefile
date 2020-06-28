.PHONY: clean	

all: main

main: main.o AES.o Handler.o
	g++  -std=c++11 main.o AES.o Handler.o -o main

AES.o: AES.cpp AES.h
	g++ -c AES.cpp

Handler.o: Handler.cpp Handler.h
	g++ -c Handler.cpp

main.o: main.cpp
	g++ -c main.cpp 

.PRECIOUS: %.o

clean:
	rm -f *.o