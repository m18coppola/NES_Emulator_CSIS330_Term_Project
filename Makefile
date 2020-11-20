# -*-Makefile-*-

CC=gcc
FLAGS=-W -Wall -g `sdl2-config --libs --cflags`

all: emu

emu: emu.o bus.o cpu.o
	$(CC) emu.o bus.o cpu.o $(FLAGS) -o emu

emu.o: emu.c
	$(CC) emu.c $(FLAGS) -c -o emu.o

bus.o: bus.c
	$(CC) bus.c $(FLAGS) -c -o bus.o

cpu.o: cpu.c
	$(CC) cpu.c $(FLAGS) -c -o cpu.o

clean:
	rm -f *.o emu
