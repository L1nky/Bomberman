#ifndef SPRITES_H_
#define SPRITES_H_

#include "definitions.h"
#include <nds.h>

void configureSprites();

void updateSprites(player player1, player player2, u8 isPaused, u8 inMainMenu);

#endif /* SPRITES_H_ */
