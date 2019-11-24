#include "pch.h"
#include "BirdObject.h"


BirdObject::BirdObject()
{
	x_pos = 0;
	y_pos = 0;
	x_val = 0;
	y_val = 0;
	frame_id = 0;
	frame_height = 0;
	frame_width = 0;
}


BirdObject::~BirdObject()
{
}

void BirdObject::setClips()
{
	if (frame_width > 0 && frame_height > 0) {
		for (int i = 0; i < FRAME_NUM; i++) {
			frame_clip[i].x = i * frame_width;
			frame_clip[i].y = 0;
			frame_clip[i].w = frame_width;
			frame_clip[i].h = frame_height;
		}
	}
}

bool BirdObject::loadImg(std::string path, SDL_Renderer * screen)
{
	bool ret = BaseObject::loadImg(path, screen);
	if (ret) {
		frame_width = rect_.w / FRAME_NUM;
		frame_height = rect_.h;
	}
	return ret;
}

void BirdObject::show(SDL_Renderer * des)
{
	rect_.x = x_pos - map_x;
	rect_.y = y_pos - map_y;
	frame_id++;
	if (frame_id > FRAME_NUM)
		frame_id = 0;

	SDL_Rect* currentClip = &frame_clip[frame_id];
	SDL_Rect rendQuad = { rect_.x, rect_.y, frame_width, frame_height };
	SDL_RenderCopy(des, p_object_, currentClip, &rendQuad);
}

void BirdObject::doBird(Map & gMap)
{
	x_val = BIRD_SPEED_X;
	y_val = 0;

	//checkToMap(gMap);

	x_pos += x_val;
	y_pos += y_val;
}

void BirdObject::checkToMap(Map & map_data)
{
	int x1 = 0, y1 = 0;
	int x2 = 0, y2 = 0;

	// check horizontal
	int height_min = frame_height < TILE_SIZE ? frame_height : TILE_SIZE;
	x1 = (x_pos + x_val) / TILE_SIZE; // so thu tu cua tile map
	x2 = (x_pos + x_val + frame_width - 1) / TILE_SIZE;
	y1 = (y_pos) / TILE_SIZE;
	y2 = (y_pos + height_min - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
		if (x_val > 0) // chuyen dong sang phai
		{
			int val1 = map_data.tile[y1][x2];
			int val2 = map_data.tile[y2][x2];

			if ((val1 != BLANK_TILE && val1 != SUPPORT_TILE) || (val2 != BLANK_TILE && val2 != SUPPORT_TILE)) {
				x_pos = x2 * TILE_SIZE; // dung tai vi tri x2
				x_pos -= (frame_width + 1);
				x_val = 0;
			}
		}
		else if (x_val < 0) {
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y2][x1];

			if ((val1 != BLANK_TILE && val1 != SUPPORT_TILE) || (val2 != BLANK_TILE && val2 != SUPPORT_TILE)) {
				x_pos = (x1 + 1) * TILE_SIZE; // dung tai vi tri x1
				x_val = 0;
			}
		}
	}

	// check vertical
	int width_min = frame_width < TILE_SIZE ? frame_width : TILE_SIZE;
	x1 = (x_pos) / TILE_SIZE; // so thu tu cua tile map
	x2 = (x_pos + width_min) / TILE_SIZE;
	y1 = (y_pos + y_val) / TILE_SIZE;
	y2 = (y_pos + y_val + frame_height - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
		if (y_val > 0) // chuyen dong xuong duoi
		{
			int val1 = map_data.tile[y2][x1];
			int val2 = map_data.tile[y2][x2];

			if ((val1 != BLANK_TILE && val1 != SUPPORT_TILE) || (val2 != BLANK_TILE && val2 != SUPPORT_TILE)) {
				y_pos = y2 * TILE_SIZE; // dung tai vi tri x2
				y_pos -= (frame_height + 1);
				y_val = 0;
			}
		}
		else if (y_val < 0) {
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y1][x2];

			if ((val1 != BLANK_TILE && val1 != SUPPORT_TILE) || (val2 != BLANK_TILE && val2 != SUPPORT_TILE)) {
				y_pos = (y1 + 1) * TILE_SIZE; // dung tai vi tri x1
				y_val = 0;
			}
		}
	}
}
