#pragma once
#ifndef TEXT_OBJECT_H
#define TEXT_OBJECT_H

#include "CommonFunc.h"
#include "BaseObject.h"
#include <string>

class TextObject : public BaseObject
{
public:
	enum TextColor {
		RED_TEXT = 0,
		WHILE_TEXT = 1,
		BLUE_TEXT = 2
	};
	TextObject();
	~TextObject();

	void setText(const std::string &txt) { textVal = txt; };
	void setColor(const int &type);
	void setWidth(const int &length) { rect_.w = length * TILE_SIZE / 3; };
	void drawText(TTF_Font* font, SDL_Renderer* des);
	std::string getText() { return textVal; };

private:
	std::string textVal;
	SDL_Color textColor;
};



#endif // !TEXT_OBJECT_H
