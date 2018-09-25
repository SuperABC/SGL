#include "winsgl.h"

static char *Manager = "Manager";
static char *Vip = "Vip";
static char *Usual = "Usual";

void createDialog(widgetObj *w) {
	if (strcmp(getWidgetByName("InputPw")->content, Manager) == 0&&
		getWidgetByName("List")->value==0)
		strcpy(getWidgetByName("Dialog")->content, "Manager login success!");
	else if (strcmp(getWidgetByName("InputPw")->content, Vip) == 0 &&
		getWidgetByName("List")->value == 1)
		strcpy(getWidgetByName("Dialog")->content, "Vip login success!");
	else if (strcmp(getWidgetByName("InputPw")->content, Usual) == 0 &&
		getWidgetByName("List")->value == 2)
		strcpy(getWidgetByName("Dialog")->content, "Usual login success!");
	else
		strcpy(getWidgetByName("Dialog")->content, "Wrong password!");
	showWidget("Dialog");
}
void layoutWidget() {
	widgetObj *Button;
	widgetObj *Input;
	widgetObj *Dialog;
	widgetObj *List;
	widgetObj *Label;

	Label = newWidget(SG_LABEL, "LabelUser");
	Label->pos.x = 40;
	Label->pos.y = 42;
	strcpy(Label->content, "用户名：");
	registerWidget(Label);
	free(Label);

	Label = newWidget(SG_LABEL, "LabelPwr");
	Label->pos.x = 40;
	Label->pos.y = 82;
	strcpy(Label->content, "密码：");
	registerWidget(Label);
	free(Label);

	Input = newWidget(SG_INPUT, "InputUser");
	Input->pos.x = 120;
	Input->pos.y = 40;
	Input->size.x = 160;
	Input->size.y = 24;
	strcpy(Input->content, "");
	registerWidget(Input);
	free(Input);

	Input = newWidget(SG_INPUT, "InputPw");
	Input->pos.x = 120;
	Input->pos.y = 80;
	Input->size.x = 160;
	Input->size.y = 24;
	strcpy(Input->content, "");
	registerWidget(Input);
	free(Input);

	List = newWidget(SG_LIST, "List");
	List->pos.x = 40;
	List->pos.y = 120;
	List->size.x = 80;
	List->size.y = 24;
	memcpy(List->content, "管理员\0会员\0用户", 17);
	List->hide = 3;
	registerWidget(List);
	free(List);

	Button = newWidget(SG_BUTTON, "Register");
	Button->pos.x = 220;
	Button->pos.y = 120;
	Button->size.x = 60;
	Button->size.y = 24;
	Button->mouseUser = createDialog;
	strcpy(Button->content, "登录");
	registerWidget(Button);
	free(Button);

	Dialog = newWidget(SG_DIALOG, "Dialog");
	Dialog->pos.x = 160;
	Dialog->pos.y = 160;
	Dialog->size.x = 320;
	Dialog->size.y = 168;
	strcpy(Dialog->content, "成功弹出对话框！");
	registerWidget(Dialog);
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