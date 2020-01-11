#include "winsgl.h"

void callback(widget *obj) {
	showWidget("dialog");
}
void sgSetup() {
	initWindow(640, 480, "Submit", BIT_MAP);

	easyWidget(SG_INPUT, "input", 100, 100, 200, 40, "ÄãºÃ", NULL);
	easyWidget(SG_BUTTON, "button", 320, 100, 120, 40, "È·ÈÏ", callback);
	easyWidget(SG_DIALOG, "dialog", 100, 100, 400, 100, "dialog", NULL);
}
void sgLoop() {

}