#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);
	initMouse(SG_COORDINATE);
	initKey();

	struct JSON *json = createJson();
	setStringElement(json, "abc", "abc");
	setStringElement(json, "def", "def");

	json = readJson(writeJson(json));
	debugf(writeJson(json));
}
void sgLoop() {
	return;
}
