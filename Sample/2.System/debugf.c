#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "Debugf", BIT_MAP);
}
void sgLoop() {
	static int tmp = 0;
	debugf("%d\n", tmp++);
}