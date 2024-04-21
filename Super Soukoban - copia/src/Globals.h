#pragma once

//Displaying scale factor
#define GAME_SCALE_FACTOR	2.0f

//Function return values
enum class AppStatus { OK = 0, ERROR, QUIT };

//Main return values
#define EXIT_SUCCESS	0
#define EXIT_FAILURE	1

//Log messages
#define LOG(text, ...) log(__FILE__, __LINE__, text, __VA_ARGS__);
void log(const char file[], int line, const char* text, ...);

//Window size
#define WINDOW_WIDTH    352
#define WINDOW_HEIGHT   320

//Game constants
#define MARGIN_GUI_Y	0
#define TILE_SIZE		32
#define LEVEL_WIDTH		11
#define LEVEL_HEIGHT	10

//LEVEL 1
#define WINCOUNT_LVL1   3
#define STEPCOUNT_LVL1  90

//Entities animation delay
#define ANIM_DELAY		1