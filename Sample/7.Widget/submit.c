#include "winsgl.h"

void callback(widget *obj) {
	showWidget("combined/dialog");
}
void sgSetup() {
	initWindow(640, 480, "Submit", BIT_MAP);

	registerWidget(easyCombinedWidget("combined", 100, 100, 400, 100, 3,
		easyWidget(SG_INPUT, "input", 0, 0, 200, 40, "1234", NULL),
		easyWidget(SG_BUTTON, "button", 220, 0, 120, 40, "hello", callback),
		easyWidget(SG_DIALOG, "dialog", 0, 100, 400, 100, "dialog", NULL)));
}
void sgLoop() {

}
