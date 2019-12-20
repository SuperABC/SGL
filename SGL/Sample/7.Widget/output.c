#include "../../winsgl.h"

void callback() {
	char *buffer = getWidgetByName("output")->content;
	char *add = getWidgetByName("input")->content;
	strcat(buffer, add);
	strcat(buffer, "\n");
}
void sgSetup() {
	initWindow(640, 480, "Output", BIT_MAP);

	easyWidget(SG_INPUT, "input", 100, 100, 200, 40, "1234", NULL);
	easyWidget(SG_BUTTON, "button", 320, 100, 120, 40, "hello", callback);
	easyWidget(SG_OUTPUT, "output", 100, 200, 400, 200, "", NULL);
}
void sgLoop() {

}