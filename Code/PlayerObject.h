#pragma once
#ifndef PLAYER_OBJECT_H
#define PLAYER_OBJECT_H

#include "CommonFunc.h"
#include "BaseObject.h"
#include <iostream>

#define NUMBER_FRAME 7

class PlayerObject : public BaseObject
{
public:
	PlayerObject();
	~PlayerObject();

	enum runType {
		RUN_RIGHT = 0,
		RUN_LEFT = 1
	};
	bool loadImg(std::string path, SDL_Renderer* screen); // khai bao lai de lay thong so cua tam anh
	void show(SDL_Renderer* des); // show theo tung frame
	void handelInputAction(SDL_Event events, SDL_Renderer* screen);
	void setClips(); // animation

	void doPlayer(Map& map_data, bool *drawedPL);
	void checkToMap(Map& map_data, bool *drawedPL); // kiem tra va cham voi ban do
	void setMapXY(const int mapX, int mapY) { map_x = mapX; map_y = mapY; };
	void centerEntityOnMap(Map &map_data); // tinh toan vi tri ban do
	bool isDied() { return is_died; };
	void increaseBlood();

	int get_width_frame() const { return width_frame; };
	int get_height_frame() const { return height_frame; };
	float getPosX() const { return x_pos - map_x; };
	float getPosY() const { return y_pos - map_y; };
	bool isStopping() { return stop; };
	SDL_Rect getRectFrame() const;

private:
	float x_val; // do di chuyen sang phai
	float y_val; // do di chuyen len tren
	bool stop;

	float x_pos;
	float y_pos;
	int width_frame; // kich thuoc 1 frame
	int height_frame;
	SDL_Rect frame_clip[NUMBER_FRAME];
	Input input_type;
	int frame_id;
	int status_; // left _ right
	bool on_ground;

	int map_x; // vi tri hien tai cua ban do
	int map_y;

	bool is_died;
	bool leaved; // roi khoi vung dat cu
	
	void updateImgPlayer(SDL_Renderer* des);
};

#endif // !PLAYER_OBJECT_H

