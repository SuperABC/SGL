#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "SGL sample", BIT_MAP);
}
void sgLoop() {
	static int r = 0, g = 0, b = 0;

	b = (b + 8) % 256;
	if(!b)g = (g + 8) % 256;
	if(!g&&!b)r = (r + 8) % 256;
	setColor(r, g, b);
	clearScreen();
}
