#pragma once
#ifndef MENU_H
#define MENU_H

#include "CommonFunc.h"
#include "BaseObject.h"
#include "TextObject.h"
#include "ImpTimer.h"

typedef struct Item {
	SDL_Rect pos;			// vi tri
	TextObject content;		// noi dung 
	bool selected;			// kiem tra duoc chon
	int length;				// do dai
};

class Menu : public BaseObject
{
public:
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
	ImpTimer* fpsTime;
};
#endif // !MENU_H

