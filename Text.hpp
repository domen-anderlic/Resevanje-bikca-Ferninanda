#pragma once
#include <SDL_ttf.h>
#include <string>

class Text {
	private:
		std::string data;
		TTF_Font* font;
		SDL_Color color;
		SDL_Texture* texture;
	public:
		Text() = default;
		Text(SDL_Renderer*&,std::string, TTF_Font*, SDL_Color);
		Text(SDL_Renderer*&, std::string, TTF_Font*, SDL_Color, bool);

		void draw(SDL_Renderer*&, float, float);
		int get_w();
		int get_h();
		~Text() {
			SDL_DestroyTexture(texture);
			data.~basic_string();
		}
};