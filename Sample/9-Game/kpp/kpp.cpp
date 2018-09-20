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
	int idFile = addMenuList("文件", 0);
	int idEdit = addMenuList("编辑", 0);
	int idView = addMenuList("视图", 0);
	int idSetting = addMenuList("设置", 0);
	int idHelp = addMenuList("帮助", 0);
	addMenuItem("新建\tCtrl + N", idFile, nullFunc);
	addMenuItem("打开\tCtrl + O", idFile, nullFunc);
	addMenuItem("保存\tCtrl + S", idFile, nullFunc);
	addMenuItem("关闭", idFile, nullFunc);
	addMenuItem("退出\tAlt + F4", idFile, nullFunc);
	addMenuItem("撤销\tCtrl + Z", idEdit, nullFunc);
	addMenuItem("重做\tCtrl + Y", idEdit, nullFunc);
	addMenuItem("复制\tCtrl + C", idEdit, nullFunc);
	addMenuItem("剪切\tCtrl + X", idEdit, nullFunc);
	addMenuItem("粘贴\tCtrl + V", idEdit, nullFunc);
	addMenuItem("清空", idEdit, nullFunc);
	addMenuItem("琴键", idView, nullFunc);
	addMenuItem("瀑布", idView, nullFunc);
	addMenuItem("乐谱", idView, nullFunc);
	addMenuItem("调音板", idView, nullFunc);
	addMenuItem("文件", idView, nullFunc);
	addMenuItem("键盘", idView, nullFunc);
	addMenuItem("显示", idSetting, nullFunc);
	addMenuItem("声音", idSetting, nullFunc);
	addMenuItem("键盘", idSetting, nullFunc);
	addMenuItem("关于", idHelp, nullFunc);
	addMenuItem("说明书", idHelp, nullFunc);
}
void setPanel() {
	widgetObj *volume, *instrument, *sympho, *style, *lasting;

	lasting = newWidget(SG_CHECK, SGstring("lasting"));
	lasting->pos.x = 460;
	lasting->pos.y = 336;
	lasting->size.x = 48;
	lasting->size.y = 20;
	strcpy((char *)lasting->content, "延音");
	registerWidget(lasting);
	free(lasting);

	instrument = newWidget(SG_LIST, SGstring("instrument"));
	instrument->pos.x = 520;
	instrument->pos.y = 334;
	instrument->size.x = 60;
	instrument->size.y = 20;
	instrument->hide = 6;
	memcpy((char *)instrument->content, "钢琴\0和声\0风琴\0吉他\0贝斯\0弦乐", 30);
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
	loadBmp(0, 0, "Source//kpp//琴键.bmp");
	loadBmp(0, 360, "Source//kpp//键盘.bmp");
}