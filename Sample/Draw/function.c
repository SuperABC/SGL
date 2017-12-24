#include "winsgl.h"

float f(float x) {
	return 20*sinf(x/4)+50;
	//return sqrt(900-x*x);
	//return x*x - 80 * x + 1600;
}

void sgSetup() {
	initWindow(160, 120, "SGL sample", BIT_MAP);
}
void sgLoop() {
	setColor(0, 0, 0);
	funcMap(0, 100, 0, 100, f);
}
