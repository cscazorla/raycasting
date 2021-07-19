#include <math.h>
#include "map.h"
#include "player.h"
#include "ray.h"

/*
 * Function: movePlayer
 * -------------------
 * Move the player depending on the turn and walk direction and delta time
 * 
 * float dt: Delta time since the last loop iteration
 * 
 * returns: void
 */
void movePlayer(float dt) {
    player.rotationAngle += player.turnDirection * player.turnSpeed * dt;
    float moveStep = player.walkDirection * player.walkSpeed * dt;
    float newPlayerX = player.x + cos(player.rotationAngle) * moveStep;
    float newPlayerY = player.y + sin(player.rotationAngle) * moveStep;

    if(!mapHasWallAt(newPlayerX, newPlayerY)) {
        player.x = newPlayerX;
        player.y = newPlayerY;
    }
}

/*
 * Function: castRays
 * -------------------
 * Cast rays from the player position considering a FOV angle
 * 
 * returns: void
 */
void castRays() {
    for (int column = 0; column < NUM_RAYS; column++) {
        float angle = player.rotationAngle + atan((column-NUM_RAYS/2) / DIST_PROJ_PLANE);
        castRay(angle, player.x, player.y, column);
    }
}