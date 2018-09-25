#include "winsgl.h"

void addCard(widgetObj *w) {
	widgetObj *color, *num, *back;
	color = newWidget(SG_LABEL, "color");
	color->pos.x = color->pos.y = 40;
	color->size.x = color->size.y = 0;
	strcpy(color->content, "ºìÌÒ");
	num = newWidget(SG_LABEL, "num");
	num->pos.x = 80;
	num->pos.y = 40;
	num->size.x = num->size.y = 0;
	strcpy(num->content, "A");
	back = newWidget(SG_OUTPUT, "back");
	back->pos.x = back->pos.y = 40;
	back->size.x = 60;
	back->size.y = 100;

	widgetObj *card = newCombinedWidget(3, "card", back, num, color);
	registerWidget(card);
}
void layoutWidget() {
	easyWidget(SG_BUTTON, "add", 520, 440, 80, 20, "ÃþÅÆ", addCard);
}

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);
	initMouse(SG_COORDINATE);
	initKey();
	layoutWidget();
}
void sgLoop() {
	return;
}