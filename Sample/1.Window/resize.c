#include "winsgl.h"

void resize(int x, int y) {
	debugf("resize to %d * %d\n", x, y);
}

void sgSetup() {
	initWindow(640, 480, "Resize", BIT_MAP);
	resizeFuntion(resize);
}
void sgLoop() {
	putQuad(270, 190, 370, 290, SOLID_FILL);
}