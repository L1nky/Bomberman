#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "definitions.h"
#include <nds.h>

void configureGraphics();

void updateGraphics(u8* world, player player1, player player2, u8* board);

#endif /* GRAPHICS_H_ */
