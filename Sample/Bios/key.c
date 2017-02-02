#include "screen.h"
void sgSetup() {
	initWindow(640, 480, "SGL sample");
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
		case SG_UP | 0x8000:
		case 'w' | 0x8000:
			if (y >= 20)y -= 20;
			break;
		case SG_DOWN | 0x8000:
		case 's' | 0x8000:
			if (y <= 440)y += 20;
			break;
		case SG_LEFT | 0x8000:
		case 'a' | 0x8000:
			if (x >= 20)x -= 20;
			break;
		case SG_RIGHT | 0x8000:
		case 'd' | 0x8000:
			if (x <= 600)x += 20;
			break;
		}
		setColor(0, 0, 0);
		putQuad(x, y, x + 19, y + 19, SOLID_FILL);
	}
	clearKeyBuffer();
}
