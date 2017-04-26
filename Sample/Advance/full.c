#include "winsgl.h"
#define WIN_WIDTH 640
#define WIN_HEIGHT 480

void sgSetup() {
	initWindow(WIN_WIDTH, WIN_HEIGHT, "SGL Sample", BIT_MAP);
	initMouse(SG_COORDINATE);
}
void sgLoop() {
	vecTwo tmp = mousePos();
	if (tmp.x > WIN_WIDTH - 4 && tmp.x < WIN_WIDTH) {
		setMouse(3 * getWidth(SG_WINDOW) / getWidth(SG_SCREEN),
			tmp.y * getHeight(SG_WINDOW) / getHeight(SG_SCREEN));
	}
	if (tmp.x < 3 && tmp.x >= 0) {
		setMouse((WIN_WIDTH - 4) * getWidth(SG_WINDOW) / getWidth(SG_SCREEN),
			tmp.y * getHeight(SG_WINDOW) / getHeight(SG_SCREEN));
	}
	if (tmp.y > WIN_HEIGHT - 4 && tmp.y < WIN_HEIGHT) {
		setMouse(tmp.x * getWidth(SG_WINDOW) / getWidth(SG_SCREEN),
			3 * getHeight(SG_WINDOW) / getHeight(SG_SCREEN));
	}
	if (tmp.y < 3 && tmp.y >= 0) {
		setMouse(tmp.x * getWidth(SG_WINDOW) / getWidth(SG_SCREEN),
			(WIN_HEIGHT - 4) * getHeight(SG_WINDOW) / getHeight(SG_SCREEN));
	}
	if (biosMouse(1).m)exit(0);
}

