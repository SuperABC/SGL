#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "Init", BIT_MAP);
	enablePanel();
	addPanelItem("1", NULL, 0, 0);
	addPanelItem("2", NULL, 0, 0);
	addPanelItem("3", NULL, 0, 0);
	addPanelItem("4", NULL, 0, 0);
}
void sgLoop() {
	setColor(0, 0, 0);
	clearScreen();
}