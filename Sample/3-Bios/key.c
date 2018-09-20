#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "SGL sample", BIT_MAP);
	initKey();
}
void sgLoop() {
	static int x = 0, y = 0, first = 0;
	int key;

	setColor(0, 0, 0);
	if (!first) {
		putQuad(0, 0, 19, 19, SOLID_FILL);
		first = 1;
	}

	if (biosKey(1)) {
		key = biosKey(0);
		setColor(255, 255, 255);
		putQuad(x, y, x + 19, y + 19, SOLID_FILL);
		switch (key) {
		case SG_UP:
		case 'w':
			if (y >= 20)y -= 20;
			break;
		case SG_DOWN:
		case 's':
			if (y <= 440)y += 20;
			break;
		case SG_LEFT:
		case 'a':
			if (x >= 20)x -= 20;
			break;
		case SG_RIGHT:
		case 'd':
			if (x <= 600)x += 20;
			break;
		}

		setColor(0, 0, 0);
		putQuad(x, y, x + 19, y + 19, SOLID_FILL);
	}

	clearKeyBuffer();
}
