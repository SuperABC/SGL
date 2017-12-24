#include "winsgl.h"

char *obj;

void copy() {
	copyText(obj);
}
void cut() {
	copyText(obj);
	free(obj);
	obj = NULL;
}
void paste() {
	char *s = pasteText();
	if (obj) {
		free(obj);
	}
	obj = s;
}

void sgSetup() {
	initWindow(640, 480, "SGL sample", BIT_MAP);
	initMenu();
	int idEdit = addMenuList("Edit", 0);
	addMenuItem("Copy\tCtrl + C", idEdit, copy);
	addMenuItem("Cut\tCtrl + X", idEdit, cut);
	addMenuItem("Paste\tCtrl + V", idEdit, paste);

	obj = (char *)malloc(sizeof("Hello SGL"));
	strcpy(obj, "Hello SGL");
}
void sgLoop() {
	setColor(0, 0, 0);
	clearScreen();

	setColor(255, 255, 255);
	setFontSize(40);
	putString(obj, 0, 0);
}
