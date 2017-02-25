#include "sgl.h"

void sgSetup() {
	initWindow(100, 100, "SGL Sample", BIT_MAP);
	loadWave("bgm.wav", SG_LOOP);
}
void sgLoop() {
	return;
}