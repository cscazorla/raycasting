#include <math.h>
#include "map.h"
#include "player.h"
#include "ray.h"

struct Player player;

void initializePlayer() {
    player.x = 6 * TILE_SIZE;
    player.y = 3.5 * TILE_SIZE;
    player.width = 10;
    player.height = 10;
    player.turnDirection = 0;
    player.walkDirection = 0;
    player.rotationAngle = 0;
    player.walkSpeed = 100;
    player.turnSpeed = 90 * (PI / 180);
}

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

struct Player getPlayer() {
    return player;
}

void setPlayerWalkDirection(int dir) {
    player.walkDirection = dir;
}

void setPlayerTurnDirection(int dir) {
    player.turnDirection = dir;
}