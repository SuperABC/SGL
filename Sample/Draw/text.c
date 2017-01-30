#include "screen.h"
#pragma comment(lib, SG_Lib("sgl"))
extern struct _win *Window;
void sgSetup() {
	Window->winWidth = 640;
	Window->winHeight = 480;
	strcpy(Window->winName, "SGL Sample");
}
void sgLoop() {
	setColor(0, 0, 0);
	putString("Hello world!", 0, 0);
	putString("SGL is the best graphic library in the world!", 100, 100);
}
