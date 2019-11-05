#pragma once
#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "CommonFunc.h"
#include "BaseObject.h"

#define MAX_TILES 20

class TileMat : public BaseObject
{
public:
	TileMat() { ; }
	~TileMat() { ; }
};

// fill tile to struct map
class GameMap
{
public:
	GameMap() { ; }
	~GameMap() { ; }

	void loadMap(const char* name);
	void loadTiles(SDL_Renderer* screen);
	void drawMap(SDL_Renderer* screen);
	void setMap(Map& map_data) { game_map_ = map_data; };
	Map getMap() const { return game_map_; };
private:
	Map game_map_; // toan bo map
	TileMat tileMat[MAX_TILES]; // man hinh hien tai
};

#endif // !GAME_MAP_H

