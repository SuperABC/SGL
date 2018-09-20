#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "SGL sample", BIT_MAP);
}
void sgLoop() {
	static int x, y;
	RGB p;

	x = random(640);
	y = random(480);

	p = getPixel(x, y);
	setColor(0, 0, 0);
	if(p.r==255&&p.g==255&&p.b==255)putPixel(x, y);
}
