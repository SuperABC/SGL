#include "screen.h"
#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
void sgSetup() {
	initWindow(WIN_WIDTH, WIN_HEIGHT, "SGL Sample");
	initMouse(SG_COORDINATE);
	fullScreen();
}
void sgLoop() {
	vecTwo tmp = mousePos();
	if (tmp.x > WIN_WIDTH - 3) {
		setMouse(2, tmp.y);
		tmp.x = 2;
	}
	if (tmp.x < 2) {
		setMouse(WIN_WIDTH - 3, tmp.y);
		tmp.x = WIN_WIDTH - 3;
	}
	if (tmp.y > WIN_HEIGHT - 3) {
		setMouse(tmp.x, 2);
		tmp.y = 2;
	}
	if (tmp.y < 2) {
		setMouse(tmp.x, WIN_HEIGHT - 3);
		tmp.y = WIN_HEIGHT - 3;
	}
	if (biosMouse(1).m)exit(0);
}