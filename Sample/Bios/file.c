#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "SGL sample", BIT_MAP);
	initKey();
}
void sgLoop() {
	char *file = (char *)malloc(260);

	if (biosKey(1)) {
		biosKey(0);
		if(selectDir(file, NULL))
			alertInfo("Hello", NULL, 0);
	}
	clearKeyBuffer();
}