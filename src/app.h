#ifndef APP_H
#define APP_H

#include <stdbool.h>

struct Game {
    bool showMiniMap;
    bool isGameRunning;
};

// Game
#define FPS 60
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 400

// Map
#define TILE_SIZE 64
#define MAP_NUM_ROWS 12
#define MAP_NUM_COLS 20
#define MAP_WIDTH (TILE_SIZE * MAP_NUM_COLS)
#define MAP_HEIGHT (TILE_SIZE * MAP_NUM_ROWS)
#define MINIMAP_WIDTH_TILE_SIZE (WINDOW_WIDTH / MAP_NUM_COLS)
#define MINIMAP_HEIGHT_TILE_SIZE (WINDOW_HEIGHT / MAP_NUM_ROWS)

// Math constants
#define PI 3.14159265
#define TWO_PI 6.28318530
#define FRAME_TIME_LENGTH (1000 / FPS)
#define FOV_ANGLE (60 * (PI/180))
#define NUM_RAYS WINDOW_WIDTH
#define DIST_PROJ_PLANE ((WINDOW_WIDTH/2)/tan(FOV_ANGLE/2))

// Player movements
#define PLAYER_TURN_DIRECTION_LEFT -1
#define PLAYER_TURN_DIRECTION_RIGHT 1
#define PLAYER_WALK_DIRECTION_BACK -1
#define PLAYER_WALK_DIRECTION_FRONT 1

#endif