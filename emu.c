#include <stdio.h>
#include <unistd.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "cpu.h"

int startSDL();
void closeSDL();
void startEmu();
void drawMemory();
void drawString();

/* macros */
#define TITLE "6502 Emulator"
#define WIDTH 960
#define HEIGHT 720

/* Variables */
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Bus nes;
CPU cpu_descriptor;
CPU* cpu;
TTF_Font* font = NULL;
SDL_Surface* string = NULL;
SDL_Color fontColor = {0x00, 0x00, 0xFF, 0xFF};
SDL_Texture* stringTexture = NULL;

/* 
 * starts the SDL system for graphics
 * returns 1 on success
 * returns 0 on fail
 */
int
startSDL()
{
	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 0;
	}

	/* load font renderer */
	if (TTF_Init()) {
		fprintf(stderr, "SDL could not initialize TTF font rendering! SDL_Error: %s\n", SDL_GetError());
		return 0;
	}

	/* create window */
	window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		fprintf(stderr, "Window could not be created! SDLE_Error: %s\n", SDL_GetError());
		return 0;
	}

	/* create renderer */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		fprintf(stderr, "Renderer could not be created! SDL_Error:%s\n", SDL_GetError());
		return 0;
	}


	

	/* load font into memory */
	font = TTF_OpenFont("./clacon.ttf", 30);
	if(font == NULL){
		printf("Could not load font! TTF_Error: %s\n", TTF_GetError());
	}

	/* set initial color */
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	return 1;
}

/* 
 * clean up SDL and close system
 */
void
closeSDL()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	renderer= NULL;
	window = NULL;

	TTF_CloseFont(font);
	font = NULL;

	TTF_Quit();
	SDL_Quit();
}

void
startEmu()
{
	

	bus_clearMem(&nes);
	cpu = &cpu_descriptor;
	cpu->bus = &nes;

	
	// Load Program (assembled at https://www.masswerk.at/6502/assembler.html)
	/*
		*=$8000
		LDX #10
		STX $0000
		LDX #3
		STX $0001
		LDY $0000
		LDA #0
		CLC
		loop
		ADC $0001
		DEY
		BNE loop
		STA $0002
		NOP
		NOP
		NOP
	*/
	char* ss = strdup("48 28 08 68 8D 87 13 48 28 08 68 8D 86 13 60");
	unsigned short nOffset = 0x8000;
	char* hex_str;
	unsigned char value;
	while ((hex_str = strsep(&ss, " ")) != NULL) {
		value = strtol(hex_str, NULL, 16);
		nes.ram[nOffset++] = value;
	}

	/* set reset vectors */
	nes.ram[0xFFFC] = 0x00;
	nes.ram[0xFFFD] = 0x80;

	cpu_reset(cpu);
}

void
drawMemory()
{

	char* temp = malloc (sizeof(char) * 3);

	for(int i = 0; i < 16*16; i++) {
		sprintf(temp, "%02X", nes.ram[i]);
		drawString(temp, (i%16) * 30, (i/16) * 20);
	}

	free(temp);

}

void
drawString(char* chars, int x, int y)
{
	string = TTF_RenderText_Solid(font, chars, fontColor);
	stringTexture = SDL_CreateTextureFromSurface(renderer, string);

	int w, h;
	SDL_QueryTexture(stringTexture, NULL, NULL, &w, &h);
	SDL_Rect location = {x, y, w, h};
	SDL_RenderCopy(renderer, stringTexture, NULL, &location);

	SDL_DestroyTexture(stringTexture);
	SDL_FreeSurface(string);
}

int
main()
{
	/* loop flag */
	int quit = 0;

	/* Event handler */
	SDL_Event e;
	
	startSDL();	

	startEmu();

	while(0){
	
		for(int i = 0; i < 16*16; i++) {
			printf("%2X ", nes.ram[i]);
			if (i%16 == 0) {
				printf("\n");
			}

		}

		printf("\n");

		for(int i = 0x0100; i < (16*16) + 0x0100; i++) {
			printf("%2X ", nes.ram[i]);
			if (i%16 == 0) {
				printf("\n");
			}

		}

		do
		{
			cpu_clock(cpu);
		} while (cpu->cycles != 0);

		getchar();
	}

	while ( !quit ) {
		/* handle events on the queue */
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = 1;
			}
		}


		/* drawing */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		/* draw ram */
		drawMemory();

		/* update screen */
		SDL_RenderPresent(renderer);
	}

	closeSDL();

	return 0;
}
