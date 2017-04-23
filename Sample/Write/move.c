#include "winsgl.h"

void sgSetup() {
	initWindow(80, 25, "SGL Sample", TEXT_MAP);
}
void sgLoop() {
	static int first = 1;
	static int x = 0, y = 0;
	static textMap *txt;

	if (first) {
		first = 0;
		txt = (textMap*)malloc(sizeof(textMap));
		setBfc(BLUE, LIGHTCYAN);
		clearText();
	}
	else {
		putText(x, y, txt);
		free(txt->content);
	}

	x = random(69);
	y = random(25);
	getText(x, y, x + 11, y, txt);
	writeString("Hello world!", x, y);
	delay(400);
	
}