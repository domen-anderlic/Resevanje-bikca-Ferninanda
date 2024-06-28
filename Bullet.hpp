#pragma once
#include <memory>
#include <vector>
#include "Enemy.hpp"
#include "MapManager.hpp"
#include "SDL.h"
#include "Arena.hpp"

class MapManager;
class Enemy;
class Arena;
class Bullet : public std::enable_shared_from_this<Bullet>
{
	bool dead;
	bool direction; //0 == left, 1 == right
	float horizontal_speed;
	float x;
	float y;
	std::string tex;
public:
	Bullet(const float i_x, const float i_y, const bool i_direction);
	SDL_FRect get_hit_box() const;
	bool get_dead() const;
	void die();
	void draw(SDL_Renderer*&, float);
	void update(std::vector<std::shared_ptr<Enemy>>&, MapManager&, float i_pos, std::vector<Arena>&);
	bool get_direction();

};