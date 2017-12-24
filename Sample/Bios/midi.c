#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "SGL sample", BIT_MAP);
	initKey();
	initMidi();
	changeInstrument(0);
}
void sgLoop() {
	static int first = 1;
	static int tune = 0x3C;
	int key;
	if (first) {
		playMidi(tune, 100, 1);
		first = 0;
	}
	if (biosKey(1)) {
		key = biosKey(0);
		if (key == SG_UP) {
			playMidi(tune, 100, 0);
			tune++;
			playMidi(tune, 100, 1);
		}
		if (key == SG_DOWN) {
			playMidi(tune, 100, 0);
			tune--;
			playMidi(tune, 100, 1);
		}
	}
}
