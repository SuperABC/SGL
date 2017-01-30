#include "screen.h"
#pragma comment(lib, SG_Lib("sgl"))
#define DRAG
extern struct _win *Window;
void sgSetup() {
	Window->winWidth = 640;
	Window->winHeight = 480;
	strcpy(Window->winName, "SGL Sample");
	initMouse(SG_COORDINATE);
}
void sgLoop() {
#ifdef CLICK
	vecThree v;
	setColor(0, 0, 0);
	if (biosMouse(1).m) {
		v = biosMouse(0);
		if (v.m == SG_BUTTON_DOWN)putCircle(v.x, v.y, 10, SOLID_FILL);
	}
	clearMouseBuffer();
#endif
#ifdef DRAG
	vecTwo v;
	setColor(0, 0, 0);
	if (mouseStatus(SG_LEFT_BUTTON)) {
		v = mousePos();
		putCircle(v.x, v.y, 10, SOLID_FILL);
	}
#endif
}
