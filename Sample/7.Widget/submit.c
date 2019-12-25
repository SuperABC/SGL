#include "winsgl.h"

void callback() {
	showWidget("dialog");
}
void sgSetup() {
	initWindow(640, 480, "Submit", BIT_MAP);

	easyWidget(SG_INPUT, "input", 100, 100, 200, 40, "1234", NULL);
	easyWidget(SG_BUTTON, "button", 320, 100, 120, 40, "х╥хо", callback);
	easyWidget(SG_DIALOG, "dialog", 100, 100, 400, 100, "dialog", NULL);
}
void sgLoop() {
	
}