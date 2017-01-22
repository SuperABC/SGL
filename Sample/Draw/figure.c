#include "screen.h"
extern struct _win *Window;
void sgSetup() {
	Window->winWidth = 640;
	Window->winHeight = 480;
	strcpy(Window->winName, "SGL Sample");
}
void sgLoop() {
	RGB p;
	p.r = p.g = p.b = 0;
	setColor(0, 0, 0);
	putQuad(80, 80, 160, 160, SOLID_FILL);
	putQuad(240, 80, 320, 160, EMPTY_FILL);
	putCircle(120, 280, 40, SOLID_FILL);
	putCircle(280, 280, 40, EMPTY_FILL);
	putLine(80, 400, 320, 400, 0);

	putLine(400, 80, 480, 80, 0);
	putLine(440, 400, 480, 80, 0);
	putLine(400, 80, 440, 400, 0);
	setColor(255, 255, 0);
	floodFill(440, 81, p);
}
