#ifndef SPRITE_H
#define SPRITE_H

#define NUM_SPRITES 3

typedef struct {
    float x;
    float y;
    float distance;
    float angle;
    bool visible;
    int textureIndex;
} sprite_t;

void drawSpritesInMiniMap(void);
void drawSpriteProjection(void);

#endif