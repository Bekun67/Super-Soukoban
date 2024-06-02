#pragma once

//Displaying scale factor
#define GAME_SCALE_FACTOR	4.5f

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

//LEVEL 2
#define WINCOUNT_LVL2   3
#define STEPCOUNT_LVL2  120

//LEVEL 3
#define WINCOUNT_LVL3   5
#define STEPCOUNT_LVL3  50

//LEVEL 4
#define WINCOUNT_LVL4   4
#define STEPCOUNT_LVL4  140

//LEVEL 5
#define WINCOUNT_LVL5   3
#define STEPCOUNT_LVL5  65

//LEVEL 6
#define WINCOUNT_LVL6   5
#define STEPCOUNT_LVL6  60

//LEVEL 7
#define WINCOUNT_LVL7   5
#define STEPCOUNT_LVL7  80

//LEVEL 8
#define WINCOUNT_LVL8   6
#define STEPCOUNT_LVL8  160

//Entities animation delay
#define ANIM_DELAY		1