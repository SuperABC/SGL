#include "winsgl.h"

void callBack() {

}

int idPopup;
void sgSetup() {
	initWindow(640, 480, "Menu", BIT_MAP);

	idPopup = createPopupMenu();
	addPopupMenuItem("1", idPopup, callBack);
	addPopupMenuItem("2", idPopup, callBack);
	addPopupMenuItem("3", idPopup, callBack);
	addPopupMenuItem("4", idPopup, callBack);
	finishPopupMenu(idPopup);
}
void sgLoop() {

}
