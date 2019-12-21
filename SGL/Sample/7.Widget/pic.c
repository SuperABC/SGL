#include "../../winsgl.h"

void callback() {
	bitMap tmp = getWidgetByName("pic")->bgImg;
	for (int i = 0; i < tmp.sizeX; i++) {
		for (int j = 0; j < tmp.sizeY; j++) {
			tmp.data[(j * tmp.sizeX + i) * 3] = 255 - tmp.data[(j * tmp.sizeX + i) * 3];
			tmp.data[(j * tmp.sizeX + i) * 3 + 1] = 255 - tmp.data[(j * tmp.sizeX + i) * 3 + 1];
			tmp.data[(j * tmp.sizeX + i) * 3 + 2] = 255 - tmp.data[(j * tmp.sizeX + i) * 3 + 2];
		}
	}
	refreshWidget("pic");
}
void sgSetup() {
	initWindow(640, 480, "Pic", BIT_MAP);

	easyWidget(SG_LABEL, "label", 100, 100, 120, 40, "picture", NULL);
	easyWidget(SG_PIC, "pic", 100, 140, 0, 0, "source/pic.bmp", NULL);
	easyWidget(SG_CHECK, "check", 240, 100, 80, 20, "inverse", callback);
}
void sgLoop() {

}