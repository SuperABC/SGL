#include "winsgl.h"

int loading = 0;
char *clipboard;

void buttonLoad(widgetObj *w) {
	showWidget("Process");
	getWidgetByName("Process")->value = 0;
	loading = 1;
}
void optionChoose(widgetObj *w) {
	widgetObj *tmp = getWidgetByName("Input");
	switch (w->value) {
	case 0:
		strcpy(tmp->content, "");
		break;
	case 1:
		strcpy(clipboard, tmp->content);
		strcpy(tmp->content, "");
		break;
	case 2:
		strcpy(clipboard, tmp->content);
		break;
	case 3:
		strcpy(tmp->content, clipboard);
		break;
	case 4:
		break;
	}
}
void load() {
	if (++getWidgetByName("Process")->value == 100) {
		loading = 0;
		ceaseWidget("Process");
	}
}

void layoutWidget() {
	widgetObj *Button;
	widgetObj *Input;
	widgetObj *Process;
	widgetObj *Check;
	widgetObj *Option;

	Input = newWidget(SG_INPUT, "Input");
	Input->pos.x = 40;
	Input->pos.y = 40;
	Input->size.x = 160;
	Input->size.y = 24;
	strcpy(Input->content, "");
	registerWidget(Input);
	free(Input);

	Button = newWidget(SG_BUTTON, "Load");
	Button->pos.x = 240;
	Button->pos.y = 40;
	Button->size.x = 60;
	Button->size.y = 24;
	Button->mouseUser = buttonLoad;
	strcpy(Button->content, "载入");
	registerWidget(Button);
	free(Button);

	Check = newWidget(SG_CHECK, "Check");
	Check->pos.x = 40;
	Check->pos.y = 80;
	Check->size.x = 48;
	Check->size.y = 24;
	strcpy(Check->content, "bmp");
	registerWidget(Check);
	free(Check);

	Process = newWidget(SG_PROCESS, "Process");
	Process->pos.x = 160;
	Process->pos.y = 160;
	Process->size.x = 320;
	Process->size.y = 168;
	strcpy(Process->content, "加载中...");
	registerWidget(Process);
	free(Process);

	Option = newWidget(SG_OPTION, "Option");
	Option->size.x = 160;
	Option->hide = 5;
	memcpy(Option->content, "清空\0剪切\0复制\0粘贴\0撤销", 25);
	Option->associate = getWidgetByName("Input");
	Option->mouseUser = optionChoose;
	registerWidget(Option);
	free(Option);
}

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);
	initMouse(SG_COORDINATE);
	initKey();
	layoutWidget();
	clipboard = malloc(100 * sizeof(char));
}
void sgLoop() {
	if (loading)
		load();
	return;
}