#include "winsgl.h"

int mciId;
void sgSetup() {
	initWindow(640, 480, "Music", BIT_MAP);
	initMci(); 
	mciId = loadMciSrc("Source/test.mp3");
}
void sgLoop() {
	int key;
	if (biosKey(1)) {
		key = biosKey(0);
		switch (key) {
		case 'b':
			playMci(mciId);
			break;
		case 'p':
			pauseMci(mciId);
			break;
		case 'r':
			resumeMci(mciId);
			break;
		case 's':
			stopMci(mciId);
			break;
		}
	}
}