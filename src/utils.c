#include <math.h>
#include "constants.h"
#include "utils.h"

/*
 * Function: normalizeAngle
 * -------------------
 * Returns the remainder angle when it is bigger than 2*PI
 * 
 * float angle: The angle to normalize
 * 
 * returns: float angle
 */
float normalizeAngle(float angle) {
    angle = remainder(angle, TWO_PI);
    if (angle < 0) {
        angle = TWO_PI + angle;
    }
    return angle;
}

/*
 * Function: distanceBetweenPoints
 * -------------------
 * Returns the distance between two points (x1, y1) and (x2, y2)
 * 
 * float x1: The X coordinate of the first point
 * float y1: The Y coordinate of the first point
 * float x2: The X coordinate of the second point
 * float y2: The Y coordinate of the second point
 * 
 * returns: float Distance between the two points
 */
float distanceBetweenPoints(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}