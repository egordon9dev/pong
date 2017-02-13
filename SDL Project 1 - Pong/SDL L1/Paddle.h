#pragma once
#include <SDL/SDL.h>
#include "Texture.h"

class Paddle
{
public:
	//initialize variables
	Paddle();

	//set the velocity to v and move the paddle based on the velocity
	void Move(int v);

	void Render(SDL_Renderer* Renderer);
	
	//get and set boxes
	void SetBox(int x, int y);
	SDL_Rect GetBox() { return m_Box; }

private:
	SDL_Rect m_Box;
	int Vel;
};

