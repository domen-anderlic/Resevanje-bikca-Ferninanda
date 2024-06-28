#pragma once
#include "SDL.h"
#include <string>
#include <SDL_ttf.h>

class TextureManager {	
public:
	static SDL_Texture* LoadTexture(SDL_Renderer*&,const char* fileName);
	static SDL_Texture* LoadTexture(SDL_Renderer*&, std::string, TTF_Font*, SDL_Color);
	static SDL_Texture* LoadTexture(SDL_Renderer*&, std::string, TTF_Font*, SDL_Color, bool);
	static void Draw(SDL_Renderer*&,SDL_Texture* tex, SDL_Rect &dst, SDL_RendererFlip flip);
	static void Draw(SDL_Renderer*&, SDL_Texture* tex,SDL_Rect &dst);
};