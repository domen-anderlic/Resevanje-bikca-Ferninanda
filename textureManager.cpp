#include "textureManager.hpp"
#include "SDL_image.h"
#include "global.hpp"

SDL_Texture* TextureManager::LoadTexture(SDL_Renderer*& rend, const char* texture)
{
	SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, tempSurface);
	SDL_FreeSurface(tempSurface);
	return tex;
}

SDL_Texture* TextureManager::LoadTexture(SDL_Renderer*& rend, std::string i_data, TTF_Font* i_font, SDL_Color i_color)
{
	SDL_Surface* textSurface = TTF_RenderText_Solid(i_font, i_data.c_str(), i_color);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(rend, textSurface);
	SDL_FreeSurface(textSurface);
	return textTexture;
}

SDL_Texture* TextureManager::LoadTexture(SDL_Renderer*& rend, std::string i_data, TTF_Font* i_font, SDL_Color i_color, bool)
{
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(i_font, i_data.c_str(), i_color, SCREEN_WIDTH * 0.7);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(rend, textSurface);
	SDL_FreeSurface(textSurface);
	return textTexture;
}


void TextureManager::Draw(SDL_Renderer*& rend, SDL_Texture* tex, SDL_Rect &dst, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(rend, tex,NULL, &dst, NULL, NULL, flip);
}

void TextureManager::Draw(SDL_Renderer*& rend, SDL_Texture* tex,SDL_Rect& dst)
{
	SDL_RenderCopy(rend, tex, NULL, &dst);
}

