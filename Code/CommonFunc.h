#pragma once
#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_ 

#include <Windows.h>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h> // sound
#include <SDL_ttf.h> // font

static SDL_Window* gWindow[2] = {NULL, NULL};
static SDL_Renderer* gScreen[2] = { NULL, NULL };
static SDL_Event gEvent;

// Audio
static Mix_Chunk* gSoundScreen = NULL;

// font text
static TTF_Font* gFont = NULL;

const int FRAME_PER_SECOND = 25; // fps

// parameter of screen
const int SCREEN_WIDTH = 1280; // 20*64
const int SCREEN_HEIGHT = 640; // 10*64
const int SCREEN_BPP = 32;

// ma mau cho background de cho trung voi ma mau background cua anh nhan vat
const int COLOR_KEY_R = 255;
const int COLOR_KEY_G = 255;
const int COLOR_KEY_B = 255;

const int RENDER_DRAW_COLOR = 0xff; // 255

#define BLANK_TILE 0
#define SUPPORT_TILE 4
#define TILE_SIZE 64 
#define MAX_MAP_X 400 // so o tile map trong 1 man hinh nhin duoc
#define MAX_MAP_Y 10 //

// Map la ban do hien tai chung ta thay
typedef struct Map {
	int start_x;
	int start_y;
	int max_x;
	int max_y;
	int tile[MAX_MAP_Y][MAX_MAP_X];
	const char* file_name;
};

#define PLAYER_SPEED_Y 0.5
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED_X 5
#define PLAYER_JUMP 17

// cau truc di chuyen cho input
typedef struct Input {
	int left_;
	int right_;
	int up_;
	int down_;
	int jump_;
};

// type of Menu
enum TypeMenu {
	Play = 0,
	Instruction = 1,
	Documentation = 2,
	Exit = 3,
	GoToStart = 4,
	Continue = 5
};

namespace SDLCommonFunc
{
	bool checkCollision(const SDL_Rect &object1, const SDL_Rect &object2);
	bool checkFocus(const int &x, const int &y, const SDL_Rect& rect);
	TypeMenu checkType(std::string txt);
}

#endif // ! COMMON_FUNCTION_H_
