#include "winsgl.h"
#define CLICK

void sgSetup() {
	initWindow(640, 480, "Mouse", BIT_MAP);
}
void sgLoop() {

#ifdef CLICK
	vec3i mouse;
	setColor(0, 0, 0);
	if (biosMouse(1).z) {
		mouse = biosMouse(0);
		if (mouse.z == SG_LEFT_BUTTON)putCircle(mouse.x, mouse.y, 10, SOLID_FILL);
	}
	clearMouseBuffer();
#endif

#ifdef DRAG
	vec2i v;
	setColor(0, 0, 0);
	if (!mouseStatus(SG_LEFT_BUTTON)) {
		v = mousePos();
		putCircle(v.x, v.y, 10, SOLID_FILL);
	}
#endif

}
