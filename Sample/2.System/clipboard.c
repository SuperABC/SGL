#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "Clipboard", BIT_MAP);
	copyText("hello");
}
void sgLoop() {
	setColor(255, 255, 255);
	clearScreen();
	setColor(0, 0, 0);
	putString(pasteText(), 0, 0);
}