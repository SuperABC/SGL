#include "sgl.h"

void sgSetup() {
	initWindow(40, 16, "SGL Sample", TEXT_MAP);
}
void sgLoop() {
	static int first = 1;
	if (first) {
		first = 0;
		setBfc(BLACK, WHITE);
		clearText();
		writeString("BLACK", 0, 0);
		writeString("BLUE", 0, 1);
		writeString("GREEN", 0, 2);
		writeString("CYAN", 0, 3);
		writeString("RED", 0, 4);
		writeString("MAGENTA", 0, 5);
		writeString("BROWN", 0, 6);
		writeString("DARKGRAY", 0, 7);
		writeString("LIGHTGRAY", 0, 8);
		writeString("LIGHTBLUE", 0, 9);
		writeString("LIGHTGREEN", 0, 10);
		writeString("LIGHTCYAN", 0, 11);
		writeString("LIGHTRED", 0, 12);
		writeString("LIGHTMAGENTA", 0, 13);
		writeString("YELLOW", 0, 14);
		writeString("WHITE", 0, 15);
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 20; j++) {
				setCharFgc(i, j, i);
				setCharBgc(i, j + 20, i);
			}
		}
		for (int i = 0; i < 16; i++) {
			setCharColor(WHITE << 4 | BLACK, 0, i);
		}
	}
}