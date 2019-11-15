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
	posY = pY;
	SDL_SetRenderDrawColor(des, (Uint8) 80, (Uint8) 80, (Uint8) 150, 0xFF);
	SDL_RenderDrawLine(des, posX, posY - height, posX, posY);
	SDL_RenderDrawLine(des, posX, posY - height, posX + width, posY - height);
	SDL_RenderDrawLine(des, posX, posY, posX + width, posY);
	SDL_RenderDrawLine(des, posX + width, posY, posX + width, posY - height);
}

void PillarObject::handelInputAction(SDL_Event events, SDL_Renderer * des)
{
	if (events.type == SDL_KEYDOWN) {
		if (events.key.keysym.sym == SDLK_SPACE) {
			draw(100, 400, des);
		}
	}
	if (events.type == SDL_KEYUP) {
		if (events.key.keysym.sym == SDLK_SPACE) {
			recline(des);
		}
	}
}

void PillarObject::recline(SDL_Renderer * des)
{
	int numStep = 5;
	for (int i = 0; i < numStep; i++) {
		double angle = i * pi / (2.0*5.0);
		SDL_SetRenderDrawColor(des, (Uint8)80, (Uint8)80, (Uint8)150, 0xFF);
		SDL_RenderDrawLine(des, posX, posY, posX + height * sin(angle), posY - height * cos(angle));
		SDL_RenderDrawLine(des, posX, posY, posX + width * cos(angle), posY + width * sin(angle));
		SDL_RenderDrawLine(des, posX + width * cos(angle), posY + width * sin(angle), posX + height * sin(angle) + width * cos(angle), posY - height * cos(angle) + width * sin(angle));
		SDL_RenderDrawLine(des, posX + height * sin(angle), posY - height * cos(angle), posX + height * sin(angle) + width * cos(angle), posY - height * cos(angle) + width * sin(angle));
		SDL_Delay(1000);
	}
	height = 0;
}
