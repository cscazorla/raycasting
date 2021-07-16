#include <math.h>
#include "map.h"
#include "player.h"

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