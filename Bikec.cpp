#include "Bikec.hpp"
#include <iostream>

Bikec::Bikec()
{
	prej.first = -1.0f;
	prej.second = -1.0f;
	tex = "assets/bikec.bmp";
}
void Bikec::draw(SDL_Renderer*& rend, std::pair<float,float> i_positions)
{
	//std::cout << prej.first << ' ' << i_positions.first << '\n';
	if (prej.first == -1.0f && prej.second == -1.0f)
	{
		prej = i_positions;
	}
	//std::cout << prej.first << ' ' << i_positions.first << '\n';
	SDL_Texture* texture = TextureManager::LoadTexture(rend, tex.c_str());
	SDL_Rect tmp;
	tmp.w = tmp.h = CELL_SIZE;
	
	tmp.x = (i_positions.first + CELL_SIZE / 2 < SCREEN_WIDTH / 2) ? (int)(i_positions.first) : (int)((SCREEN_WIDTH / 2.0f - CELL_SIZE / 2.0f) - 10*(i_positions.first - prej.first));
	tmp.y = (int)i_positions.second;
	TextureManager::Draw(rend, texture, tmp, SDL_RendererFlip(prej.first <= i_positions.first));
	SDL_DestroyTexture(texture);
	prej.first = i_positions.first;
	prej.second = i_positions.second;

}