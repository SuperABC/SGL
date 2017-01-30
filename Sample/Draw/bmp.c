#include "screen.h"
#pragma comment(lib, SG_Lib("sgl"))
extern struct _win *Window;
void sgSetup() {
	Window->winWidth = 680;
	Window->winHeight = 520;
	strcpy(Window->winName, "SGL Sample");
}
void sgLoop() {
	static int first = 0;
	bitMap *buf = (bitMap*)malloc(sizeof(bitMap));
	if (!first) {
		loadBmp(0, 0, "tower.bmp");
		getImage(0, 0, 39, 39, buf);
		putImage(320, 200, buf, 0);
		first = 1;
	}
}
