#include "Arena.hpp"
#include "global.hpp"

Arena::Arena(const float i_x, const float i_y) :
	x(i_x),
	y(i_y),
	health(3),
	tex("assets/arena.bmp")
{
}

bool Arena::get_dead() const
{
	if (0 == health)
	{
		return 1;
	}
	return 0;
}

SDL_FRect Arena::get_hit_box() const
{
	SDL_FRect tmp;
	tmp.x = x;
	tmp.y = y;
	tmp.w = ARENA_WIDTH;
	tmp.h = ARENA_HEIGHT;
	return tmp;
}

void Arena::draw(SDL_Renderer*& rend, Player i_player)
{
	SDL_Texture* texture;
	

	if (health > 0)
	{
		if (0 == is_shown(i_player.get_x()))
		{
			return;
		}
		SDL_FRect t = i_player.get_hit_box(), u = get_hit_box();
		u.x -= u.w;
		u.w *= 3;
		u.y = 0;
		u.h = SCREEN_HEIGHT;
		if (0 == SDL_HasIntersectionF(&t, &u))
		{
			return;
		}

		texture = TextureManager::LoadTexture(rend, tex.c_str());
		
	}
	else
	{
		texture = TextureManager::LoadTexture(rend, "assets/arena2.bmp");
	}
	SDL_Rect tmp;
	tmp.w = ARENA_WIDTH;
	tmp.h = ARENA_HEIGHT;
	tmp.x = (i_player.get_x() + CELL_SIZE / 2 < SCREEN_WIDTH / 2) ? (int)(x + 0) : (int)(x - (i_player.get_x() - (SCREEN_WIDTH / 2 - CELL_SIZE / 2)));
	tmp.y = y;
	TextureManager::Draw(rend, texture, tmp);
	SDL_DestroyTexture(texture);	
}

void Arena::hit() {
	--health;
}

bool Arena::is_shown(float i_pos)
{
	SDL_FRect t,u = get_hit_box();
	t.w = t.h = CELL_SIZE;
	t.x = i_pos;
	t.y = SCREEN_HEIGHT / 2; //ker nimamo maria
	u.x -= u.w;
	u.w *= 3;
	u.y = 0;
	u.h = SCREEN_HEIGHT;
	return SDL_HasIntersectionF(&t, &u);
}