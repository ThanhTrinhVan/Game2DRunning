#include "TextObject.h"



TextObject::TextObject()
{
	rect_ = { 0,0, 3 * TILE_SIZE, TILE_SIZE };
	textColor = { 255,0,0 };
}


TextObject::~TextObject()
{
}

void TextObject::setColor(const int & type)
{
	if (type == RED_TEXT)
		textColor = { 255, 0,0 };
	else if (type == WHITE_TEXT)
		textColor = { 255, 255, 255 };
	else if (type == BLUE_TEXT)
		textColor = { 0,0,255 };
	else
		textColor = { 0,0,0 };
}

void TextObject::drawText(TTF_Font * font, SDL_Renderer * des)
{
	SDL_Surface* message = TTF_RenderText_Solid(font, textVal.c_str(), textColor);
	p_object = SDL_CreateTextureFromSurface(des, message);
	SDL_FreeSurface(message);
	render(des, NULL);
}
