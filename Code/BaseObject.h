#pragma once
#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_ 

#include "CommonFunc.h"

// Khoi tao cac phuong thuc load va render anh, dung de xay dung cac class cu the sau
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
		return p_object;
	}

	virtual bool loadImg(std::string path, SDL_Renderer* screen); // tai anh va luu vao p_object_
	void render(SDL_Renderer* des, const SDL_Rect* clip = NULL);  // render p_object_ len des
	void free();												  // giai phong bo nho

protected:
	SDL_Texture* p_object; // luu buc anh hien tai cua doi tuong
	SDL_Rect	 rect_;     // luu kich thuoc va toa do cua p_object_
};

#endif // !BASE_OBJECT_H_
