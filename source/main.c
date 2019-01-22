#include "graphics.h"
#include "game_logic.h"
#include "definitions.h"
#include <nds.h>
#include <stdio.h>
//SOUND INCLUDES
#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"
#include "graphics_main.h"

int main(void) {
	
    consoleDemoInit();
    //SOUND INIT
    mmInitDefaultMem((mm_addr)soundbank_bin);
    mmLoad(MOD_CLONE_WARPING);
    mmLoad(MOD_DUKE_NUKEM);
    mmLoadEffect(SFX_WOOSH);
    mmLoadEffect(SFX_BOOM);
    mmSetModuleVolume(250);
    mmLoadEffect(SFX_COME_GET_SOME_X);
    mmLoadEffect(SFX_GOTTA_HURT);
    mmLoadEffect(SFX_YOU_SUCK);
    mmLoadEffect(SFX_BLOW_IT_X);
    mmLoadEffect(SFX_HURTS_2BU);
    mmLoadEffect(SFX_TERMINATED);
    mmLoadEffect(SFX_MARIO_1_UP);
    mmLoadEffect(SFX_MARIO_TOUCH_ENNEMY);

    configureGraphics();
    loadMainMenu();

    scanKeys();

    while(1)
    {
    	update_game();
        swiWaitForVBlank();
    }
}
