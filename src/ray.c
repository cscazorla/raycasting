#include <float.h>
#include <math.h>
#include <stdbool.h>

#include "constants.h"
#include "map.h"
#include "ray.h"
#include "utils.h"

struct Ray {
    float rayAngle;
    float wallHitX;
    float wallHitY;
    float distance;
    int wasHitVertical;
    int isFacingUp;
    int isFacingDown;
    int isFacingLeft;
    int isFacingRight;
    int wallHitContent;
} rays[NUM_RAYS];

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
    rayAngle = normalizeAngle(rayAngle);
    
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
    int foundHorzWallHit = false;
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
    while (horizontalIntersectionNextX >= 0 && horizontalIntersectionNextX <= WINDOW_WIDTH && horizontalIntersectionNextY >= 0 && horizontalIntersectionNextY <= WINDOW_HEIGHT) {
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
    int foundVertWallHit = false;
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
    while (verticalIntersectionNextX >= 0 && verticalIntersectionNextX <= WINDOW_WIDTH && verticalIntersectionNextY >= 0 && verticalIntersectionNextY <= WINDOW_HEIGHT) {
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
    } else {
        rays[stripId].distance = horzHitDistance;
        rays[stripId].wallHitX = horzWallHitX;
        rays[stripId].wallHitY = horzWallHitY;
        rays[stripId].wallHitContent = horzWallContent;
        rays[stripId].wasHitVertical = false;
    }
    rays[stripId].rayAngle = rayAngle;
    rays[stripId].isFacingDown = isFacingDown;
    rays[stripId].isFacingUp = isFacingUp;
    rays[stripId].isFacingLeft = isFacingLeft;
    rays[stripId].isFacingRight = isFacingRight;
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