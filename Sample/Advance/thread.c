#include "winsgl.h"

void func() {
	while (1) {
		setColor(0, 0, 0);
		clearScreen();
	}
}

void sgSetup() {
	initWindow(640, 480, "SGL sample", BIT_MAP);
}
void sgLoop() {
	static first = 1;
	if (first) {
		createThread(func);
		first = 0;
	}
	setColor(255, 255, 255);
	clearScreen();
	return;
}