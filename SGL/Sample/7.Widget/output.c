#include "../../winsgl.h"

void callback() {
	char *buffer = getWidgetByName("output")->content;
	char *add = getWidgetByName("input")->content;
	strcat(buffer, add);
	strcat(buffer, "\n");
	refreshWidget("output");
}
void sgSetup() {
	initWindow(640, 480, "Output", BIT_MAP);

	easyWidget(SG_INPUT, "input", 240, 100, 200, 40, "1234", NULL);
	easyWidget(SG_BUTTON, "button", 460, 100, 120, 40, "hello", callback);
	easyWidget(SG_OUTPUT, "output", 100, 200, 400, 200, "", NULL);
	getWidgetByName("output")->content = (char *)malloc(1000);
	memset(getWidgetByName("output")->content, 0, 1000);
	easyWidget(SG_LIST, "list", 100, 100, 120, 40, "1\n2\n3\n4\n", NULL);
	getWidgetByName("list")->extra = 4;
	refreshWidget("list");

}
void sgLoop() {

}