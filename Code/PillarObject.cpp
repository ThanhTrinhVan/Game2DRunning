#include "PillarObject.h"

PillarObject::PillarObject()
{
	height = 0;
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
	height += 5;
	posX = pX - 10;
	posY = pY;
	SDL_SetRenderDrawColor(des, (Uint8)80, (Uint8)80, (Uint8)150, 0xFF);
	SDL_Rect rect = { posX, posY - height, WIDTH_PILLAR, height };
	SDL_RenderFillRect(des, &rect);
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
	/*if (events.type == SDL_MOUSEBUTTONDOWN) {
		if (events.button.button == SDL_BUTTON_RIGHT && playerStop) {
			draw(px, py, des);
		}
	}
	if (events.type == SDL_MOUSEBUTTONUP) {
		if (events.button.button == SDL_BUTTON_RIGHT) {
			reclined = false;
		}
	}*/
}

void PillarObject::recline(const int step, SDL_Renderer * des)
{
	double angle = step * pi / (2.0 * NUM_RECLINE_STEPS);
	/*SDL_SetRenderDrawColor(des, (Uint8)80, (Uint8)80, (Uint8)150, 0xFF);
	SDL_RenderDrawLine(des, posX, posY, posX + height * sin(angle), posY - height * cos(angle));
	SDL_RenderDrawLine(des, posX, posY, posX + width * cos(angle), posY + width * sin(angle));
	SDL_RenderDrawLine(des, posX + width * cos(angle), posY + width * sin(angle), posX + height * sin(angle) + width * cos(angle), posY - height * cos(angle) + width * sin(angle));
	SDL_RenderDrawLine(des, posX + height * sin(angle), posY - height * cos(angle), posX + height * sin(angle) + width * cos(angle), posY - height * cos(angle) + width * sin(angle));*/
	double dx = (height * sin(angle) + WIDTH_PILLAR * cos(angle) - WIDTH_PILLAR) / 2.0;
	double dy = (-height * cos(angle) + WIDTH_PILLAR * sin(angle) + height) / 2.0;
	SDL_Rect rect = { posX + dx, posY - height + dy, WIDTH_PILLAR, height };
	angle = step * 90.0 / NUM_RECLINE_STEPS;
	SDL_RenderCopyEx(des, p_object, NULL, &rect, angle, NULL, SDL_FLIP_NONE);

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