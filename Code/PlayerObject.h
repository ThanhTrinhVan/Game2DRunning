#pragma once
#ifndef PLAYER_OBJECT_H
#define PLAYER_OBJECT_H

#include "CommonFunc.h"
#include "BaseObject.h"

#define NUMBER_FRAME 7

class PlayerObject : public BaseObject
{
public:
	PlayerObject();
	~PlayerObject();

	bool loadImg(std::string path, SDL_Renderer* screen);			// Dinh nghia lai ham virtual de thuc hien chia cat frame
	void show(SDL_Renderer* des);									// Hien thi theo tung frame => tao hieu ung dong
	void handelInputAction(SDL_Event events, SDL_Renderer* screen); // Kiem tra su kien dieu khien nhan vat
	void setClips();												// Phan chia frame de tao hieu ung dong

	void doPlayer(Map& map_data, bool *connected);					// hanh dong cua nhan vat 
	void checkToMap(Map& map_data, bool *connected);					// kiem tra va cham cua nhan vat voi ban do
	void setMapXY(const int mapX, int mapY) {						// cap nhat ban do
		map_x = mapX; 
		map_y = mapY; 
	};
	void centerEntityOnMap(Map &map_data);							// Thiet lap ban do chay theo nhan vat
	bool isDied() { return is_died; };								// Kiem tra nhan vat
	void increaseBlood();											// An mau

	int get_width_frame() const { return width_frame; };
	int get_height_frame() const { return height_frame; };
	float getPosX() const { return x_pos - map_x; };
	float getPosY() const { return y_pos - map_y; };
	bool isStopping() { return stop; };
	SDL_Rect getRectFrame() const;

private:
	float x_val;						// Toc do di chuyen theo chieu ngang tren mot khung hinh
	float y_val;						// Toc do di chuyen theo chieu doc tren mot khung hinh
	bool stop;							// Dung lai khi gap phai vuc tham

	float x_pos;						// Vi tri cua mep ben trai theo truc ngang
	float y_pos;						// Vi tri cua mep ben tren theo truc doc
	int width_frame;					// Kich thuoc 1 frame
	int height_frame;
	SDL_Rect frame_clip[NUMBER_FRAME];	// Tap hop cac frame => tao thanh chuyen dong
	TypeMotion input_type;				// Hinh thuc chuyen dong cua nhan vat
	int frame_id;						// Id cua frame hien tai trong ds frame_clip
	bool on_ground;						// Tren mat dat hay dang nhay

	int map_x;							// Vi tri cua ban do hien tai trong mang toan bo ban do
	int map_y;

	bool is_died;
	bool leaved;						// Kiem tra xem da di qua vuc tham chua - roi khoi vung dat cu

	Mix_Chunk *gSound = NULL;
	void updateImgPlayer(SDL_Renderer* des);
};
#endif // !PLAYER_OBJECT_H