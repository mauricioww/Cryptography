PATH1 = -I"/usr/lib/jvm/java-14-openjdk/include"
PATH2 = -I"/usr/lib/jvm/java-14-openjdk/include/linux"

.PHONY: clean	

all: main javaDriver.h javaDriver.class libmiddleware.so #plain.class

main: main.o AES.o Library.o
	g++  -std=c++11 main.o AES.o Library.o -o main

AES.o: AES.cpp AES.h
	g++ -c AES.cpp

Library.o: Library.cpp Library.h
	g++ -c Library.cpp

main.o: main.cpp
	g++ -c main.cpp
	
javaDriver.h: javaDriver.java
	javac -h . javaDriver.java

javaDriver.class:
	javac javaDriver.java

libmiddleware.so: c++driver.cpp Library.cpp AES.cpp javaDriver.h Menu.h
	g++ -Wall -fPIC $(PATH1) $(PATH2) $< Library.cpp AES.cpp -shared -o $@

# plain.class: plain.java
# 	javac plain.java

.PRECIOUS: *.o

clean:
	rm -f *.o