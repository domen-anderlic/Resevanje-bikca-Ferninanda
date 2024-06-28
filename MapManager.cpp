#include <fstream>
#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "global.hpp"
#include "MapManager.hpp"
#include "textureManager.hpp"
#include <string>
#include "Walker.hpp"
#include "Rider.hpp"
#include "Bikec.hpp"



MapManager::MapManager(SDL_Renderer*&renderer):rend(renderer)
{
	std::string path;
	for (int i = 0; i < 26; ++i) {
		path.clear();
		path += "assets/texture" + std::to_string(i) + ".bmp";
		textures.push_back(TextureManager::LoadTexture(rend, path.c_str())); //.c_str() pretvori string v const char*
	}
	background_texture = TextureManager::LoadTexture(rend, "assets/background.bmp");
}


void MapManager::update_map_sketch(const unsigned char i_current_level, std::vector<std::shared_ptr<Enemy>>& i_enemies, Player& i_player, std::vector<Arena>& i_arenas)
{
	i_enemies.clear();
	i_player.reset();
	i_arenas.clear();
	colliders.clear();
	std::string current_map_file = "assets/map" + std::to_string(static_cast<int>(i_current_level)) + ".txt";
	std::ifstream data(current_map_file);
	if (!data.is_open())
	{
		std::cout << "Could not open " << current_map_file << "!\n";
		return;
	}
	unsigned short int ch;
	for (int i = 0; i < 14; ++i)
	{
		for (int j = 0; j < 212; ++j)
		{
			data >> ch;
			map[i][j] = Cell(ch, j * CELL_SIZE, i * CELL_SIZE,colliders);
			data.ignore();
		}
	}
	data.close();
	current_map_file.clear();
	current_map_file = "assets/objects" + std::to_string(static_cast<int>(i_current_level)) + ".txt";
	data.open(current_map_file);
	unsigned short int ch2;
	if (!data.is_open())
	{
		std::cout << "Could not open " << current_map_file << "!\n";
		return;
	}
	for (char x; (x = data.get()) != EOF; )
	{
		data >> ch;
		data >> ch2;
		data.ignore();
		switch (x)
		{
		case 'g':
		{
			i_enemies.push_back(std::make_shared<Walker>(rand() % (2), ch * CELL_SIZE, ch2 * CELL_SIZE));
			//std::cout << "Walker!\n";
			break;
		}
		case 'k':
		{
			i_enemies.push_back(std::make_shared<Rider>(rand() % (2), ch * CELL_SIZE, ch2 * CELL_SIZE));
			//std::cout << "Rider!\n";
			break;
		}
		case 'p':
		{
			i_player.set_position(ch * CELL_SIZE, ch2 * CELL_SIZE);
			//std::cout << "Player!\n";
			break;

		}
		default:break;
		}
	}
	if (0 == i_current_level)
	{
		i_player.set_position(rand() % (189 * CELL_SIZE - 0 + 1), 0);

		
		i_arenas.push_back(Arena(193 * CELL_SIZE, 12 * CELL_SIZE - ARENA_HEIGHT));
		i_arenas.push_back(Arena(82 * CELL_SIZE, 4 * CELL_SIZE - ARENA_HEIGHT));
	}
	else
	{
		i_player.set_position(rand() % (200*CELL_SIZE - 20*CELL_SIZE + CELL_SIZE) + 20*CELL_SIZE, 0);
		
		i_arenas.push_back(Arena(193 * CELL_SIZE, 12 * CELL_SIZE - ARENA_HEIGHT));
		i_arenas.push_back(Arena(79 * CELL_SIZE, 12 * CELL_SIZE - ARENA_HEIGHT));
	}	
}

void MapManager::continue_map_sketch(unsigned char& i_current_level, std::vector<std::shared_ptr<Enemy>>& i_enemies, Player& i_player, std::vector<Arena>& i_arenas, std::vector<std::shared_ptr<Bullet>>& i_bullets, std::vector<Bikec>& i_bikci, int& i_score)
{
	i_enemies.clear();
	i_player.reset();
	i_arenas.clear();
	i_bullets.clear();
	i_bikci.clear();
	colliders.clear();
	std::ifstream condata("assets/positions.txt");

	bool run = true;
	while (run)
	{
		switch (condata.get())
		{
		case EOF: run = false; break;
		case 'c':
		{
			condata.ignore();
			int x;
			condata >> x;
			i_current_level = x;
			
			std::string current_map_file = "assets/map" + std::to_string(x) + ".txt";

			std::ifstream data(current_map_file);
			if (!data.is_open())
			{
				std::cout << "Could not open " << current_map_file << "!\n";
				return;
			}
			unsigned short int ch;
			for (int i = 0; i < 14; ++i)
			{
				for (int j = 0; j < 212; ++j)
				{
					data >> ch;
					map[i][j] = Cell(ch, j * CELL_SIZE, i * CELL_SIZE, colliders);
					data.ignore();
				}
			}
			data.close();
			current_map_file.clear();
			condata.ignore();
			break;
		}
		case 'p':
		{
			condata.ignore();
			float x, y;
			condata >> x >> y;
			i_player.set_position(x, y);
			condata.ignore();
			break;
		}
		case 'r':
		{
			condata.ignore();
			bool t;
			float x, y;
			condata >> t >> x >> y;
			if (!t)
			{
				i_enemies.push_back(std::make_shared<Walker>(rand() % (2), x, y));
			}
			else
			{
				i_enemies.push_back(std::make_shared<Rider>(rand() % (2), x, y));
			}
			condata.ignore();
			break;
		}
		case 'a':
		{
			condata.ignore();
			float x, y;
			condata >> x >> y;
			i_arenas.push_back(Arena(x, y));
			condata.ignore();
			break;
		}
		case 'x':
		{
			condata.ignore();
			int x;
			condata >> x;
			for (int i = 0; i < x; ++i)
			{
				i_bikci.push_back(Bikec());
			}
			condata.ignore();
			condata >> i_score;
			run = 0;
			break;
		}
		case 'b':
		{
			condata.ignore();
			float x, y;
			bool z;
			condata >> x >> y >> z;
			i_bullets.push_back(std::make_shared<Bullet>(x, y, z));
			condata.ignore();
			break;
		}

		}
	}
	condata.close();
}






	

void MapManager::draw_background()
{
	SDL_Rect tmp;
	tmp.x = tmp.y = 0;
	tmp.h = 14 * CELL_SIZE;
	tmp.w = 212*CELL_SIZE;
	TextureManager::Draw(rend, background_texture, tmp);
}

void MapManager::draw_map(float i_offset)
{
	for (int i = 0; i < 14; ++i) { //15
		for (int j = 0; j < 212; ++j) { //224
			map[i][j].draw(rend, textures, i_offset);
		}
	}
}

void MapManager::getColliders()
{
	for (auto i : colliders) {
		std::cout << i.cell_id << ' ' << i.x << ' ' << i.y << '\n';
	}
}

bool MapManager::collisionX(SDL_FRect a)
{	
	SDL_FRect t;
	t.w = t.h = CELL_SIZE;
	for (auto i : colliders)
	{
		t.x = i.x;
		t.y = i.y;
		if (SDL_HasIntersectionF(&a, &t))
		{
			return true;
		}

	}
	return false;
}

bool MapManager::collisionY(SDL_FRect a)
{
	SDL_FRect t;
	t.w = t.h = CELL_SIZE;
	for (auto i : colliders)
	{
		t.x = i.x;
		t.y = i.y;
		if (SDL_HasIntersectionF(&a, &t))
		{
			return true;
		}
	}
	return false;
}

