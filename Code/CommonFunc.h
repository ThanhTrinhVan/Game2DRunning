#pragma once
#ifndef COMMON_FUNC_H_
#define COMMON_FUNC_H_

#include <Windows.h>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h> // image
#include <SDL_mixer.h> // sound
#include <SDL_ttf.h>   // font

// Thiet lap bien cuc bo, dieu khien chuong trinh
extern SDL_Window* gWindow[2];	  // extern: Tao mot doi tuong, su dung cho tat ca cac class
extern SDL_Renderer* gScreen[2];
static SDL_Event gEvent;		  // static: Voi moi class, se tao mot doi tuong copy cua doi tuong nay de xu ly
static Mix_Chunk* gSound = NULL;  // audio
extern TTF_Font* gFont;			  // font of text

// Thiet lap thong so man hinh
const int FRAME_PER_SECOND = 25;					// FPS
const int TIME_ONE_FRAME = 1000 / FRAME_PER_SECOND; // (ms); FPS cang nho => delay cang lon => game chay cham
const int SCREEN_WIDTH = 1280;						// 20*64
const int SCREEN_HEIGHT = 640;						// 10*64

// Thiet lap che do mau trong suot, nhung diem anh co gia tri mau nhu sau se trong suot khi tai len
const int COLOR_KEY_R = 255;
const int COLOR_KEY_G = 255;
const int COLOR_KEY_B = 255;
const int RENDER_DRAW_COLOR = 0xff; // 255

// Thiet lap thong so cho ky thuat TileMap
#define BLANK_TILE 0
#define SUPPORT_TILE 4
#define TILE_SIZE 64  // Kich thuoc cua 1 o tile 
#define MAX_MAP_X 400 // Tong so o TileMap tren truc ngang
#define MAX_MAP_Y 10  // Tong so o TileMap tren truc doc

typedef struct Map {
	int start_x;		// Toa do bat dau cua ban do dang hien thi [0...SCREEN_WIDTH]
	int start_y;		
	int max_x;			// do dai cua map = tong so tile * tile_size
	int max_y;			// do cao cua map
	int tile[MAX_MAP_Y][MAX_MAP_X]; // Mang luu gia tri cua cac tile tren ban do
	const char* file_name;			// File luu ban do
};

// Thiet lap thong so cho Ninja
#define PLAYER_DELTA_SPEED_Y 0.5
#define MAX_FALL_SPEED 10
#define PLAYER_JUMP 17				// Do thay doi cua toa do doc khi nhay
#define LENGTH_BLOOD_MAX 300
#define DELTA_BLOOD 30
#define COEFFICIENT_SPEED 0.5		// v = v0 + score*coefficient
extern float PLAYER_SPEED_X;
extern int Score;
extern int Blood;					// [0...LENGTH_BLOOD_MAX]

// Cac phuong thuc di chuyen chua Ninja
typedef struct TypeMotion {
	int left_;
	int right_;
	int up_;
	int down_;
	int jump_;
};

// Cac kieu Menu
enum TypeMenu {
	Play = 0,
	Instruction = 1,
	Documentation = 2,
	Exit = 3,
	GoToStart = 4,
	Continue = 5,
	Level = 6,
	Beginner = 7,
	Professional = 8,
	Nothing = 9
};

namespace SDLCommonFunc
{
	bool checkCollision(const SDL_Rect &object1, const SDL_Rect &object2); // kiem tra va cham giua 2 doi tuong
	bool checkFocus(const int &x, const int &y, const SDL_Rect& rect);	   // kiem tra chuot luot tren rect
	TypeMenu checkType(std::string txt);								   // kiem tra loai Menu duoc chon
	void playSound(short typeSound, Mix_Chunk* gSound);					   // thay doi am thanh
}
#endif // !COMMON_FUNC_H_


