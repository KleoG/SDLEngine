#pragma once
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "main.h"

class Animal
{
public:
	std::string name;
	int health, hunger, thirst, happiness, age;
	float weight;
	SDL_Rect characterDescription;
	SDL_Texture *character;
	void LoadCharacter(const char *file) {
		character = IMG_LoadTexture(renderer, file);
	}

	Animal(const char *file, int width, int height);

	void Render();

	void Feed();

	~Animal();
};


