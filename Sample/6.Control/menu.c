#include "../../winsgl.h"

void callBack() {
	alertInfo("success.", "Alert", ALERT_BUTTON_OKCANCEL | ALERT_ICON_EXCLAMATION);
}
void hide() {
	hideToTray();
}
void open() {
	restoreFromTray();
}
void close() {
	exit(0);
}

void sgSetup() {
	initWindow(640, 480, "Menu", BIT_MAP);

	int idMain = initMainMenu();
	int idFile = addMainMenuList("File", idMain);
	int idEdit = addMainMenuList("Edit", idMain);
	int idHelp = addMainMenuList("Help", idMain);
	addMainMenuItem("Open\tCtrl + O", idFile, callBack);
	addMainMenuItem("Close", idFile, callBack);
	addMainMenuSeparator(idFile);
	addMainMenuItem("Save\tCtrl + S", idFile, callBack);
	addMainMenuItem("Hide to tray", idFile, hide);
	addMainMenuItem("Exit\tF4", idFile, callBack);
	addMainMenuItem("Copy\tCtrl + C", idEdit, callBack);
	addMainMenuItem("Cut\tCtrl + X", idEdit, callBack);
	addMainMenuItem("Paste\tCtrl + V", idEdit, callBack);
	addMainMenuItem("Clear", idEdit, callBack);
	int idAbout = addMainMenuList("About", idHelp);
	addMainMenuItem("Version", idAbout, callBack);
	addMainMenuItem("Author", idAbout, callBack);
	addMainMenuItem("Company", idAbout, callBack);

	int idTray = initTrayMenu();
	addTrayMenuItem("´ò¿ª", idTray, open);
	addTrayMenuItem("¹Ø±Õ", idTray, close);
}
void sgLoop() {

}
