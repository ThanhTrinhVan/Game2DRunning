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

	void handelInputAction(SDL_Event events, SDL_Renderer * des, float &px, float &py, bool playerStop);
	void recline(const int step, SDL_Renderer* des);
	bool checkReclined() { return reclined; };
	void setReclined(bool sts) { reclined = sts; };

	int getHeight() { return lastHeight; };
	int getPosX() { return posX; };
	int getPosY() { return posY; };
	void reset();

private:
	int height, width, lastHeight;
	int posX, posY;
	bool reclined;
	void draw(int pX, int pY, SDL_Renderer* des);
};

#endif // !PILLAR_OBJECT_H

