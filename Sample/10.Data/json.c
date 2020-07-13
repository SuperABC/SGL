#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);

	struct JSON *json1 = createJson();
	setStringContent(json1, "abc", "abc");
	setStringContent(json1, "def", "def");
	struct JSON *json2 = createJson();
	setStringContent(json2, "abc", "abc");
	setStringContent(json2, "def", "def");
	struct JSON *json3 = createJson();
	setStringContent(json3, "abc", "abc");
	setStringContent(json3, "def", "def");
	setObjectContent(json3, "obj", json2);
	struct JSON *json = createJsonArray();
	setObjectElement(json, -1, json1);
	setObjectElement(json, -1, json2);
	setObjectElement(json, -1, json3);

	json = readJson(writeJson(json));
	debugf(writeJson(json));
}
void sgLoop() {
	return;
}