#include <math.h>
#include "constants.h"
#include "display.h"
#include "player.h"
#include "ray.h"
#include "sprite.h"
#include "textures.h"
#include "utils.h"
#include "upng.h"

static sprite_t sprites[NUM_SPRITES] = {
    { .x = 300, .y = 300, .textureIndex = 4},
    { .x = 350, .y = 350, .textureIndex = 4},
    { .x = 700, .y = 300, .textureIndex = 4}
};

/*
 * Function: drawSpritesInMiniMap
 * -------------------
 * Draw a small square in the minimap for sprites that are visible
 * 
 * returns: void
 */
void drawSpritesInMiniMap() {
    for (int i = 0; i < NUM_SPRITES; i++) {
        if(sprites[i].visible) {
            draw_rect(
                sprites[i].x * MINIMAP_SCALE_FACTOR,
                sprites[i].y * MINIMAP_SCALE_FACTOR,
                5,
                5,
                0xFFFF0000
            );
        }
    }
}
/*
 * Function: drawSpriteProjection
 * -------------------
 * Draw the sprites projection on the screen for sprites that are visible
 * 
 * returns: void
 */
void drawSpriteProjection() {
    sprite_t visibleSprites[NUM_SPRITES];
    int numVisibleSprites = 0;
    struct Player player = getPlayer();

    // Which sprites are visible?
    for (int i = 0; i < NUM_SPRITES; i++) {
        float angleSpritePlayer = player.rotationAngle - atan2(sprites[i].y - player.y, sprites[i].x - player.x);

        // Make sure the angle is between 0 and 180 degrees
        if (angleSpritePlayer > PI)
            angleSpritePlayer -= TWO_PI;
        if (angleSpritePlayer < -PI)
            angleSpritePlayer += TWO_PI;
        angleSpritePlayer = fabs(angleSpritePlayer);

        // Which sprite are under our FoV
        const float EPSILON = 0.2;
        if (angleSpritePlayer < (FOV_ANGLE / 2) + EPSILON) {
            sprites[i].visible = true;
            sprites[i].angle = angleSpritePlayer;
            sprites[i].distance = distanceBetweenPoints(sprites[i].x, sprites[i].y, player.x, player.y);
            visibleSprites[numVisibleSprites] = sprites[i];
            numVisibleSprites++;
        } else {
            sprites[i].visible = false;
        }
    }

    // bubble-sort sprites by distance (back from front - painter's algorithm)
    for (int i = 0; i < numVisibleSprites - 1; i++) {
        for (int j = i + 1; j < numVisibleSprites; j++) {
            sprite_t tmp = visibleSprites[i];
            visibleSprites[i] = visibleSprites[j];
            visibleSprites[j] = tmp;
        }
    }

    // Draw the visible sprite
    for (int i = 0; i < numVisibleSprites; i++) {
        sprite_t sprite = visibleSprites[i];
        float perpDistance = sprite.distance * cos(sprite.angle);
        float spriteHeight = ((float)TILE_SIZE / perpDistance) * DIST_PROJ_PLANE;
        float spriteWidth = spriteHeight;

        // Sprite top Y
        float spriteTopY = (WINDOW_HEIGHT/2) - (spriteHeight/2);
        spriteTopY = (spriteTopY < 0) ? 0 : spriteTopY;

        // Sprite bottom Y
        float spriteBottomY = (WINDOW_HEIGHT/2) + (spriteHeight/2);
        spriteBottomY = (spriteBottomY > WINDOW_HEIGHT) ? WINDOW_HEIGHT : spriteBottomY;

        // Sprite X position
        float spriteAngle = atan2(sprite.y - player.y, sprite.x - player.x) - player.rotationAngle;
        float spritePosX = tan(spriteAngle) * DIST_PROJ_PLANE;
        float spriteLeftX = (WINDOW_WIDTH / 2) + spritePosX - (spriteWidth / 2);
        float spriteRightX = spriteLeftX + spriteWidth;

        // Query the texture
        int textureWidth = upng_get_width(textures[sprite.textureIndex]);
        int textureHeight = upng_get_height(textures[sprite.textureIndex]);

        // Draw on the screen
        for (int x = spriteLeftX; x < spriteRightX; x++) {
            float pixelWidth = textureWidth / spriteWidth;
            int textureOffsetX = (x - spriteLeftX) * pixelWidth;
            for (int y = spriteTopY; y < spriteBottomY; y++) {
                int distanceFromTop = y + (spriteHeight / 2) - (WINDOW_HEIGHT/2);
                int textureOffsetY = distanceFromTop * (textureHeight / spriteHeight);
                uint32_t* spriteTextureBuffer = (uint32_t*)upng_get_buffer(textures[sprite.textureIndex]);
                uint32_t color = spriteTextureBuffer[(textureWidth * textureOffsetY) + textureOffsetX];
                if((sprite.distance < getRayWallHitDistance(x)) && (color != 0xFF880098)) // Our transparency color
                    draw_pixel(x, y, color);
            }
        }
    }
}