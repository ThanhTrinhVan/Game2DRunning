#pragma once
#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_ 

#include "CommonFunc.h"

// base class for loading image and render image on dst
class BaseObject
{
public:
	BaseObject();
	~BaseObject();

	void setRect(const int &x, const int &y) {
		rect_.x = x;
		rect_.y = y;
	}
	SDL_Rect getRect() const {
		return rect_;
	}
	SDL_Texture* getObject() const {
		return p_object_;
	}

	virtual bool loadImg(std::string path, SDL_Renderer* screen); // load image to object
	void render(SDL_Renderer* des, const SDL_Rect* clip = NULL); // render img to des
	void free();

protected:
	SDL_Texture* p_object_; // for saving image
	SDL_Rect rect_;// for saving size of img
};

#endif // ! BASE_OBJECT_H_