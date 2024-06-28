#pragma once
#include <string>
#include "SDL.h"
#include "Player.hpp"
class Player;
class Arena {
	float x;
	float y;
	unsigned char health;
	std::string tex;
public:
	Arena(const float i_x, const float i_y);
	bool get_dead() const;
	SDL_FRect get_hit_box() const;
	void draw(SDL_Renderer*&, Player);	
	void hit();
	bool is_shown(float);
};