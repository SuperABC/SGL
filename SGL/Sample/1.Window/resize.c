#include "../../winsgl.h"

void resize(int x, int y) {
	char text[64];
	sprintf(text, "resize to %d * %d", x, y);
	alertInfo(text, "resize", 0);
}

void sgSetup() {
	initWindow(640, 480, "Resize", BIT_MAP);
	resizeFuntion(resize);
}
void sgLoop() {
	putQuad(270, 190, 370, 290, SOLID_FILL);
}