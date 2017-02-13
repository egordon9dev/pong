#ifndef DOT_H
#define DOT_H

#include <string>
#include <SDL/SDL.h>
#include "Texture.h"
class Dot
{
public:

	//initialize variables
	Dot();

	//render dot
	void Render(SDL_Renderer* renderer);

	//reverse the sign of the x and y velocities
	void Bounce();

	//move dot
	void Dot::Move(SDL_Rect topPaddle, SDL_Rect bottomPaddle);

	//get collision box of the dot
	SDL_Rect& GetBox() { return m_Box; }

	//set x, y, w, and h of m_Box
	void SetBox(int x, int y, int w, int h);

	//set velocities
	void setVel(int xVel, int yVel);

	//set the dot back to the middle of the screen
	void Respawn();

	//render particle effect
	void RenderTrail();

private:

	SDL_Rect m_Box;

	// x and y velocity
	int m_xVel = 0, m_yVel = 0;
};
#endif