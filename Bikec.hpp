#pragma once
#include "Arena.hpp"

class Bikec {
	std::pair<float,float> prej;
	std::string tex;
public:
	Bikec();
	void draw(SDL_Renderer*&, std::pair<float, float>);
};