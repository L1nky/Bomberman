#include "sprites.h"
#include "sprites_tiles.h"
#include "definitions.h"
#include <nds.h>
#include "pause.h"

u16* gfx;
u16* gfx2;
u16* gfx3;

void configureSprites()
{
	VRAM_B_CR = VRAM_ENABLE | VRAM_B_MAIN_SPRITE_0x06400000;
	oamInit(&oamMain, SpriteMapping_1D_32, false);
	gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
	gfx2 = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
	gfx3 = oamAllocateGfx(&oamMain, SpriteSize_64x32, SpriteColorFormat_256Color);

	SPRITE_PALETTE[0 + SPRITE_OFFSET] = 0;
	SPRITE_PALETTE[1 + SPRITE_OFFSET] = RGB15(31, 31, 31);
	SPRITE_PALETTE[2 + SPRITE_OFFSET] = RGB15(0, 0, 0);
	SPRITE_PALETTE[3 + SPRITE_OFFSET] = RGB15(0, 31, 0);
	SPRITE_PALETTE[4 + SPRITE_OFFSET] = RGB15(0, 0, 31);

	//sprite tiles 0 to 3
	dmaCopy(player_1_tile_0, gfx, 64);
	dmaCopy(player_1_tile_1, &gfx[32], 64);
	dmaCopy(player_1_tile_2, &gfx[64], 64);
	dmaCopy(player_1_tile_3, &gfx[96], 64);

	//sprite tiles 4 to 7
	dmaCopy(player_2_tile_0, gfx2, 64);
	dmaCopy(player_2_tile_1, &gfx2[32], 64);
	dmaCopy(player_2_tile_2, &gfx2[64], 64);
	dmaCopy(player_2_tile_3, &gfx2[96], 64);

	dmaCopy(pausePal, SPRITE_PALETTE, pausePalLen);
	dmaCopy(pauseTiles, gfx3, pauseTilesLen);
}

void updateSprites(player player1, player player2)
{
	oamSet(&oamMain, 0, player1.x, player1.y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, gfx, -1, false, false, false, false, false);
	oamSet(&oamMain, 1, player2.x, player2.y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, gfx2, -1, false, false, false, false, false);
	oamSet(&oamMain, 2, 96, 64, 0, 0, SpriteSize_64x32, SpriteColorFormat_256Color, gfx3, -1, false, false, false, false, false);
	oamUpdate(&oamMain);
}
