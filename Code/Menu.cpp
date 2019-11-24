 #include "pch.h"
#include "Menu.h"


Menu::Menu()
{
	numItems = 0;
}


Menu::~Menu()
{
}

TypeMenu Menu::showMenu(TTF_Font * font, SDL_Renderer * des)
{
	for (int i = 0; i < numItems; i++) {
		lstItems[i].selected = 0;
	}

	int xm = 0, ym = 0;
	while (true) {
		SDL_SetRenderDrawColor(des, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(des);

		render(des, NULL);

		for (int i = 0; i < numItems; i++)
		{
			lstItems[i].content.drawText(font, des);
		}
		while (SDL_PollEvent(&gEvent)) {
			switch (gEvent.type)
			{
			case SDL_QUIT:
				return TypeMenu::Exit;
			case SDL_MOUSEMOTION:
			{
				xm = gEvent.motion.x;
				ym = gEvent.motion.y;
				for (int i = 0; i < numItems; i++) {
					if (SDLCommonFunc::checkFocus(xm, ym, lstItems[i].content.getRect())) {
						if (lstItems[i].selected == false) {
							lstItems[i].selected = true;
							lstItems[i].content.setColor(TextObject::RED_TEXT);
						}
					}
					else {
						if (lstItems[i].selected == true) {
							lstItems[i].selected = false;
							lstItems[i].content.setColor(TextObject::BLUE_TEXT);
						}
					}
				}
			}
			break;
			case SDL_MOUSEBUTTONDOWN:
			{
				xm = gEvent.motion.x; // vi tri chuot
				ym = gEvent.motion.y;
				for (int i = 0; i < numItems; i++) {
					if (SDLCommonFunc::checkFocus(xm, ym, lstItems[i].content.getRect())) {
						return SDLCommonFunc::checkType(lstItems[i].content.getText());
					}
				}
			}
			break;
			case SDL_KEYDOWN:
				if (gEvent.key.keysym.sym == SDLK_ESCAPE)
					return TypeMenu::Exit;
			default:
				break;
			}
		}
		// update screen
		SDL_RenderPresent(des);
	}

	return TypeMenu::Exit;
}

void Menu::setPos(int px[], int py[])
{
	lstItems.clear();
	lstItems = std::vector<Item>(numItems);
	for (int i = 0; i < numItems; i++) {
		lstItems[i].pos.x = px[i];
		lstItems[i].pos.y = py[i];
	}
}

void Menu::setLength(int pz[])
{
	for (int i = 0; i < numItems; i++)
	{
		lstItems[i].length = pz[i];
	}
}

void Menu::setTextItems(std::string * lst)
{
	for (int i = 0; i < numItems; i++) {
		lstItems[i].content.setText(lst[i]);
		lstItems[i].content.setColor(TextObject::BLUE_TEXT);
		lstItems[i].content.setRect(lstItems[i].pos.x, lstItems[i].pos.y);
		lstItems[i].content.setWidth(lstItems[i].length);
	}
}
