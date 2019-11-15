#include "pch.h"
#include "PillarObject.h"


PillarObject::PillarObject()
{
	height = 0;
	width = 10;
	posX = 0;
	posY = 0;
}


PillarObject::~PillarObject()
{
}

void PillarObject::draw(int pX, int pY, SDL_Renderer * des)
{
	// draw
	height += 5;
	posX = pX;
	posY = pY - height;
	SDL_Rect fill_rect = { posX, posY, width, height };
	SDL_SetRenderDrawColor(des, (Uint8) 80, (Uint8) 80, (Uint8) 150, 0xFF);
	SDL_RenderFillRect(des, &fill_rect);
}

void PillarObject::handelInputAction(SDL_Event events, SDL_Renderer * des)
{
	if (events.type == SDL_KEYDOWN) {
		if (events.key.keysym.sym == SDLK_SPACE) {
			draw(100, 400, des);
		}
	}
	if (events.type == SDL_KEYUP) {
		if (events.key.keysym.sym == SDLK_SPACE)
			recline(des);
	}
}

void PillarObject::recline(SDL_Renderer * des)
{
	height = 0;
}
