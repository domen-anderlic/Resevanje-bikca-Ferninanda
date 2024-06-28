#pragma once
#include <vector>
#include "textureManager.hpp"
#define T 1

constexpr float WALKER_SPEED = 1 * T;
constexpr float GRAVITY = 0.27f * T;
constexpr float RIDER_HURT_SPEED = 4;
constexpr float RIDER_SPEED = 1*T;
constexpr float PLAYER_ACCELARATION = 0.25f*T;
constexpr float PLAYER_JUMP_SPEED = -6*T;
constexpr float PLAYER_WALK_SPEED = 4*T;
constexpr float MAX_VERTICAL_SPEED = 16*T;

constexpr unsigned char PLAYER_DEATH_DURATION = 32;
constexpr unsigned char PLAYER_JUMP_TIMER = 10;



constexpr unsigned short WALKER_DEATH_DURATION = 32;
constexpr unsigned short RIDER_RECOVER_DURATION = 512;

constexpr unsigned short SCREEN_HEIGHT = 14*32*T;
constexpr unsigned short SCREEN_WIDTH = 25*32*T;
constexpr unsigned char CELL_SIZE = 32*T;

constexpr float BULLET_SPEED = 7*T;
constexpr unsigned short int BULLET_COOLDOWN = 40;

constexpr unsigned short int ARENA_WIDTH = 446/3*T;
constexpr unsigned short int ARENA_HEIGHT = 342/3*T;

struct Cell {
	Cell()
	{
		cell_id = collision = x = y = 0;
	}
	Cell(unsigned short int i_id, float i_x, float i_y,std::vector<Cell>& coll) {
		cell_id = i_id;
		x = i_x;
		y = i_y;
		if (i_id > 0 && i_id < 22) {
			collision = true;
			coll.push_back(*this);
		}
	}
	unsigned short int cell_id;
	float x, y;
	bool collision = false;
	void draw(SDL_Renderer*& rend, std::vector<SDL_Texture*>& tex, float i_pos) {
		if(i_pos + CELL_SIZE/2 > SCREEN_WIDTH/ 2)
		{
			if (x > (i_pos +CELL_SIZE/2 + SCREEN_WIDTH / 2) || x < (i_pos + CELL_SIZE/2 - SCREEN_WIDTH / 2) - CELL_SIZE)
			{
				return;			
			}			
		}
		else if (x > SCREEN_WIDTH) {
			return;
		}
		
		SDL_Rect a;
		a.w = a.h = CELL_SIZE;
		a.x = (i_pos + CELL_SIZE / 2 < SCREEN_WIDTH / 2) ? (int)(x + 0) : (int)(x - (i_pos - (SCREEN_WIDTH / 2 - CELL_SIZE / 2)));
		a.y = (int)y;
		TextureManager::Draw(rend, tex.at(cell_id), a, SDL_RendererFlip(0));
	}
};

struct Object
{
	float horizontal_speed;
	float vertical_speed;
	float x;
	float y;

	Object(const float i_x, const float i_y, const float i_horizontal_speed = 0, const float i_vertical_speed = 0) :
		horizontal_speed(i_horizontal_speed),
		vertical_speed(i_vertical_speed),
		x(i_x),
		y(i_y)
	{

	}
};