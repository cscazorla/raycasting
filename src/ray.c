#include <float.h>
#include <math.h>
#include <stdbool.h>
#include "constants.h"
#include "map.h"
#include "player.h"
#include "ray.h"

struct Ray {
    float rayAngle;
    float wallHitX;
    float wallHitY;
    float distance;
    bool wasHitVertical;
    int wallHitContent;
} rays[NUM_RAYS];

/*
 * Function: castRays
 * -------------------
 * Cast rays from the player position considering a FOV angle
 * 
 * returns: void
 */
void castRays() {
    struct Player player = getPlayer();
    for (int column = 0; column < NUM_RAYS; column++) {
        float angle = player.rotationAngle + atan((column-NUM_RAYS/2) / DIST_PROJ_PLANE);
        castRay(angle, player.x, player.y, column);
    }
}

/*
 * Function: castRay
 * -------------------
 * Cast a ray from a specific coordinate (x,y) and angle. Once the
 * ray hits a wall, it stores information in the rays array.
 * 
 * DDA algorithm:
 * To be more efficient, instead of checking every single pixel of
 * the ray, we just check the horizontal and vertical intersections
 * of the mapa grid (every TILE_SIZE) and confirm whether it is a wall.
 * 
 * float rayAngle: angle of the ray from the player position
 * float x: Horizontal coordinate
 * float y: Vertical coordinate
 * int stripId: Ray index
 * 
 * returns: void
 */
void castRay(float rayAngle, float x, float y, int stripId) {
    normalizeAngle(&rayAngle);
    
    // Which direction is the player facing?
    int isFacingDown = rayAngle > 0 && rayAngle < PI;
    int isFacingUp = !isFacingDown;
    int isFacingRight = rayAngle < 0.5 * PI || rayAngle > 1.5 * PI;
    int isFacingLeft = !isFacingRight;
    
    // Intersections are separated evenly
    float xstep, ystep;

    // We check for horizontal and vertical intersections
    // separately. The distance to both horizontal
    // and vertical intersection points is compared
    // (we select the closest one)

    /***********************************************
    * Horizontal intersections check
    ************************************************/
    bool foundHorzWallHit = false;
    float horzWallHitX = 0;
    float horzWallHitY = 0;
    int horzWallContent = 0;

    // Find the y-coordinate and x-coordinate of the closest horizontal grid intersection
    float xintercept, yintercept;
    yintercept = floor(y / TILE_SIZE) * TILE_SIZE;
    yintercept += isFacingDown ? TILE_SIZE : 0;
    xintercept = x + (yintercept - y) / tan(rayAngle);

    // xstep and ystep calculation
    ystep = TILE_SIZE;
    ystep *= isFacingUp ? -1 : 1;
    xstep = ystep / tan(rayAngle);
    xstep *= (isFacingLeft && xstep > 0) ? -1 : 1;
    xstep *= (isFacingRight && xstep < 0) ? -1 : 1;

    // Main loop to find an horizontal wall hit
    float horizontalIntersectionNextX = xintercept;
    float horizontalIntersectionNextY = yintercept;
    while (isInMap(horizontalIntersectionNextX, horizontalIntersectionNextY)) {
        float xToCheck = horizontalIntersectionNextX;
        float yToCheck = horizontalIntersectionNextY + (isFacingUp ? -1 : 0);
        
        if (mapHasWallAt(xToCheck, yToCheck)) {
            // found a wall hit
            horzWallHitX = horizontalIntersectionNextX;
            horzWallHitY = horizontalIntersectionNextY;
            horzWallContent = getMapTileContent(xToCheck, yToCheck);
            foundHorzWallHit = true;
            break;
        } else {
            horizontalIntersectionNextX += xstep;
            horizontalIntersectionNextY += ystep;
        }
    }

    /***********************************************
    * Vertical intersections check
    ************************************************/
    bool foundVertWallHit = false;
    float vertWallHitX = 0;
    float vertWallHitY = 0;
    int vertWallContent = 0;

    // Find the y-coordinate and x-coordinate of the closest vertical grid intersection
    xintercept = floor(x / TILE_SIZE) * TILE_SIZE;
    xintercept += isFacingRight ? TILE_SIZE : 0;
    yintercept = y + (xintercept - x) * tan(rayAngle);

    // xstep and ystep calculation
    xstep = TILE_SIZE;
    xstep *= isFacingLeft ? -1 : 1;
    ystep = xstep * tan(rayAngle);
    ystep *= (isFacingUp && ystep > 0) ? -1 : 1;
    ystep *= (isFacingDown && ystep < 0) ? -1 : 1;

    // Main loop to find an horizontal wall hit
    float verticalIntersectionNextX = xintercept;
    float verticalIntersectionNextY = yintercept;
    while (isInMap(verticalIntersectionNextX, verticalIntersectionNextY)) {
        float xToCheck = verticalIntersectionNextX + (isFacingLeft ? -1 : 0);
        float yToCheck = verticalIntersectionNextY;
        
        if (mapHasWallAt(xToCheck, yToCheck)) {
            // found a wall hit
            vertWallHitX = verticalIntersectionNextX;
            vertWallHitY = verticalIntersectionNextY;
            vertWallContent = getMapTileContent(xToCheck, yToCheck);
            foundVertWallHit = true;
            break;
        } else {
            verticalIntersectionNextX += xstep;
            verticalIntersectionNextY += ystep;
        }
    }

    // Which distance (vertical or horizontal) is the smallest?
    float horzHitDistance = foundHorzWallHit
        ? distanceBetweenPoints(x, y, horzWallHitX, horzWallHitY)
        : FLT_MAX;
    float vertHitDistance = foundVertWallHit
        ? distanceBetweenPoints(x, y, vertWallHitX, vertWallHitY)
        : FLT_MAX;

    // Information about the hit (e.g. coordinates or map content) is stored in the array of rays
    if (vertHitDistance < horzHitDistance) {
        rays[stripId].distance = vertHitDistance;
        rays[stripId].wallHitX = vertWallHitX;
        rays[stripId].wallHitY = vertWallHitY;
        rays[stripId].wallHitContent = vertWallContent;
        rays[stripId].wasHitVertical = true;
        rays[stripId].rayAngle = rayAngle;
    } else {
        rays[stripId].distance = horzHitDistance;
        rays[stripId].wallHitX = horzWallHitX;
        rays[stripId].wallHitY = horzWallHitY;
        rays[stripId].wallHitContent = horzWallContent;
        rays[stripId].wasHitVertical = false;
        rays[stripId].rayAngle = rayAngle;
    }
}

/*
 * Function: getRayWallHitX
 * -------------------
 * For the ray "i" this function returns the X coordinate where the
 * ray hits a wall.
 * 
 * int i: Ray index
 * 
 * returns: float X coordinate
 */
float getRayWallHitX(int i) {
    return rays[i].wallHitX;
}

/*
 * Function: getRayWallHitY
 * -------------------
 * For the ray "i" this function returns the Y coordinate where the
 * ray hits a wall.
 * 
 * int i: Ray index
 * 
 * returns: float Y coordinate
 */
float getRayWallHitY(int i) {
    return rays[i].wallHitY;
}

/*
 * Function: getRayWallHitDistance
 * -------------------
 * For the ray "i" this function returns the distance to the wall hit
 * 
 * int i: Ray index
 * 
 * returns: float distance
 */
float getRayWallHitDistance(int i) {
    return rays[i].distance;
}

/*
 * Function: getRayAngle
 * -------------------
 * For the ray "i" this function returns the ray angle
 * 
 * int i: Ray index
 * 
 * returns: float Ray angle
 */
float getRayAngle(int i) {
    return rays[i].rayAngle;
}

/*
 * Function: getRayWasHitVertical
 * -------------------
 * For the ray "i" this function returns the ray angle
 * 
 * int i: Ray index
 * 
 * returns: float Ray angle
 */
int getRayWasHitVertical(int i) {
    return rays[i].wasHitVertical;
}

/*
 * Function: getRayHitContent
 * -------------------
 * For the ray "i" this function returns the content of the hit wall
 * 
 * int i: Ray index
 * 
 * returns: int Content of the hit wall
 */
int getRayHitContent(int i) {
    return rays[i].wallHitContent;
}

/*
 * Function: normalizeAngle
 * -------------------
 * Returns the remainder angle when it is bigger than 2*PI
 * 
 * float angle: The angle to normalize
 * 
 * returns: float angle
 */
void normalizeAngle(float* angle) {
    *angle = remainder(*angle, TWO_PI);
    if (*angle < 0) {
        *angle = TWO_PI + *angle;
    }
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