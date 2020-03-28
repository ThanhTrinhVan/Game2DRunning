#include "CommonFunc.h"

SDL_Window* gWindow[2] = { NULL, NULL };
SDL_Renderer* gScreen[2] = { NULL, NULL };
TTF_Font* gFont = NULL;

float PLAYER_SPEED_X = 5.0;
int Score = 0;
int Blood = LENGTH_BLOOD_MAX;


bool SDLCommonFunc::checkCollision(const SDL_Rect & object1, const SDL_Rect & object2)
{
	if (&object1 == NULL || &object2 == NULL)
		return false;
	int left_a = object1.x;
	int right_a = object1.x + object1.w;
	int top_a = object1.y;
	int bottom_a = object1.y + object1.h;

	int left_b = object2.x;
	int right_b = object2.x + object2.w;
	int top_b = object2.y;
	int bottom_b = object2.y + object2.h;

	// Case 1: size object 1 < size object 2
	if (left_a > left_b && left_a < right_b)
	{
		if (top_a > top_b && top_a < bottom_b)
		{
			return true;
		}
	}

	if (left_a > left_b && left_a < right_b)
	{
		if (bottom_a > top_b && bottom_a < bottom_b)
		{
			return true;
		}
	}

	if (right_a > left_b && right_a < right_b)
	{
		if (top_a > top_b && top_a < bottom_b)
		{
			return true;
		}
	}

	if (right_a > left_b && right_a < right_b)
	{
		if (bottom_a > top_b && bottom_a < bottom_b)
		{
			return true;
		}
	}

	// Case 2: size object 1 < size object 2
	if (left_b > left_a && left_b < right_a)
	{
		if (top_b > top_a && top_b < bottom_a)
		{
			return true;
		}
	}

	if (left_b > left_a && left_b < right_a)
	{
		if (bottom_b > top_a && bottom_b < bottom_a)
		{
			return true;
		}
	}

	if (right_b > left_a && right_b < right_a)
	{
		if (top_b > top_a && top_b < bottom_a)
		{
			return true;
		}
	}

	if (right_b > left_a && right_b < right_a)
	{
		if (bottom_b > top_a && bottom_b < bottom_a)
		{
			return true;
		}
	}

	// Case 3: size object 1 = size object 2
	if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
	{
		return true;
	}

	return false;
}

bool SDLCommonFunc::checkFocus(const int & x, const int & y, const SDL_Rect & rect)
{
	if (&rect == NULL)
		return false;
	if (x >= rect.x && x < (rect.x + rect.w) &&
		y >= rect.y && y < (rect.y + rect.h))
		return true;
	return false;
}

TypeMenu SDLCommonFunc::checkType(std::string txt)
{
	if (txt == "Play" || txt == "Play Again")
		return TypeMenu::Play;
	else if (txt == "Instruction")
		return TypeMenu::Instruction;
	else if (txt == "Documentation")
		return TypeMenu::Documentation;
	else if (txt == "Exit")
		return TypeMenu::Exit;
	else if (txt == "Go to StartMenu")
		return TypeMenu::GoToStart;
	else if (txt == "Continue")
		return TypeMenu::Continue;
	else if (txt == "Beginner")
		return TypeMenu::Beginner;
	else if (txt == "Professional")
		return TypeMenu::Professional;
	else if (txt == "Level")
		return TypeMenu::Level;
	else
		return TypeMenu::Nothing;
	return TypeMenu::Nothing;
}

bool SDLCommonFunc::playSound(short typeSound, Mix_Chunk * gSound)
{
	if (gSound != NULL)
		gSound = NULL;
	switch (typeSound)
	{
	case 1:
		gSound = Mix_LoadWAV("./Data/audio/menu.wav");
		Mix_PlayChannel(-1, gSound, -1);
		break;

	case 2:
		gSound = Mix_LoadWAV("./Data/audio/nhac_nen.wav");
		Mix_PlayChannel(-1, gSound, -1);
		break;

	case 3:
		gSound = Mix_LoadWAV("./Data/audio/vavaochim.wav");
		Mix_PlayChannel(-1, gSound, 0);
		break;

	case 4:
		gSound = Mix_LoadWAV("./Data/audio/roixuongvuc.wav");
		Mix_PlayChannel(-1, gSound, 0);
		break;
	case 5:
		gSound = Mix_LoadWAV("./Data/audio/an_mau.wav");
		Mix_PlayChannel(-1, gSound, 0);
		break;
	}

	if (gSound == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error loading sound!!!", Mix_GetError(), NULL);
		return false;
	}
	return true;
}

bool initData()
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0)
		return false;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	gWindow[0] = SDL_CreateWindow("Running game2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow[0] == NULL)
		return false;
	else {
		gScreen[0] = SDL_CreateRenderer(gWindow[0], -1, SDL_RENDERER_ACCELERATED);
		if (gScreen[0] == NULL)
			return false;
		else {
			SDL_SetRenderDrawColor(gScreen[0], RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			if (!(IMG_Init(IMG_INIT_PNG) && IMG_INIT_PNG))
				success = false;

			if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
				return false;
			}
			gSound = Mix_LoadWAV("./Data/audio/nhac_nen.wav");
			if (gSound == NULL) {
				return false;
			}

			if (TTF_Init() == -1)
				return false;
			gFont = TTF_OpenFont("./Data/8-BIT WONDER.ttf", 20);
			if (gFont == NULL)
				return false;
		}
	}
	return success;
}