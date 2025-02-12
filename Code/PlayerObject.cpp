#include "PlayerObject.h"
#include <iostream>

PlayerObject::PlayerObject()
{
	frame_id = 0;
	x_pos = 0;
	y_pos = 0;
	x_val = 0;
	y_val = 0;
	stop = false;
	width_frame = 64;
	height_frame = 64;
	input_type.left_ = 0;
	input_type.right_ = 0;
	input_type.jump_ = 0;
	input_type.down_ = 0;
	input_type.up_ = 0;
	on_ground = false;
	map_x = 0;
	map_y = 0;
	is_died = false;
	leaved = false;
}

PlayerObject::~PlayerObject()
{
}

bool PlayerObject::loadImg(std::string path, SDL_Renderer * screen)
{
	if (BaseObject::loadImg(path, screen) == true) {
		width_frame = rect_.w / NUMBER_FRAME;
		height_frame = 1 * rect_.h;
		return true;
	}
	return false;
}

void PlayerObject::show(SDL_Renderer * des)
{
	updateImgPlayer(des);
	frame_id++;
	if (frame_id >= NUMBER_FRAME)
		frame_id = 0;

	rect_.x = x_pos - map_x; // Vi tri tren ban do hien tai
	rect_.y = y_pos - map_y;

	SDL_Rect* current_clip = &frame_clip[frame_id];
	SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame, height_frame };
	SDL_RenderCopy(des, p_object, current_clip, &renderQuad);
}

void PlayerObject::handelInputAction(SDL_Event events, SDL_Renderer * screen)
{
	if (events.type == SDL_MOUSEBUTTONDOWN) {
		if (events.button.button == SDL_BUTTON_LEFT)
			input_type.jump_ = 1;
	}
}

void PlayerObject::setClips()
{
	if (width_frame > 0 && height_frame > 0) {
		for (int i = 0; i < NUMBER_FRAME; i++) {
			frame_clip[i].x = i * width_frame;
			frame_clip[i].y = 0;
			frame_clip[i].w = width_frame;
			frame_clip[i].h = height_frame;
		}
	}
}

void PlayerObject::doPlayer(Map & map_data, bool *connected)
{
	x_val = PLAYER_SPEED_X + Score * COEFFICIENT_SPEED;
	stop = false;
	y_val += PLAYER_DELTA_SPEED_Y;
	if (y_val >= MAX_FALL_SPEED) {
		y_val = MAX_FALL_SPEED;
	}
	if (input_type.jump_ == 1) {
		if (on_ground) {
			y_val = -PLAYER_JUMP;
			on_ground = false;
		}
		input_type.jump_ = 0;
	}

	checkToMap(map_data, connected);
	centerEntityOnMap(map_data);
}

bool PlayerObject::checkToMap(Map & map_data, bool* connected)
{
	if (connected == NULL || &map_data == NULL)
		return false;
	int x1 = 0, y1 = 0;
	int x2 = 0, y2 = 0;

	// Kiem tra va cham theo truc ngang
	int height_min = height_frame < TILE_SIZE ? height_frame : TILE_SIZE;
	x1 = (x_pos + x_val) / TILE_SIZE;					 // So thu tu cua o TILE
	x2 = (x_pos + x_val + width_frame - 1) / TILE_SIZE;
	y1 = (y_pos) / TILE_SIZE;
	y2 = (y_pos + height_min - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
		if (x_val > 0) // Chuyen dong sang phai
		{
			int val1 = map_data.tile[y1][x2];
			int val2 = map_data.tile[y2][x2];
			// va cham voi o mau, o do se bien mat, gan vao 0
			if (val1 == SUPPORT_TILE || val2 == SUPPORT_TILE) {
				map_data.tile[y1][x2] = 0;
				map_data.tile[y2][x2] = 0;
				SDLCommonFunc::playSound(5, gSound);
				increaseBlood();
			}
			else {
				if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
					x_pos = x2 * TILE_SIZE;		// Dung tai vi tri x2
					x_pos -= (width_frame + 1);
					x_val = 0;
					stop = true;
				}
			}
		}
		else if (x_val < 0) {
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y2][x1];
			if (val1 == SUPPORT_TILE || val2 == SUPPORT_TILE) {
				map_data.tile[y1][x1] = 0;
				map_data.tile[y2][x1] = 0;
				SDLCommonFunc::playSound(5, gSound);
				increaseBlood();
			}
			else {
				if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
					x_pos = (x1 + 1) * TILE_SIZE; // Dung tai vi tri x1
					x_val = 0;
					stop = true;
				}
			}
		}
	}

	// Kiem tra va cham theo truc doc
	int width_min = width_frame < TILE_SIZE ? width_frame : TILE_SIZE;
	x1 = (x_pos) / TILE_SIZE; 
	x2 = (x_pos + width_min) / TILE_SIZE;
	y1 = (y_pos + y_val) / TILE_SIZE;
	y2 = (y_pos + y_val + height_frame - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
		if (y_val > 0) // Chuyen dong xuong duoi
		{
			int val1 = map_data.tile[y2][x1];
			int val2 = map_data.tile[y2][x2];
			// va cham voi o mau, o do se bien mat, gan vao 0
			if (val1 == SUPPORT_TILE || val2 == SUPPORT_TILE) {
				map_data.tile[y1][x2] = 0;
				map_data.tile[y2][x2] = 0;
				SDLCommonFunc::playSound(5, gSound);
				increaseBlood();
			}
			else {
				if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
					y_pos = y2 * TILE_SIZE;		// Dung tai vi tri x2
					y_pos -= (height_frame + 1);
					y_val = 0;
					on_ground = true;
				}
			}
		}
		else if (y_val < 0) {
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y1][x2];
			// va cham voi o mau, o do se bien mat, gan vao 0
			if (val1 == SUPPORT_TILE || val2 == SUPPORT_TILE) {
				map_data.tile[y1][x2] = 0;
				map_data.tile[y2][x2] = 0;
				SDLCommonFunc::playSound(5, gSound);
				increaseBlood();
			}
			else {
				if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
					y_pos = (y1 + 1) * TILE_SIZE; // Dung tai vi tri x1
					y_val = 0;
				}
			}
		}
	}

	// Kiem tra viec di tu vung dat cu sang vung dat moi => qua cau => tinh diem
	x2 = (x_pos) / TILE_SIZE;
	x1 = (x_pos - width_min) / TILE_SIZE;
	y2 = (y_pos + height_min) / TILE_SIZE + 1;
	if (map_data.tile[y2][x2] != 2) // Roi khoi vung dat cu, 2 - la gia tri cua TILE mat dat
		leaved = true;

	if (leaved && (map_data.tile[y2][x2] == 7 || map_data.tile[y2][x2] == 6 
		|| map_data.tile[y2][x2] == 2)) {
		// Den vung dat moi
		*connected = false;
		leaved = false;
	}
	// Kiem tra vuc tham
	if (!(*connected)) {
		x2 = (x_pos + width_min) / TILE_SIZE;
		x1 = x_pos / TILE_SIZE;
		y2 = (y_pos + height_min) / TILE_SIZE + 1;
		if (on_ground && map_data.tile[y2][x2] == BLANK_TILE && 
			(map_data.tile[y2][x1] == 5 || map_data.tile[y2][x1] == 7)) {
			x_val = 0;
			stop = true;
		}
		else {
			x_val = PLAYER_SPEED_X + Score * COEFFICIENT_SPEED;
		}
	}

	x_pos += x_val;
	y_pos += y_val;
	if (x_pos < 0)
		x_pos = 0;
	else if (x_pos + width_frame > map_data.max_x)
		x_pos = map_data.max_x - width_frame - 1;
	if (y_pos > map_data.max_y)
	{
		is_died = true;
	}
	return true;
}

bool PlayerObject::centerEntityOnMap(Map & map_data)
{
	if (&map_data == NULL)
		return false;
	map_data.start_x = x_pos - (SCREEN_WIDTH / 3); // Khi nhan vat di den 1/3 ban do thi ban do duoc cuon theo
	if (map_data.start_x < 0) {
		map_data.start_x = 0;
	}
	else if (map_data.start_x + SCREEN_WIDTH >= map_data.max_x) {
		map_data.start_x = map_data.max_x - SCREEN_WIDTH;
	}
	map_data.start_y = y_pos - (SCREEN_HEIGHT / 2);
	if (map_data.start_y < 0)
		map_data.start_y = 0;
	else if (map_data.start_y + SCREEN_HEIGHT >= map_data.max_y)
		map_data.start_y = map_data.max_y - SCREEN_HEIGHT;
	return true;
}

void PlayerObject::increaseBlood()
{
	Blood += DELTA_BLOOD;
	if (Blood > LENGTH_BLOOD_MAX)
		Blood = LENGTH_BLOOD_MAX;
}

SDL_Rect PlayerObject::getRectFrame() const
{
	SDL_Rect rt = { rect_.x, rect_.y, width_frame, height_frame };
	return rt;
}

void PlayerObject::updateImgPlayer(SDL_Renderer * des)
{
	if (on_ground == true) {
		if (stop == false) {
			loadImg("./Data/player/run.png", des);
		}
		else {
			loadImg("./Data/player/jump.png", des);
		}
		//loadImg("./Data/player/run.png", des);
	}
	else {
		loadImg("./Data/player/jump.png", des);
	}
}