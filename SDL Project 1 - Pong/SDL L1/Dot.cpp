#include "stdafx.h"
#include "Dot.h"
#include <SDL/SDL.h>
#include <iostream>
#include <stdlib.h> // for absolute value function abs(int)
#include <time.h> //for time()

//Dot dimensions
extern int nDotW, nDotH;

//screen dimensions
extern int nWidth, nHeight;

//global dot texture
extern Texture gcDotTexture;

//collision detection function
bool CheckCollision(SDL_Rect a, SDL_Rect b);


Dot::Dot()
{
	//center the dot on the screen
	m_Box = { nWidth / 2, nHeight / 2, nDotW, nDotH };

	m_xVel = 1;
	m_yVel = 1;
}

void Dot::Render(SDL_Renderer* renderer)
{
	//Render dot texture with the Texture class's version of the Render function
	gcDotTexture.Render(renderer, m_Box);
}

void Dot::Bounce()
{
	m_yVel *= -1;
	m_xVel *= -1;
	using namespace std;
	cout << "\n\n\nbounced\nbounced\nbounced\nbounced\nbounced\n\n\n\n";
}

void Dot::Move(SDL_Rect topPaddle, SDL_Rect bottomPaddle)
{

	//apply x velocity
	m_Box.x += m_xVel;

	//move back if the dot goes off the screen on the x axis
	if ((m_Box.x + m_Box.w) >= nWidth)
	{
		//bounce of the right wall
		m_xVel *= -1;
	}

	//move back if the dot goes to the left of the screen
	if (m_Box.x < 0)
	{
		//bounce off the left wall
		m_xVel *= -1;
	}

	//apply y velocity
	m_Box.y += m_yVel;

	//move back if dot goes off the screen on the y axis
	if ((m_Box.y + m_Box.h) >= nHeight)
	{
		//move back
		m_Box.y -= m_yVel;
	}

	//if the dot goes above the screen move back
	if (m_Box.y < 0)
	{
		//set at left of screen
		m_Box.y = 0;
	}

	int nOldxVel = m_xVel;
	int nOldyVel = m_yVel;

	//make ball bounce of the paddles
	//if the dot hits a paddle
	if (CheckCollision(m_Box, topPaddle) || CheckCollision(m_Box, bottomPaddle))
	{
		//if dot is on bottom half of screen set it to a negative velocity (up)
		if (m_Box.y >= (nHeight / 2))
			m_yVel = -abs(m_yVel);

		//if dot is on top half of screen set it to a posative velocity (down)
		if (m_Box.y <= (nHeight / 2))
			m_yVel = abs(m_yVel);

		//if the ball hits one of the paddles on the left side
		if ((m_Box.x + m_Box.w) < (topPaddle.x + (topPaddle.w / 2)) || (m_Box.x + m_Box.w) < (bottomPaddle.x + (bottomPaddle.w / 2)))
		{
			//bounce of the paddle
			m_xVel = -abs(m_xVel);
		}

		//if the ball hits one of the paddles on the right side
		if ((m_Box.x + m_Box.w) > (bottomPaddle.x + (topPaddle.w / 2)) || (m_Box.x + m_Box.w) > (bottomPaddle.x + (bottomPaddle.w / 2)))
		{
			//bounce of the paddle
			m_xVel = abs(m_xVel);
		}

		//if the dot hits the top of a paddle
		if (m_Box.y <= (topPaddle.y + topPaddle.w) || (m_Box.y + m_Box.w) >= bottomPaddle.y)
		{
			//if the dot hits the middle of a paddle
			if (m_Box.x > (bottomPaddle.x + 5) && (m_Box.x + m_Box.w) < (bottomPaddle.x + bottomPaddle.w - 5))
				m_xVel *= -1;
		}

	}
}

void Dot::Respawn()
{
	m_Box.x = (nWidth / 2) + (m_Box.w / 2);
	m_Box.y = (nHeight / 2) + (m_Box.h / 2);
	m_xVel = 1;
	m_yVel = 1;
}


void Dot::SetBox(int x, int y, int w, int h)
{
	m_Box.x = x;
	m_Box.y = y;
	m_Box.w = w;
	m_Box.h = h;
}

void Dot::setVel(int xV, int yV)
{
	m_xVel = xV;
	m_yVel = yV;
}