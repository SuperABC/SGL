#include "winsgl.h"

int cardNum = 0;

void addCard(widgetObj *w) {
	widgetObj *color, *num, *back;
	color = newWidget(SG_LABEL, "color");
	strcpy(color->content, "ºìÌÒ");
	num = newWidget(SG_LABEL, "num");
	num->pos.x = 40;
	strcpy(num->content, "A");
	back = newWidget(SG_OUTPUT, "back");
	back->size.x = 60;
	back->size.y = 100;

	char name[8];
	sprintf(name, "card%d", cardNum++);
	widgetObj *card = newCombinedWidget(3, name, back, num, color);
	card->pos.x = cardNum * 40;
	card->pos.y = 40;
	registerWidget(card);
}
void subCard(widgetObj *w) {
	if (cardNum <= 0)return;

	char name[8];
	sprintf(name, "card%d", --cardNum);
	deleteWidgetByName(name);
}
void layoutWidget() {
	easyWidget(SG_BUTTON, "add", 400, 440, 80, 20, "ÃþÅÆ", addCard);
	easyWidget(SG_BUTTON, "add", 520, 440, 80, 20, "ÆúÅÆ", subCard);
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