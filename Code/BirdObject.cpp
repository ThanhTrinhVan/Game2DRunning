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
	if (BaseObject::loadImg(path, screen)) {
		frame_width = rect_.w / FRAME_NUM;
		frame_height = rect_.h;
		return true;
	}
	return false;
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
	SDL_RenderCopy(des, p_object, currentClip, &rendQuad);
}

void BirdObject::doBird()
{
	x_val = BIRD_SPEED_X;
	y_val = 0;

	x_pos += x_val;
	y_pos += y_val;
}
