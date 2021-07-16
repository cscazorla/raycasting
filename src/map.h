#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include <stdint.h>
#include "constants.h"

bool mapHasWallAt(float x, float y);
uint32_t getMapTilecolor(int i, int j);

#endif