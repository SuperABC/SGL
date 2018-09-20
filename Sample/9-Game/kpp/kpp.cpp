#include "main.h"
#include "Music/phone.h"

int state = KS_LAUNCH;

extern Phone phone;

void sgSetup() {
	initWindow(800, 600, "Keyboard Piano Player", BIT_MAP);
	setMenu();
	setPanel();
	initKey();
	initMouse(SG_COORDINATE);
	initMidi();
}
void sgLoop() {
	switch (state) {
	case KS_LAUNCH:
		refreshLayout();
		state = KS_FREE;
		break;
	case KS_FREE:
		freePlay();
		break;
	}
}

void nullFunc() {}
void setVolume(widgetObj *w, int x, int y) {
	mouseMoveDrag(w, x, y);
	if (w->status&WIDGET_PRESSED) {
		phone.setVolume(w->value);
	}
}
void setMenu() {
	initMenu();
	int idFile = addMenuList("�ļ�", 0);
	int idEdit = addMenuList("�༭", 0);
	int idView = addMenuList("��ͼ", 0);
	int idSetting = addMenuList("����", 0);
	int idHelp = addMenuList("����", 0);
	addMenuItem("�½�\tCtrl + N", idFile, nullFunc);
	addMenuItem("��\tCtrl + O", idFile, nullFunc);
	addMenuItem("����\tCtrl + S", idFile, nullFunc);
	addMenuItem("�ر�", idFile, nullFunc);
	addMenuItem("�˳�\tAlt + F4", idFile, nullFunc);
	addMenuItem("����\tCtrl + Z", idEdit, nullFunc);
	addMenuItem("����\tCtrl + Y", idEdit, nullFunc);
	addMenuItem("����\tCtrl + C", idEdit, nullFunc);
	addMenuItem("����\tCtrl + X", idEdit, nullFunc);
	addMenuItem("ճ��\tCtrl + V", idEdit, nullFunc);
	addMenuItem("���", idEdit, nullFunc);
	addMenuItem("�ټ�", idView, nullFunc);
	addMenuItem("�ٲ�", idView, nullFunc);
	addMenuItem("����", idView, nullFunc);
	addMenuItem("������", idView, nullFunc);
	addMenuItem("�ļ�", idView, nullFunc);
	addMenuItem("����", idView, nullFunc);
	addMenuItem("��ʾ", idSetting, nullFunc);
	addMenuItem("����", idSetting, nullFunc);
	addMenuItem("����", idSetting, nullFunc);
	addMenuItem("����", idHelp, nullFunc);
	addMenuItem("˵����", idHelp, nullFunc);
}
void setPanel() {
	widgetObj *volume, *instrument, *sympho, *style, *lasting;

	lasting = newWidget(SG_CHECK, SGstring("lasting"));
	lasting->pos.x = 460;
	lasting->pos.y = 336;
	lasting->size.x = 48;
	lasting->size.y = 20;
	strcpy((char *)lasting->content, "����");
	registerWidget(lasting);
	free(lasting);

	instrument = newWidget(SG_LIST, SGstring("instrument"));
	instrument->pos.x = 520;
	instrument->pos.y = 334;
	instrument->size.x = 60;
	instrument->size.y = 20;
	instrument->hide = 6;
	memcpy((char *)instrument->content, "����\0����\0����\0����\0��˹\0����", 30);
	registerWidget(instrument);
	free(instrument);

	sympho = newWidget(SG_LIST, SGstring("sympho"));
	sympho->pos.x = 588;
	sympho->pos.y = 334;
	sympho->size.x = 40;
	sympho->size.y = 20;
	sympho->hide = 12;
	memcpy((char *)sympho->content, "C\0#C\0D\0#D\0E\0F\0#F\0G\0#G\0A\0#A\0B", 29);
	registerWidget(sympho);
	free(sympho);

	style = newWidget(SG_LIST, SGstring("style"));
	style->pos.x = 632;
	style->pos.y = 334;
	style->size.x = 60;
	style->size.y = 20;
	style->hide = 12;
	memcpy((char *)style->content, "M\0m\0\x37\0\x37M\0m7\0m7M\0\x36\0m6\0sus2\0sus4\0\x37sus2\0\x37sus4", 48);
	registerWidget(style);
	free(style);

	volume = newWidget(SG_DRAG, SGstring("volume"));
	volume->pos.x = 700;
	volume->pos.y = 334;
	volume->size.x = 80;
	volume->size.y = 20;
	volume->mouseIn = (mouseMoveCall)setVolume;
	registerWidget(volume);
	free(volume);
}

void refreshLayout() {
	loadBmp(0, 0, "Source//kpp//�ټ�.bmp");
	loadBmp(0, 360, "Source//kpp//����.bmp");
}