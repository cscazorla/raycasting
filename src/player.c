#include <math.h>
#include "player.h"

void movePlayer(float dt) {
    player.rotationAngle += player.turnDirection * player.turnSpeed * dt;
    float moveStep = player.walkDirection * player.walkSpeed * dt;
    player.x += cos(player.rotationAngle) * moveStep;
    player.y += sin(player.rotationAngle) * moveStep;
}