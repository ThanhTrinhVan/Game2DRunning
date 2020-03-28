#include "Game_map.h"

bool GameMap::loadMap(const char* name)
{
	FILE* fp = NULL;
	fopen_s(&fp, name, "rb");
	if (fp == NULL)
		return false;
	game_map.max_x = 0;
	game_map.max_y = 0;
	for (int i = 0; i < MAX_MAP_Y; i++) {
		for (int j = 0; j < MAX_MAP_X; j++) {
			fscanf_s(fp, "%d", &game_map.tile[i][j]);
			if (game_map.tile[i][j] > 0) {
				if (j > game_map.max_x)
					game_map.max_x = j;
				if (i > game_map.max_y)
					game_map.max_y = i;
			}
		}
	}
	game_map.max_x = (game_map.max_x + 1)*TILE_SIZE;
	game_map.max_y = (game_map.max_y + 1)*TILE_SIZE;

	game_map.start_x = 0;
	game_map.start_y = 0;
	game_map.file_name = name;
	fclose(fp);
	return true;
}

bool GameMap::loadTiles(SDL_Renderer* screen)
{
	if (screen == NULL)
		return false;
	char file_img[100];
	FILE* fp = NULL;
	for (int i = 0; i < MAX_TILES_ONSCREEN; i++) {
		sprintf_s(file_img, "./Data/map/%d.png", i);
		fopen_s(&fp, file_img, "rb");
		if (fp == NULL)
			continue;
		fclose(fp);
		tileMat[i].loadImg(file_img, screen);
	}
	return true;
}

bool GameMap::drawMap(SDL_Renderer* screen)
{
	if (screen == NULL)
		return false;
	int x1 = 0, y1 = 0;
	int x2 = 0, y2 = 0;
	int map_x = 0, map_y = 0;

	map_x = game_map.start_x / TILE_SIZE;
	x1 = (game_map.start_x % TILE_SIZE)*-1;
	x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);

	map_y = game_map.start_y / TILE_SIZE;
	y1 = (game_map.start_y % TILE_SIZE)*-1;
	y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);

	for (int i = y1; i < y2; i += TILE_SIZE) {
		map_x = game_map.start_x / TILE_SIZE;
		for (int j = x1; j < x2; j += TILE_SIZE) {
			int val = game_map.tile[map_y][map_x];
			if (val > 0) {
				tileMat[val].setRect(j, i);
				tileMat[val].render(screen);
			}
			map_x++;
		}
		map_y++;
	}
	return true;
}