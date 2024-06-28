#pragma once
#include "Enemy.hpp"

class Rider : public Enemy
{
	
	bool check_collision;
	bool flipped;
	bool no_collision_dying;
	bool underground;
	unsigned char state;
	unsigned short RECOVER_timer;
	std::string tex;
public:
	Rider(const bool i_underground, const float i_x, const float i_y);

	bool get_dead(const bool i_deletion) const;

	void die(const unsigned char i_death_type);
	void draw(SDL_Renderer*&, float);
	void update(const std::vector<std::shared_ptr<Enemy>>& i_enemies, MapManager& i_map_manager, Player& i_player,float);
};