#include "Player.hpp"
#include "global.hpp"
#include "textureManager.hpp"
#include "Text.hpp"


Player::Player(SDL_Renderer*& renderer) :
	dead(0),
	flipped(0),
	on_ground(0),
	enemy_bounce_speed(0),
	horizontal_speed(0),
	vertical_speed(0),
	x(0),
	y(0),
	jump_timer(0),
	death_timer(PLAYER_DEATH_DURATION),
	rend(renderer),
	invincible_timer(0),
	u(0),
	d(0),
	l(0),
	r(0),
	shot(0),
	bullet_cooldown(BULLET_COOLDOWN)
{
	texture = TextureManager::LoadTexture(rend, "assets/player.bmp");
}

bool Player::get_dead() const
{
	return dead;
}

float Player::get_vertical_speed() const
{
	return vertical_speed;
}

float Player::get_x() const
{
	return x;
}

float Player::get_y() const
{
	return y;
}

void Player::die(const bool i_instant_death)
{
	dead = 1;
}

void Player::draw()
{	
	SDL_Rect a;
	a.x = (x + CELL_SIZE/2 < SCREEN_WIDTH/2) ? (int)x : (int)(SCREEN_WIDTH/2 - CELL_SIZE/2);//(int)x;
	a.y = (int)y;
	a.w = a.h = CELL_SIZE;
	TextureManager::Draw(rend, texture,a, SDL_RendererFlip(flipped));
}

void Player::reset()
{
	dead = 0;
	flipped = 0;
	on_ground = 0;
	invincible_timer = 0;
	enemy_bounce_speed = 0;
	horizontal_speed = 0;
	vertical_speed = 0;
	x = 0;
	y = 0;

	jump_timer = 0;
	death_timer = PLAYER_DEATH_DURATION;

	//texture = TextureManager::LoadTexture(rend, "assets/playerdeath.bmp");
}

void Player::set_position(const float i_x, const float i_y)
{
	x = i_x;
	y = i_y;
}

void Player::set_vertical_speed(const float i_value)
{
	enemy_bounce_speed = i_value;
}

SDL_FRect Player::get_hit_box() const
{
	SDL_FRect tmp;
	tmp.w = tmp.h = CELL_SIZE;
	tmp.x = x;
	tmp.y = y;
	return tmp;
}

bool Player::update(SDL_Event& i_event, MapManager& i_map_manager, std::vector<std::shared_ptr<Bullet>>& bullets, bool can_shoot)
{
	if(SDL_KEYDOWN == i_event.type)
	{
		switch (i_event.key.keysym.scancode)
		{
			case SDL_SCANCODE_UP: u = 1; break;
			case SDL_SCANCODE_DOWN: d = 1; break;
			case SDL_SCANCODE_LEFT: l = 1; break;
			case SDL_SCANCODE_RIGHT: r = 1; break;
			case SDL_SCANCODE_SPACE: shot = 1; break;
			//case SDL_SCANCODE_GRAVE: i_isRunning = false;// ` - haha (get it? it's grave)
			default:break;
		}
	}
	if (SDL_KEYUP == i_event.type)
	{
		switch (i_event.key.keysym.scancode)
		{
			case SDL_SCANCODE_UP: u = 0; break;
			case SDL_SCANCODE_DOWN: d = 0; break;
			case SDL_SCANCODE_LEFT: l = 0; break;
			case SDL_SCANCODE_RIGHT: r = 0; break;
			case SDL_SCANCODE_SPACE: shot = 0; break;
			default:break;
		}
	}
	if (0 != enemy_bounce_speed)
	{
		vertical_speed = enemy_bounce_speed;

		enemy_bounce_speed = 0;
	}
	if (0 == dead)
	{
		bool moving = 0;	

		on_ground = 0;

		if (1 == l &&  0 == r)
		{
			moving = 1;
			horizontal_speed = std::max(horizontal_speed - PLAYER_ACCELARATION, -PLAYER_WALK_SPEED);
		}

		if (0 == l && 1 == r)
		{
			moving = 1;
			horizontal_speed = std::min(PLAYER_ACCELARATION + horizontal_speed, PLAYER_WALK_SPEED);
		}

		if (0 == moving)
		{
			if (0 < horizontal_speed)
			{
				horizontal_speed = std::max<float>(0, horizontal_speed - PLAYER_ACCELARATION);
			}
			else if (0 > horizontal_speed)
			{
				horizontal_speed = std::min<float>(0, PLAYER_ACCELARATION + horizontal_speed);
			}
		}

		SDL_FRect tmpHitbox;
		tmpHitbox.w = tmpHitbox.h = CELL_SIZE;
		tmpHitbox.x = x + horizontal_speed;
		tmpHitbox.y = y;

		if (i_map_manager.collisionX(tmpHitbox))//if(collision left/right)
		{
			moving = 0;

			if (0 < horizontal_speed)
			{
				x = CELL_SIZE * (ceil((horizontal_speed + x) / CELL_SIZE) - 1);
			}
			else if (0 > horizontal_speed)
			{
				x = CELL_SIZE * (1 + floor((horizontal_speed + x) / CELL_SIZE));
			}

			horizontal_speed = 0;
		}
		else
		{
			x += horizontal_speed;
		}
		tmpHitbox.x = x;
		tmpHitbox.y = y+1; //+1 da preverimo, ali stoji na tleh (je 1px nad tlemi)		

		if (1 == u)
		{
			if (0 == vertical_speed && 1 == i_map_manager.collisionY(tmpHitbox))
			{
				vertical_speed = PLAYER_JUMP_SPEED;

				jump_timer = PLAYER_JUMP_TIMER;
			}
			else if (0 < jump_timer) //dlje kot drzimo, visje skocimo
			{
				vertical_speed = PLAYER_JUMP_SPEED;

				jump_timer--;
			}
			else
			{
				vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);
			}
		}
		else
		{
			vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);
			jump_timer = 0;
		}

		tmpHitbox.x = x;
		tmpHitbox.y = y + vertical_speed;

		if (1 == i_map_manager.collisionY(tmpHitbox))
		{
			if (0 > vertical_speed)
			{
				y = CELL_SIZE * (1 + floor((vertical_speed + y) / CELL_SIZE));
			}
			else if (0 < vertical_speed)
			{
				y = CELL_SIZE * (ceil((vertical_speed + y) / CELL_SIZE) - 1);
			}

			jump_timer = 0;

			vertical_speed = 0;
		}
		else
		{
			y += vertical_speed;
		}
		if (0 == horizontal_speed)
		{
			if (1 == moving)
			{
				flipped = 1 - flipped;
			}
		}
		else if (0 < horizontal_speed)
		{
			flipped = 0;
		}
		else if (0 > horizontal_speed)
		{
			flipped = 1;
		}

		tmpHitbox.x = x;
		tmpHitbox.y = y+1;
		
		if (0 == i_map_manager.collisionY(tmpHitbox))
		{
			on_ground = 1;
		}

		tmpHitbox.x = x;
		tmpHitbox.y = y;
		if (0 < invincible_timer)
		{
			invincible_timer--;
		}

		if (y >= SCREEN_HEIGHT - tmpHitbox.h) //y>= SCREEN_HEIGHT
		{
			die(1);
		}
	}
	else
	{
		if (0 == death_timer)
		{
			vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);
			y += vertical_speed;
		}
		else if (1 == death_timer)
		{
			vertical_speed = PLAYER_JUMP_SPEED;
		}

		death_timer = std::max(0, death_timer - 1);
		
	}

	if (1 == shot && 1 == can_shoot && bullets.size() < 1)
	{
		bullets.push_back(std::make_shared<Bullet>(x, y, !flipped));
		return 1; //he shot
	}
	return 0; //did not shoot
}