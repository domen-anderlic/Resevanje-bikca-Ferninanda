#include "Text.hpp"
#include "global.hpp"

Text::Text(SDL_Renderer*& rend, std::string i_data, TTF_Font* i_font, SDL_Color i_color):
	data(i_data),
	font(i_font),
	color(i_color),
	texture(TextureManager::LoadTexture(rend,data,font,color))
{

}

Text::Text(SDL_Renderer*& rend, std::string i_data, TTF_Font* i_font, SDL_Color i_color, bool) :
	data(i_data),
	font(i_font),
	color(i_color),
	texture(TextureManager::LoadTexture(rend, data, font, color, 1))
{

}

int Text::get_w()
{
	int h;
	int w;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	return w;
}

int Text::get_h()
{
	int h;
	int w;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	return h;
}

void Text::draw(SDL_Renderer*& rend, float i_x, float i_y)
{
	SDL_Rect tmp;
	tmp.w = tmp.h = (int)CELL_SIZE;
	tmp.x = i_x;
	tmp.y = i_y;
	SDL_QueryTexture(texture, NULL, NULL, &tmp.w, &tmp.h);

	TextureManager::Draw(rend, texture, tmp);
	SDL_DestroyTexture(texture);
}

