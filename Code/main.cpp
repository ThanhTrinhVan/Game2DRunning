#undef main // SDL_main error

#include "CommonFunc.h"
#include "BaseObject.h"
#include "ImpTimer.h"
#include "Game_map.h"
#include "TextObject.h"
#include "Menu.h"
#include "PlayerObject.h"
#include "BirdObject.h"
#include "PillarObject.h"

#include <iostream>
#include <fstream>

#define NUMBER_BIRD 20
#define MINUS_BLOOD 1

using namespace std;

BaseObject gBackground;
TextObject drawText;
Menu menuGame;
bool menuLoaded = false;

// Khoi tao
bool loadBackGround(std::string path)
{
	if (!gBackground.loadImg(path, gScreen[0]))
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
	}
}

std::vector<BirdObject*> createThreatList()
{
	std::vector<BirdObject*> list_threats;
	BirdObject* threatObj = new BirdObject[NUMBER_BIRD];
	for (int i = 0; i < NUMBER_BIRD; i++) {
		BirdObject* p_threat = (threatObj + i);
		if (p_threat != NULL)
		{
			p_threat->loadImg("./Data/threat/bird.png", gScreen[0]);
			p_threat->setClips();
			p_threat->set_x_pos(700 + i * 1200);
			p_threat->set_y_pos(250);
			list_threats.push_back(p_threat);
		}
	}
	return list_threats;
}

int endMenu()
{
	Mix_HaltChannel(-1);
	SDLCommonFunc::playSound(1, gSound);
	std::ifstream fileIn;
	fileIn.open("./Data/score.txt");
	int highScore = 0;
	fileIn >> highScore;
	fileIn.close();

	if (Score > highScore) {
		highScore = Score;
		std::ofstream fileOut;
		fileOut.open("./Data/score.txt");
		fileOut << highScore;
		fileOut.close();
	}

	menuGame.setNumItems(7);
	vector<int> px{ 100, 900, 1100, 450, 500, 450, 500 };
	vector<int> py{ 200, 200, 550, 100, 164, 228, 292 };
	vector<int> pz{ 10, 15, 4, 10, 2, 10, 2 };
	menuGame.setPos(px, py);
	menuGame.setLength(pz);
	vector<std::string> lstItems{ "Play Again", "Go to StartMenu", "Exit", "Your score", std::to_string(Score),
		"High score", std::to_string(highScore) };
	menuGame.setTextItems(lstItems);
	Score = 0;
	switch (menuGame.showMenu(gFont, gScreen[0]))
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
		menuLoaded = false;
		break;
	}
	return 0;
}

int play()
{
	if (!loadBackGround("./Data/background.png"))
		return -1;

	ImpTimer fpsTimer;
	Mix_HaltChannel(-1);
	SDLCommonFunc::playSound(2, gSound);

	GameMap game_map;
	game_map.loadMap("./Data/map/map01.dat");
	game_map.loadTiles(gScreen[0]);

	drawText.setColor(TextObject::RED_TEXT);
	std::string txt;

	PlayerObject* player = new PlayerObject();
	player->loadImg("./Data/player/run.png", gScreen[0]);
	player->setClips();
	float playerPos[2] = { 0, 0 }; // Toa do cua goc ngoai cung ben duoi cua nhan vat

	std::vector<BirdObject*> lstBirds = createThreatList();
	PillarObject* pillar = new PillarObject();
	pillar->loadImg("./Data/map/pillar.png", gScreen[0]);
	bool connected = false;
	int stepRecline = 0;
	int remainLength = 0, xTile = 0, yTile = 0, numTiles = 0;
	int defaultLength = 0;
	
	bool isQuit = false;
	int start_time = SDL_GetTicks();
	while (!isQuit) {
		if (player->isDied()) {
			SDLCommonFunc::playSound(4, gSound);
			SDL_Delay(500); // Tam dung de nghe am thanh chet
			isQuit = true;
			break;
		}
		fpsTimer.start();
		SDL_SetRenderDrawColor(gScreen[0], RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(gScreen[0]);

		gBackground.render(gScreen[0], NULL);
		Map map_data = game_map.getMap();
		playerPos[0] = player->getPosX() + player->get_width_frame();
		playerPos[1] = player->getPosY() + player->get_height_frame();
		// Kiem tra su kien
		while (SDL_PollEvent(&gEvent) != 0) {
			switch (gEvent.type)
			{
			case SDL_QUIT:
				isQuit = true;
				break;
			case SDL_KEYDOWN:
				if (gEvent.key.keysym.sym == SDLK_ESCAPE) {
					fpsTimer.paused();
					gWindow[1] = SDL_CreateWindow("Pause", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
						400, 300, SDL_WINDOW_SHOWN);
					gScreen[1] = SDL_CreateRenderer(gWindow[1], -1, SDL_RENDERER_ACCELERATED);
					Menu* subMenu = new Menu();
					subMenu->loadImg("./Data/pause.png", gScreen[1]);
					subMenu->setNumItems(2);
					vector<int> px{ 130, 130 };
					vector<int> py{ 160, 60 };
					vector<int> pz{ 8, 4 };
					vector<std::string> lstItem{ "Continue", "Exit" };
					subMenu->setPos(px, py);
					subMenu->setLength(pz);
					subMenu->setTextItems(lstItem);
					switch (subMenu->showMenu(gFont, gScreen[1]))
					{
					case TypeMenu::Exit:
						isQuit = true;
					case TypeMenu::Continue:
						fpsTimer.continue_();
					default:
					{
						SDL_DestroyWindow(gWindow[1]);
						SDL_DestroyRenderer(gScreen[1]);
						delete subMenu;
					}
						break;
					}
				}
				break;
			default:
				break;
			}
			player->handelInputAction(gEvent, gScreen[0]);
			pillar->handelInputAction(gEvent, gScreen[0], playerPos[0], playerPos[1], player->isStopping());
		}

		// Tinh toan do dai yeu cau
		if (player->isStopping()) {
			int xTile2 = (playerPos[0] + map_data.start_x) / TILE_SIZE;
			int yTile2 = (playerPos[1] - 1 + map_data.start_y) / TILE_SIZE + 1;
			defaultLength = 0;
			while (map_data.tile[yTile2][xTile2 + defaultLength] == 0) {
				defaultLength++;
			}
		}
		else
			defaultLength = 0;

		// Thuc hien viec nga cua cay cau
		if (!pillar->isReclined() && stepRecline < NUM_RECLINE_STEPS) {
			stepRecline++;
			pillar->recline(stepRecline, gScreen[0]);
		}
		else {
			stepRecline = 0;
			if (pillar->getHeight() > 0) {
				numTiles = pillar->getHeight() / TILE_SIZE;
				if (numTiles <= defaultLength && numTiles >= (defaultLength - 1))
					Score++;
				remainLength = pillar->getHeight() - numTiles * TILE_SIZE;
				xTile = (playerPos[0] + map_data.start_x) / TILE_SIZE;
				yTile = (playerPos[1] - 1 + map_data.start_y) / TILE_SIZE + 1;
				for (int i = 0; i < numTiles; i++) {
					if (map_data.tile[yTile][xTile + i] == 0)
						map_data.tile[yTile][xTile + i] = 10; // O co ve san cay cau
					else
						map_data.tile[yTile][xTile + i] = 11;
				}
				pillar->reset();
				connected = true;
			}
		}

		player->setMapXY(map_data.start_x, map_data.start_y); // Cap nhat vi tri ban do hien tai
		player->doPlayer(map_data, &connected);				 // Thuc hien tuong tac giua nhan vat va ban do
		player->show(gScreen[0]);

		game_map.setMap(map_data);
		game_map.drawMap(gScreen[0]);

		// Kiem tra va cham voi chim
		SDL_Rect pRect = player->getRectFrame();
		for (int i = 0; i < lstBirds.size(); i++) {
			BirdObject* threat = lstBirds.at(i);
			if (threat != NULL) {
				threat->setMapXY(map_data.start_x, map_data.start_y);
				threat->doBird();
				if (threat->get_x_pos() > map_data.start_x) {
					threat->show(gScreen[0]);
					SDL_Rect bRect = { threat->getRect().x, threat->getRect().y, threat->get_width_frame(), threat->get_height_frame() };
					if (SDLCommonFunc::checkCollision(pRect, bRect)) {
						Blood -= (DELTA_BLOOD / 3);
						SDLCommonFunc::playSound(3, gSound);
					}
				}
			}
		}

		// Hien thi thong tin ve diem so va thoi gian
		txt = "time " + std::to_string((SDL_GetTicks() - start_time) / 1000);
		drawText.setRect(SCREEN_WIDTH - 4 * TILE_SIZE, TILE_SIZE / 2);
		drawText.setSize(12, 1);
		drawText.setText(txt);
		drawText.drawText(gFont, gScreen[0]);
		
		txt = "score " + std::to_string(Score);
		drawText.setRect(SCREEN_WIDTH - 4 * TILE_SIZE, 0);
		drawText.setSize(10, 1);
		drawText.setText(txt);
		drawText.drawText(gFont, gScreen[0]);

		// Hien thi thanh mau
		SDL_SetRenderDrawColor(gScreen[0], (Uint8)255, (Uint8)0, (Uint8)0, 0xFF);
		SDL_Rect rectBlob = { 0,0,LENGTH_BLOOD_MAX,30 };
		SDL_RenderDrawRect(gScreen[0], &rectBlob);
		if (player->isStopping()) {
			Blood -= MINUS_BLOOD;
		}
		if (Blood < 0)
			break;
		rectBlob = { 0,0, Blood, 30 };
		SDL_RenderFillRect(gScreen[0], &rectBlob);

		// Hien thi phan con lai cua cay cau ket noi
		if (remainLength > 0) {
			SDL_SetRenderDrawColor(gScreen[0], (Uint8)80, (Uint8)80, (Uint8)150, 0xFF);
			SDL_Rect rect = { (xTile + numTiles)*TILE_SIZE - map_data.start_x, yTile*TILE_SIZE - map_data.start_y,
				remainLength, WIDTH_PILLAR };
			SDL_RenderFillRect(gScreen[0], &rect);
		}
		// update screen
		SDL_RenderPresent(gScreen[0]);
		int render_time = fpsTimer.get_ticks();
		if (render_time < TIME_ONE_FRAME)
			SDL_Delay(TIME_ONE_FRAME - render_time);// uint32 x; if x < 0 then x = 0
	}
	fpsTimer.stop();
	Blood = LENGTH_BLOOD_MAX;
	delete player, pillar;
	return endMenu();
}

int main(int argc, char* argv[])
{
	if (!initData())
		return 2;
	
	menuGame.loadImg("./Data/menu_background.png", gScreen[0]);
	Menu* subMenu = NULL; // Thiet lap menu phu
	SDLCommonFunc::playSound(1, gSound);
	while (true) {
		if (!menuLoaded) {
			menuGame.setNumItems(5);
			vector<int> px{ 200, 200, 800, 800, 1100 };
			vector<int> py{ 200, 300, 200, 300, 550 };
			vector<int> pz{ 4,5,11,13,4 };
			menuGame.setPos(px, py);
			menuGame.setLength(pz);
			vector<std::string> lstItem{ "Play", "Level", "Instruction", "Documentation", "Exit" };
			menuGame.setTextItems(lstItem);
			
			switch (menuGame.showMenu(gFont, gScreen[0]))
			{
			case TypeMenu::Exit:
				return 0;
				break;
			case TypeMenu::Play:
			{
				menuLoaded = true;
				if (play() == -1)
					return -1;
			}
				break;
			case TypeMenu::Instruction:
			{
				//SDL_Init(SDL_INIT_VIDEO);
				//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
				gWindow[1] = SDL_CreateWindow("Instruction", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
					SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
				gScreen[1] = SDL_CreateRenderer(gWindow[1], -1, SDL_RENDERER_ACCELERATED);
				subMenu = new Menu();
				subMenu->loadImg("./Data/Instruction.png", gScreen[1]);
				if (subMenu->showMenu(gFont, gScreen[1]) == TypeMenu::Exit) {
					SDL_DestroyWindow(gWindow[1]);
					SDL_DestroyRenderer(gScreen[1]);
					delete subMenu;
					IMG_Quit();
				}
			}
				break;
			case TypeMenu::Documentation:
			{
				gWindow[1] = SDL_CreateWindow("Documentation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
					SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
				gScreen[1] = SDL_CreateRenderer(gWindow[1], -1, SDL_RENDERER_ACCELERATED);
				subMenu = new Menu();
				subMenu->loadImg("./Data/Documentation.png", gScreen[1]);
				if (subMenu->showMenu(gFont, gScreen[1]) == TypeMenu::Exit) {
					SDL_DestroyWindow(gWindow[1]);
					SDL_DestroyRenderer(gScreen[1]);
					delete subMenu;
				}
			}
				break;
			case TypeMenu::Level:
			{
				gWindow[1] = SDL_CreateWindow("Game Level", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
					400, 300, SDL_WINDOW_SHOWN);
				gScreen[1] = SDL_CreateRenderer(gWindow[1], -1, SDL_RENDERER_ACCELERATED);
				subMenu = new Menu();
				subMenu->loadImg("./Data/level.png", gScreen[1]);
				subMenu->setNumItems(2);
				vector<int> px1{ 50, 50 };
				vector<int> py1{ 60, 160 };
				vector<int> pz1{ 8, 12 };
				vector<std::string> lstItem1{ "Beginner", "Professional" };
				subMenu->setPos(px1, py1);
				subMenu->setLength(pz1);
				subMenu->setTextItems(lstItem1);
				switch (subMenu->showMenu(gFont, gScreen[1]))
				{
				case TypeMenu::Beginner:
					PLAYER_SPEED_X = 5.0;
				case TypeMenu::Exit:
				{
					SDL_DestroyWindow(gWindow[1]);
					SDL_DestroyRenderer(gScreen[1]);
					delete subMenu;
				}
					break;
				case TypeMenu::Professional:
					PLAYER_SPEED_X = 8.0;
				default:
				{
					SDL_DestroyWindow(gWindow[1]);
					SDL_DestroyRenderer(gScreen[1]);
					delete subMenu;
				}
					break;
				}
			}
				break;
			default:
				break;
			}
		}
		else {
			menuLoaded = true;
			if (play() == -1)
				return -1;
		}
	}
	close();
	return 0;
}