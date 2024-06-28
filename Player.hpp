#pragma once
#include "SDL.h"
#include "global.hpp"
#include "MapManager.hpp"
#include "Bullet.hpp"
class MapManager;
class Bullet;
class Player
{
	SDL_Renderer* &rend;
	bool dead;
	bool flipped;
	bool on_ground;
	bool shot;

	float enemy_bounce_speed;
	float horizontal_speed;
	float vertical_speed;
	float x;
	float y;

	float bullet_cooldown;

	unsigned char jump_timer;

	unsigned short death_timer;
	unsigned short invincible_timer;

	bool u, d, l, r;
	SDL_Texture* texture;
public:
	Player() = default;
	Player(SDL_Renderer*&);

	bool get_dead() const;

	float get_vertical_speed() const;
	float get_x() const;
	float get_y() const;

	SDL_FRect get_hit_box() const;

	void die(const bool i_instant_death);
	void draw();
	void reset();
	void set_position(const float i_x, const float i_y);
	void set_vertical_speed(const float i_value);
	bool update(SDL_Event& i_event,MapManager &i_map_manager, std::vector<std::shared_ptr<Bullet>>&, bool can_shoot);
};