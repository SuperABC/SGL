#include "sgl.h"

void sgSetup() {
	initWindow(100, 32, "text mode", TEXT_MAP);
}
void sgLoop() {
	static int x = 0, y = 0;
	setBfc(LIGHTGREEN, LIGHTRED);
	writeChar('a', x, y);
	delay(50);
	setBfc(WHITE, BLACK);
	writeChar(' ', x, y);
	if (++x == 100) {
		y++;
		x = 0;
	}
}