#include "graphics_main.h"
#include "tiles.h"
#include "definitions.h"
#include <nds.h>

#define BG0_MAIN_MAP_BASE
#define BG1_MAIN_MAP_BASE 1
#define BG1_MAIN_TILE_BASE 0
#define BG2_MAIN_MAP_BASE 2
#define BG2_MAIN_TILE_BASE 1
#define BG3_MAIN_MAP_BASE 3
#define BG3_MAIN_TILE_BASE 2

void configBG0_main(int map_base, int tile_base);
void configBG1_main(int map_base, int tile_base);
void configBG2_main(int map_base, int tile_base);
void configBG3_main(int map_base, int tile_base);

void updateBG1_main(u8* world);
void updateBG2_main(u8* world);

void configureGraphics_main()
{
	REG_DISPCNT = MODE_0_2D | DISPLAY_BG1_ACTIVE | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE;
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;

	configBG1_main(BG1_MAIN_MAP_BASE, BG1_MAIN_TILE_BASE);
	configBG2_main(BG2_MAIN_MAP_BASE, BG2_MAIN_TILE_BASE);
	configBG3_main(BG3_MAIN_MAP_BASE, BG3_MAIN_TILE_BASE);
}

void configBG1_main(int map_base, int tile_base)
{
	BGCTRL[1] = BG_COLOR_256 | BG_MAP_BASE(map_base) | BG_TILE_BASE(tile_base) | BG_32x32;

	dmaCopy(empty_tile, BG_TILE_RAM(tile_base), 64);
	dmaCopy(bomb_tile_corner, &BG_TILE_RAM(tile_base)[32], 64);
	dmaCopy(bomb_tile_edge_top, &BG_TILE_RAM(tile_base)[64], 64);
	dmaCopy(bomb_tile_edge_bottom, &BG_TILE_RAM(tile_base)[96], 64);
	dmaCopy(bomb_tile_edge_vert, &BG_TILE_RAM(tile_base)[128], 64);
	dmaCopy(bomb_tile_center, &BG_TILE_RAM(tile_base)[160], 64);
	dmaCopy(storage_upgrade_tile, &BG_TILE_RAM(tile_base)[192], 64);
	dmaCopy(range_upgrade_tile, &BG_TILE_RAM(tile_base)[224], 64);
	dmaCopy(vert_explosion_tile_edge, &BG_TILE_RAM(tile_base)[256], 64);
	dmaCopy(vert_explosion_tile_center, &BG_TILE_RAM(tile_base)[288], 64);
	dmaCopy(hor_explosion_tile_edge, &BG_TILE_RAM(tile_base)[320], 64);
	dmaCopy(hor_explosion_tile_center, &BG_TILE_RAM(tile_base)[352], 64);
	dmaCopy(cen_explosion_corner, &BG_TILE_RAM(tile_base)[384], 64);
	dmaCopy(cen_explosion_hor_edge, &BG_TILE_RAM(tile_base)[416], 64);
	dmaCopy(cen_explosion_ver_edge, &BG_TILE_RAM(tile_base)[448], 64);
	dmaCopy(cen_explosion_center, &BG_TILE_RAM(tile_base)[480], 64);

	BG_PALETTE[0] = 0;
	BG_PALETTE[20] = RGB15(0, 0, 0);
	BG_PALETTE[21] = RGB15(31, 31, 0);
	BG_PALETTE[30] = RGB15(0, 0, 31);
	BG_PALETTE[40] = RGB15(31, 0, 0);
	BG_PALETTE[50] = RGB15(31, 0, 0);
	BG_PALETTE[51] = RGB15(31, 5, 0);
	BG_PALETTE[52] = RGB15(31, 10, 0);
	BG_PALETTE[53] = RGB15(31, 15, 0);
	BG_PALETTE[54] = RGB15(31, 20, 0);

	int row, column;
	for(row = 0; row < 24; row++)
		for(column = 0; column < 32; column++)
			BG_MAP_RAM(map_base)[row*32 + column] = 0;
}

void configBG2_main(int map_base, int tile_base)
{
	BGCTRL[2] = BG_COLOR_256 | BG_MAP_BASE(map_base) | BG_TILE_BASE(tile_base) | BG_32x32;

	dmaCopy(empty_tile, BG_TILE_RAM(tile_base), 64);
	dmaCopy(crate_tile, &BG_TILE_RAM(tile_base)[32], 64);

	BG_PALETTE[0] = 0;
	BG_PALETTE[10] = RGB15(31, 25, 0);
	BG_PALETTE[11] = RGB15(19, 13, 0);

	int row, column;
	for(row = 0; row < 24; row++)
		for(column = 0; column < 32; column++)
			BG_MAP_RAM(map_base)[row*32 + column] = 0;
}

void configBG3_main(int map_base, int tile_base)
{
	BGCTRL[3] = BG_COLOR_256 | BG_MAP_BASE(map_base) | BG_TILE_BASE(tile_base) | BG_32x32;

	dmaCopy(ground_tile, BG_TILE_RAM(tile_base), 64);
	dmaCopy(wall_tile_corner, &BG_TILE_RAM(tile_base)[32], 64);
	dmaCopy(wall_tile_edge_hor, &BG_TILE_RAM(tile_base)[64], 64);
	dmaCopy(wall_tile_edge_vert, &BG_TILE_RAM(tile_base)[96], 64);
	dmaCopy(wall_tile_center, &BG_TILE_RAM(tile_base)[128], 64);
	dmaCopy(outside_tile, &BG_TILE_RAM(tile_base)[160], 64);

	BG_PALETTE[1] = RGB15(20, 20, 20);
	BG_PALETTE[2] = RGB15(17, 17, 17);
	BG_PALETTE[3] = RGB15(14, 14, 14);

	int row, column, rel_id;
	for(row = 0; row < 24; row++)
		for(column = 0; column < 32; column++)
		{
			if(row > 20 || column > 26)
				BG_MAP_RAM(map_base)[row*32 + column] = 5;
			else if((row/3) % 2 == 0 || (column/3) % 2 == 0)
				BG_MAP_RAM(map_base)[row*32 + column] = 0;
			else
			{
				rel_id = 3*(row % 3) + column % 3;
				switch(rel_id)
				{
					case 0: BG_MAP_RAM(map_base)[row*32 + column] = 1; break;
					case 1: BG_MAP_RAM(map_base)[row*32 + column] = 2; break;
					case 2: BG_MAP_RAM(map_base)[row*32 + column] = (1 << 10) + 1; break;
					case 3: BG_MAP_RAM(map_base)[row*32 + column] = 3; break;
					case 4: BG_MAP_RAM(map_base)[row*32 + column] = 4; break;
					case 5: BG_MAP_RAM(map_base)[row*32 + column] = (1 << 10) + 3; break;
					case 6: BG_MAP_RAM(map_base)[row*32 + column] = (1 << 11) + 1; break;
					case 7: BG_MAP_RAM(map_base)[row*32 + column] = (1 << 11) + 2; break;
					case 8: BG_MAP_RAM(map_base)[row*32 + column] = (1 << 10) + (1 << 11) + 1;
				}
			}
		}
}

void configBG0(int map_base, int tile_base)
{

}

void updateGraphics_main(u8* world)
{
	updateBG1_main(world);
	updateBG2_main(world);
}

void updateBG1_main(u8* world)
{
	int row, column, type, rel_id;
	for(row = 0; row < 21; row++)
		for(column = 0; column < 27; column++)
		{
			type = world[(row/3)*GAME_TILE_SIZE_X + column/3];
			if(type == BOMB)
			{
				rel_id = 3*(row % 3) + column % 3;
				switch(rel_id)
				{
					case 0: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = 1; break;
					case 1: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = 2; break;
					case 2: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = (1 << 10) + 1; break;
					case 3: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = 4; break;
					case 4: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = 5; break;
					case 5: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = (1 << 10) + 4; break;
					case 6: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = (1 << 11) + 1; break;
					case 7: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = 3; break;
					case 8: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = (1 << 10) + (1 << 11) + 1;
				}
			}
			else if(type == STORAGE_UPGRADE)
			{
				BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = 6;
			}
			else if(type == RANGE_UPGRADE)
			{
				BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = 7;
			}
			else if(VERT_EXPLOSION_START <= type && type <= VERT_EXPLOSION_END)
			{
				rel_id = column % 3;
				switch(rel_id)
				{
					case 0: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = 8; break;
					case 1: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = 9; break;
					case 2: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = (1 << 10) + 8;
				}
			}
			else if(HOR_EXPLOSION_START <= type && type <= HOR_EXPLOSION_END)
			{
				rel_id = row % 3;
				switch(rel_id)
				{
					case 0: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = 10; break;
					case 1: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = 11; break;
					case 2: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = (1 << 11) + 10;
				}
			}
			else if(CEN_EXPLOSION_START <=type && type <= CEN_EXPLOSION_END){
				rel_id = 3*(row % 3) + column % 3;
				switch(rel_id){
				case 0: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = 12; break;
				case 1: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = 13; break;
				case 2: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = (1 << 10) + 12; break;
				case 3: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = 14; break;
				case 4: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = 15; break;
				case 5: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = (1 << 10) + 14; break;
				case 6: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = (1 << 11) + 12; break;
				case 7: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = (1 << 11) + 13; break;
				case 8: BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = (1 << 10) + (1 << 11) + 12; break;
				}
			}
			else
				BG_MAP_RAM(BG1_MAIN_MAP_BASE)[row*32 + column] = 0;
		}
}

void updateBG2_main(u8* world)
{
	int row, column;
	for(row = 0; row < 21; row++)
		for(column = 0; column < 27; column++)
			BG_MAP_RAM(BG2_MAIN_MAP_BASE)[row*32 + column] = world[(row/3)*9 + (column/3)] == CRATE;
}
