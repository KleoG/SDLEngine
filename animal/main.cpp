#include <stdlib.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "main.h"
#include "Animal.h"
#include "Texture.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event event;

void Init() {
	bool success = true;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Tamagochi", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		success = false;
	}
}

void Close() {
	//Destroy window	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;

	// Exit SDL subsystems
	SDL_Quit();
	IMG_Quit();
}

int main(int argc, char *argv[])
{
	Init();

	Animal *animal = new Animal("animal.bmp", 200, 200);

	bool quit = false;

	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}
		
		loadMedia();
		

		//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		//SDL_RenderClear(renderer);
		////////////////////////////////////
		
		//animal->Render();
		runAnimation();
		///////////////////////////////////
		//SDL_RenderPresent(renderer);

		//SDL_Delay(100);
	}

	Close();

	return 0;
}