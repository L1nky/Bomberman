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

void updateGraphics(u8* world, player player1, player player2, u8* board)
{
	updateGraphics_main(world);
	updateSprites(player1, player2);
	updateGraphics_sub(board);
}
