#include "winsgl.h"
#define DRAG

void sgSetup() {
	initWindow(640, 480, "SGL sample", BIT_MAP);
	initMouse(SG_ACCURATE);
}
void sgLoop() {

#ifdef CLICK
	vecThree mouse;
	setColor(0, 0, 0);
	if (biosMouse(1).m) {
		mouse = biosMouse(0);
		if (mouse.m == SG_LEFT_BUTTON)putCircle(mouse.x, mouse.y, 10, SOLID_FILL);
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
