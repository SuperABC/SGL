#include "screen.h"
#pragma comment(lib, SG_Lib("sgl"))
extern struct _win *Window;
void sgSetup() {
	Window->winWidth = 640;
	Window->winHeight = 480;
	strcpy(Window->winName, "SGL Sample");
}
void sgLoop() {
	static int x, y;
	RGB p;
	x = random(640);
	y = random(480);
	p = getPixel(x, y);
	setColor(0, 0, 0);
	if(p.r==255&&p.g==255&&p.b==255)putPixel(x, y);
}
