#include <stdlib.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event event;

void Init() {
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void Close() {
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}

class GameCharacter {
protected:
	std::string m_name;
	SDL_Rect m_characterDescription;
	int m_velocity, m_directionX, m_directionY;
	SDL_Texture *m_character;
	void m_LoadCharacter(const char *file) {
		m_character = IMG_LoadTexture(renderer, file);
	}
public:
	GameCharacter(const char *file, const char *name, int width, int height, int x, int y) {
		m_characterDescription.w = width;
		m_characterDescription.h = height;
		m_characterDescription.x = x;
		m_characterDescription.y = y;
		m_directionX = m_directionY = 1;
		m_velocity = 23;

		m_name = name;
		m_LoadCharacter(file);
	}
	virtual int m_SetX(int x) { m_characterDescription.x = x; return x; }
	virtual int m_SetY(int y) { m_characterDescription.x = y; return y; }
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
	Dot(const char *file, int width, int height)
		: GameCharacter(file, "dot", width, height, width, height) {
	}
	void m_Move() {
		m_characterDescription.x += m_velocity * m_directionX;
		m_characterDescription.y += m_velocity * m_directionY;
		SDL_RenderCopy(renderer, m_character, 0, &m_characterDescription);
	}
};

class Paddle : public GameCharacter {
public:
	Paddle(const char *file, int width, int height)
		: GameCharacter(file, "paddle", width, height, SCREEN_WIDTH / 2 - width / 2, SCREEN_HEIGHT - height) {
	}
	void m_Move() {
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_RIGHT: m_characterDescription.x += m_velocity; break;
			case SDLK_LEFT: m_characterDescription.x -= m_velocity; break;
			}
		}
		SDL_RenderCopy(renderer, m_character, 0, &m_characterDescription);
	}
};

void OutOfScreen(GameCharacter &character) {
	int x = character.m_GetX();
	int y = character.m_GetY();
	int w = character.m_GetWidth();
	int h = character.m_GetHeight();
	if (x < 0 || (x + w) > SCREEN_WIDTH) {
		character.m_ChangeDirectionX();
		// don't let the paddle to escape from the screen
		if (x < 0) character.m_SetX(0);
		else character.m_SetX(SCREEN_WIDTH - w);
	}
	if (y < 0 || (y + w) > SCREEN_HEIGHT) {
		character.m_ChangeDirectionY();
	}
	if ((y + w) > SCREEN_HEIGHT && character.m_GetName() == "dot") {
		exit(-1);
	}
}

void CollisionDetection(GameCharacter &dot, GameCharacter &paddle) {
	int c1x = dot.m_GetX();
	int c1y = dot.m_GetY();
	int c1w = dot.m_GetWidth();
	int c1h = dot.m_GetHeight();
	int c2x = paddle.m_GetX();
	int c2y = paddle.m_GetY();
	int c2w = paddle.m_GetWidth();
	int c2h = paddle.m_GetHeight();
	if (c1y + c1h > SCREEN_HEIGHT - c2h && (c1x > c2x - c2w && c1x < c2x + c2w)) {
		dot.m_ChangeDirectionY();
	}
}

int main(int argc, char *argv[])
{
	Init();

	GameCharacter *paddle = new Paddle("paddle.bmp", 100, 16);
	GameCharacter *dot = new Dot("dot.bmp", 20, 20);

	bool quit = false;

	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		dot->m_Move();
		paddle->m_Move();
		OutOfScreen(*dot);
		OutOfScreen(*paddle);
		CollisionDetection(*dot, *paddle);

		SDL_RenderPresent(renderer);

		SDL_Delay(100);
	}

	Close();

	return 0;
}

