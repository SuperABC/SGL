#include "screen.h"
#define DRAG
void sgSetup() {
	initWindow(640, 480, "SGL sample");
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
