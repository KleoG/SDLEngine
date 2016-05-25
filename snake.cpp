#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>

const int     SCREEN_WIDTH = 640;
const int     SCREEN_HEIGHT = 480;

SDL_Window    *window = NULL;
SDL_Renderer  *renderer = NULL;
SDL_Event     event;

void Init() {
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void Close() {
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}

class Snake {
protected:
	std::string m_name;
	int m_velocity, m_directionX, m_directionY;
	SDL_Texture *m_character;
	SDL_Rect m_characterDescription;
	void m_LoadCharacter(const char *file) {
		m_character = IMG_LoadTexture(renderer, file);
	}
public:
	Snake(const char *file, int width, int height) {
		m_name = "snake";
		m_LoadCharacter(file);
		m_characterDescription.w = width;
		m_characterDescription.h = height;
		m_characterDescription.x = width;
		m_characterDescription.y = height;
		m_directionX = m_directionY = 1;
		m_velocity = 25;
	}
	void m_Move() {
		if (event.type == SDL_KEYUP || SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_UP: m_characterDescription.y = m_characterDescription.y - m_velocity; break;
			case SDLK_DOWN: m_characterDescription.y = m_characterDescription.y + m_velocity; break;
			case SDLK_RIGHT: m_characterDescription.x += m_velocity; break;
			case SDLK_LEFT: m_characterDescription.x -= m_velocity; break;
			}
		}
		SDL_RenderCopy(renderer, m_character, 0, &m_characterDescription);
	}
	int m_SetX(int x) { m_characterDescription.x = x; return m_characterDescription.x; }
	int m_SetY(int y) { m_characterDescription.y = y; return m_characterDescription.y; }
	int m_GetX() { return m_characterDescription.x; }
	int m_GetY() { return m_characterDescription.y; }
	int m_GetWidth() { return m_characterDescription.w; }
	int m_GetHeight() { return m_characterDescription.h; }
	std::string m_GetName() { return m_name; }
	void m_ChangeDirectionX() { m_directionX *= -1; }
	void m_ChangeDirectionY() { m_directionY *= -1; }
};

class Dot {
protected:
	std::string m_name;
	int m_velocity, m_directionX, m_directionY;
	SDL_Texture *m_character;
	SDL_Rect m_characterDescription;
	void m_LoadCharacter(const char *file) {
		m_character = IMG_LoadTexture(renderer, file);
	}
public:
	Dot(const char *file, int width, int height) {
		m_name = "dot";
		m_LoadCharacter(file);
		m_characterDescription.w = width;
		m_characterDescription.h = height;
		m_characterDescription.x = SCREEN_WIDTH / 2 - width / 2;
		m_characterDescription.y = SCREEN_HEIGHT - height;
	}
	void Render() {
		SDL_RenderCopy(renderer, m_character, 0, &m_characterDescription);
	}
};

/*
class GameCharacter {
protected:
	std::string m_name;
	int m_velocity, m_directionX, m_directionY;
	SDL_Texture *m_character;
	SDL_Rect m_characterDescription;
	void m_LoadCharacter(const char *file) {
		m_character = IMG_LoadTexture(renderer, file);
	}
public:
	GameCharacter() {
		m_characterDescription.x = m_characterDescription.y = 0;
		m_directionX = m_directionY = 1;
		m_velocity = 25;
	}
	virtual int m_SetX(int x) { m_characterDescription.x = x; return m_characterDescription.x; }
	virtual int m_SetY(int y) { m_characterDescription.y = y; return m_characterDescription.y; }
	virtual int m_GetX() { return m_characterDescription.x; }
	virtual int m_GetY() { return m_characterDescription.y; }
	virtual int m_GetWidth() { return m_characterDescription.w; }
	virtual int m_GetHeight() { return m_characterDescription.h; }
	virtual std::string m_GetName() { return m_name; }
	virtual void m_ChangeDirectionX() { m_directionX *= -1; }
	virtual void m_ChangeDirectionY() { m_directionY *= -1; }
	virtual void m_Move() {}
};

class Dot : public GameCharacter {
public:
	Dot(const char *file, int width, int height) : GameCharacter() {
		m_name = "dot";
		m_LoadCharacter(file);
		m_characterDescription.w = width;
		m_characterDescription.h = height;
		m_characterDescription.x = width;
		m_characterDescription.y = height;
	}
	void m_Move() {
		m_characterDescription.x = m_characterDescription.x + m_velocity * m_directionX;
		m_characterDescription.y = m_characterDescription.y + m_velocity * m_directionY;
		SDL_RenderCopy(renderer, m_character, 0, &m_characterDescription);
	}
};

class Paddle : public GameCharacter {
public:
	Paddle(const char *file, int width, int height) : GameCharacter() {
		m_name = "paddle";
		m_LoadCharacter(file);
		m_characterDescription.w = width;
		m_characterDescription.h = height;
		m_characterDescription.x = SCREEN_WIDTH / 2 - width / 2;
		m_characterDescription.y = SCREEN_HEIGHT - height;
	}
	void m_Move() {
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_UP: m_characterDescription.x = m_characterDescription.x + m_velocity; break;
			case SDLK_DOWN: m_characterDescription.x -= m_velocity; break;
			case SDLK_RIGHT: m_characterDescription.x += m_velocity; break;
			case SDLK_LEFT: m_characterDescription.x -= m_velocity; break;
			}
		}
		SDL_RenderCopy(renderer, m_character, 0, &m_characterDescription);
	}
};

*/

void OutOfScreen(Snake &character) {
	int x = character.m_GetX();
	int y = character.m_GetY();
	int w = character.m_GetWidth();
	int h = character.m_GetHeight();
	if (x < 0 || (x + w) > SCREEN_WIDTH) {
		character.m_ChangeDirectionX();
		// don't let the paddle to escape from the screen
		if (x < 0) { 
			character.m_SetX(0); 
		}
		else {
			character.m_SetX(SCREEN_WIDTH - w);
		}
	}
	if (y < 0 || (y + w) > SCREEN_HEIGHT) {
		character.m_ChangeDirectionY();
	}
	if ((y + w) > SCREEN_HEIGHT && character.m_GetName() == "dot") {
		exit(-1);
	}
}

int main(int argc, char *argv[])
{
	Init();

	Snake *snake = new Snake("snake2.bmp", 20, 20);
	Dot *dot = new Dot("dot.bmp", 20, 20);

	bool quit = false;

	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);		// select colour for drawing
		SDL_RenderClear(renderer);									// clear entire screen to our selected colour

		snake->m_Move();  // (*snake).Move();
		dot->Render();
		OutOfScreen(*snake);

		SDL_RenderPresent(renderer);								/* everything before this was drawn behind the scenes this actually
																	puts the colour on screen for us */
		SDL_Delay(70);												// gives us time to see the screen
	}

	Close();

	return 0;
}
