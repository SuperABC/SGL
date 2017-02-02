#include "main.h"
knight *player;
tower *wall[81];
opponent *mons[41];
int pro, obj, maxFloor = 0;
void sgSetup() {
	initWindow(680, 520, "Magic tower --SGL Sample Game");
	loadWave("Source\\tower\\bgm.wav", SG_LOOP);
	initKey();
	initMouse(SG_COORDINATE);
	pro = START;
	maxFloor = 0;
	initPro();
}
void sgLoop() {
	switch (pro) {
	case START:
		start();
		break;
	case INSTRUCT:
		instruct();
		break;
	case ARCHIVE:
		archive();
		break;
	case FIGURE:
		figure();
		break;
	case DIFFICULTY:
		difficulty();
		break;
	case NAME:
		name();
		break;
	case PLAYING:
		playing();
		break;
	case FIGHT:
		fight();
		break;
	case PAUSE:
		pause();
		break;
	case BUY:
		buy();
		break;
	case DIE:
		die();
		break;
	case SAVE:
		save();
		break;
	case DICTION:
		diction();
		break;
	default:
		break;
	}
	clearKeyBuffer();
	clearMouseBuffer();
}
