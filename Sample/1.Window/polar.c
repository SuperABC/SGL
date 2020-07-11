#include "winsgl.h"

void subSetup() {}
void subLoop() {}

void sgSetup() {
	initPolarWindow(0, 0);
}
void sgLoop() {
	static int first = 1;
	if (first) {
		first = 0;
		createPolarWindow(100, 100, subSetup, subLoop);
	}
}