#include "winsgl.h"

int sub;

void sgSubSetup() {

}
void sgSubLoop() {
	static int first = 1000;
	if (first == 0)closeWindow(sub);
	else {
		setCurrentWindow(sub);
		setColor(0, 0, 0);
		clearScreen();
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
	setCurrentWindow(-1);
	setColor(255, 255, 255);
	clearScreen();
}