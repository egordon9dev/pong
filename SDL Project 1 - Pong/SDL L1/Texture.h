 #ifndef Texture_H
#define Texture_H

#include "stdafx.h"
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

class Texture
{
public:
	//initializes variables and sets m_Texture to null
	Texture();


	//destroys texture and sets it to null
	~Texture();

	
	//load texture from a given path
	bool LoadFromFile(SDL_Renderer* Renderer, std::string path);

	//render texture
	void Render(SDL_Renderer* Renderer, SDL_Rect drect);
	
	//free variables
	void Free();

	bool LoadText(std::string text, SDL_Color color);

	//get texture
	SDL_Texture* GetTexture() { return m_Texture; }
	//set texture
	void SetTexture(SDL_Texture* texture) { m_Texture = texture; }

	void SetBox(int x, int y, int w, int h);
	SDL_Rect GetBox() { return m_Box; }

private:
	SDL_Texture* m_Texture;
	SDL_Rect m_Box;
};
#endif