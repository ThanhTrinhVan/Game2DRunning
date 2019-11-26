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
Menu menu;
bool menuLoaded = false;

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
			gSoundScreen = Mix_LoadWAV("F:/7th/Software_engineering/game2D/Data/audio/nhac_nen.wav");
			if (gSoundScreen == NULL) {
				return false;
			}

			if (TTF_Init() == -1)
				return false;
			gFont = TTF_OpenFont("F:/7th/Software_engineering/game2D/Data/8-BIT WONDER.ttf", 20);
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
	for (int i = 0; i < 2; i++) {
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
			p_threat->loadImg("F:/7th/Software_engineering/game2D/Data/threat/bird.png", gScreen[0]);
			p_threat->setClips();
			p_threat->set_x_pos(700 + i*1200);
			p_threat->set_y_pos(250);

			list_threats.push_back(p_threat);
		}
	}
	return list_threats;
}

int endMenu()
{
	menu.setNumItems(3);
	int px2[3] = { 200, 800, 1100 };
	int py2[3] = { 200, 200, 550 };
	int pz2[3] = { 10, 15, 4 };
	menu.setPos(px2, py2);
	menu.setLength(pz2);
	std::string lstt[3] = { "Play Again", "Go to StartMenu", "Exit" };
	menu.setTextItems(lstt);
	switch (menu.showMenu(gFont, gScreen[0]))
	{
	case Play:
		break;
	case GoToStart:
		menuLoaded = false;
		break;
	case Exit:
		return -1;
		break;
	default:
		break;
	}
	return 0;
}

int play(ImpTimer &fpsTime)
{
	if (loadBackGround("F:/7th/Software_engineering/game2D/Data/background.png") == false)
		return -1;

	GameMap game_map;
	game_map.loadMap("F:/7th/Software_engineering/game2D/Data/map_good/map01.dat"); // file luu trang thai ban do
	game_map.loadTiles(gScreen[0]);

	PlayerObject p_player;
	p_player.loadImg("F:/7th/Software_engineering/game2D/Data/player_run/run.png", gScreen[0]);
	p_player.setClips();

	std::vector<BirdObject*> lstBird = makeBirdList();
	TextObject mark_game;
	mark_game.setColor(TextObject::RED_TEXT);

	PillarObject pillar;

	bool isQuit = false;
	unsigned int score = 100;
	Mix_PlayChannel(-1, gSoundScreen, 1);
	int stepPillar = 0;
	bool drawedPl = false;
	int remainH = 0, xTile = 0, yTile = 0, numTiles = 0;
	while (!isQuit)
	{
		if (p_player.isDied() == true)
			break;
		fpsTime.start();
		SDL_SetRenderDrawColor(gScreen[0], RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(gScreen[0]);

		gBackground.render(gScreen[0], NULL);
		
		Map map_data = game_map.getMap();

		float playerX = p_player.getPosX() + p_player.get_width_frame();
		float playerY = p_player.getPosY() + p_player.get_height_frame();
		while (SDL_PollEvent(&gEvent) != 0)
		{
			if (gEvent.type == SDL_QUIT) {
				isQuit = true;
			}
			if (gEvent.type == SDL_KEYDOWN) {
				if (gEvent.key.keysym.sym == SDLK_p) {
					SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
					gWindow[1] = SDL_CreateWindow("Pause", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
						400, 300, SDL_WINDOW_SHOWN);
					gScreen[1] = SDL_CreateRenderer(gWindow[1], -1, SDL_RENDERER_ACCELERATED);
					Menu PauseMenu;
					PauseMenu.setNumItems(2);
					int px[2] = { 130, 130 };
					int py[2] = { 160, 60 };
					int pz[2] = { 8, 4 };
					PauseMenu.setPos(px, py);
					PauseMenu.setLength(pz);
					std::string lst[2] = { "Continue", "Exit" };
					PauseMenu.setTextItems(lst);
					TypeMenu ret1 = PauseMenu.showMenu(gFont, gScreen[1]);

					switch (ret1)
					{
					case Continue:
					{
						SDL_DestroyWindow(gWindow[1]);
						SDL_DestroyRenderer(gScreen[1]);
					}
					break;
					case Exit:
					{
						SDL_DestroyWindow(gWindow[1]);
						SDL_DestroyRenderer(gScreen[1]);
						return endMenu();
					}
					break;
					}
				}
			}
			p_player.handelInputAction(gEvent, gScreen[0]);
			pillar.handelInputAction(gEvent, gScreen[0], playerX, playerY, p_player.isStopping());
		}
		if (pillar.checkReclined() == false && stepPillar < 5) {
			stepPillar++;
			pillar.recline(stepPillar, gScreen[0]);
		}
		else {
			stepPillar = 0;
			if (pillar.getHeight() > 0) {
				numTiles = pillar.getHeight() / TILE_SIZE;
				remainH = pillar.getHeight() - numTiles * TILE_SIZE;
				xTile = (p_player.getPosX() + map_data.start_x + p_player.get_width_frame()) / TILE_SIZE;
				yTile = (p_player.getPosY() - 1 + map_data.start_y + p_player.get_height_frame()) / TILE_SIZE + 1;
				for (int i = 0; i < numTiles; i++) {
					map_data.tile[yTile][xTile + i] = 10; // o trong co chua hinh ve cay cau
				}
				pillar.reset();
				drawedPl = true;
			}
			/*if (remainH > 0) {
				int posX = (xTile + numTiles)*TILE_SIZE - map_data.start_x - 10;
				int posY = (yTile)*TILE_SIZE - map_data.start_y;
				SDL_SetRenderDrawColor(gScreen[0], (Uint8)80, (Uint8)80, (Uint8)150, 0xFF);
				SDL_RenderDrawLine(gScreen[0], posX, posY + 20, posX, posY);
				SDL_RenderDrawLine(gScreen[0], posX, posY + 20, posX + remainH, posY + 20);
				SDL_RenderDrawLine(gScreen[0], posX, posY, posX + remainH, posY);
				SDL_RenderDrawLine(gScreen[0], posX + remainH, posY, posX + remainH, posY + 20);
			}*/
		}

		p_player.setMapXY(map_data.start_x, map_data.start_y);
		p_player.doPlayer(map_data, &drawedPl);
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

		int real_time = fpsTime.get_ticks();
		int time_one_frame = 1000 / FRAME_PER_SECOND; // ms; FPS cang nho thi delay cang lon, ctrinh cang cham
		if (real_time < time_one_frame)
			SDL_Delay(time_one_frame - real_time); // uint32 x, neu x < 0 se tu dong chuyen ve 0
	}
	Mix_HaltChannel(-1);
	return endMenu();
}

int main(int argc, char* argv[])
{
	ImpTimer fps_time;
    std::cout << "Wellcome to running game!\n"; 
	if (initData() == false)
		return 2;

	menu.loadImg("F:/7th/Software_engineering/game2D/Data/menu_background.png", gScreen[0]);
	while (true) {
		if (!menuLoaded) {
			menu.setNumItems(5);
			int px[5] = { 200, 200, 800, 800, 1100 };
			int py[5] = { 200, 300, 200, 300, 550 };
			int pz[5] = { 4,5,11,13,4 };
			menu.setPos(px, py);
			menu.setLength(pz);
			std::string lst[5] = { "Play", "Level", "Instruction", "Documentation", "Exit" };
			menu.setTextItems(lst);
			menuLoaded = true;
			TypeMenu ret = menu.showMenu(gFont, gScreen[0]);
			switch (ret)
			{
			case Exit:
			{
				return 0;
			}
			break;
			case Instruction:
			{
				SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
				gWindow[1] = SDL_CreateWindow("Instruction", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
					1000, 500, SDL_WINDOW_SHOWN);
				Menu InstrMenu;
				gScreen[1] = SDL_CreateRenderer(gWindow[1], -1, SDL_RENDERER_ACCELERATED);
				InstrMenu.loadImg("F:/7th/Software_engineering/game2D/Data/Instruction.png", gScreen[1]);
				TypeMenu ret = InstrMenu.showMenu(gFont, gScreen[1]);
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
					1000, 500, SDL_WINDOW_SHOWN);
				Menu DocMenu;
				gScreen[1] = SDL_CreateRenderer(gWindow[1], -1, SDL_RENDERER_ACCELERATED);
				DocMenu.loadImg("F:/7th/Software_engineering/game2D/Data/Documentation.png", gScreen[1]);
				TypeMenu ret = DocMenu.showMenu(gFont, gScreen[1]);
				if (ret == TypeMenu::Exit) {
					SDL_DestroyWindow(gWindow[1]);
					SDL_DestroyRenderer(gScreen[1]);
				}
			}
			break;
			case Play:
			{
				int retPlay = play(fps_time);
				if (retPlay == -1)
					return -1;
			}
			break;
			}
		}
		else {
			int retPlay = play(fps_time);
			if (retPlay == -1)
				return -1;
		}
	}
	close();
	return 0;
}
