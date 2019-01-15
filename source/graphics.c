#include "graphics.h"
#include "graphics_main.h"
#include "graphics_sub.h"
#include "sprites.h"
#include "definitions.h"

void configureGraphics()
{
	REG_POWERCNT ^= BIT(15);
	configureGraphics_main();
	configureSprites();
	configureGraphics_sub();
}

void updateGraphics(u8* world, player player1, player player2, u8* board, u8 isPaused, u8 inMainMenu)
{
	updateGraphics_main(world);
	updateSprites(player1, player2, isPaused, inMainMenu);
	updateGraphics_sub(board);
}
