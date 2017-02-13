#include "stdafx.h"
#include "Paddle.h"
#include <iostream>

extern int nPaddleW, nPaddleH;
extern int nWidth, nHeight;
extern Texture gcPaddleTexture;

Paddle::Paddle()
{
	m_Box = { 0, 0, 256, 64 };
}

void Paddle::Move(int v)
{
	Vel = v;

	//set the velocity to increase/decrease the paddle's x value
	m_Box.x += Vel;

	//move paddle back if it goes of the screen
	if (m_Box.x < 0 || m_Box.x >= (nWidth - nPaddleW))
	{
		m_Box.x -= Vel;
	}
}

void Paddle::Render(SDL_Renderer* Renderer)
{
	gcPaddleTexture.Render(Renderer, m_Box);
}

void Paddle::SetBox(int x, int y)
{
	m_Box = { x, y, m_Box.w, m_Box.h };
}