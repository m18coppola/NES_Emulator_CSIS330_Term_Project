#include <stdio.h>
#include <unistd.h>
#include <SDL.h>
#include <getopt.h>

#include "cpu.h"

int startSDL();
void closeSDL();

/* macros */
#define TITLE "6502 Emulator"
#define WIDTH 800
#define HEIGHT 600

/* Variables */
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

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
	window = NULL;

	SDL_Quit();
}

/*
 * Usage Function
 * Called when something isn't right with the command line parameters.
 */
 void
 usage (char* program) {
	 printf("Usage: %s [--viewportA] [--viewportB] [--file filename]\n", program);
 }

int
main(int argc, char* argv[])
{
	/* Input File */
	char file[FILENAME_MAX] = "default.txt";

	/* Viewports */
	char* viewportA = "0x0000";
	char* viewportB = "0x0100";

	/* Params for getopt */
	int ch;
	int option_index = 0;

	/* Defines the options and their long/short equivalents. */
	struct option longopts[] = {
		{ "file", required_argument, NULL, 'f'},
		{ "viewport-a", no_argument, NULL, 'a' },
		{ "viewport-b", no_argument, NULL, 'b' }
	};

	/* loop flag */
	int quit = 0;

	/* Event handler */
	SDL_Event e;

	/* Processes the command-line parameters */
	while ((ch = getopt_long(argc, argv, "a:b:f", longopts, &option_index)) != -1) {
		switch (ch) {

			case 'a':
				strcpy(viewportA, optarg);
				break;

			case 'b':
				strcpy(viewportB, optarg);
				break;

			case 'f':
				strcpy(file, optarg);
				break;

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

	printf("Variables: %s \n %s \n %s \n", file, viewportA, viewportB);
	
	startSDL();	

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

		/* update screen */
		SDL_RenderPresent(renderer);
	}

	closeSDL();

	return 0;
}
