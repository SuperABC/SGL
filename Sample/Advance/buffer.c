#include "sgl.h"
//#define DB

int page = 0;

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);
#ifdef DB
	setActivePage(!page);
	setVisualPage(page);
#endif
}
void sgLoop() {
	static char color = 0xff;
	int i;
	color = ~color;
	setColor(color, color, color);
	for (i = 0; i < 480; i++) {
		putQuad(0, i, 639, i, SOLID_FILL);
		delay(1);
	}
#ifdef DB
	page = !page;
	setActivePage(!page);
	setVisualPage(page);
#endif
}