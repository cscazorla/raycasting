#ifndef PLAYER_H
#define PLAYER_H

struct Player {
    float x;
    float y;
    float minimap_x;
    float minimap_y;
    float width;
    float height;
    int turnDirection; // -1 for left, +1 for right
    int walkDirection; // -1 for back, +1 for front
    float rotationAngle;
    float walkSpeed;
    float turnSpeed;
};

struct Player getPlayer();
void initializePlayer();
void movePlayer(float dt);
void setPlayerTurnDirection(int dir);
void setPlayerWalkDirection(int dir);

#endif