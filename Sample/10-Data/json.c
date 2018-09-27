#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);
	initMouse(SG_COORDINATE);
	initKey();

	struct JSON *json = createJson();
	setIntElement(json, "a1", 1);
	setIntElement(json, "a2", 2);
	setIntElement(json, "a1", 2);

	debugf(writeJson(json));
}
void sgLoop() {
	return;
}
