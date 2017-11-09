# makefile for enVR

CC = g++ --std=c++11 -Wall
thegame : thegame.cpp
	$(CC) thegame.cpp -o thegame

clean:
	rm -f thegame
