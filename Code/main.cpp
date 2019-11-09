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
	gWindow = SDL_CreateWindow("Running game2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
		return false;
	else {
		gScreen = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
		if (gScreen == NULL)
			return false;
		else {
			SDL_SetRenderDrawColor(gScreen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) && imgFlags))
				success = false;

			if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
				return false;
			}
			gSoundScreen = Mix_LoadWAV("G:/7th/Software_engineering/game2D/Data/audio/nhac_nen.wav");
			if (gSoundScreen == NULL) {
				return false;
			}

			if (TTF_Init() == -1)
				return false;
			gFont = TTF_OpenFont("G:/7th/Software_engineering/game2D/Data/Photograph Signature.ttf", 20);
			if (gFont == NULL)
				return false;
		}
	}
	return success;
}

bool loadBackGround()
{
	bool ret = gBackground.loadImg("G:/7th/Software_engineering/game2D/Data/background.png", gScreen);
	if (!ret)
		return false;
	return true;
}

void close()
{
	gBackground.free();
	SDL_DestroyRenderer(gScreen);
	gScreen = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	IMG_Quit();
	IMG_Quit();
}

std::vector<BirdObject*> makeBirdList()
{
	std::vector<BirdObject*> list_threats;
	BirdObject* threatObj = new BirdObject[NUMBER_BIRD];
	for (int i = 0; i < NUMBER_BIRD; i++) {
		BirdObject* p_threat = (threatObj + i);
		if (p_threat != NULL)
		{
			p_threat->loadImg("G:/7th/Software_engineering/game2D/Data/threat/bird.png", gScreen);
			p_threat->setClips();
			p_threat->set_x_pos(700 + i*1200);
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
		return -1;

	if (loadBackGround() == false)
		return -1;

	GameMap game_map;
	game_map.loadMap("G:/7th/Software_engineering/game2D/Data/map_good/map01.dat"); // file luu trang thai ban do
	game_map.loadTiles(gScreen);

	PlayerObject p_player;
	p_player.loadImg("G:/7th/Software_engineering/game2D/Data/player_run/run.png", gScreen);
	p_player.setClips();

	std::vector<BirdObject*> lstBird = makeBirdList();
	TextObject mark_game;
	mark_game.setColor(TextObject::RED_TEXT);

	bool isQuit = false;
	unsigned int score = 100;
	//Mix_PlayChannel(-1, gSoundScreen, 1);
	while (!isQuit)
	{
		if (p_player.isDied() == true)
			break;
		fps_time.start();
		while (SDL_PollEvent(&gEvent) != 0)
		{
			if (gEvent.type == SDL_QUIT) {
				isQuit = true;
			}
			p_player.handelInputAction(gEvent, gScreen);
		}

		SDL_SetRenderDrawColor(gScreen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(gScreen);

		gBackground.render(gScreen, NULL);

		Map map_data = game_map.getMap();
		
		p_player.setMapXY(map_data.start_x, map_data.start_y);
		p_player.doPlayer(map_data);
		p_player.show(gScreen);

		game_map.setMap(map_data);
		game_map.drawMap(gScreen);

		SDL_Rect pRect = p_player.getRectFrame();
		for (int i = 0; i < lstBird.size(); i++) {
			BirdObject* p_threat = lstBird.at(i);
			if (p_threat != NULL) {
				p_threat->setMapXY(map_data.start_x, map_data.start_y);
				p_threat->doBird(map_data);
				if (p_threat->get_x_pos() > map_data.start_x) {
					p_threat->show(gScreen);

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
		mark_game.drawText(gFont, gScreen);

		txt = "Time: " + std::to_string(SDL_GetTicks() / 1000);
		mark_game.setRect(SCREEN_WIDTH - 3 * TILE_SIZE, TILE_SIZE);
		mark_game.setText(txt);
		mark_game.drawText(gFont, gScreen);
		// update screen
		SDL_RenderPresent(gScreen);

		int real_time = fps_time.get_ticks();
		int time_one_frame = 1000 / FRAME_PER_SECOND; // ms; FPS cang nho thi delay cang lon, ctrinh cang cham
		if (real_time < time_one_frame)
			SDL_Delay(time_one_frame - real_time); // uint32 x, neu x < 0 se tu dong chuyen ve 0
	}

	close();
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
