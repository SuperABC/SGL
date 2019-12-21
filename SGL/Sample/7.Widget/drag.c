#include "../../winsgl.h"

int value = -1;

void callback() {

}
void background(int left, int top, int right, int bottom) {
	setColor(value * 2.55f, value * 2.55f, value * 2.55f);
	putQuad(left, top, right, bottom, SOLID_FILL);
}

void sgSetup() {
	initWindow(640, 480, "Drag", BIT_MAP);

	easyWidget(SG_DRAG, "drag", 0, 0, 640, 20, "", NULL);
	easyWidget(SG_PROCESS, "process", 100, 100, 400, 200, "loading", NULL);
	easyWidget(SG_SCROLLVERT, "vert", 620, 100, 20, 380, "", NULL);
	easyWidget(SG_SCROLLHORIZ, "horizon", 0, 460, 620, 20, "", NULL);
	setBackgroundRefresh(background);
	showWidget("process");
	getWidgetByName("process")->value = 0;
}
void sgLoop() {
	if (getWidgetByName("process")->value++ >= 100)ceaseWidget("process");
	refreshWidget("process");
	if (value != getWidgetByName("drag")->value) {
		value = getWidgetByName("drag")->value;
		setColor(value * 2.55f, value * 2.55f, value * 2.55f);
		clearScreen();
	}
}