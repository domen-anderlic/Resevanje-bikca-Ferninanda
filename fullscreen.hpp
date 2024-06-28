#pragma once


class Global {
public:
	float X;
	float WALKER_SPEED ;
	float GRAVITY;
	float RIDER_HURT_SPEED;
	float RIDER_SPEED;
	float PLAYER_ACCELARATION;
	float PLAYER_JUMP_SPEED;
	float PLAYER_WALK_SPEED;
	float MAX_VERTICAL_SPEED;

	unsigned char PLAYER_DEATH_DURATION;
	unsigned char PLAYER_JUMP_TIMER;



	unsigned short WALKER_DEATH_DURATION;
	unsigned short RIDER_RECOVER_DURATION;

	unsigned short SCREEN_HEIGHT;
	unsigned short SCREEN_WIDTH;
	unsigned char CELL_SIZE;

	float BULLET_SPEED;
	unsigned short int BULLET_COOLDOWN;

	unsigned short int ARENA_WIDTH;
	unsigned short int ARENA_HEIGHT;
	Global(float i_T)
	{
		X = i_T;
		WALKER_SPEED = 1 * T;
		GRAVITY = 0.27f * T;
		RIDER_HURT_SPEED = 4 * T;
		RIDER_SPEED = 1 * T;
		PLAYER_ACCELARATION = 0.25f * T;
		PLAYER_JUMP_SPEED = -6 * T;
		PLAYER_WALK_SPEED = 4 * T;
		MAX_VERTICAL_SPEED = 16 * T;

		PLAYER_DEATH_DURATION = 32;
		PLAYER_JUMP_TIMER = 10;



		WALKER_DEATH_DURATION = 32;
		RIDER_RECOVER_DURATION = 512;

		SCREEN_HEIGHT = 14 * 32 * T;
		SCREEN_WIDTH = 25 * 32 * T;
		CELL_SIZE = 32 * T;

		BULLET_SPEED = 7 * T;
		BULLET_COOLDOWN = 40;

		ARENA_WIDTH = (446 / 3) * T;
		ARENA_HEIGHT = (342 / 3) * T;
	}
};