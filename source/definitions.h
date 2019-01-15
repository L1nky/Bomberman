#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <nds.h>

#define GAME_TILE_SIZE_X 9
#define GAME_TILE_SIZE_Y 7
#define GAME_TILE_SIZE (GAME_TILE_SIZE_X*GAME_TILE_SIZE_Y)
#define GAME_SIZE_X 216
#define GAME_SIZE_Y 168

#define BOARD_TILE_SIZE_X 8
#define BOARD_TILE_SIZE_Y 6
#define BOARD_TILE_SIZE (BOARD_TILE_SIZE_X*BOARD_TILE_SIZE_Y)
#define BOARD_TILE_SIDE 4
#define BOARD_SIZE_X 256
#define BOARD_SIZE_Y 192

#define SCREEN_TILE_NUMBER (BOARD_SIZE_X*BOARD_SIZE_Y/BOARD_TILE_SIDE)

#define BOARD_ROW_LIVES 3
#define BOARD_ROW_BOMBS 4
#define BOARD_ROW_POWER 5
#define BOARD_COL_PLAYER1 0
#define BOARD_COL_PLAYER2 4

#define GROUND 0
#define WALL 1
#define CRATE 2
#define BOMB 3
#define STORAGE_UPGRADE 4
#define RANGE_UPGRADE 5
#define LIFE 6
#define SCORE 7
//values of the start/stop counter for displaying the explosion tiles
#define VERT_EXPLOSION_START 10
#define VERT_EXPLOSION_END 50
#define HOR_EXPLOSION_START 60
#define HOR_EXPLOSION_END 100
#define CEN_EXPLOSION_START 110
#define CEN_EXPLOSION_END 150

#define PLAYER_SIZE_X 16
#define PLAYER_SIZE_Y 16
#define PLAYER_SPEED 1.f

#define PLAYER1_START_X 4
#define PLAYER1_START_Y 100
#define PLAYER1_KEY_UP KEY_UP
#define PLAYER1_KEY_DOWN KEY_DOWN
#define PLAYER1_KEY_LEFT KEY_LEFT
#define PLAYER1_KEY_RIGHT KEY_RIGHT
#define PLAYER1_KEY_BOMB KEY_L

#define PLAYER2_START_X (GAME_SIZE_X - PLAYER_SIZE_X - 4)
#define PLAYER2_START_Y 100
#define PLAYER2_KEY_UP KEY_X
#define PLAYER2_KEY_DOWN KEY_B
#define PLAYER2_KEY_LEFT KEY_Y
#define PLAYER2_KEY_RIGHT KEY_A
#define PLAYER2_KEY_BOMB KEY_R

#define BOMB_BASE_STORAGE 1
#define BOMB_MAX_STORAGE 4
#define BOMB_BASE_RANGE 1
#define BOMB_MAX_RANGE 3

#define PLAYER_MAX_LIVES 4
//TO BE ERASED IF WE INITIALIZE MAX LIVES BY THE MENU
#define PLAYER_INITIAL_LIVES 3

#define AVERAGE_STORAGE_UPGRADE_APPARITION_TIME 600
#define AVERAGE_RANGE_UPGRADE_APPARITION_TIME 600

typedef struct
{
	u8 used;
	u8 x;
	u8 y;
	u16 ticks_before_explosion;
}bomb;

typedef struct
{
	float x;
	float y;
	u16 keyUp;
	u16 keyDown;
	u16 keyLeft;
	u16 keyRight;
	u16 keyBomb;
	u8 bomb_storage_upgrade;
	u8 bomb_range_upgrade;
	u8 bomb_used;
	bomb bombs[BOMB_MAX_STORAGE];
	s8 lives;
	u8 hurt;

}player;

#endif /* DEFINITIONS_H_ */
