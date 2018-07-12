#include "winsgl.h"

SOCKET client;

void sendString() {
	widgetObj *Input = getWidgetByName("Input");
	socketSend(client, Input->content);
}
void layoutWidget() {
	widgetObj *Button;
	widgetObj *Input;

	Button = newWidget(SG_BUTTON, "Button");
	Button->pos.x = 540;
	Button->pos.y = 40;
	Button->size.x = 60;
	Button->size.y = 24;
	strcpy(Button->content, "х╥хо");
	Button->mouseClick = sendString;
	registerWidget(Button);
	free(Button);

	Input = newWidget(SG_INPUT, "Input");
	Input->pos.x = 40;
	Input->pos.y = 40;
	Input->size.x = 480;
	Input->size.y = 24;
	Input->associate = getWidgetByName("Button");
	strcpy(Input->content, "");
	registerWidget(Input);
	free(Input);
}

void sgSetup() {
	initWindow(640, 120, "SGL Sample", BIT_MAP);
	initMouse(SG_COORDINATE);
	initKey();
	layoutWidget();

	client = createClient("192.168.1.209", 6000);
}
void sgLoop() {
	return;
}