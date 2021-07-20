#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "constants.h"
#include "map.h"

const int map[MAP_NUM_ROWS][MAP_NUM_COLS] = {
    {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3},
    {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
    {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
    {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
    {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
    {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3},
    {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 3},
    {3, 0, 0, 1, 0, 1, 0, 0, 0, 0, 4, 0, 2, 0, 0, 0, 0, 0, 0, 3},
    {3, 0, 0, 1, 0, 1, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3},
    {3, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
    {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
    {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}
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
    if(x < 0 || x >= MAP_NUM_COLS * TILE_SIZE || y < 0 || y >= MAP_NUM_ROWS * TILE_SIZE) 
        return true;
    int mapGridIndexX = (int)floor(x / TILE_SIZE);
    int mapGridIndexY = (int)floor(y / TILE_SIZE);
    return map[mapGridIndexY][mapGridIndexX] != 0;
}

/*
 * Function: getMapTileColor
 * -------------------
 * Returns the color of a tile located in the (i, j) cell
 * 
 * int i: Cell row
 * int j: Cell column
 * 
 * returns: true/false if there is a wall
 */
uint32_t getMapTileColor(int i, int j) {
    return map[i][j] != 0 ? 0xFFFFFFFF : 0xFF000000;
}

/*
 * Function: getMapTileContent
 * -------------------
 * Returns the content of a tile located at a position. It traduces from coordinates to grid position
 * 
 * float x: Horizontal coordinate
 * float y: Vertical coordinate
 * 
 * returns: int with the content
 */
int getMapTileContent(float x, float y) {
    int i = (int)floor(y / TILE_SIZE);
    int j = (int)floor(x / TILE_SIZE);
    return map[i][j];
}

/*
 * Function: isInMap
 * -------------------
 * Check if a coordinate (x,y) is inside the map boundaries
 * 
 * float x: Horizontal coordinate
 * float y: Vertical coordinate
 * 
 * returns: bool with the response
 */
bool isInMap(float x, float y) {
    return (x >= 0 && x <= (MAP_NUM_COLS * TILE_SIZE) && y >= 0 && y <= (MAP_NUM_ROWS * TILE_SIZE)); 
}