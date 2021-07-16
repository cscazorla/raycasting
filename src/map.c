#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "constants.h"
#include "map.h"

const int map[MAP_NUM_ROWS][MAP_NUM_COLS] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

/*
 * Function: mapHasWallAt
 * -------------------
 * Checks if the map contains a wall at a position. It traduces from coordinates to grid position
 * 
 * float x: Horizontal coordinate
 * float y: Vertical coordinate
 * 
 * returns: true/false if there is a wall
 */
bool mapHasWallAt(float x, float y) {
    if(x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT) 
        return true;
    int mapGridIndexX = (int)floor(x / TILE_SIZE);
    int mapGridIndexY = (int)floor(y / TILE_SIZE);
    return map[mapGridIndexY][mapGridIndexX] != 0;
}

/*
 * Function: getMapTilecolor
 * -------------------
 * Returns the color of a tile located in the (i, j) cell
 * 
 * int i: Cell row
 * int j: Cell column
 * 
 * returns: true/false if there is a wall
 */
uint32_t getMapTilecolor(int i, int j) {
    return map[i][j] != 0 ? 0xFFFFFFFF : 0xFF000000;
}