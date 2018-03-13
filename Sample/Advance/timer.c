#include "winsgl.h"

void func() {
	static int r = 0, g = 0, b = 0;
	setColor(r, g, b);
	clearScreen();
	r += 10;
	g += 10;
	b += 10;
}

void sgSetup() {
	initWindow(640, 480, "SGL sample", BIT_MAP);
}
void sgLoop() {
	static first = 1;
	if (first) {
		timerThread(func, 100, 25);
		first = 0;
	}
	return;
}