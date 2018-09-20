#include "winsgl.h"

const char *txt = "Hello, welcome to sgl text mode!\n"
							"\n"
							"Text mode is coded for those who want to build an environment like DOS. "
							"In this mode, programmers can't operate each pixel, but it's easy to write letters on the screen. "
							"Like this, just put down the string and it will be sent to the screen. Very convenient, isn't it?\n";

void sgSetup() {
	initWindow(40, 16, "SGL Sample", TEXT_MAP);
}
void sgLoop() {
	static int first = 1;
	static int charNum = 0;
	static int x = 0, y = 0;

	if (first) {
		first = 0;
		setBfc(BLUE, LIGHTCYAN);
		clearText();
		return;
	}

	if (txt[charNum]) {
		if (txt[charNum] == '\n') {
			x = 0;
			y++;
			charNum++;
			return;
		}
		else if (txt[charNum] >= 0x20 && txt[charNum] < 0x80)writeChar(txt[charNum], x, y);
		if (++x == 40) {
			x = 0;
			y++;
		}
		charNum++;
		delay(50);
	}
}