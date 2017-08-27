#include "winsgl.h"

void sgSetup() {
	initWindow(680, 520, "SGL sample", BIT_MAP);
}
void sgLoop() {
	static int first = 0;
	bitMap *buf = (bitMap*)malloc(sizeof(bitMap));

	if (!first) {
		loadBmp(0, 0, "Source/tower/tower.bmp");
		getImage(0, 0, 39, 39, buf);
		putImage(320, 200, buf, COPY_PUT);

		setColor(127, 127, 127);
		setFontSize(128);
		putString("Success!", 10, 10);
		first = 1;
	}
}
