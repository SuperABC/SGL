#include "screen.h"
#pragma comment(lib, SG_Lib("sgl"))
extern struct _win *Window;
void sgSetup() {
	Window->winWidth = 680;
	Window->winHeight = 520;
	strcpy(Window->winName, "SG test");
	
	initKey();
	initMouse(SG_COORDINATE);
}
void sgLoop() {
	return; 
}
