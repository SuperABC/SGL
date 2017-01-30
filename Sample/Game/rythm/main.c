#include "main.h"
extern struct _win *Window;

int pro;
struct set Set;
struct archive Archive;
struct status Status[8];

void sgSetup() {
	Window->winWidth = 640;
	Window->winHeight = 640;
	strcpy(Window->winName, "Rythm Master --SGL Sample Game");
	initKey();
	initMouse(SG_COORDINATE);
	pro = START;
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
	case SETTING:
		setting();
		break;
	case CHOOSE:
		choose();
		break;
	case PLAYING:
		playing();
		break;
	case SCORE:
		score();
		break;
	default:
		break;
	}
	clearKeyBuffer();
	clearMouseBuffer();
}