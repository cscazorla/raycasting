#ifndef RAY_H
#define RAY_H

void castRay(float rayAngle, float x, float y, int stripId);
float getRayWallHitX(int i);
float getRayWallHitY(int i);
float getRayWallHitDistance(int i);
#endif