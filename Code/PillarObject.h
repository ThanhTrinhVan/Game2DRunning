#pragma once
#ifndef PILLAR_OBJECT_H
#define PILLAR_OBJECT_H

#include "CommonFunc.h"

class PillarObject
{
public:
	PillarObject();
	~PillarObject();

	void handelInputAction(SDL_Event events, SDL_Renderer * des);

private:
	int height, width;
	int posX, posY;
	void recline(SDL_Renderer* des);
	void draw(int pX, int pY, SDL_Renderer* des);
};

#endif // !PILLAR_OBJECT_H

