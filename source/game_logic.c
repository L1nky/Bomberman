#include "game_logic.h"
#include "graphics.h"
#include "definitions.h"
#include "sounds.h"
#include "graphics_sub.h"
#include "graphics_main.h"

#include <nds.h>
#include <stdio.h>

#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"

#define TICKS_BEFORE_EXPLOSION 100

void do_game_tick(unsigned down, unsigned held);
void compute_move(float player_x, float player_y, float move_x, float move_y, float* result_x, float* result_y);
u8 is_walkable(u8 x, u8 y);
void update_bombs();
void explode_bomb(u8 x, u8 y, u8 range);
u8 explode_tile(u8 x, u8 y, u8 dir);
void handle_upgrade();
void updateBoard();
u8 select_lives();

u8 board[BOARD_TILE_SIZE];

u8 world[GAME_TILE_SIZE];

u8 crates_destroyed[GAME_TILE_SIZE] = {0};

u8 isPaused = 0;
u8 inMainMenu = 1;
u8 chosen_lives = 0;

player player1, player2;

void init_game(int initial_lives)
{
	isPaused = 0;
	u8 i;
	for(i = 0; i < GAME_TILE_SIZE; i++)
		world[i] = (i % GAME_TILE_SIZE_X) % 2 && (i/GAME_TILE_SIZE_X) % 2 ? WALL : CRATE;
	for(i = 0; i < BOARD_TILE_SIDE; i++)
		board[i] = 0;

	world[27] = GROUND;
	world[35] = GROUND;
	world[36] = GROUND;
	world[37] = GROUND;
	world[43] = GROUND;
	world[44] = GROUND;

	player1.x = PLAYER1_START_X;
	player1.y = PLAYER1_START_Y;
	player1.keyUp = PLAYER1_KEY_UP;
	player1.keyDown = PLAYER1_KEY_DOWN;
	player1.keyLeft = PLAYER1_KEY_LEFT;
	player1.keyRight = PLAYER1_KEY_RIGHT;
	player1.keyBomb = PLAYER1_KEY_BOMB;
	player1.bomb_storage_upgrade = 0;
	player1.bomb_used = 0;
	player1.bomb_range_upgrade = 0;
	player1.lives = initial_lives;

	player2.x = PLAYER2_START_X;
	player2.y = PLAYER2_START_Y;
	player2.keyUp = PLAYER2_KEY_UP;
	player2.keyDown = PLAYER2_KEY_DOWN;
	player2.keyLeft = PLAYER2_KEY_LEFT;
	player2.keyRight = PLAYER2_KEY_RIGHT;
	player2.keyBomb = PLAYER2_KEY_BOMB;
	player2.bomb_storage_upgrade = 0;
	player2.bomb_used = 0;
	player2.bomb_range_upgrade = 0;
	player2.lives = initial_lives;

	for(i = 0; i < BOMB_MAX_STORAGE; i++)
	{
		player1.bombs[i].used = 0;
		player2.bombs[i].used = 0;
	}

	//MUSIC MODULE START
	mmStop();
	mmStart(MOD_CLONE_WARPING, MM_PLAY_LOOP);

	updateBoard();
	srand(time(0));
}

void update_game()
{
	scanKeys();
	unsigned down = keysDown();
	unsigned held = keysHeld();

	if(inMainMenu)
	{
		mmStart(MOD_DUKE_NUKEM, MM_PLAY_LOOP);
		if(held & KEY_TOUCH)
		{
			touchPosition touch;
			touchRead(&touch);
			if(touch.py > 128)
			{
				if(touch.px < 64){
					init_game(1); chosen_lives =1;}
				else if(touch.px < 128){
					init_game(2);chosen_lives =2;}
				else if(touch.px < 192){
					init_game(3);chosen_lives =3;}
				else if(touch.px < 256){
					init_game(4);chosen_lives =4;}

				loadGameBoard();
				inMainMenu = 0;
			}
		}
		return;
	}

	if(down & KEY_START)
	{
		isPaused = !isPaused;
		if(isPaused)
			mmPause();
		else mmResume();

		return;
	}

	if(isPaused)
	{
		if(held & KEY_TOUCH)
		{
			touchPosition touch;
			touchRead(&touch);
			if(touch.px >= 96 && touch.px < 160 && touch.py >= 84 && touch.py < 116)
			{
				loadMainMenu();
				inMainMenu = 1;
			}
		}
	}
	else
	{
		do_game_tick(down, held);
	}

	updateBoard();
	updateGraphics(world, player1, player2, board, isPaused, inMainMenu);
}

void do_game_tick(unsigned down, unsigned held)
{
	if(player1.lives <= 0 || player2.lives <= 0)
	{
		init_game(chosen_lives);
		return;
	}
	update_bombs();

	//printf("%c %c %c %c\n", held & KEY_LEFT ? '<' : ' ', held & KEY_DOWN ? 'v' : ' ', held & KEY_UP ? '^' : ' ', held & KEY_RIGHT ? '>' : ' ');
	if(down & player1.keyBomb)
	{
		if(player1.bomb_used < BOMB_BASE_STORAGE + player1.bomb_storage_upgrade)
		{
			bomb b;
			b.x = (player1.x + PLAYER_SIZE_X/2)/24;
			b.y = (player1.y + PLAYER_SIZE_Y/2)/24;
			if(world[b.y*GAME_TILE_SIZE_X + b.x] == GROUND)
			{
				mmEffect(SFX_WOOSH);
				b.used = 1;
				b.ticks_before_explosion = TICKS_BEFORE_EXPLOSION;
				player1.bombs[player1.bomb_used] = b;
				player1.bomb_used++;
				world[b.y*GAME_TILE_SIZE_X + b.x] = BOMB;
			}
		}
	}

	if(down & player2.keyBomb)
	{
		if(player2.bomb_used < BOMB_BASE_STORAGE + player2.bomb_storage_upgrade)
		{
			bomb b;
			b.x = (player2.x + PLAYER_SIZE_X/2)/24;
			b.y = (player2.y + PLAYER_SIZE_Y/2)/24;
			if(world[b.y*GAME_TILE_SIZE_X + b.x] == GROUND)
			{
				mmEffect(SFX_WOOSH);
				b.used = 2;
				b.ticks_before_explosion = TICKS_BEFORE_EXPLOSION;
				player2.bombs[player2.bomb_used] = b;
				player2.bomb_used++;
				world[b.y*GAME_TILE_SIZE_X + b.x] = BOMB;
			}
		}
	}

	compute_move(player1.x, player1.y, (held & player1.keyRight ? PLAYER_SPEED : 0) - (held & player1.keyLeft ? PLAYER_SPEED : 0),
				 (held & player1.keyDown ? PLAYER_SPEED : 0) - (held & player1.keyUp ? PLAYER_SPEED : 0), &(player1.x), &(player1.y));
	compute_move(player2.x, player2.y, (held & player2.keyRight ? PLAYER_SPEED : 0) - (held & player2.keyLeft ? PLAYER_SPEED : 0),
				 (held & player2.keyDown ? PLAYER_SPEED : 0) - (held & player2.keyUp ? PLAYER_SPEED : 0), &(player2.x), &(player2.y));
	//printf("%f %f\n", player1.x, player1.y);

	handle_upgrade();

}

void compute_move(float player_x, float player_y, float move_x, float move_y, float* result_x, float* result_y)
{
	int tile_pos_x;
	int future_tile_pos_x;
	int tile_pos_y;
	int future_tile_pos_y;
	if(move_x)
	{
		if(move_x > 0)
		{
			tile_pos_x = (int)(player_x + PLAYER_SIZE_X - 1) / 24;
			future_tile_pos_x = (int)(player_x + move_x + PLAYER_SIZE_X - 1) / 24;
			tile_pos_y = (int)player_y / 24;
			future_tile_pos_y = (int)(player_y + PLAYER_SIZE_Y - 1) / 24;
			if(tile_pos_x == future_tile_pos_x || (is_walkable(future_tile_pos_x, tile_pos_y) && is_walkable(future_tile_pos_x, future_tile_pos_y)))
				*result_x  += move_x;
			else
				*result_x = (tile_pos_x + 1)*24 - PLAYER_SIZE_X;
		}
		else
		{
			if(player_x + move_x < 0)
				*result_x = 0;
			else
			{
				tile_pos_x = (int)player_x / 24;
				future_tile_pos_x = (int)(player_x + move_x) / 24;
				tile_pos_y = (int)player_y / 24;
				future_tile_pos_y = (int)(player_y + PLAYER_SIZE_Y - 1) / 24;
				if(tile_pos_x == future_tile_pos_x || (is_walkable(future_tile_pos_x, tile_pos_y) && is_walkable(future_tile_pos_x, future_tile_pos_y)))
					*result_x  += move_x;
				else
					*result_x = tile_pos_x*24;
			}
		}
	}
	if(move_y)
	{
		if(move_y > 0)
		{
			tile_pos_y = (int)(player_y + PLAYER_SIZE_Y - 1) / 24;
			future_tile_pos_y = (int)(player_y + move_y + PLAYER_SIZE_Y - 1) / 24;
			tile_pos_x = (int)player_x / 24;
			future_tile_pos_x = (int)(player_x + PLAYER_SIZE_X - 1) / 24;
			if(tile_pos_y == future_tile_pos_y || (is_walkable(tile_pos_x, future_tile_pos_y) && is_walkable(future_tile_pos_x, future_tile_pos_y)))
				*result_y  += move_y;
			else
				*result_y = (tile_pos_y + 1)*24 - PLAYER_SIZE_Y;
		}
		else
		{
			if(player_y + move_y < 0)
				*result_y = 0;
			else
			{
				tile_pos_y = (int)player_y / 24;
				future_tile_pos_y = (int)(player_y + move_y) / 24;
				tile_pos_x = (int)player_x / 24;
				future_tile_pos_x = (int)(player_x + PLAYER_SIZE_X - 1) / 24;
				if(tile_pos_y == future_tile_pos_y || (is_walkable(tile_pos_x, future_tile_pos_y) && is_walkable(future_tile_pos_x, future_tile_pos_y)))
					*result_y  += move_y;
				else
					*result_y = tile_pos_y*24;
			}
		}
	}
}

u8 is_walkable(u8 x, u8 y)
{
	if(x >= GAME_TILE_SIZE_X || y >= GAME_TILE_SIZE_Y)
		return 0;
	u8 idx = y*GAME_TILE_SIZE_X + x;
	return !(world[idx] == WALL || world[idx] == CRATE);
}

void update_bombs()
{
	u8 i;

	for(i = 0; i < GAME_TILE_SIZE; i++)
	{
		if(VERT_EXPLOSION_START <= world[i] && world[i] < VERT_EXPLOSION_END)
			world[i]++;
		else if(HOR_EXPLOSION_START <= world[i] && world[i] < HOR_EXPLOSION_END)
			world[i]++;
		else if(CEN_EXPLOSION_START <= world[i] && world[i] < CEN_EXPLOSION_END)
			world[i]++;
		else if(world[i] == VERT_EXPLOSION_END || world[i] == HOR_EXPLOSION_END || world[i]==CEN_EXPLOSION_END)
			world[i] = GROUND;
	}

	for(i = 0; i < BOMB_MAX_STORAGE; i++)
	{
		if(player1.bombs[i].used)
		{
			if(player1.bombs[i].ticks_before_explosion)
				player1.bombs[i].ticks_before_explosion--;
			else
			{
				mmEffect(SFX_BOOM);
				player1.bombs[i].used = 0;
				player1.bomb_used--;
				explode_bomb(player1.bombs[i].x, player1.bombs[i].y, player1.bomb_range_upgrade + BOMB_BASE_RANGE);
			}
		}

		if(player2.bombs[i].used)
		{
			if(player2.bombs[i].ticks_before_explosion)
				player2.bombs[i].ticks_before_explosion--;
			else
			{
				mmEffect(SFX_BOOM);
				player2.bombs[i].used = 0;
				player2.bomb_used--;
				explode_bomb(player2.bombs[i].x, player2.bombs[i].y, player2.bomb_range_upgrade + BOMB_BASE_RANGE);
			}
		}
	}

	for(i = 0; i < GAME_TILE_SIZE; i++)
		if(crates_destroyed[i])
		{
			int random = rand();
			printf("%d    %d", random, RAND_MAX*STORAGE_UPGRADE_APPARITION_PROB);
			if(random < RAND_MAX*STORAGE_UPGRADE_APPARITION_PROB)
				world[i] = STORAGE_UPGRADE;
			else if(random - RAND_MAX*STORAGE_UPGRADE_APPARITION_PROB < RAND_MAX* RANGE_UPGRADE_APPARITION_PROB)
				world[i] = RANGE_UPGRADE;
			else
				world[i] = GROUND;
			crates_destroyed[i] = 0;
		}

	if(player1.hurt)
	{
		player1.lives--;
		if (player1.lives)
			duke_nukem();
		else
			duke_terminated();
		player1.hurt = 0;
	}
	if(player2.hurt)
	{
		player2.lives--;
		if (player2.lives)
			duke_nukem();
		else
			duke_terminated();
		player2.hurt = 0;
	}
}

void explode_bomb(u8 x, u8 y, u8 range)
{
	u8 i;

	world[y*GAME_TILE_SIZE_X + x] = GROUND;

	explode_tile(x, y, 2);

	for(i = 1; i <= range; i++)
	{
		if(x + i >= GAME_TILE_SIZE_X)
			break;
		if(explode_tile(x + i, y, 0))
			break;
	}

	for(i = 1; i <= range; i++)
	{
		if(x < i)
			break;
		if(explode_tile(x - i, y, 0))
			break;
	}

	for(i = 1; i <= range; i++)
	{
		if(y + i >= GAME_TILE_SIZE_Y)
			break;
		if(explode_tile(x, y + i, 1))
			break;
	}

	for(i = 1; i <= range; i++)
	{
		if(y < i)
			break;
		if(explode_tile(x, y - i, 1))
			break;
	}
}

u8 explode_tile(u8 x, u8 y, u8 dir)
{
	u8 hit = 0;
	u8 i;
	u8 tile_idx = y*GAME_TILE_SIZE_X + x;

	if(((int)player1.x/24 == x || ((int)player1.x + PLAYER_SIZE_X)/24 == x) && ((int)player1.y/24 == y || ((int)player1.y + PLAYER_SIZE_Y)/24 == y))
	{
		player1.hurt = 1;
	}

	if(((int)player2.x/24 == x || ((int)player2.x + PLAYER_SIZE_X)/24 == x) && ((int)player2.y/24 == y || ((int)player2.y + PLAYER_SIZE_Y)/24 == y))
	{
		player2.hurt = 1;
	}

	if(world[tile_idx] == GROUND){
		switch(dir){
						case 0: world[tile_idx] = HOR_EXPLOSION_START; break;
						case 1: world[tile_idx] = VERT_EXPLOSION_START; break;
						case 2: world[tile_idx] = CEN_EXPLOSION_START; break;
						default: break;
		}
	}

	if(world[tile_idx] == WALL)
		hit = 1;
	if(world[tile_idx] == CRATE)
	{
		crates_destroyed[tile_idx] = 1;
		hit = 1;
	}
	else if(world[tile_idx] == BOMB)
	{
		for(i = 0; i < BOMB_MAX_STORAGE; i++)
		{
			if(player1.bombs[i].used && player1.bombs[i].x == x && player1.bombs[i].y == y)
			{
				player1.bombs[i].used = 0;
				player1.bomb_used--;
				explode_bomb(x, y, player1.bomb_range_upgrade + BOMB_BASE_RANGE);
				hit = 1;
				break;
			}

			if(player2.bombs[i].used && player2.bombs[i].x == x && player2.bombs[i].y == y)
			{
				player2.bombs[i].used = 0;
				player2.bomb_used--;
				explode_bomb(x, y, player2.bomb_range_upgrade + BOMB_BASE_RANGE);
				hit = 1;
				break;
			}
		}
	}

	return hit;
}

void handle_upgrade()
{
	u8 player1_idx = ((u8)player1.y + PLAYER_SIZE_Y/2)/24 * GAME_TILE_SIZE_X + ((u8)player1.x + PLAYER_SIZE_X/2)/24;
	if(world[player1_idx] == STORAGE_UPGRADE)
	{
		player1.bomb_storage_upgrade++;
		if(player1.bomb_storage_upgrade > BOMB_MAX_STORAGE - BOMB_BASE_STORAGE)
			player1.bomb_storage_upgrade = BOMB_MAX_STORAGE - BOMB_BASE_STORAGE;
		else mmEffect(SFX_MARIO_1_UP);
		world[player1_idx] = GROUND;
	}
	else if(world[player1_idx] == RANGE_UPGRADE)
	{
		player1.bomb_range_upgrade++;
		if(player1.bomb_range_upgrade > BOMB_MAX_RANGE - BOMB_BASE_RANGE)
			player1.bomb_range_upgrade = BOMB_MAX_RANGE - BOMB_BASE_RANGE;
		else mmEffect(SFX_MARIO_TOUCH_ENNEMY);
		world[player1_idx] = GROUND;
	}

	u8 player2_idx = ((u8)player2.y + PLAYER_SIZE_Y/2)/24 * GAME_TILE_SIZE_X + ((u8)player2.x + PLAYER_SIZE_X/2)/24;
	if(world[player2_idx] == STORAGE_UPGRADE)
	{
		player2.bomb_storage_upgrade++;
		if(player2.bomb_storage_upgrade > BOMB_MAX_STORAGE - BOMB_BASE_STORAGE)
			player2.bomb_storage_upgrade = BOMB_MAX_STORAGE - BOMB_BASE_STORAGE;
		else mmEffect(SFX_MARIO_1_UP);
		world[player2_idx] = GROUND;
	}
	else if(world[player2_idx] == RANGE_UPGRADE)
	{
		player2.bomb_range_upgrade++;
		if(player2.bomb_range_upgrade > BOMB_MAX_RANGE - BOMB_BASE_RANGE)
			player2.bomb_range_upgrade = BOMB_MAX_RANGE - BOMB_BASE_RANGE;
		else mmEffect(SFX_MARIO_TOUCH_ENNEMY);
		world[player2_idx] = GROUND;
	}
}

void updateBoard()
{
	int column;
	for(column=0; column < BOARD_TILE_SIZE; column++)
		board[column]=0;
	for(column =0; column < PLAYER_MAX_LIVES; column++){
		board[BOARD_TILE_SIZE_X*BOARD_ROW_LIVES+column]=(column<player1.lives? LIFE: 0);
		board[BOARD_TILE_SIZE_X*BOARD_ROW_LIVES+BOARD_COL_PLAYER2+column]=(column<player2.lives? LIFE: 0);

		board[BOARD_TILE_SIZE_X*BOARD_ROW_BOMBS+column]=(column<player1.bomb_storage_upgrade? STORAGE_UPGRADE: 0);
		board[BOARD_TILE_SIZE_X*BOARD_ROW_BOMBS+BOARD_COL_PLAYER2+column]=(column<player2.bomb_storage_upgrade? STORAGE_UPGRADE: 0);

		board[BOARD_TILE_SIZE_X*BOARD_ROW_POWER+column]=(column<player1.bomb_range_upgrade? RANGE_UPGRADE: 0);
		board[BOARD_TILE_SIZE_X*BOARD_ROW_POWER+BOARD_COL_PLAYER2+column]=(column<player2.bomb_range_upgrade? RANGE_UPGRADE: 0);
	}
}

u8 select_lives(){
	u8 lives=0;
	while(1){
		swiWaitForVBlank();
		scanKeys();
		unsigned held = keysDown();
		if(held & KEY_TOUCH){
			touchPosition touch;
			touchRead(&touch);
			if(touch.py>128)
			{
				if(touch.px<64){
					lives = 1; break;}
				else if(touch.px<128){
					lives = 2; break;}
				else if(touch.px<192){
					lives = 3; break;}
				else if(touch.px<256){
					lives = 4; break;}
			}
		}
	}
	return lives;
}
