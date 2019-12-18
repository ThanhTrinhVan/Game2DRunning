#pragma once
#ifndef TEXT_OBJECT_H
#define TEXT_OBJECT_H

#include "CommonFunc.h"
#include "BaseObject.h"

class TextObject : public BaseObject
{
public:
	enum TextColor {
		RED_TEXT = 0,
		WHITE_TEXT = 1,
		BLUE_TEXT = 2
	};
	TextObject();
	~TextObject();

	void setText(const std::string &txt) { textVal = txt; };
	void setColor(const int &type);
	void setSize(const int &w, const int &h) {
		rect_.w = w * TILE_SIZE / 3;
		rect_.h = h * TILE_SIZE / 2;
	};
	void drawText(TTF_Font* font, SDL_Renderer* des);
	std::string getText() { return textVal; };

private:
	std::string textVal;
	SDL_Color textColor;
};
#endif // !TEXT_OBJECT_H

