#include "graphics_sub.h"
#include <nds.h>
#include "background.h"
#include "board_tiles.h"
#include "definitions.h"


#define BG0_SUB_MAP_BASE 29
#define BG0_SUB_TILE_BASE 3

#define BG2_SUB_BMP_BASE 0
#define BG2_SUB_GFX_BASE 0
#define BG2_SUB_PALETTE_SUB 0

void configBG2_Sub(int map_base);
void configBG0_Sub(int map_base, int tile_base);

void configureGraphics_sub()
{
	REG_DISPCNT_SUB = MODE_5_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG2_ACTIVE;
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;

	configBG2_Sub(BG2_SUB_BMP_BASE);
	configBG0_Sub(BG0_SUB_MAP_BASE, BG0_SUB_TILE_BASE);
}


void configBG2_Sub(int map_base)
{
	BGCTRL_SUB[2] = BG_BMP_BASE(map_base) | BG_BMP8_256x256;

	dmaCopy(backgroundBitmap, BG_GFX_SUB, backgroundBitmapLen);
	dmaCopy(backgroundPal, BG_PALETTE_SUB, backgroundPalLen);

	REG_BG2PA_SUB = 256;
	REG_BG2PB_SUB = 0;
	REG_BG2PC_SUB = 0;
	REG_BG2PD_SUB = 256;
}


void configBG0_Sub(int map_base, int tile_base)
{
	BGCTRL_SUB[0] = BG_COLOR_256 | BG_MAP_BASE(map_base) | BG_TILE_BASE(tile_base);

	dmaCopy(empty_board_tile, BG_TILE_RAM_SUB(tile_base), 64);		//0
	dmaCopy(board_heart_1, &BG_TILE_RAM_SUB(tile_base)[32], 64);	//1
	dmaCopy(board_heart_2, &BG_TILE_RAM_SUB(tile_base)[64], 64);	//2
	dmaCopy(board_heart_3, &BG_TILE_RAM_SUB(tile_base)[96], 64);	//3
	dmaCopy(board_heart_4, &BG_TILE_RAM_SUB(tile_base)[128], 64);	//4
	dmaCopy(board_heart_5, &BG_TILE_RAM_SUB(tile_base)[160], 64);	//5
	dmaCopy(board_heart_6, &BG_TILE_RAM_SUB(tile_base)[192], 64);	//6
	dmaCopy(board_heart_7, &BG_TILE_RAM_SUB(tile_base)[224], 64);	//7
	dmaCopy(board_heart_8, &BG_TILE_RAM_SUB(tile_base)[256], 64);	//8
	dmaCopy(board_bomb_2, &BG_TILE_RAM_SUB(tile_base)[288], 64);	//9
	dmaCopy(board_bomb_3, &BG_TILE_RAM_SUB(tile_base)[320], 64);	//10
	dmaCopy(board_bomb_4, &BG_TILE_RAM_SUB(tile_base)[352], 64);	//11
	dmaCopy(board_bomb_5, &BG_TILE_RAM_SUB(tile_base)[384], 64);	//12
	dmaCopy(board_bomb_6, &BG_TILE_RAM_SUB(tile_base)[416], 64);	//13
	dmaCopy(board_bomb_7, &BG_TILE_RAM_SUB(tile_base)[448], 64);	//14
	dmaCopy(board_bomb_8, &BG_TILE_RAM_SUB(tile_base)[480], 64);	//15
	dmaCopy(board_arrow_2, &BG_TILE_RAM_SUB(tile_base)[512], 64);	//16
	dmaCopy(board_arrow_4, &BG_TILE_RAM_SUB(tile_base)[544], 64);	//17
	dmaCopy(board_arrow_6, &BG_TILE_RAM_SUB(tile_base)[576], 64);	//18

	BG_PALETTE_SUB[251] = RGB15(31, 31, 31);
	BG_PALETTE_SUB[252] = RGB15(31, 0, 0);
	BG_PALETTE_SUB[253] = RGB15(31, 31, 0);
	BG_PALETTE_SUB[254] = RGB15(0, 0, 0);
	BG_PALETTE_SUB[255] = RGB15(0, 0, 31);

	int index;
	for(index=0; index<SCREEN_TILE_NUMBER; index++)
		BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[index] = 0;
}

void updateGraphics_sub(u8* board)
{
	int row, column, type, rel_id;
	for(row = 0; row < 24; row++)
		for(column = 0; column < 32; column++)
		{
			type = board[(row/BOARD_TILE_SIDE)*BOARD_TILE_SIZE_X + column/BOARD_TILE_SIDE];
			if(type == LIFE)
			{
				rel_id = BOARD_TILE_SIDE*(row % BOARD_TILE_SIDE) + column % BOARD_TILE_SIDE;
				switch(rel_id)
				{
					case 0:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 1; break;
					case 1:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 2; break;
					case 2:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1 << 10) + 2; break;
					case 3:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1 << 10) + 1; break;
					case 4:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 3; break;
					case 5:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 4; break;
					case 6:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1 << 10) + 4; break;
					case 7:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1 << 10) + 3; break;
					case 8:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 5; break;
					case 9:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 6; break;
					case 10: BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1 << 10) + 6; break;
					case 11: BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1 << 10) + 5; break;
					case 12: BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 7; break;
					case 13: BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 8; break;
					case 14: BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1 << 10) + 8; break;
					case 15: BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1 << 10) + 7; break;
				}
			}
			else if(type == STORAGE_UPGRADE)
			{
				rel_id = BOARD_TILE_SIDE*(row % BOARD_TILE_SIDE) + column % BOARD_TILE_SIDE;
				switch(rel_id)
				{
					case 0:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 0; break;
					case 1:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 9; break;
					case 2:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1<<10) + 9; break;
					case 3:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 0; break;
					case 4:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 10; break;
					case 5:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 11; break;
					case 6:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1 << 10) + 11; break;
					case 7:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1 << 10) + 10; break;
					case 8:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 12; break;
					case 9:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 13; break;
					case 10: BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1 << 10) + 13; break;
					case 11: BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1 << 10) + 12; break;
					case 12: BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 14; break;
					case 13: BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 15; break;
					case 14: BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1 << 10) + 15; break;
					case 15: BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1 << 10) + 14; break;
				}
			}
			else if(type == RANGE_UPGRADE)
			{
				rel_id = BOARD_TILE_SIDE*(row % BOARD_TILE_SIDE) + column % BOARD_TILE_SIDE;
				switch(rel_id)
				{
					case 0:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 0; break;
					case 1:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 16; break;
					case 2:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1<<10) + 16; break;
					case 3:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 0; break;
					case 4:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 16; break;
					case 5:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 17; break;
					case 6:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1 << 10) + 17; break;
					case 7:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1 << 10) + 16; break;
					case 8:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 0; break;
					case 9:  BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 18; break;
					case 10: BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1 << 10) + 18; break;
					case 11: BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 0; break;
					case 12: BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 0; break;
					case 13: BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 18; break;
					case 14: BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = (1 << 10) + 18; break;
					case 15: BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 0; break;
				}
			}
			else if(type ==0)
				BG_MAP_RAM_SUB(BG0_SUB_MAP_BASE)[row*32 + column] = 0;
		}
}
