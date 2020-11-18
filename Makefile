# -*-Makefile-*-

CC=gcc
FLAGS=-W -Wall -g

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

paper: paper.dvi
	dvipdfm paper.dvi
	xdg-open paper.pdf

paper.dvi: paper.tex
	latex paper.tex
