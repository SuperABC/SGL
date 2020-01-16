#include "winsgl.h"

void callback(widget *obj) {
	showWidget("dialog");
}
void sgSetup() {
	initWindow(640, 480, "Submit", BIT_MAP);

	registerWidget(easyCombinedWidget("combined", 100, 100, 400, 100, 3,
		easyWidget(SG_INPUT, "input", 0, 0, 200, 40, "ÄãºÃ", NULL),
		easyWidget(SG_BUTTON, "button", 220, 0, 120, 40, "È·ÈÏ", callback),
		easyWidget(SG_DIALOG, "dialog", 0, 0, 400, 100, "dialog", NULL)));
}
void sgLoop() {

}