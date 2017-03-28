#include "winsgl.h"

//Unfinished.

#define WIN_WIDTH 640
#define WIN_HEIGHT 480
void sgSetup() {
	initWindow(WIN_WIDTH, WIN_HEIGHT, "SGL Sample", BIT_MAP);
	initMouse(SG_COORDINATE);
	//fullScreen();
}
void sgLoop() {
	vecTwo tmp = mousePos();
	if (tmp.x > WIN_WIDTH - 4 && tmp.x < WIN_WIDTH) {
		setMouse(3, tmp.y);
		tmp.x = 3;
	}
	if (tmp.x < 3 && tmp.x >= 0) {
		setMouse(WIN_WIDTH - 4, tmp.y);
		tmp.x = WIN_WIDTH - 4;
	}
	if (tmp.y > WIN_HEIGHT - 4 && tmp.y < WIN_HEIGHT) {
		setMouse(tmp.x, 3);
		tmp.y = 3;
	}
	if (tmp.y < 3 && tmp.y >= 0) {
		setMouse(tmp.x, WIN_HEIGHT - 4);
		tmp.y = WIN_HEIGHT - 4;
	}
	if (biosMouse(1).m)exit(0);
}