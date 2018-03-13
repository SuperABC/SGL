#include "winsgl.h"

void callBack() {
	alertInfo("success.", "Alert", ALERT_BUTTON_OKCANCEL | ALERT_ICON_EXCLAMATION);
}
void hide() {
	hideToTray();
}
void close() {
	exit(0);
}
void open() {
	restoreFromTray();
}


void sgSetup() {
	initWindow(640, 480, "SGL sample", BIT_MAP);

	int idMain = initMenu();
	int idFile = addMenuList("File", idMain);
	int idEdit = addMenuList("Edit", idMain);
	int idHelp = addMenuList("Help", idMain);
	addMenuItem("Open\tCtrl + O", idFile, callBack);
	addMenuItem("Close", idFile, callBack);
	addMenuSeparator(idFile);
	addMenuItem("Save\tCtrl + S", idFile, callBack);
	addMenuItem("Hide to tray", idFile, hide);
	addMenuItem("Exit\tF4", idFile, callBack);
	addMenuItem("Copy\tCtrl + C", idEdit, callBack);
	addMenuItem("Cut\tCtrl + X", idEdit, callBack);
	addMenuItem("Paste\tCtrl + V", idEdit, callBack);
	addMenuItem("Clear", idEdit, callBack);
	int idAbout = addMenuList("About", idHelp);
	addMenuItem("Version", idAbout, callBack);
	addMenuItem("Author", idAbout, callBack);
	addMenuItem("Company", idAbout, callBack);

	int idTray = initTrayMenu();
	addTrayMenuItem("´ò¿ª", idTray, open);
	addTrayMenuItem("¹Ø±Õ", idTray, close);
}
void sgLoop() {
	static int first = 1;
	if (first) {
		first = 0;
		addTray();
	}
}