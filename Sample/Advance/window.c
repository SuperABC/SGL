#include "winsgl.h"

int sub;

void alert(widgetObj *w) {
	alertInfo("success", "info", 0);
}

void sgSubSetup() {
	widgetObj *button = newWidget(SG_BUTTON, "sub");
	button->pos.x = button->pos.y = 100;
	button->size.x = 100;
	button->size.y = 40;
	strcpy(button->content, "button");
	button->mouseUser = alert;
	registerSubWidget(sub, button);
}
void sgSubLoop() {
	static int first = 1000;
	if (first == 1000) {
		setSubColor(sub, 0, 0, 0);
		putSubString(sub, "hello", 0, 0);
	}
	else if (first == 0)closeWindow(sub);

	first--;
}

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);
}
void sgLoop() {
	static int first = 1;
	if (first) {
		first = 0;
		sub = createWindow(320, 240, "Sub window", (vect)sgSubSetup, (vect)sgSubLoop);
	}
}