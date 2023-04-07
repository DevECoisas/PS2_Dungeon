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

    SDL_Texture* anim[99];
    int frames;
} object;

object initObject(float x, float y, float width, float height, bool hasGravity);
object backInCollision(object a,object b);
bool hasCollision(object a,object b);


#endif