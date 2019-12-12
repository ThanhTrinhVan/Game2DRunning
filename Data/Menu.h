#pragma once
#ifndef MENU_H
#define MENU_H

#include "CommonFunc.h"
#include "BaseObject.h"
#include "TextObject.h"

class Menu : public BaseObject
{
public:
	typedef struct Item {
		SDL_Rect pos;
		TextObject content;
		bool selected;
		int length;
	};
	Menu();
	~Menu();
	TypeMenu showMenu(TTF_Font* font, SDL_Renderer* des);
	void setPos(int px[], int py[]);
	void setLength(int pz[]);
	void setNumItems(const int num) { numItems = num; };
	void setTextItems(std::string *lstItems);

private:
	int numItems;
	std::vector<Item> lstItems;
};

#endif // !MENU_H

