#ifndef IMAGE_H
#define IMAGE_H

#include "./object.h"

SDL_Texture* loadBMPText(char path[],SDL_Renderer* thisRenderer);
void blitObject(object a , SDL_Renderer* renderer);
void blitAnimateObject(object a , SDL_Renderer* renderer,int time);
object setAnimation(object a,int frames,SDL_Renderer* renderer,char* path[]);

#endif