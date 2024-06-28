#pragma once
#include <vector>
#include "Enemy.hpp"
#include "Player.hpp"
#include "Arena.hpp"


class Player;
class Enemy;
class Arena;
class Bikec;
class Bullet;
class MapManager
{
	Cell map[15][224];
	std::vector<SDL_Texture*> textures;
	SDL_Renderer* &rend;
	std::vector<Cell> colliders;
	SDL_Texture* background_texture;
public:
	MapManager() = default;
	MapManager(SDL_Renderer *& renderer);

	void draw_background();
	void draw_map(float);
	//void update();
	void update_map_sketch(const unsigned char, std::vector<std::shared_ptr<Enemy>>&, Player&, std::vector<Arena>&);
	void continue_map_sketch(unsigned char&, std::vector<std::shared_ptr<Enemy>>&, Player&, std::vector<Arena>&, std::vector<std::shared_ptr<Bullet>>&, std::vector<Bikec>&, int&);
	void getColliders();

	bool collisionX(SDL_FRect);
	bool collisionY(SDL_FRect);
};