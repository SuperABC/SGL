#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "Clipboard", BIT_MAP);
}
void sgLoop() {
	static int first = 1;
	if (first) {
		first = 0;
		copyText("hello");
		setColor(255, 255, 255);
		clearScreen();
		setColor(0, 0, 0);
		putString(pasteText(), 0, 0);
	}
}