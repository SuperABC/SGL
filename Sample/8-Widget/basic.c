#include "winsgl.h"

void bgRedraw(int left, int top, int right, int bottom) {
	setColor(255, 255, 255);
	clearScreen();
}
void createDialog(widgetObj *w) {
	char *tmp;
	tmp = malloc(10);
	strcpy(tmp, "Input");
	tmp[5] = w->name[6];
	tmp[6] = 0;

	strcpy(getWidgetByName("Dialog")->content, getWidgetByName(tmp)->content);
	showWidget("Dialog");

	free(tmp);
}
void layoutWidget() {
	widgetObj *Button;
	widgetObj *Input;
	widgetObj *Dialog;

	Button = newWidget(SG_BUTTON, "Button1");
	Button->pos.x = 540;
	Button->pos.y = 420;
	Button->size.x = 60;
	Button->size.y = 24;
	Button->mouseUser = createDialog;
	strcpy(Button->content, "确 认");
	registerWidget(Button);
	free(Button);

	Button = newWidget(SG_BUTTON, "Button2");
	Button->pos.x = 540;
	Button->pos.y = 380;
	Button->size.x = 60;
	Button->size.y = 24;
	Button->mouseUser = createDialog;
	strcpy(Button->content, "确 认");
	registerWidget(Button);
	free(Button);

	Button = newWidget(SG_BUTTON, "Button3");
	Button->pos.x = 540;
	Button->pos.y = 340;
	Button->size.x = 60;
	Button->size.y = 24;
	Button->mouseUser = createDialog;
	strcpy(Button->content, "确 认");
	registerWidget(Button);
	free(Button);

	Input = newWidget(SG_INPUT, "Input1");
	Input->pos.x = 40;
	Input->pos.y = 420;
	Input->size.x = 480;
	Input->size.y = 24;
	Input->associate = getWidgetByName("Button1");
	strcpy(Input->content, "");
	registerWidget(Input);
	free(Input);

	Input = newWidget(SG_INPUT, "Input2");
	Input->pos.x = 40;
	Input->pos.y = 380;
	Input->size.x = 480;
	Input->size.y = 24;
	Input->associate = getWidgetByName("Button2");
	strcpy(Input->content, "");
	registerWidget(Input);
	free(Input);

	Input = newWidget(SG_INPUT, "Input3");
	Input->pos.x = 40;
	Input->pos.y = 340;
	Input->size.x = 480;
	Input->size.y = 24;
	Input->associate = getWidgetByName("Button3");
	strcpy(Input->content, "abcd");
	registerWidget(Input);
	free(Input);

	Dialog = newWidget(SG_DIALOG, "Dialog");
	Dialog->pos.x = 160;
	Dialog->pos.y = 160;
	Dialog->size.x = 320;
	Dialog->size.y = 168;
	strcpy(Dialog->content, "成功弹出对话框！");
	registerWidget(Dialog);
	free(Dialog);
}

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);
	initMouse(SG_COORDINATE);
	initKey();
	layoutWidget();
	setBackgroundRefresh(bgRedraw);
}
void sgLoop() {
	return;
}