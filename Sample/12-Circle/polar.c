#include "winsgl.h"

void sgSetup() {
	initPolarWindow(320, 320, 320);
	initMouse(SG_COORDINATE);
	enablePanel();
	addPanelItem("first", vectDefault, 0, 0);
	addPanelItem("second", vectDefault, 0, 0);
	addPanelItem("third", vectDefault, 0, 0);
	addPanelItem("forth", vectDefault, 0, 0);
}
void sgLoop() {
	setColor(0, 0, 0);
	clearScreen();
}