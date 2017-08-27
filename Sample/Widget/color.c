#include "winsgl.h"

void layoutWidget() {
	widgetObj *red, *green, *blue;

	red = newWidget(SG_DRAG, "red");
	red->pos.x = 100;
	red->pos.y = 100;
	red->size.x = 100;
	red->size.y = 20;
	registerWidget(red);
	free(red);

	green = newWidget(SG_DRAG, "green");
	green->pos.x = 100;
	green->pos.y = 150;
	green->size.x = 100;
	green->size.y = 20;
	registerWidget(green);
	free(green);

	blue = newWidget(SG_DRAG, "blue");
	blue->pos.x = 100;
	blue->pos.y = 200;
	blue->size.x = 100;
	blue->size.y = 20;
	registerWidget(blue);
	free(blue);
}

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);
	initMouse(SG_COORDINATE);
	initKey();
	layoutWidget();
}
void sgLoop() {
	int r = getWidgetByName("red")->value * 255 / 100;
	int g = getWidgetByName("green")->value * 255 / 100;
	int b = getWidgetByName("blue")->value * 255 / 100;

	setColor(255, 255, 255);
	putQuad(240, 100, 300, 220, SOLID_FILL);
	setColor(0, 0, 0);
	putString("Red:", 210, 100);
	putNumber(r, 260, 104, 'l');
	putString("Green:", 210, 150);
	putNumber(g, 260, 154, 'l');
	putString("Blue:", 210, 200);
	putNumber(b, 260, 204, 'l');

	setColor(r, g, b);
	putQuad(300, 100, 420, 220, SOLID_FILL);
	return;
}