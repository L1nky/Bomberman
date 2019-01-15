#include "sounds.h"
#include "definitions.h"
#include <nds.h>

#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"

#define NUMBER_EFFECTS 5
void duke_nukem() {
	u8 r = rand()%NUMBER_EFFECTS;
	switch(r){
	case 0: mmEffect(SFX_COME_GET_SOME_X); break;
	case 1: mmEffect(SFX_GOTTA_HURT); break;
	case 2: mmEffect(SFX_YOU_SUCK); break;
	case 3: mmEffect(SFX_HURTS_2BU); break;
	case 4: mmEffect(SFX_BLOW_IT_X); break;
	default: break;
	}
}

void duke_terminated(){
	mmEffect(SFX_TERMINATED);
}
