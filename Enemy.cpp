#include "global.hpp"
#include "Enemy.hpp"

Enemy::Enemy(const float i_x, const float i_y)
{
	dead = horizontal_speed = vertical_speed = 0;
	x = i_x;
	y = i_y;
}

bool Enemy::get_dead(const bool i_deletion) const
{
	return dead;
}

void Enemy::die(const unsigned char i_death_type)
{
	dead = 1;
}

SDL_FRect Enemy::get_hit_box() const 
{
	SDL_FRect tmp;
	tmp.w = tmp.h =  CELL_SIZE;
	tmp.x = x;
	tmp.y = y;
	return tmp;
}
