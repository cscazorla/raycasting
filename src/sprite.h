#ifndef SPRITE_H
#define SPRITE_H

#define NUM_SPRITES 5

typedef struct {
    int i;
    int j;
    float x;
    float y;
    float distance;
    float angle;
    bool visible;
    int textureIndex;
} sprite_t;

void loadSprites();
void drawSpritesInMiniMap(void);
void drawSpriteProjection(void);

#endif