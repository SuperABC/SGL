#include "winsgl.h"

void callBack() {
	alertInfo("success.", "Alert", ALERT_BUTTON_OKCANCEL | ALERT_ICON_EXCLAMATION);
}

void sgSetup() {
	initWindow(640, 480, "SGL sample", BIT_MAP);
	initMenu();
	int idFile = addMenuList("File", 0);
	int idEdit = addMenuList("Edit", 0);
	int idHelp = addMenuList("Help", 0);
	addMenuItem("Open", idFile, callBack);
	addMenuItem("Close", idFile, callBack);
	addMenuItem("Save", idFile, callBack);
	addMenuItem("Exit", idFile, callBack);
	addMenuItem("Copy", idEdit, callBack);
	addMenuItem("Cut", idEdit, callBack);
	addMenuItem("Paste", idEdit, callBack);
	addMenuItem("Clear", idEdit, callBack);
	int idAbout = addMenuList("About", idHelp);
	addMenuItem("Version", idAbout, callBack);
	addMenuItem("Author", idAbout, callBack);
	addMenuItem("Company", idAbout, callBack);
}
void sgLoop() {
	setColor(0, 0, 0);
	clearScreen();
}