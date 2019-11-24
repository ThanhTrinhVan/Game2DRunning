#pragma once
#ifndef PILLAR_OBJECT_H
#define PILLAR_OBJECT_H

#include "CommonFunc.h"

const double pi = acos(-1);

class PillarObject
{
public:
	PillarObject();
	~PillarObject();

	void handelInputAction(SDL_Event events, SDL_Renderer * des);
	void recline(const int step, SDL_Renderer* des);
	bool checkReclined() { return reclined; };

private:
	int height, width;
	int posX, posY;
	bool reclined;
	void draw(int pX, int pY, SDL_Renderer* des);
};

#endif // !PILLAR_OBJECT_H
