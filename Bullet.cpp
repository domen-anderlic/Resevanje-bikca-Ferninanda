#include "Bullet.hpp"

Bullet::Bullet(const float i_x, const float i_y, const bool i_direction):
	x(i_x),
	y(i_y),
	direction(i_direction),
	tex("assets/bullet0.bmp"),
	dead(0)
{
	horizontal_speed = (0 == i_direction) ? -BULLET_SPEED : BULLET_SPEED;
}
SDL_FRect Bullet::get_hit_box() const
{
	SDL_FRect tmp;
	tmp.w = tmp.h = CELL_SIZE;
	tmp.x = x;
	tmp.y = y;
	return tmp;
}
bool Bullet::get_dead() const
{
	return dead;
}
void Bullet::die()
{
	dead = 1;
}

void Bullet::draw(SDL_Renderer*&rend, float i_pos)
{
	if (i_pos + CELL_SIZE / 2 > SCREEN_WIDTH / 2)
	{
		if (x > (i_pos + CELL_SIZE / 2 + SCREEN_WIDTH / 2) || x < (i_pos + CELL_SIZE / 2 - SCREEN_WIDTH / 2) - CELL_SIZE)
		{
			return;
		}
	}
	else if (x > SCREEN_WIDTH) {
		return;
	}

	SDL_Texture* texture = TextureManager::LoadTexture(rend, tex.c_str());
	SDL_Rect tmp;
	tmp.w = tmp.h = CELL_SIZE/2;
	tmp.x = (i_pos + CELL_SIZE / 2 < SCREEN_WIDTH / 2) ? (int)(x + 0) : (int)(x - (i_pos - (SCREEN_WIDTH / 2 - CELL_SIZE / 2)));
	tmp.y = y+CELL_SIZE/3;
	TextureManager::Draw(rend, texture, tmp, SDL_RendererFlip(!direction));
	SDL_DestroyTexture(texture);
}

void Bullet::update(std::vector<std::shared_ptr<Enemy>>& i_enemies, MapManager& i_map_manager, float i_pos, std::vector<Arena>& i_arenas)
{
	if (i_pos + CELL_SIZE / 2 > SCREEN_WIDTH / 2)
	{
		if (x > (i_pos + CELL_SIZE / 2 + SCREEN_WIDTH / 2) || x < (i_pos + CELL_SIZE / 2 - SCREEN_WIDTH / 2) - CELL_SIZE)
		{
			dead = 1;
			return;
		}
	}
	else if (x > SCREEN_WIDTH) {
		return;
	}
	SDL_FRect tmpHitbox, t;
	tmpHitbox = get_hit_box();
	tmpHitbox.x += horizontal_speed;
	
	if (1 == i_map_manager.collisionX(tmpHitbox))
	{
		die();
	}
	else
	{
		for (unsigned short int a = 0; a < i_enemies.size(); ++a)
		{
			t = i_enemies[a]->get_hit_box();
			if (0 == i_enemies[a]->get_dead(0) && 1 == SDL_HasIntersectionF(&tmpHitbox, &t))
			{
				i_enemies[a]->die(2);
				die();
			}
		}
		for (unsigned short int a = 0; a < i_arenas.size(); ++a)
		{
			t = i_arenas[a].get_hit_box();
			if (0 == i_arenas[a].get_dead() && 1==i_arenas[a].is_shown(i_pos) && 1 == SDL_HasIntersectionF(&tmpHitbox, &t))
			{
				i_arenas[a].hit();
				die();
			}
		}
	}
	
	if (0 == get_dead())
	{
		x += horizontal_speed;
	}

}

bool Bullet::get_direction()
{
	return direction;
}