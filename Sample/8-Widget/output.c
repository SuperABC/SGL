#include "winsgl.h"

void sendMessage(widgetObj *w) {
	widgetObj *dst = getWidgetByName("Output");
	widgetObj *src = getWidgetByName("Input");

	if (((char *)src->content)[0] == '\0')return;

	strcat(dst->content, src->content);
	strcat(dst->content, "\n");
	((char *)src->content)[0] = '\0';
	dst->valid = 0;
}
void layoutWidget() {
	widgetObj *Button;
	widgetObj *Input;
	widgetObj *Output;

	Button = newWidget(SG_BUTTON, "Button");
	Button->pos.x = 540;
	Button->pos.y = 420;
	Button->size.x = 60;
	Button->size.y = 24;
	Button->mouseUser = sendMessage;
	strcpy(Button->content, "х╥хо");
	registerWidget(Button);
	free(Button);

	Input = newWidget(SG_INPUT, "Input");
	Input->pos.x = 40;
	Input->pos.y = 420;
	Input->size.x = 480;
	Input->size.y = 24;
	Input->associate = getWidgetByName("Button");
	strcpy(Input->content, "");
	registerWidget(Input);
	free(Input);

	Output = newWidget(SG_OUTPUT, "Output");
	Output->pos.x = 40;
	Output->pos.y = 40;
	Output->size.x = 560;
	Output->size.y = 360;
	strcpy(Output->content, "");
	registerWidget(Output);
	free(Output);
}

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);
	initMouse(SG_COORDINATE);
	initKey();

	layoutWidget();

	return;
}
void sgLoop() {
	return;
}