// game2D.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "CommonFunc.h"
#include "BaseObject.h"
#include "Game_map.h"
#include "PlayerObject.h"
#include "ImpTimer.h"
#include "BirdObject.h"
#include "TextObject.h"
#include "Menu.h"
#include "PillarObject.h"
#include <iostream>

#undef main // SDL_main error
#define NUMBER_BIRD 20

BaseObject gBackground;

// Init enviroment SDL
bool initData()
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0)
		return false;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	gWindow[0] = SDL_CreateWindow("Running game2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow[0] == NULL)
		return false;
	else {
		gScreen[0] = SDL_CreateRenderer(gWindow[0], -1, SDL_RENDERER_ACCELERATED);
		if (gScreen[0] == NULL)
			return false;
		else {
			SDL_SetRenderDrawColor(gScreen[0], RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) && imgFlags))
				success = false;

			if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
				return false;
			}
			gSoundScreen = Mix_LoadWAV("E:/nguyen trung kien/7/Game/Ninja/Data/audio/nhac_nen.wav");
			if (gSoundScreen == NULL) {
				return false;
			}

			if (TTF_Init() == -1)
				return false;
			gFont = TTF_OpenFont("E:/nguyen trung kien/7/Game/Ninja/Data/8-BIT WONDER.TTF", 20);
			if (gFont == NULL)
				return false;
		}
	}
	
	return success;
}

bool loadBackGround(std::string path)
{
	bool ret = gBackground.loadImg(path, gScreen[0]);
	if (!ret)
		return false;
	return true;
}

void close()
{
	gBackground.free();
	for (int i = 0; i < 2; i++)
	{
		SDL_DestroyRenderer(gScreen[i]);
		gScreen[i] = NULL;

		SDL_DestroyWindow(gWindow[i]);
		gWindow[i] = NULL;

		IMG_Quit();
		IMG_Quit();
	}
}

std::vector<BirdObject*> makeBirdList()
{
	std::vector<BirdObject*> list_threats;
	BirdObject* threatObj = new BirdObject[NUMBER_BIRD];
	for (int i = 0; i < NUMBER_BIRD; i++) {
		BirdObject* p_threat = (threatObj + i);
		if (p_threat != NULL)
		{
			p_threat->loadImg("E:/nguyen trung kien/7/Game/Ninja/Data/threat/bird.png", gScreen[0]);
			p_threat->setClips();
			p_threat->set_x_pos(700 + i * 1200);
			p_threat->set_y_pos(250);

			list_threats.push_back(p_threat);
		}
	}
	return list_threats;
}

int main(int argc, char* argv[])
{
	ImpTimer fps_time;
	std::cout << "Wellcome to running game!\n";
	if (initData() == false)
		return 2;

	Menu menu;
	menu.loadImg("E:/nguyen trung kien/7/Game/Ninja/Data/menu_background2.png", gScreen[0]);
	bool menuLoaded = false;
	while (true) {
		if (!menuLoaded)
		{
			menu.setNumItems(5);
			int px[5] = { 200, 200, 800, 800, 1000 };
			int py[5] = { 200, 300, 200, 300, 550 };
			int pz[5] = { 4, 5, 11, 13, 4 };
			menu.setPos(px, py);
			menu.setLength(pz);
			std::string lst[5] = { "Play", "Level", "Instruction", "Documentation", "Exit" };
			menu.setTextItems(lst);
			TypeMenu ret = menu.showMenu(gFont, gScreen[0]);
			switch (ret)
			{
			case Exit:
			{
				return 0;
				menuLoaded = true;
			}
			break;
			case Instruction:
			{
				SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
				gWindow[1] = SDL_CreateWindow("Instruction", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
					1000, 500, SDL_WINDOW_SHOWN);
				Menu Instr;
				gScreen[1] = SDL_CreateRenderer(gWindow[1], -1, SDL_RENDERER_ACCELERATED);
				Instr.loadImg("E:/nguyen trung kien/7/Game/Ninja/Data/Instruction.png", gScreen[1]);
				TypeMenu ret = Instr.showMenu(gFont, gScreen[1]);
				if (ret == TypeMenu::Exit) {
					SDL_DestroyWindow(gWindow[1]);
					SDL_DestroyRenderer(gScreen[1]);
				}
			}
			break;
			case Documentation:
			{
				SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
				gWindow[1] = SDL_CreateWindow("Documentation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
					SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
				Menu Doc;
				gScreen[1] = SDL_CreateRenderer(gWindow[1], -1, SDL_RENDERER_ACCELERATED);
				Doc.loadImg("E:/nguyen trung kien/7/Game/Ninja/Data/Documentation.png", gScreen[1]);
				TypeMenu ret = Doc.showMenu(gFont, gScreen[1]);
				if (ret == TypeMenu::Exit) {
					SDL_DestroyWindow(gWindow[1]);
					SDL_DestroyRenderer(gScreen[1]);
				}
			}
			break;
			case Play:
			{
				if (loadBackGround("E:/nguyen trung kien/7/Game/Ninja/Data/background2.png") == false)
					return -1;

				GameMap game_map;
				game_map.loadMap("E:/nguyen trung kien/7/Game/Ninja/Data/map_good/map01.dat"); // file luu trang thai ban do
				game_map.loadTiles(gScreen[0]);

				PlayerObject p_player;
				p_player.loadImg("E:/nguyen trung kien/7/Game/Ninja/Data/player_run/run1.png", gScreen[0]);
				p_player.setClips();

				std::vector<BirdObject*> lstBird = makeBirdList();
				TextObject mark_game;
				mark_game.setColor(TextObject::RED_TEXT);

				PillarObject pillar;

				bool isQuit = false;
				unsigned int score = 100;
				Mix_PlayChannel(-1, gSoundScreen, 1);
				int stepPillar = 0;
				while (!isQuit)
				{
					if (p_player.isDied() == true)
						break;
					fps_time.start();
					SDL_SetRenderDrawColor(gScreen[0], RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
					SDL_RenderClear(gScreen[0]);

					gBackground.render(gScreen[0], NULL);

					while (SDL_PollEvent(&gEvent) != 0)
					{
						if (gEvent.type == SDL_QUIT) {
							isQuit = true;
						}
						if (gEvent.type == SDL_KEYDOWN) {
							SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
							gWindow[1] = SDL_CreateWindow("Pause", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
								400, 300, SDL_WINDOW_SHOWN);
							gScreen[1] = SDL_CreateRenderer(gWindow[1], -1, SDL_RENDERER_ACCELERATED);
							Menu Paus;
							Paus.setNumItems(2);
							int px[2] = { 130, 130 };
							int py[2] = { 160, 60 };
							int pz[2] = { 8, 4 };
							Paus.setPos(px, py);
							Paus.setLength(pz);
							std::string lst[2] = { "Continue", "Exit" };
							Paus.setTextItems(lst);
							TypeMenu ret1 = Paus.showMenu(gFont, gScreen[1]);

							switch(ret1) 
							{
							case Continue:
							{
								SDL_DestroyWindow(gWindow[1]);
								SDL_DestroyRenderer(gScreen[1]);
							}
							break;
							case Exit:
							{
								return 0;
							}
							}
						}
						p_player.handelInputAction(gEvent, gScreen[0]);
						pillar.handelInputAction(gEvent, gScreen[0]);
					}
					if (pillar.checkReclined() == false && stepPillar < 5) {
						stepPillar++;
						pillar.recline(stepPillar, gScreen[0]);
					}
					else {
						stepPillar = 0;
					}

					Map map_data = game_map.getMap();

					p_player.setMapXY(map_data.start_x, map_data.start_y);
					p_player.doPlayer(map_data);
					p_player.show(gScreen[0]);

					game_map.setMap(map_data);
					game_map.drawMap(gScreen[0]);

					SDL_Rect pRect = p_player.getRectFrame();
					for (int i = 0; i < lstBird.size(); i++) {
						BirdObject* p_threat = lstBird.at(i);
						if (p_threat != NULL) {
							p_threat->setMapXY(map_data.start_x, map_data.start_y);
							p_threat->doBird(map_data);
							if (p_threat->get_x_pos() > map_data.start_x) {
								p_threat->show(gScreen[0]);

								SDL_Rect bRect;
								bRect.x = p_threat->getRect().x;
								bRect.y = p_threat->getRect().y;
								bRect.w = p_threat->get_width_frame();
								bRect.h = p_threat->get_height_frame();
								if (SDLCommonFunc::checkCollision(pRect, bRect)) {
									isQuit = true;
									break;
								}
							}
						}
					}

					// draw text infor
					std::string txt = "Score: " + std::to_string(score);
					mark_game.setRect(SCREEN_WIDTH - 3 * TILE_SIZE, 0);
					mark_game.setText(txt);
					mark_game.drawText(gFont, gScreen[0]);

					txt = "Time: " + std::to_string(SDL_GetTicks() / 1000);
					mark_game.setRect(SCREEN_WIDTH - 3 * TILE_SIZE, TILE_SIZE);
					mark_game.setText(txt);
					mark_game.drawText(gFont, gScreen[0]);

					// update screen
					SDL_RenderPresent(gScreen[0]);

					int real_time = fps_time.get_ticks();
					int time_one_frame = 1000 / FRAME_PER_SECOND; // ms; FPS cang nho thi delay cang lon, ctrinh cang cham
					if (real_time < time_one_frame)
						SDL_Delay(time_one_frame - real_time); // uint32 x, neu x < 0 se tu dong chuyen ve 0
				}
				Mix_HaltChannel(-1);
				menu.setNumItems(3);
				int px2[3] = { 200, 800, 1100 };
				int py2[3] = { 200, 200, 550 };
				int pz2[3] = { 10, 15, 4 };
				menu.setPos(px2, py2);
				menu.setLength(pz2);
				std::string lstt[3] = { "Play Again", "Go to StartMenu", "Exit" };
				menu.setTextItems(lstt);
				TypeMenu ret = menu.showMenu(gFont, gScreen[0]);
				switch (menu.showMenu(gFont, gScreen[0]))
				{
				case Play:
					break;
				case GoToStart:
					menuLoaded = false;
					break;
				case Exit:
					return 0;
					break;
				default:
					break;
				}
			}
			break;
			}
		}

	}

	close();
	return 0;
}

