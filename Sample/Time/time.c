#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "SGL sample", BIT_MAP);
	initMouse(SG_COORDINATE);
}
void sgLoop() {
	static int x = 0, y = 0;

	setColor(255, 255, 255);
	putQuad(x, y, x + 19, y + 19, SOLID_FILL);

	x = random(621);
	y = random(461);
	setColor(0, 0, 0);
	putQuad(x, y, x + 19, y + 19, SOLID_FILL);

	delayEnd(200);
	delayBegin();
	//delay(200);
}
