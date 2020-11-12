# -*-Makefile-*-

CC=gcc
FLAGS=-W -Wall -g

all: emu

emu: emu.o bus.o
	$(CC) emu.o bus.o $(FLAGS) -o emu

emu.o: emu.c
	$(CC) emu.c $(FLAGS) -c -o emu.o

bus.o: bus.c
	$(CC) bus.c $(FLAGS) -c -o bus.o

clean:
	rm -f *.o emu
