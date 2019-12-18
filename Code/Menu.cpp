#include "Menu.h"

Menu::Menu()
{
	numItems = 0;
	fpsTime = new ImpTimer();
}

Menu::~Menu()
{
	numItems = 0;
	if (lstItems.size() > 0)
		lstItems.clear();
	if (fpsTime != NULL)
		delete fpsTime;
}

TypeMenu Menu::showMenu(TTF_Font * font, SDL_Renderer * des)
{
	for (int i = 0; i < numItems; i++) {
		lstItems[i].selected = 0;
	}

	int x_mouse = 0, y_mouse = 0;
	while (true) {
		fpsTime->start();
		SDL_SetRenderDrawColor(des, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(des);
		render(des, NULL);

		while (SDL_PollEvent(&gEvent) != 0) {
			switch (gEvent.type)
			{
			case SDL_QUIT:
				return TypeMenu::Exit;
				break;
			case SDL_MOUSEMOTION:
			{
				x_mouse = gEvent.motion.x;
				y_mouse = gEvent.motion.y;
				for (int i = 0; i < numItems; i++) {
					if (SDLCommonFunc::checkFocus(x_mouse, y_mouse, lstItems[i].content.getRect())) {
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
				x_mouse = gEvent.motion.x; 
				y_mouse = gEvent.motion.y;
				for (int i = 0; i < numItems; i++) {
					if (SDLCommonFunc::checkFocus(x_mouse, y_mouse, lstItems[i].content.getRect())) {
						if (lstItems[i].selected == true) {
							lstItems[i].selected = false;
							lstItems[i].content.setColor(TextObject::BLUE_TEXT);
						}
						return SDLCommonFunc::checkType(lstItems[i].content.getText());
					}
				}
			}
				break;
			case SDL_KEYDOWN:
				if (gEvent.key.keysym.sym == SDLK_ESCAPE)
					return TypeMenu::Exit;
				break;
			default:
				break;
			}
		}
		
		for (int i = 0; i < numItems; i++)
		{
			lstItems[i].content.drawText(font, des);
		}
		// update screen
		SDL_RenderPresent(des);
		int render_time = fpsTime->get_ticks();
		if (render_time < TIME_ONE_FRAME)
			SDL_Delay(TIME_ONE_FRAME - render_time);
	}
	return TypeMenu::Exit;
}

void Menu::setPos(int px[], int py[])
{
	if (lstItems.size() > 0)
		lstItems.clear();
	lstItems = std::vector<Item>(numItems);
	for (int i = 0; i < numItems; i++) {
		lstItems[i].pos.x = px[i];
		lstItems[i].pos.y = py[i];
	}
}

void Menu::setLength(int pz[])
{
	for (int i = 0; i < numItems; i++) {
		lstItems[i].length = pz[i];
	}
}

void Menu::setTextItems(std::string * lst)
{
	for (int i = 0; i < numItems; i++) {
		lstItems[i].content.setText(lst[i]);
		lstItems[i].content.setColor(TextObject::BLUE_TEXT);
		lstItems[i].content.setRect(lstItems[i].pos.x, lstItems[i].pos.y);
		lstItems[i].content.setSize(lstItems[i].length, 2);
	}
}
