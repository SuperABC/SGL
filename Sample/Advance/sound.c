#include "screen.h"

void sgSetup() {
	initWindow(100, 100, "SGL Sample");
	loadWave("bgm.wav", SG_LOOP);
}
void sgLoop() {
	return;
}