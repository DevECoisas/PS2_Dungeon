#include "./image.h"

void blitObject(object a , SDL_Renderer* renderer){
	SDL_Rect thisRect = {a.x,a.y,a.width,a.height};
    SDL_RenderCopy(renderer, a.text, NULL, &thisRect);
}

void blitAnimateObject(object a , SDL_Renderer* renderer,int time){
	SDL_Rect thisRect = {a.x,a.y,a.width,a.height};
	for (int i = 0; i < a.frames; ++i)
	{
		if(time>30*(i+1) && time<60*(i+1))		
    	SDL_RenderCopy(renderer, a.anim[i], NULL, &thisRect);
	}	
}

object setAnimation(object a,int frames,SDL_Renderer* renderer,char* path[]){
	a.frames = frames;
	for (int i = 0; i < a.frames; ++i)
	{
		a.anim[i] = loadBMPText(path[i],renderer);
	}
	return a;
}

SDL_Texture* loadBMPText(char path[],SDL_Renderer* thisRenderer){
	SDL_Surface* thisSurface = SDL_LoadBMP(path);
	SDL_Texture* thisTexture;
    thisTexture = SDL_CreateTextureFromSurface(thisRenderer, thisSurface);
    SDL_FreeSurface(thisSurface);
    return thisTexture;
}
