#include <stdio.h>
#include <unistd.h>
#include <SDL.h>
#include <getopt.h>

#include "cpu.h"

int startSDL();
void closeSDL();

/* Macros for parsing */
// TODO - Add them if needed.

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
	 printf("Usage: \n");

 }

int
main(int argc, char* argv)
{
	/* Params for getopt */
	int num_iters = -1;
	int ch;
	int option_index = 0;

	/* Defines the options and their long/short equivalents. */
	struct option longopts[] = {
		// TODO - Add all options.
	};

	/* loop flag */
	int quit = 0;

	/* Event handler */
	SDL_Event e;

	/* Processes the command-line parameters */
	while ((ch = getopt_long(argc, argv, "", longopts, &option_index)) != -1) {
		switch (ch) {
			default: 
				break;
		}
	}
	
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
