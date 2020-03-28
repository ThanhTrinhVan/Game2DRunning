#pragma once
#ifndef BIRD_OBJECT_H
#define BIRD_OBJECT_H

#include "CommonFunc.h"
#include "BaseObject.h"

#define FRAME_NUM 8
#define BIRD_SPEED_Y 0.5
#define BIRD_SPEED_MAX 10
#define BIRD_SPEED_X -5

class BirdObject : public BaseObject
{
public:
	BirdObject();
	~BirdObject();

	void set_x_val(float &xval) { x_val = xval; };
	void set_y_val(float &yval) { y_val = yval; };

	void set_x_pos(const float &xp) { x_pos = xp; };
	void set_y_pos(const float &yp) { y_pos = yp; };
	float get_x_pos() const { return x_pos; };
	float get_y_pos() const { return y_pos; };
	void setMapXY(int &mx, int &my) { map_x = mx; map_y = my; };

	void setClips();
	bool loadImg(std::string path, SDL_Renderer* screen);
	void show(SDL_Renderer* des);
	int get_width_frame() const { return frame_width; };
	int get_height_frame() const { return frame_height; };

	void doBird();

private:
	int map_x;						// Vi tri cua ban do hien tai trong mang ban do
	int map_y;
	float x_pos;					// Vi tri cua chim
	float y_pos;
	float x_val;					// Toc do di chuyen
	float y_val;
	SDL_Rect frame_clip[FRAME_NUM];	// Cac khung hinh 
	int frame_width;				// Kich thuoc 1 khung hinh
	int frame_height;
	int frame_id;					// Chi so cua khung hinh hien tai
};
#endif // !BIRD_OBJECT_H