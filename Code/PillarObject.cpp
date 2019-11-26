#include "pch.h"
#include "PillarObject.h"


PillarObject::PillarObject()
{
	height = 0;
	width = 20;
	lastHeight = height;
	posX = 0;
	posY = 0;
	reclined = false;
}


PillarObject::~PillarObject()
{
}

void PillarObject::draw(int pX, int pY, SDL_Renderer * des)
{
	// draw
	height += 5;
	posX = pX - 10;
	posY = pY;
	SDL_SetRenderDrawColor(des, (Uint8) 80, (Uint8) 80, (Uint8) 150, 0xFF);
	SDL_RenderDrawLine(des, posX, posY - height, posX, posY);
	SDL_RenderDrawLine(des, posX, posY - height, posX + width, posY - height);
	SDL_RenderDrawLine(des, posX, posY, posX + width, posY);
	SDL_RenderDrawLine(des, posX + width, posY, posX + width, posY - height);
}

void PillarObject::handelInputAction(SDL_Event events, SDL_Renderer * des, float &px, float &py, bool playerStop)
{
	if (events.type == SDL_KEYDOWN) {
		if (events.key.keysym.sym == SDLK_SPACE && playerStop) {
			draw(px, py, des);
		}
	}
	if (events.type == SDL_KEYUP) {
		if (events.key.keysym.sym == SDLK_SPACE) {
			reclined = false;
		}
	}
}

void PillarObject::recline(const int step, SDL_Renderer * des)
{
	double angle = step * pi / (2.0 * 5);
	SDL_SetRenderDrawColor(des, (Uint8)80, (Uint8)80, (Uint8)150, 0xFF);
	SDL_RenderDrawLine(des, posX, posY, posX + height * sin(angle), posY - height * cos(angle));
	SDL_RenderDrawLine(des, posX, posY, posX + width * cos(angle), posY + width * sin(angle));
	SDL_RenderDrawLine(des, posX + width * cos(angle), posY + width * sin(angle), posX + height * sin(angle) + width * cos(angle), posY - height * cos(angle) + width * sin(angle));
	SDL_RenderDrawLine(des, posX + height * sin(angle), posY - height * cos(angle), posX + height * sin(angle) + width * cos(angle), posY - height * cos(angle) + width * sin(angle));

	if (step >= 5) {
		lastHeight = height;
		height = 0;
		reclined = true;
	}
	else {
		reclined = false;
	}
}

void PillarObject::reset()
{
	height = 0;
	posX = 0;
	posY = 0;
	reclined = false;
}
