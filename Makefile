# -*-Makefile-*-

CC=gcc
FLAGS=-W -Wall -g

all: emu

emu: emu.o
	$(CC) emu.o $(FLAGS) -o emu

emu.o: emu.c
	$(CC) emu.c $(FLAGS) -c -o emu.o

clean:
	rm -f *.o emu
