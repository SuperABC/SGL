#include "../../winsgl.h"

void sgSetup() {
	initWindow(640, 480, "Init", BIT_MAP);
}
void sgLoop() {
	static int rounds = 1000;
	if (rounds == 1000)
		renameCaption("Caption");
	else if (rounds == 500)
		hideCaption();
	else if (rounds == 0)
		showCaption();

	rounds--;
}