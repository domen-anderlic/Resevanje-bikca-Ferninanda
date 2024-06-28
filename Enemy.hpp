#pragma once

#include "MapManager.hpp"
#include "Player.hpp"
#include <memory>

class MapManager;
class Player;
class Enemy : public std::enable_shared_from_this<Enemy>
{
protected:
	
	bool dead;
	float horizontal_speed;
	float vertical_speed;
	float x;
	float y;
public:
	bool type;
	Enemy(const float i_x, const float i_y);
	SDL_FRect get_hit_box() const;

	virtual bool get_dead(const bool i_deletion) const;

	virtual void die(const unsigned char i_death_type);
	virtual void draw(SDL_Renderer*&,float) = 0;
	virtual void update(const std::vector<std::shared_ptr<Enemy>>& i_enemies, MapManager& i_map_manager, Player& i_player, float) = 0;//const MapManager&
};