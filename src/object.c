#include "object.h"

bool hasCollision(object a,object b){
	if (a.x < b.x + b.width && a.x + a.width > b.x &&
		a.y < b.y + b.height && a.y + a.height > b.y) {
		return true;
	}else{
		return false;
	}
}

object backInCollision(object a,object b){
	if(hasCollision(a,b)){
		if(a.y + a.height > b.y && a.y + a.height < b.y + b.height/3){
			a.y = b.y - a.height;
			a.onfloor = true;
		}
		if(a.y < b.y + b.height && a.y > b.y + (b.height/4)*3){
			a.y = b.y + b.height;
			a.onfloor = false;
		}
		if(a.x < b.x + b.width && a.x > b.x + (b.width/8)*7){
			a.x = b.x + b.width;
		}
		if(a.x + a.width > b.x && a.x + a.width < b.x + b.width/8){
			a.x = b.x - a.width;
		}
	}
	return a;
}

object initObject(float x, float y, float width, float height, bool hasGravity){
	object obj;
	obj.x = x;
	obj.y = y;
	obj.width  = width;
	obj.height = height;

	if(!hasGravity){
		obj.gravity = 0.0;
	}else{
		obj.gravity = 0.15;
	}
	obj.gravitySpeed = 0;
	return obj;
}

SDL_Texture* loadBMPText(char path[],SDL_Renderer* thisRenderer){
	SDL_Surface* thisSurface = SDL_LoadBMP(path);
	SDL_Texture* thisTexture;
    thisTexture = SDL_CreateTextureFromSurface(thisRenderer, thisSurface);
    SDL_FreeSurface(thisSurface);
    return thisTexture;
}