#include "winsgl.h"

int sub;

void sgSubSetup() {

}
void sgSubLoop() {
	static int first = 1000;
	if (first == 0)closeWindow(sub);
	else {
		debugf("lock3\n");
		startSubWindow(sub);
		setColor(0, 0, 0);
		clearScreen();
		setColor(255, 255, 255);
		putQuad(100, 100, 200, 200, SOLID_FILL);
		endSubWindow();
		debugf("unlock3\n");
	}

	first--;
}

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);
}
void sgLoop() {
	static int first = 1;
	if (first) {
		first = 0;
		sub = createWindow(320, 240, "Sub window", (vect)sgSubSetup, (vect)sgSubLoop);
	}
	setColor(255, 255, 255);
	clearScreen();
}