#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include <stdint.h>
#include "constants.h"

bool mapHasWallAt(float x, float y);
bool isInMap(float x, float y);
uint32_t getMapTileColor(int i, int j);
int getMapTileContent(float x, float y);

#endif