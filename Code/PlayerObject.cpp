#include "pch.h"
#include "PlayerObject.h"


PlayerObject::PlayerObject()
{
	frame_id = 0;
	x_pos = 0;
	y_pos = 0;
	x_val = 0;
	y_val = 0;
	width_frame = 0;
	height_frame = 0;
	status_ = -1;
	input_type.left_ = 0;
	input_type.right_ = 0;
	input_type.jump_ = 0;
	input_type.down_ = 0;
	input_type.up_ = 0;
	on_ground = false;
	map_x = 0;
	map_y = 0;
	is_died = false;
	money_count = 0;
}

PlayerObject::~PlayerObject()
{
}

bool PlayerObject::loadImg(std::string path, SDL_Renderer * screen)
{
	bool ret = BaseObject::loadImg(path, screen);
	if (ret == true) {
		width_frame = rect_.w / NUMBER_FRAME;
		height_frame = rect_.h;
	}
	return ret;
}

void PlayerObject::show(SDL_Renderer * des)
{
	updateImgPlayer(des);

	if (input_type.left_ == 1 || input_type.right_ == 1) {
		frame_id++;
	}
	else {
		frame_id = 0;
	}
	if (frame_id >= NUMBER_FRAME)
		frame_id = 0;

	rect_.x = x_pos - map_x; // khoang cach tu vi tri hien tai vua vat den vi tri hien thi cua ban do
	rect_.y = y_pos - map_y;

	SDL_Rect* current_clip = &frame_clip[frame_id];
	SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame, height_frame };
	SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
}

void PlayerObject::handelInputAction(SDL_Event events, SDL_Renderer * screen)
{
	if (events.type == SDL_KEYDOWN) {
		// nhan phim
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			status_ = RUN_RIGHT;
			input_type.right_ = 1;
			input_type.left_ = 0;
			updateImgPlayer(screen);
		}
		break;
		case SDLK_LEFT:
		{
			status_ = RUN_LEFT;
			input_type.left_ = 1;
			input_type.right_ = 0;
			updateImgPlayer(screen);
		}
		break;
		default:
			break;
		}
	}
	else if (events.type == SDL_KEYUP) {
		// tha phim
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			input_type.right_ = 0;
		}
		break;
		case SDLK_LEFT:
		{
			input_type.left_ = 0;
		}
		break;
		default:
			break;
		}
	}

	if (events.type == SDL_MOUSEBUTTONDOWN) {
		if (events.button.button == SDL_BUTTON_RIGHT)
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

void PlayerObject::doPlayer(Map & map_data)
{
	x_val = 0;
	y_val += PLAYER_SPEED_Y;
	if (y_val >= MAX_FALL_SPEED) {
		y_val = MAX_FALL_SPEED;
	}

	if (input_type.left_ == 1) {
		x_val -= PLAYER_SPEED_X;
	}
	else if (input_type.right_ == 1) {
		x_val += PLAYER_SPEED_X;
	}

	if (input_type.jump_ == 1) {
		if (on_ground) {
			y_val = -PLAYER_JUMP;
		}
		on_ground = false;
		input_type.jump_ = 0;
	}

	checkToMap(map_data);
	centerEntityOnMap(map_data);
}

void PlayerObject::checkToMap(Map & map_data)
{
	int x1 = 0, y1 = 0;
	int x2 = 0, y2 = 0;

	// check horizontal
	int height_min = height_frame < TILE_SIZE ? height_frame : TILE_SIZE;
	x1 = (x_pos + x_val) / TILE_SIZE; // so thu tu cua tile map
	x2 = (x_pos + x_val + width_frame - 1) / TILE_SIZE;
	y1 = (y_pos) / TILE_SIZE;
	y2 = (y_pos + height_min - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
		if (x_val > 0) // chuyen dong sang phai
		{
			int val1 = map_data.tile[y1][x2];
			int val2 = map_data.tile[y2][x2];

			// va cham voi o tien, o do se bien mat, gan vao 0
			if (val1 == SUPPORT_TILE || val2 == SUPPORT_TILE) {
				map_data.tile[y1][x2] = 0;
				map_data.tile[y2][x2] = 0;
				increaseMoney();
			}
			else {
				if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
					x_pos = x2 * TILE_SIZE; // dung tai vi tri x2
					x_pos -= (width_frame + 1);
					x_val = 0;
				}
			}
		}
		else if (x_val < 0) {
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y2][x1];

			if (val1 == SUPPORT_TILE || val2 == SUPPORT_TILE) {
				map_data.tile[y1][x1] = 0;
				map_data.tile[y2][x1] = 0;
				increaseMoney();
			}
			else {
				if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
					x_pos = (x1 + 1) * TILE_SIZE; // dung tai vi tri x1
					x_val = 0;
				}
			}
		}
	}

	// check vertical
	int width_min = width_frame < TILE_SIZE ? width_frame : TILE_SIZE;
	x1 = (x_pos) / TILE_SIZE; // so thu tu cua tile map
	x2 = (x_pos + width_min) / TILE_SIZE;
	y1 = (y_pos + y_val) / TILE_SIZE;
	y2 = (y_pos + y_val + height_frame - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
		if (y_val > 0) // chuyen dong xuong duoi
		{
			int val1 = map_data.tile[y2][x1];
			int val2 = map_data.tile[y2][x2];

			// va cham voi o tien, o do se bien mat, gan vao 0
			if (val1 == SUPPORT_TILE || val2 == SUPPORT_TILE) {
				map_data.tile[y1][x2] = 0;
				map_data.tile[y2][x2] = 0;
				increaseMoney();
			}
			else {
				if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
					y_pos = y2 * TILE_SIZE; // dung tai vi tri x2
					y_pos -= (height_frame + 1);
					y_val = 0;
					on_ground = true;
				}
			}
		}
		else if (y_val < 0) {
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y1][x2];

			// va cham voi o tien, o do se bien mat, gan vao 0
			if (val1 == SUPPORT_TILE || val2 == SUPPORT_TILE) {
				map_data.tile[y1][x2] = 0;
				map_data.tile[y2][x2] = 0;
				increaseMoney();
			}
			else {
				if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
					y_pos = (y1 + 1) * TILE_SIZE; // dung tai vi tri x1
					y_val = 0;
				}
			}
		}
	}

	x_pos += x_val;
	y_pos += y_val;
	if (x_pos < 0)
		x_pos = 0;
	else if (x_pos + width_frame > map_data.max_x)
		x_pos = map_data.max_x - width_frame - 1;
	if (y_pos > map_data.max_y)
		is_died = true;
}

void PlayerObject::centerEntityOnMap(Map & map_data)
{
	map_data.start_x = x_pos - (SCREEN_WIDTH / 3); // khi nhan vat di den 1/3 ban do thi ban do duoc cuon theo
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
}

void PlayerObject::increaseMoney()
{
	money_count++;
}

SDL_Rect PlayerObject::getRectFrame() const
{
	SDL_Rect rt;
	rt.x = rect_.x;
	rt.y = rect_.y;
	rt.w = width_frame;
	rt.h = height_frame;
	return rt;
}

void PlayerObject::updateImgPlayer(SDL_Renderer * des)
{
	if (on_ground == true) {
		loadImg("E:/nguyen trung kien/7/Game/Ninja/Data/player_run/run1.png", des);
	}
	else {
		loadImg("E:/nguyen trung kien/7/Game/Ninja/Data/player_run/jump1.png", des);
	}
}
