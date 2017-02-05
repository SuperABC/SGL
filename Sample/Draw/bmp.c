#include "screen.h"
void sgSetup() {
	initWindow(680, 520, "SGL sample");
}
void sgLoop() {
	static int first = 0;
	bitMap *buf = (bitMap*)malloc(sizeof(bitMap));
	if (!first) {
		loadBmp(0, 0, "tower.bmp");
		getImage(0, 0, 39, 39, buf);
		putImage(320, 200, buf, COPY_PUT);
		first = 1;
	}
}
