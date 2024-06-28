#include "global.hpp"
#include "MapManager.hpp"
#include "textureManager.hpp"
#include "Player.hpp"
#include "Walker.hpp"


Walker::Walker(const bool i_underground, const float i_x, const float i_y) :
	Enemy(i_x, i_y),
	no_collision_dying(0),
	underground(i_underground),
	death_timer(WALKER_DEATH_DURATION),
	tex("assets/walker.bmp")
{
	type = 0;
	horizontal_speed = -WALKER_SPEED;

	if (1 == underground)
	{
		tex = "assets/walker2.bmp";
	}
}

bool Walker::get_dead(const bool i_deletion) const
{
	if (1 == i_deletion)
	{
		return dead;
	}
	else
	{
		return dead || no_collision_dying || WALKER_DEATH_DURATION > death_timer;
	}
}

void Walker::die(const unsigned char i_death_type)
{
	switch (i_death_type)
	{
	case 0:
	{
		//Instant death. Setting dead to 1 will immediately delete the object.
		dead = 1;

		break;
	}
	case 1:
	{
		//walker is squished by player.
		if (0 == no_collision_dying)
		{
			death_timer--;
		}

		break;
	}
	case 2:
	{
		if (WALKER_DEATH_DURATION == death_timer)
		{
			//Walker dies from Rider's shell.
			no_collision_dying = 1;

			vertical_speed = 0.5f * PLAYER_JUMP_SPEED;
		}
	}
	}
	if (underground)
	{
		tex = "assets/walker2death.bmp";
	}
	else
	{
		tex = "assets/walkerdeath.bmp";
	}
}

void Walker::draw(SDL_Renderer*& rend, float i_pos)
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
	tmp.w = tmp.h = CELL_SIZE;
	tmp.x =  (i_pos + CELL_SIZE / 2.0f < SCREEN_WIDTH / 2.0f) ? (int)(x + 0) : (int)(x - (i_pos - (SCREEN_WIDTH / 2.0f - CELL_SIZE / 2.0f)));
	tmp.y = y;
	TextureManager::Draw(rend, texture, tmp);
	SDL_DestroyTexture(texture); //memory leak
}

void Walker::update(const std::vector<std::shared_ptr<Enemy>>& i_enemies,MapManager& i_map_manager, Player& i_player,float i_pos) //const MapManager&
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

	SDL_FRect tmpHitbox, t;
	tmpHitbox = get_hit_box();

	vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);
	tmpHitbox.y = y + vertical_speed;

	if (0 == no_collision_dying)
	{
		if (1 == i_map_manager.collisionY(tmpHitbox))//0
		{
			if (0 > vertical_speed)
			{
				y = CELL_SIZE * (1 + floor((vertical_speed + y) / CELL_SIZE));
			}
			else
			{
				y = CELL_SIZE * (ceil((vertical_speed + y) / CELL_SIZE) - 1);
			}

			vertical_speed = 0;
		}
		else
		{
			bool changed = 0;
			if (0 == get_dead(0))
			{
				for (unsigned short a = 0; a < i_enemies.size(); a++)
				{
					t = i_enemies[a]->get_hit_box();

					if (shared_from_this() != i_enemies[a] && 0 == i_enemies[a]->get_dead(0) && 1 == SDL_HasIntersectionF(&tmpHitbox, &t))
					{
						changed = 1;

						if (0 > vertical_speed)
						{
							y = i_enemies[a]->get_hit_box().h + i_enemies[a]->get_hit_box().y;
						}
						else
						{
							y = i_enemies[a]->get_hit_box().y - CELL_SIZE;
						}

						vertical_speed = 0;

						break;
					}
				}
			}

			if (0 == changed)
			{
				y += vertical_speed;
			}
		}
		if (0 == get_dead(0))
		{
			tmpHitbox = get_hit_box();
			tmpHitbox.x += horizontal_speed;

			if (1 == i_map_manager.collisionX(tmpHitbox))//0
			{
				if (0 < horizontal_speed)
				{
					x = CELL_SIZE * (ceil((horizontal_speed + x) / CELL_SIZE) - 1);
				}
				else
				{
					x = CELL_SIZE * (1 + floor((horizontal_speed + x) / CELL_SIZE));
				}

				horizontal_speed *= -1;
			}
			else
			{
				bool changed = 0;

				//zamenja smer ce se dotakne druzga sovraznika
				for (unsigned short a = 0; a < i_enemies.size(); ++a)
				{
					t = i_enemies[a]->get_hit_box();
					if (shared_from_this() != i_enemies[a] && 0 == i_enemies[a]->get_dead(0) && 1 == SDL_HasIntersectionF(&tmpHitbox, &t)) 
					{
						changed = 1;

						horizontal_speed *= -1;

						break;
					}
				}

				if (0 == changed)
				{
					x += horizontal_speed;
				}
			}
			t = i_player.get_hit_box();
			if (0 == i_player.get_dead() && 1 == SDL_HasIntersectionF(&tmpHitbox, &t))
			{
				if (0 < i_player.get_vertical_speed())
				{
					//igralec gre dol->smrt
					die(1);

					i_player.set_vertical_speed(0.5f * PLAYER_JUMP_SPEED);
				}
				else
				{
					//drugace pa ubijemo igralca
					i_player.die(0);
				}
			}
		}
		else if (WALKER_DEATH_DURATION > death_timer)
		{
			if (0 < death_timer)
			{
				death_timer--;
			}
			else
			{
				die(0);
			}
		}
	}
	else
	{
		y += vertical_speed;
	}

	if (SCREEN_HEIGHT <= y)
	{
		die(0);
	}
}