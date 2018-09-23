#include "winsgl.h"

int sub;

void sgSubSetup() {

}
void sgSubLoop() {
	static int first = 1000;
	if (first == 0)closeWindow(sub);
	else {
		startSubWindow(sub);
		setColor(0, 0, 0);
		clearScreen();
		endSubWindow();
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
		//sub = createWindow(320, 240, "Sub window", (vect)sgSubSetup, (vect)sgSubLoop);
		easyWidget(SG_BUTTON, "button", 100, 100, 200, 200, "hello", vectDefault);
	}
	setColor(255, 255, 255);
	clearScreen();
}