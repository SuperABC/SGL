#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);
	initKey();
}
void sgLoop() {
	int key;
	bitMap *oldImg = (bitMap *)malloc(sizeof(bitMap));

	if (biosKey(1)) {
		key = biosKey(0);

		getImage(0, 20, 639, 479, oldImg);
		putImage(0, 0, oldImg, COPY_PUT);
		free(oldImg->data);

		setColor(255, 255, 255);
		putQuad(0, 460, 639, 479, SOLID_FILL);

		setColor(0, 0, 0);
		setFontSize(20);
		if (key & 0x8000)putString("Up", 0, 458);
		else putString("Down", 0, 458);
		putNumber(key & 0x7FFF, 80, 462, 'l');
		if((key & 0x7FFF) < 256)putChar(key & 0x7FFF, 160, 462);
	}
}