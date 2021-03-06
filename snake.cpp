#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>

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

class Character {
protected:
	std::string m_name;
	int m_velocity, m_directionX, m_directionY, m_randomX, m_randomY;
	SDL_Texture *m_character;
	SDL_Rect m_characterDescription;
	void m_LoadCharacter(const char *file) {
		m_character = IMG_LoadTexture(renderer, file);
	}
public:
	Character(){
	m_characterDescription.x = m_characterDescription.y = 70;
	m_directionX = m_directionY = 1;
	m_velocity = 10;
	m_randomX = m_randomY = 0;
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
	virtual void Render() {}
	virtual void NewDot() {}
};

class Snake : public Character{
public:
	Snake(const char *file, int width, int height) : Character() {
		m_name = "snake";
		m_LoadCharacter(file);
		m_characterDescription.w = width;
		m_characterDescription.h = height;
		m_characterDescription.x = width;
		m_characterDescription.y = height;
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
};

class Dot : public Character{
public:
	Dot(const char *file, int width, int height) : Character() {
		m_name = "dot";
		m_LoadCharacter(file);
		m_characterDescription.w = width;
		m_characterDescription.h = height;
		m_characterDescription.x = m_GetX();//SCREEN_WIDTH / 2 - width / 2;
		m_characterDescription.y = m_GetY();//SCREEN_HEIGHT - height;
		srand(time(0));
	}
	void Render() {
		SDL_RenderCopy(renderer, m_character, 0, &m_characterDescription);
	}
	void NewDot(){
		m_SetX(rand() % 640 + 1);
		m_SetY(rand() % 480 + 1);
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

void OutOfScreen(Character &character) {
	int x = character.m_GetX();				
	int y = character.m_GetY();				
	int w = character.m_GetWidth();			//20
	int h = character.m_GetHeight();		//20

	if ((x + w) > SCREEN_WIDTH || x < 0) {
		if (x < 0) {
			character.m_SetX(0);
		}
		else {
			character.m_SetX(SCREEN_WIDTH - w);
		}
	}

	if ((y + h) > SCREEN_HEIGHT || y < 0) {
		if (y < 0) {
			character.m_SetY(0);
		}
		else {
			character.m_SetY(SCREEN_HEIGHT - h);
		}
	}



	/*
	if ((x < 0) || (x + w) > (SCREEN_WIDTH - w)) {
		character.m_ChangeDirectionX();
		// don't let the paddle to escape from the screen
		if (x < 0) { 
			character.m_SetX(0); 
		}
		else {
			character.m_SetX(SCREEN_WIDTH - h);
		}
	}
	if (y < 0 || (y + w) > SCREEN_HEIGHT) {
		character.m_ChangeDirectionY();
	}
	if ((y + w) > SCREEN_HEIGHT && character.m_GetName() == "dot") {
		exit(-1);
	}
	*/
}

bool collisionDetection(Character &snake, Character &dot) {
	int c1x = snake.m_GetX();
	int c1y = snake.m_GetY();
	int c1w = snake.m_GetWidth();	//20
	int c1h = snake.m_GetHeight();	//20
	int c2x = dot.m_GetX();			//420
	int c2y = dot.m_GetY();			//100
	int c2w = dot.m_GetWidth();		//20
	int c2h = dot.m_GetHeight();	//20

	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of snake
	leftA = c1x;
	rightA = c1x + c1w;
	topA = c1y;
	bottomA = c1y + c1h;

	//Calculate the sides of dot
	leftB = c2x;
	rightB = c2x + c2w;
	topB = c2y;
	bottomB = c2y + c2h;

	// Checks if there is a space between the x and y axis coordinates between the two points
	if (bottomA <= topB){
		return false; //return false
	}
	else if (topA >= bottomB){
		return false;
	}
	else if (rightA <= leftB){
		return false;
	}
	else if (leftA >= rightB){
		return false;
	}
	//If none of the sides from A are outside B
	else {
		return true;
	}
}


int main(int argc, char *argv[])
{
	Init();

	Character *snake = new Snake("snake2.bmp", 20, 20);
	Character *dot = new Dot("dot.bmp", 20, 20);

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
		collisionDetection(*snake, *dot);

		if (collisionDetection(*snake, *dot)) {						// checks IF TRUE
			dot->NewDot();
			dot->Render();
		}
		

		SDL_RenderPresent(renderer);								/* everything before this was drawn behind the scenes this actually
																	puts the colour on screen for us */
		SDL_Delay(70);												// gives us time to see the screen
	}

	Close();

	return 0;
}

