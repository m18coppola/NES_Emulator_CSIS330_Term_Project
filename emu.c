#include <stdio.h>
#include <unistd.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <getopt.h>

#include "cpu.h"

int startSDL();
void closeSDL();
void startEmu();
void drawMemory();
void drawString();
void usage(char* program);

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
char* viewport1;
char* viewport2;

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
	char* ss = strdup("A9 00 8D F0 00 A9 01 8D F1 00 A2 00 AD F1 00 9D 1B 0F 8D F2 00 6D F0 00 8D F1 00 AD F2 00 8D F0 00 E8 E0 0A 30 E6 60");
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

	int a = strtol(viewport1+2, NULL, 16);
	int b = strtol(viewport2+2, NULL, 16);

	for(int i = 0; i < 16*16; i++) {
		sprintf(temp, "%02X", nes.ram[i + a]);
		drawString((i%16) * 30, (i/16) * 20, temp);
	}

	for(int i = 0; i < 16*16; i++) {
		sprintf(temp, "%02X", nes.ram[i + b]);
		drawString((i%16) * 30, ((i/16) * 20)+350, temp);
	}

	free(temp);

}

void
drawCPU()
{
	char buff[200];
	int x = 500;
	int y = 0;
	drawString(x, y, "STATUS:");
	drawString(x + 100, y, (cpu->status&N)? "N":"-");
	drawString(x + 140, y, (cpu->status&V)? "V":"-");
	drawString(x + 180, y, (cpu->status&U)? "-":"-");
	drawString(x + 220, y, (cpu->status&B)? "B":"-");
	drawString(x + 260, y, (cpu->status&D)? "D":"-");
	drawString(x + 300, y, (cpu->status&I)? "I":"-");
	drawString(x + 340, y, (cpu->status&Z)? "Z":"-");
	drawString(x + 380, y, (cpu->status&C)? "C":"-");
	sprintf(buff, "PC: $%04X", cpu->pc);
	drawString(x, y + 20, buff);
	sprintf(buff, "A: $%04X", cpu->a);
	drawString(x, y + 40, buff);
	sprintf(buff, "X: $%04X", cpu->x);
	drawString(x, y + 60, buff);
	sprintf(buff, "Y: $%04X", cpu->y);
	drawString(x, y + 80, buff);
	sprintf(buff, "STACK: $%04X", cpu->stkp);
	drawString(x, y + 100, buff);
	sprintf(buff, "OPCODE: %s", cpu_getOpcode(cpu));
	drawString(x, y + 120, buff);
}

void
drawString (int x, int y, char* chars)
{ string = TTF_RenderText_Solid(font, chars, fontColor);
	stringTexture = SDL_CreateTextureFromSurface(renderer, string);

	int w, h;
	SDL_QueryTexture(stringTexture, NULL, NULL, &w, &h);
	SDL_Rect location = {x, y, w, h};
	SDL_RenderCopy(renderer, stringTexture, NULL, &location);

	SDL_DestroyTexture(stringTexture);
	SDL_FreeSurface(string);
}
/*
 * Usage Function
 * Called when something isn't right with the command line parameters.
 */
 void
 usage (char* program) {
	 printf("Usage: %s \n[--file filename] [--viewport1] [--viewport2] \n[--initA] [--initX] [--initY]\n", program);
 }

int
main(int argc, char* argv[])
{
	/* Input File */
	char file[FILENAME_MAX] = "default.txt";

	/* Viewports */
	viewport1 = "0x0000";
	viewport2 = "0x0100";

	/* Initial Register Values */
	char* initA = "0x00";
	char* initX = "0x00";
	char* initY = "0x00";

	/* Params for getopt */
	int ch;
	int option_index = 0;

	/* Defines the options and their long/short equivalents. */
	struct option longopts[] = {
		{ "file", required_argument, NULL, 'f'},
		{ "viewport-1", required_argument, NULL, '1' },
		{ "viewport-2", required_argument, NULL, '2' }
	};

	/* loop flag */
	int quit = 0;

	/* Event handler */
	SDL_Event e;

	/* Processes the command-line parameters */
	while ((ch = getopt_long(argc, argv, "f:1:2:a:x:y:h", longopts, &option_index)) != -1) {
		switch (ch) {

			case '1':
				viewport1 = optarg;
				break;

			case '2':
				viewport2 = optarg;
				break;

			case 'f':
				strcpy(file, optarg);
				break;

			case 'a':
				initA = optarg;
				break;

			case 'x':
				initX = optarg;
				break;

			case 'y':
				initY = optarg;
				break;

			case 'h':
				printf("Enter a filename with -f. Change viewport areas with -1 and -2. \nEnter initial register values with -a, -x, and -y.\n\n");
      			usage(argv[0]);
      			return 0;

			default: 
				usage(argv[0]);
				return 1;
		}
	}

	/* Makes sure the arguments were received. */
	if (strcmp(file, "default.txt") == 0) {
		fprintf(stderr, "Required file-name not specified.\n");
    	usage(argv[0]);
    	return 1;
	}

	printf("Variables: %s \n %s \n %s \n %s \n %s \n %s \n", file, viewport1, viewport2, initA, initX, initY);
	
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
			
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					case SDLK_SPACE:
						do {
							cpu_clock(cpu);
						} while (cpu->cycles != 0);
					break;
				}
			}
		}


		/* drawing */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		/* draw ram */
		drawMemory();

		/* draw cpu */
		drawCPU();

		/* update screen */
		SDL_RenderPresent(renderer);
	}

	closeSDL();

	return 0;
}
