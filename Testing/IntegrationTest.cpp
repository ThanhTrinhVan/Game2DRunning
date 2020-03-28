#include "pch.h"
#include "CppUnitTest.h"
#include "../GameSDL/CommonFunc.h"
#include "../GameSDL/CommonFunc.cpp"
#include "../GameSDL/Menu.h"
#include "../GameSDL/Menu.cpp"
#include "../GameSDL/BirdObject.h"
#include "../GameSDL/BirdObject.cpp"
#include "../GameSDL/PlayerObject.h"
#include "../GameSDL/PlayerObject.cpp"
#include "../GameSDL/BaseObject.h"
#include "../GameSDL/BaseObject.cpp"
#include "../GameSDL/ImpTimer.h"
#include "../GameSDL/ImpTimer.cpp"
#include "../GameSDL/Game_map.h"
#include "../GameSDL/Game_map.cpp"
#include "../GameSDL/TextObject.h"
#include "../GameSDL/TextObject.cpp"
#include "../GameSDL/PillarObject.h"
#include "../GameSDL/PillarObject.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTest
{
	TEST_CLASS(IntegrationTest)
	{
	public:
		TEST_METHOD(PlayerBird)
		{
			PlayerObject* p = new PlayerObject();
			p->setRect(10, 10);
			p->setSizeRect(64, 64);
			SDL_Rect pRect = p->getRect();
			BirdObject* b = new BirdObject();
			b->setRect(50, 50);
			b->setSizeRect(64, 64);
			SDL_Rect bRect = b->getRect();
			
			Assert::AreEqual(true, SDLCommonFunc::checkCollision(pRect, bRect));
			p->setRect(200, 200);
			pRect = p->getRectFrame();
			Assert::AreEqual(false, SDLCommonFunc::checkCollision(pRect, bRect));
		}
		TEST_METHOD(PlayerMap_Heart)
		{
			Map* m = createMap();
			bool connected = true;
			PlayerObject* p = new PlayerObject();
			p->setRect(10, 10);
			p->setSizeRect(64, 64);
			p->setPos(1280, 91);
			p->setVal(5, 2.5);
			p->doPlayer(*m, &connected);
			Mix_Chunk* gsound = Mix_LoadWAV("./Data/audio/an_mau.wav");
			Mix_PlayChannel(-1, gsound, -1);
			bool passed = true;
			if (gsound != gSound)
				passed = false;
			Assert::AreEqual(true, passed);
		}
		TEST_METHOD(PlayerMap_Abyss)
		{
			Map* m = createMap();
			bool connected = true;
			PlayerObject* p = new PlayerObject();
			p->setPos(1664, 383.5);
			p->setVal(5, 0);
			connected = false;
			p->setOnGround(true);
			p->doPlayer(*m, &connected);
			Assert::AreEqual(true, p->isStopping());
		}
	private:
		Map* createMap() {
			Map* game_map = new Map();
			FILE* fp = NULL;
			fopen_s(&fp, "./Data/map/map01.dat", "rb");
			if (fp == NULL)
				return NULL;
			game_map->max_x = 0;
			game_map->max_y = 0;
			for (int i = 0; i < MAX_MAP_Y; i++) {
				for (int j = 0; j < MAX_MAP_X; j++) {
					fscanf_s(fp, "%d", &game_map->tile[i][j]);
					if (game_map->tile[i][j] > 0) {
						if (j > game_map->max_x)
							game_map->max_x = j;
						if (i > game_map->max_y)
							game_map->max_y = i;
					}
				}
			}
			game_map->max_x = (game_map->max_x + 1)*TILE_SIZE;
			game_map->max_y = (game_map->max_y + 1)*TILE_SIZE;

			game_map->start_x = 0;
			game_map->start_y = 0;
			game_map->file_name = "./Data/map/map01.dat";
			fclose(fp);
			return game_map;
		};
	};
}
