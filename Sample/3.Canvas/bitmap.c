#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "Bitmp", BIT_MAP);
}
void sgLoop() {
	static int first = 0;
	bitMap *buf = (bitMap*)malloc(sizeof(bitMap));

	if (!first) {
		putBitmap(0, 0, loadBmp("Source/pic.bmp"));
		putBitmap(100, 0, loadPng("Source/pic.png"));
		getImage(0, 0, 39, 39, buf);
		putImage(320, 200, buf, COPY_PUT);
		saveBmp("Result/out.bmp", *buf);
		savePng("Result/out.png", *buf);
		free(buf->data);
		free(buf);

		first = 1;
	}
}