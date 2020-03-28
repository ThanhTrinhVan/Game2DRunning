#pragma once
#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "CommonFunc.h"
#include "BaseObject.h"

#define MAX_TILES_ONSCREEN 20 // SCREEN_WIDTH / TILE_SIZE

// Quan ly moi mot TILE ve hinh anh va vi tri render
class TileMat : public BaseObject
{
public:
	TileMat() { ; }
	~TileMat() { ; }
};

// Quan ly toan bo map ( man hinh render truoc + hien tai + sau )
class GameMap
{
public:
	GameMap() { ; }
	~GameMap() { ; }

	bool loadMap(const char* name);
	bool loadTiles(SDL_Renderer* screen);
	bool drawMap(SDL_Renderer* screen);

	void setMap(Map& map_data) { game_map = map_data; };
	Map getMap() const { return game_map; };
private:
	Map game_map;						 // quan ly toan bo ban do
	TileMat tileMat[MAX_TILES_ONSCREEN]; // mang cac TILE cua man hinh render hien tai
};

#endif // !GAME_MAP_H
