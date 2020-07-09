#include "winsgl.h"

int color = 32;

void sgSetup() {
	initWindow(640, 480, "Delay", BIT_MAP);
}
void sgLoop() {
	delay(1000);
	setColor(color * random(8), color * random(8), color * random(8));
	clearScreen();
}