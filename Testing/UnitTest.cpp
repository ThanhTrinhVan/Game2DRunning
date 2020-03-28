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

namespace UnitTest
{
	TEST_CLASS(CommonFuncClass)
	{
	public:
		TEST_METHOD(Collision)
		{
			SDL_Rect* obj1 = NULL;
			SDL_Rect* obj2 = NULL;
			Assert::AreEqual(false, SDLCommonFunc::checkCollision(*obj1, *obj2));
			obj1 = new SDL_Rect{ 0,0, 100,100 };
			obj2 = new SDL_Rect{ 200,200, 10, 10 };
			Assert::AreEqual(false, SDLCommonFunc::checkCollision(*obj1, *obj2));
			obj1->w = 250;
			obj1->h = 250;
			Assert::AreEqual(true, SDLCommonFunc::checkCollision(*obj1, *obj2));
		}
		TEST_METHOD(Focus)
		{
			SDL_Rect* obj1 = NULL;
			Assert::AreEqual(false, SDLCommonFunc::checkFocus(10, 10, *obj1));
			obj1 = new SDL_Rect{ 0,0, 100,100 };
			Assert::AreEqual(true, SDLCommonFunc::checkFocus(10, 10, *obj1));
		}
		TEST_METHOD(TypeMenu)
		{
			bool passed = true;
			if (SDLCommonFunc::checkType("Play") != TypeMenu::Play)
				passed = false;
			Assert::AreEqual(true, passed);
		}
		TEST_METHOD(Play_Sound)
		{
			bool passed = true;
			Mix_Chunk* gsound = Mix_LoadWAV("./Data/audio/menu.wav");
			Mix_PlayChannel(-1, gsound, -1);
			SDLCommonFunc::playSound(1, gSound);
			if (gSound != gsound)
				passed = false;
			Assert::AreEqual(true, passed);
			Assert::AreEqual(false, SDLCommonFunc::playSound(6, gSound));
		}
	};
	
	TEST_CLASS(MenuClass)
	{
	public:
		TEST_METHOD(SetPos)
		{
			menu->setNumItems(5);
			vector<int> tmp;
			Assert::AreEqual(false, menu->setPos(tmp, tmp));
			tmp = vector<int>(5);
			Assert::AreEqual(true, menu->setPos(tmp, tmp));
		}
		TEST_METHOD(SetLength)
		{
			menu->setNumItems(5);
			vector<int> tmp;
			Assert::AreEqual(false, menu->setLength(tmp));
			tmp = vector<int>(5);
			Assert::AreEqual(true, menu->setLength(tmp));
		}
		TEST_METHOD(SetTextItems)
		{
			menu->setNumItems(5);
			vector<std::string> tmp;
			Assert::AreEqual(false, menu->setTextItems(tmp));
			tmp = vector<std::string>(5);
			Assert::AreEqual(true, menu->setTextItems(tmp));
		}
	private:
		Menu* menu = new Menu();
	};
	
	TEST_CLASS(PlayerObjectClass)
	{
	public:
		TEST_METHOD(LoadImg)
		{
			Assert::AreEqual(false, player->loadImg("", gScreen[0]));
			Assert::AreEqual(false, player->loadImg("./Data/player/run.png", NULL));
			//initData();
			//Assert::AreEqual(true, player->loadImg("./Data/player/run.png", gScreen[0]));
		}
		TEST_METHOD(CheckToMap)
		{
			Map* game_map = NULL;
			bool cnt = false;
			Assert::AreEqual(false, player->checkToMap(*game_map, &cnt));
			game_map = createMap();
			Assert::AreEqual(true, player->checkToMap(*game_map, &cnt));
			Assert::AreEqual(false, player->checkToMap(*game_map, NULL));
		}
		TEST_METHOD(CenterEntityOnMap)
		{
			Map* map = NULL;
			Assert::AreEqual(false, player->centerEntityOnMap(*map));
			map = createMap();
			Assert::AreEqual(true, player->centerEntityOnMap(*map));
		}
	private:
		PlayerObject* player = new PlayerObject();
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
	
	TEST_CLASS(BirdObjectClass)
	{
	public:
		TEST_METHOD(LoadImg)
		{
			Assert::AreEqual(false, bird->loadImg("", gScreen[0]));
			Assert::AreEqual(false, bird->loadImg("./Data/threat/bird.png", NULL));
			//initData();
			//                Assert::AreEqual(true, bird->loadImg("./Data/threat/bird.png", gScreen[0]));
		}
	private:
		BirdObject* bird = new BirdObject();
	};

	TEST_CLASS(BaseObjectClass)
	{
	public:
		TEST_METHOD(Init)
		{
			BaseObject Base;
			SDL_Rect rect_;
			int x, y, w, h;
			rect_ = Base.getRect();
			x = rect_.x;
			y = rect_.y;
			w = rect_.w;
			h = rect_.h;
			Assert::AreEqual(0, x);
			Assert::AreEqual(0, y);
			Assert::AreEqual(0, w);
			Assert::AreEqual(0, h);
		}
		TEST_METHOD(Delete)
		{
			BaseObject* Base = new BaseObject();
			SDL_Texture* p_object;
			SDL_Rect rect_;
			bool deleted = false;
			p_object = Base->getObject();
			if (p_object != NULL)
			{
				Base->free();
			}
			if (Base->getObject() == NULL)
			{
				deleted = true;
			}
			Assert::AreEqual(true, deleted);
		}
		TEST_METHOD(LoadImg)
		{
			BaseObject Base;
			Assert::AreEqual(false, Base.loadImg("", gScreen[0]));
			Assert::AreEqual(false, Base.loadImg("./Data/player/run.png", NULL));
		}
	};

	TEST_CLASS(TextObjectClass)
	{
	public:
		TEST_METHOD(Init)
		{
			TextObject Text;
			int x, y;
			x = Text.getRect().x;
			y = Text.getRect().y;
			Assert::AreEqual(0, x);
			Assert::AreEqual(0, y);
		}
		TEST_METHOD(SetColor)
		{
			TextObject Text;
			Text.setColor(0);
			bool check = false;
			SDL_Color color;
			color = Text.getTextColor();
			if ((color.r = 255)||(color.g = 0)||(color.b = 0))
			{
				check = true;
			}
			Assert::AreEqual(true, check);
		}
		TEST_METHOD(SetText)
		{
			TextObject Text;
			std::string textVal;
			std::string str = "Hello";
			Text.setText("Hello");
			Assert::AreEqual(1, str.compare(textVal));
		}
	};

	TEST_CLASS(PillarObjectClass)
	{
	public:
		TEST_METHOD(reset)
		{
			PillarObject Pillar;
			Pillar.reset();
			Assert::AreEqual(false, Pillar.isReclined());
			Assert::AreEqual(0, Pillar.getHeight());
			Assert::AreEqual(0, Pillar.getPosX());
			Assert::AreEqual(0, Pillar.getPosY());
		}
	};

	TEST_CLASS(ImpTimerObject)
	{
	public:
		TEST_METHOD(Start)
		{
			ImpTimer Timer;
			Timer.start();
			Assert::AreEqual(true, Timer.is_start());
			Assert::AreEqual(false, Timer.is_pause());
		}
		TEST_METHOD(Stop)
		{
			ImpTimer Timer;
			Timer.start();
			Timer.stop();
			Assert::AreEqual(false, Timer.is_pause());
			Assert::AreEqual(false, Timer.is_start());
		}
		TEST_METHOD(Pause)
		{
			ImpTimer Timer;
			Timer.start();
			Timer.paused();
		    Assert::AreEqual(true, Timer.is_pause());
		}
		TEST_METHOD(Continue)
		{
			ImpTimer Timer;
			Timer.start();
			Timer.paused();
			Timer.continue_();
			Assert::AreEqual(false, Timer.is_pause());
			Assert::AreEqual(true, Timer.is_start());
		}
	};

	TEST_CLASS(Game_map)
	{
	public:
		TEST_METHOD(loadMap)
		{
			GameMap* gm = new GameMap();
			Assert::AreEqual(false, gm->loadMap(""));

		}
		TEST_METHOD(loadTiles)
		{
			GameMap* gm = new GameMap();
			Assert::AreEqual(false, gm->loadTiles(NULL));
		}
		TEST_METHOD(drawMap)
		{
			GameMap* gm = new GameMap();
			Assert::AreEqual(false, gm->drawMap(NULL));
		}
	};
};
