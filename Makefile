all: main

main: main.o sqlite3.o
	g++ -o main main.o sqlite3.o -pthread -ldl

main.o: main.cpp sqlite3.h
	g++ -c main.cpp

sqlite3.o: sqlite3.h sqlite3.c
	gcc -c sqlite3.c


