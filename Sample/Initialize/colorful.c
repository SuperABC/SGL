#include "screen.h"
extern struct _win *Window;
void sgSetup() {
	Window->winWidth = 640;
	Window->winHeight = 480;
	strcpy(Window->winName, "SGL Sample");
}
void sgLoop() {
	static int r = 0, g = 0, b = 0;
	b = (b + 8) % 256;
	if(!b)g = (g + 8) % 256;
	if(!g&&!b)r = (r + 8) % 256;
	setColor(r, g, b);
	clearScreen();
}
