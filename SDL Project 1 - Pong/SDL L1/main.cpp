#include "stdafx.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include "Texture.h"
#include "Dot.h"
#include "Paddle.h"

//window we'll be rendering to
SDL_Window* gWindow = NULL;

//surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//create a global dot and background textures
Texture gcDotTexture;
Texture gcBackgroundTexture;
Texture gcPaddleTexture;

//textures to hold the top and bottom scores
Texture gcTopScoreTextTexture;
Texture gcBottomScoreTextTexture;

//renderer
SDL_Renderer* gRenderer = NULL;

//font
TTF_Font* gFont = NULL;

//screen dimensions
int nWidth = 1400;
int nHeight = 700;

//dot dimensions
int nDotW = 20, nDotH = 20;

//paddle dimensions
int nPaddleW = 256, nPaddleH = 64;

//scores
std::string gstrPlayerScore = "0", gstrComputerScore = "0";

bool Init()
{
	using namespace std;

	bool success = true;

	//initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL Error: " << SDL_GetError() << "failed to initialize SDL" << endl;
		success = false;
	}
	else
	{
		//enable linear texture filtering
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			cout << "Warning: Linear filtering is not enabled" << endl;
		}
		//Create window
		gWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, nWidth, nHeight, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			cout << "SDL Error: " << SDL_GetError() << "couldn't create window" << endl;
			success = false;
		}
		else
		{
			//create vsynced, accelerated renderer for gWindow
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				cout << "SDL Error: " << SDL_GetError() << "; couldn't create renderer" << endl;
				success = false;
			}
			else
			{
				//set render draw color to opaque white
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					cout << "SDL_Image could not be initialized, SDL_Image Error: " << IMG_GetError() << endl;
					success = false;
				}

				//initializ TTF
				if (TTF_Init() == -1)
				{
					std::cout << "SDL_ttf couldn't be initialized, ttf error: " << TTF_GetError() << std::endl;
					success = false;
				}
			}
		}
	}
	return success;
}

void LoadScoreText(std::string strTopScore, std::string strBottomScore)
{
	using namespace std;

	//set color to white
	SDL_Color textColor = { 255, 255, 255 };

	//render the top score
	if (!gcTopScoreTextTexture.LoadText(strTopScore, textColor))
	{
		cout << "failed to load top score" << endl;
	}

	//render the bottom score
	if (!gcBottomScoreTextTexture.LoadText(strBottomScore, textColor))
	{
		cout << "failed to load bottom score" << endl;
	}
}


bool LoadMedia()
{
	using namespace std;

	bool success = true;

	//load dot image
	if (!gcBackgroundTexture.LoadFromFile(gRenderer, "background.bmp"))
	{
		cout << "SDL Error: " << SDL_GetError() << "couldn't load background.bmp" << endl;
		success = false;
	}
	else
		cout << "background.bmp was successfully loaded" << endl;

	//load background image
	if (!gcDotTexture.LoadFromFile(gRenderer, "dot.bmp"))
	{
		cout << "SDL Error: " << SDL_GetError() << "couldn't load dot.bmp" << endl;
		success = false;
	}
	else
		cout << "dot.bmp was successfully loaded" << endl;

	//load Paddle image 1
	if (!gcPaddleTexture.LoadFromFile(gRenderer, "paddle.png"))
	{
		cout << "SDL Error: " << SDL_GetError() << "couldn't load dot.bmp" << endl;
		success = false;
	}
	else
		cout << "paddle.bmp was successfully loaded" << endl;

	//open font
	gFont = TTF_OpenFont("lazy.ttf", 28);
	if (gFont == NULL)
	{
		cout << "font couldn't be initialized, ttf error: " << TTF_GetError() << endl;
		success = false;
	}

	return success;
}

//cleanup function
void Close()
{
	//free textures
	gcDotTexture.Free();
	gcBackgroundTexture.Free();
	gcPaddleTexture.Free();
	gcTopScoreTextTexture.Free();
	gcBottomScoreTextTexture.Free();

	//free font
	TTF_CloseFont(gFont);
	gFont = NULL;
	
	//destroy and dealocate window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//destroy and dealocate renderer
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	//quit SDL subsystems
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

bool CheckCollision(SDL_Rect a, SDL_Rect b)
{
	//set sides of box a
	int LeftA  = a.x;
	int RightA = a.x + a.w;
	int TopA   = a.y;
	int BottomA= a.y + a.h;
	//set sides of box b
	int LeftB  = b.x;
	int RightB = b.x + b.w;
	int TopB   = b.y;
	int BottomB= b.y + b.h;

	//check collision
	//if any sides are seperated return false
	if (BottomA <= TopB)
		return false;
	if (BottomB <= TopA)
		return false;
	if (RightA <= LeftB)
		return false;
	if (RightB <= LeftA)
		return false;
	//if no sides are seperated return true
	using namespace std;
	cout << "\n\nCollision detected!\n";
	return true;
}

int ChooseDifficulty()
{
	using namespace std;
	int nDifficulty;

	cout << "What difficulty would you like, easy, medium, or hard?" << endl;
	cout << "enter a number" << endl;
	cout << "1 = easy" << endl;
	cout << "2 = medium" << endl;
	cout << "3 = hard" << endl;
	cout << "4 = insane" << endl;
	do
	{
		cin >> nDifficulty;
		switch (nDifficulty)
		{
		//easy
		case 1:
			return 1;

		//medium
		case 2:
			return 2;
	
		//hard
		case 3:
			return 3;

		//insane
		case 4:
			return 5;

		default:
			break;
		}
	} while (1);
}

int ChooseNumOfPlayers()
{
	using namespace std;

	int nNumOfPlayers;

	do
	{
		cout << "enter the number of people playing ( 0, 1, or 2 )" << endl;
		cin >> nNumOfPlayers;
	} while (!(nNumOfPlayers == 0 || nNumOfPlayers == 1 || nNumOfPlayers == 2));

	return nNumOfPlayers;
}

int main(int argc, char *argv[])
{
	using namespace std;

	int nDifficulty = ChooseDifficulty();
	int nNumOfPlayers = ChooseNumOfPlayers();

	if (!Init())
	{
		cout << "Error: Failed to initialize" << endl;
	}
	else
	{
		if (!LoadMedia())
		{
			cout << "Error: Failed to load media" << endl;
		}

		bool quit = false;

		SDL_Event e;

		Dot cDot;

		Paddle cTopPaddle;
		Paddle cBottomPaddle;
		//distance betweeen paddle and wall
		int PaddleOffset = 0;

		//scores
		int nPlayerScore = 0;
		int nComputerScore = 0;

		//center top paddle at the top of the screen
		cTopPaddle.SetBox(((nWidth / 2) - (nPaddleW / 2)), PaddleOffset);

		//center bottom paddle at the bottom of the screen
		cBottomPaddle.SetBox(((nWidth / 2) - (nPaddleW / 2)), (nHeight - PaddleOffset) - nPaddleH);

		//flag for whether or not the window is shown
		bool bShown = false;

		//delay time until game starts
		SDL_Delay(4000);

		//string stream to convert an int to a string
		stringstream convert;

		while (!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				else
				{
					//check for key events
					switch (e.key.keysym.sym)
					{
						//right arrow
					case SDLK_RIGHT:
						cBottomPaddle.Move(100);
						break;

						//left arrow
					case SDLK_LEFT:
						cBottomPaddle.Move(-100);
						break;

						//letter 'a'
					case SDLK_a:
						if (nNumOfPlayers == 2)
							cTopPaddle.Move(-100);
						break;

						//letter 'd'
					case SDLK_d:
						if (nNumOfPlayers == 2)
							cTopPaddle.Move(100);
						break;

					default:
						//if the paddle hasn't moved, then don't move it
						cBottomPaddle.Move(0);
						//if there are two players and no movement key is pressed, then stop the paddle from moveing
						if (nNumOfPlayers == 2)
							cTopPaddle.Move(0);
						break;
						}

					//check for window events
					switch (e.window.event)
					{
					case SDL_WINDOWEVENT_SHOWN:
						bShown = true;
						break;
					case SDL_WINDOWEVENT_HIDDEN:
						bShown = false;
						break;
					}
				}

			}

			//clear renderer
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderClear(gRenderer);

			
			//create destination rects for each texture:

			//set background texture to dimensions of the screen
			SDL_Rect BackgroundRect = { 0, 0, nWidth, nHeight };


			//move dot
			cDot.Move(cTopPaddle.GetBox(), cBottomPaddle.GetBox());

			if (nNumOfPlayers == 1 || nNumOfPlayers == 0)
			{
				//move the top paddle using A.I.

				//if ball is to the left of paddle
				if ((cDot.GetBox().x + cDot.GetBox().w) < cTopPaddle.GetBox().x)
					{
						//move left
						cTopPaddle.Move(-nDifficulty);

						//move bottom paddle if no one is playing
						if (nNumOfPlayers == 0)
							cBottomPaddle.Move(-nDifficulty);
					}
					

				//if the ball is to the right of the paddle
				if (cDot.GetBox().x > (cTopPaddle.GetBox().x + cTopPaddle.GetBox().w))
				{
					//move right
					cTopPaddle.Move(nDifficulty);

					//move bottom paddle if no one is playing
					if (nNumOfPlayers == 0)
						cBottomPaddle.Move(nDifficulty);
				}
			}

			//render background texture
			gcBackgroundTexture.Render(gRenderer, BackgroundRect);

			
			//Render dot
			cDot.Render(gRenderer);
			
			//Render paddles
			cTopPaddle.Render(gRenderer);
			cBottomPaddle.Render(gRenderer);

			//show window if it's not shown
			if (!bShown)
			{
				SDL_ShowWindow(gWindow);
			}
			
			if (cDot.GetBox().y == 2)
			{
				cout << "gStrPlayerScore2: " << gstrPlayerScore << endl;
				cout << "nPlayerScore2: " << nPlayerScore << endl;

				//increment score
				++nPlayerScore;

				//clear string stream
				convert << " " << endl;

				//convert int to string
				convert << nPlayerScore << endl;
				convert >> gstrPlayerScore;

				cout << "gStrPlayerScore2: "  << gstrPlayerScore << endl;
				cout <<  "nPlayerScore2: " << nPlayerScore << endl;

				//end game when score is 5
				if (nPlayerScore == 5)
				{
					cout << "\n\n\n\n\n\n\n\nGAME OVER: YOU WIN!\n\n\n\n\n\n\n\n";
					quit = true;

					//delay 1 second (1000 milliseconds)
					SDL_Delay(1000);
				}
				else
					//respawn dot
					cDot.Respawn();

			}

			if (((cDot.GetBox()).y + (cDot.GetBox()).h) == nHeight - 1)
			{
				//increment score
				++nComputerScore;

				//clear string stream
				convert << " " << endl;

				//convert in to string
				convert << nComputerScore << endl;
				convert >> gstrComputerScore;

				//and game when score is 5
				if (nComputerScore == 5)
				{
					cout << "\n\n\n\n\n\n\n\nGAME OVER: YOU LOSE\n\n\n\n\n\n\n\n";
					quit = true;

					//delay 1 second (1000 milliseconds)
					SDL_Delay(1000);
				}
				else
					cDot.Respawn();
			}


			//center score on the middle of the screen:

			SDL_Rect TopScoreRect;
			SDL_Rect BottomScoreRect;

			//width
			BottomScoreRect.w = gcBottomScoreTextTexture.GetBox().w;
			TopScoreRect.w = gcTopScoreTextTexture.GetBox().w;
			
			//height
			BottomScoreRect.h = gcBottomScoreTextTexture.GetBox().h;
			TopScoreRect.h = gcTopScoreTextTexture.GetBox().h;
			
			//x position
			BottomScoreRect.x = (nWidth / 2) - (BottomScoreRect.w / 2);
			TopScoreRect.x = (nWidth / 2) - (TopScoreRect.w / 2);
			
			//y position
			BottomScoreRect.y = nHeight / 2;
			TopScoreRect.y = BottomScoreRect.y - TopScoreRect.h;
			
			//load scores
			LoadScoreText(gstrComputerScore, gstrPlayerScore);

			//render score
			gcBottomScoreTextTexture.Render(gRenderer, BottomScoreRect);
			gcTopScoreTextTexture.Render(gRenderer, TopScoreRect);

			//update renderer
			SDL_RenderPresent(gRenderer);
		}
		
		//free recources and close SDL subsystems
		Close();
	}
	return 0;
}
