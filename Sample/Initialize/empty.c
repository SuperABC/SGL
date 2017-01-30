#include "screen.h"
#pragma comment(lib, SG_Lib("sgl"))
extern struct _win *Window;
void sgSetup() {
	Window->winWidth = 640;
	Window->winHeight = 480;
	strcpy(Window->winName, "SGL Sample");
}
void sgLoop() {
	return;
}
