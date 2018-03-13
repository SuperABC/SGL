#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "SGL sample", BIT_MAP);
}
void sgLoop() {
	setColor(0, 0, 0);
	putQuad(80, 80, 160, 160, SOLID_FILL);
	putQuad(240, 80, 320, 160, EMPTY_FILL);
	putCircle(120, 280, 40, EMPTY_FILL);
	putEllipse(280, 280, 40, 80, SOLID_FILL);
	putLine(80, 400, 320, 400, SOLID_LINE);

	setColor(255, 255, 0);
	putTriangle(400, 80, 480, 80, 440, 400, SOLID_FILL);
}
