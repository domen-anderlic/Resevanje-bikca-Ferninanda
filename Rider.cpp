#include "global.hpp"
#include "MapManager.hpp"
#include "Player.hpp"
#include "Rider.hpp"

Rider::Rider(const bool i_underground, const float i_x, const float i_y) :
	Enemy(i_x, i_y),
	check_collision(1),
	flipped(1),
	no_collision_dying(0),
	underground(i_underground),
	state(0),
	RECOVER_timer(RIDER_RECOVER_DURATION),
	tex("assets/rider.bmp")
{
	type = 1;
	horizontal_speed = -RIDER_SPEED;

	if (1 == underground)
	{
		tex = "assets/rider2.bmp";
	}
}

bool Rider::get_dead(const bool i_deletion) const
{
	if (1 == i_deletion)
	{
		return dead;
	}
	else
	{
		return dead || no_collision_dying;
	}
}

void Rider::die(const unsigned char i_death_type)
{
	switch (i_death_type)
	{
	case 0:
	{
		dead = 1;

		break;
	}
	case 2:
	{
		//Dying from Rider's shell.
		no_collision_dying = 1;

		vertical_speed = 0.5f * PLAYER_JUMP_SPEED;
		
	}
	}
	if (0 == underground)
	{
		tex = "assets/riderdeath.bmp";
	}
	else
	{
		tex = "assets/rider2death.bmp";
	}
}

void Rider::draw(SDL_Renderer*& rend, float i_pos)
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
	tmp.w = tmp.h = CELL_SIZE * 1.5;
	tmp.x = (i_pos + CELL_SIZE / 2.0f < SCREEN_WIDTH / 2.0f) ? (int)(x + 0) : (int)(x - (i_pos - (SCREEN_WIDTH / 2.0f - CELL_SIZE / 2.0f)));
	tmp.y = y - (CELL_SIZE/2);
	bool flip = (horizontal_speed > 0);
	
	TextureManager::Draw(rend,texture,tmp,SDL_RendererFlip(flip));
	SDL_DestroyTexture(texture);
}

void Rider::update(const std::vector<std::shared_ptr<Enemy>>& i_enemies, MapManager& i_map_manager, Player& i_player, float i_pos)
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

	vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);

	if (0 == get_dead(0))
	{
		SDL_FRect tmpHitbox = get_hit_box(), t;

		tmpHitbox.y += vertical_speed;

		if (1 == i_map_manager.collisionY(tmpHitbox))
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

			for (unsigned short a = 0; a < i_enemies.size(); a++)
			{
				t = i_enemies[a]->get_hit_box();
				if (shared_from_this() != i_enemies[a] && 0 == i_enemies[a]->get_dead(0) && 1 == SDL_HasIntersectionF(&tmpHitbox, &t))
				{
					changed = 1;

					if (0 > vertical_speed)
					{
						y = i_enemies[a]->get_hit_box().y + i_enemies[a]->get_hit_box().h;
					}
					else
					{
						y = i_enemies[a]->get_hit_box().y - CELL_SIZE;
					}

					vertical_speed = 0;

					break;
				}
			}

			if (0 == changed)
			{
				y += vertical_speed;
			}
		}

		tmpHitbox = get_hit_box();
		tmpHitbox.x += horizontal_speed;

		if (0 == no_collision_dying && 1 == i_map_manager.collisionX(tmpHitbox))
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

			for (unsigned short a = 0; a < i_enemies.size(); a++)
			{
				t = i_enemies[a]->get_hit_box();
				if (shared_from_this() != i_enemies[a] && 0 == i_enemies[a]->get_dead(0) && 1 == SDL_HasIntersectionF(&tmpHitbox, &t))
				{
					if (0 == state)
					{
						changed = 1;

						horizontal_speed *= -1;
					}
					else if (2 == state)
					{
						i_enemies[a]->die(2);
					}

					break;
				}
			}

			if (0 == changed)
			{
				x += horizontal_speed;
			}
		}
		//tmpHitbox = get_hit_box();
		t = i_player.get_hit_box();
		if (0 == i_player.get_dead() && 1 == SDL_HasIntersectionF(&tmpHitbox, &t))
		{
			if (1 == check_collision)
			{
				if (0 == state)
				{
					if (0 < i_player.get_vertical_speed())
					{
						check_collision = 0;

						horizontal_speed = 0;

						state = 1;

						RECOVER_timer = RIDER_RECOVER_DURATION;

						i_player.set_vertical_speed(0.5f * PLAYER_JUMP_SPEED);
						tex = "assets/riderhurt.bmp";
					}
					else
					{
						i_player.die(0);
					}
				}
				else if (1 == state)
				{
					check_collision = 0;

					state = 2;

					if (x < i_player.get_x())
					{
						horizontal_speed = -RIDER_HURT_SPEED;
					}
					else
					{
						horizontal_speed = RIDER_HURT_SPEED;
					}

					if (0 < i_player.get_vertical_speed())
					{
						i_player.set_vertical_speed(0.5f * PLAYER_JUMP_SPEED);
					}
				}
				else
				{
					
					if (0 < i_player.get_vertical_speed())
					{
						check_collision = 0;

						horizontal_speed = 0;

						state = 1;

						RECOVER_timer = RIDER_RECOVER_DURATION;

						i_player.set_vertical_speed(0.5f * PLAYER_JUMP_SPEED);
					}
					else if ((0 < horizontal_speed && x < i_player.get_x()) ||
						(0 > horizontal_speed && x > i_player.get_x()))
					{
						i_player.die(0);
					}
				}
			}
		}
		else
		{
			check_collision = 1;
		}

		if (0 < horizontal_speed)
		{
			flipped = 0;
		}
		else if (0 > horizontal_speed)
		{
			flipped = 1;
		}

		if (1 == state)
		{
			RECOVER_timer--;

			if (0 == RECOVER_timer)
			{
				state = 0;

				if (0 == flipped)
				{
					horizontal_speed = RIDER_SPEED;
				}
				else
				{
					horizontal_speed = -RIDER_SPEED;
				}
			}
			else
			{
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
