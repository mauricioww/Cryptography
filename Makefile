PATH1 = -I"/usr/lib/jvm/java-14-openjdk/include"
PATH2 = -I"/usr/lib/jvm/java-14-openjdk/include/linux"

.PHONY: clean	

all: main javaDriver.h javaDriver.class libmiddleware.so #plain.class

main: main.o AES.o Handler.o
	g++  -std=c++11 main.o AES.o Handler.o -o main

AES.o: AES.cpp AES.h
	g++ -c AES.cpp

Handler.o: Handler.cpp Handler.h
	g++ -c Handler.cpp

main.o: main.cpp
	g++ -c main.cpp
	
javaDriver.h: javaDriver.java
	javac -h . javaDriver.java

javaDriver.class:
	javac javaDriver.java

libmiddleware.so: c++driver.cpp Handler.cpp AES.cpp javaDriver.h Menu.h Library.cpp
	g++ -Wall -fPIC $(PATH1) $(PATH2) $< Handler.cpp AES.cpp Library.cpp -shared -o $@

# plain.class: plain.java
# 	javac plain.java

.PRECIOUS: *.o

clean:
	rm -f *.o