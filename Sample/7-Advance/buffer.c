#include "winsgl.h"
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
	static char color = 0x00;
	static int i = 0;

	setColor(color, color, color);
	if(i < 480) {
		putQuad(0, i, 639, i+3, SOLID_FILL);
		delay(1);
		i += 4;
	}
	else {
		i = 0;
		color = ~color;
#ifdef DB
		page = !page;
		setActivePage(!page);
		setVisualPage(page);
#endif
	}

}