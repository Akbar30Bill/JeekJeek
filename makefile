CC=g++
STD=-std=c++11 -Wall -pedantic

all: main.o sha512.o
	g++ main.o sha512.o

main.o:main.cpp main.hpp
	g++ -c main.cpp

sha512.o:sha512.cpp sha512.h
	g++ -c sha512.cpp

.INTERMEDIATE: main.o sha512.o
.PHONY: clean
clean:
	rm -rf *.o