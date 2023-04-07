#ifndef OBJECT_H
#define OBJECT_H

#include "./constants.h"

typedef struct {
    float x;
    float y;
    float width;
    float height;
    float speedY;
    float speedX;
    float gravity;
    float gravitySpeed;
    bool  onfloor;
    SDL_Texture* text;
} object;

object initObject(float x, float y, float width, float height, bool hasGravity);
SDL_Texture* loadBMPText(char path[],SDL_Renderer* thisRenderer);
object backInCollision(object a,object b);
#endif