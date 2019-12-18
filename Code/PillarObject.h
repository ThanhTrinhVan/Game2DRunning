#pragma once
#ifndef PILLAR_OBJECT_H
#define PILLAR_OBJECT_H

#include "CommonFunc.h"
#include "BaseObject.h"

#define NUM_RECLINE_STEPS 10
const int WIDTH_PILLAR = 29;
const double pi = acos(-1);

class PillarObject : public BaseObject
{
public:
	PillarObject();
	~PillarObject();

	void handelInputAction(SDL_Event events, SDL_Renderer * des, float &px, float &py, bool playerStop);
	void recline(const int step, SDL_Renderer* des);	// Hien thi cay cau dang nga xuong theo tung buoc
	bool isReclined() { return reclined; };
	void setReclined(bool sts) { reclined = sts; };

	int getHeight() { return lastHeight; };
	int getPosX() { return posX; };
	int getPosY() { return posY; };
	void reset();

private:
	int height, lastHeight;
	int posX, posY;
	bool reclined;
	void draw(int pX, int pY, SDL_Renderer* des);		// Hien thi xay dung cay cau
};
#endif // !PILLAR_OBJECT_H