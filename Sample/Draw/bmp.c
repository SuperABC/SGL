#include "winsgl.h"

void sgSetup() {
	initWindow(0, 0, "SGL sample", BIT_MAP);
}
void sgLoop() {
	static int first = 0;
	bitMap *buf = (bitMap*)malloc(sizeof(bitMap));

	if (!first) {
		loadBmp(0, 0, "Source/tower/tower.bmp");
		getImage(0, 0, 39, 39, buf);
		putImage(320, 200, buf, COPY_PUT);
		free(buf->data);
		free(buf);

		first = 1;
	}
}
