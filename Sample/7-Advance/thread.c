#include "winsgl.h"

int value = 0;

void func() {
	while (1) {
		value++;
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
	setColor(0, 0, 0);
	putNumber(value, 0, 0, 'l');
	return;
}