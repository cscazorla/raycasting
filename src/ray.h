#ifndef RAY_H
#define RAY_H

#include "player.h"

struct Ray {
    float rayAngle;
    float wallHitX;
    float wallHitY;
    float distance;
    bool wasHitVertical;
    int textureIndex;
} rays[NUM_RAYS];

void castRays();
void castRay(float rayAngle, float x, float y, int stripId);
float getRayWallHitX(int i);
float getRayWallHitY(int i);
float getRayWallHitDistance(int i);
float getRayAngle(int i);
int getRayWasHitVertical(int i);
int getRayHitTexture(int i);
#endif