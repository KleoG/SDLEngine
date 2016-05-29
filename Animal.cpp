#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Animal.h"



Animal::Animal(const char *file, int width, int height)
{
	characterDescription.w = width;
	characterDescription.h = height;
	characterDescription.x = (SCREEN_WIDTH/2) - width/2;
	characterDescription.y = (SCREEN_HEIGHT/2) - height/2;

	LoadCharacter(file);
}

void Animal::Render() {
	SDL_RenderCopy(renderer, character, 0, &characterDescription);
}

void Animal::Feed() {
}


Animal::~Animal()
{
}
