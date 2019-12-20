#include "../../winsgl.h"

int subWindowId = -1;

void sgSubSetup() {

}
void sgSubLoop() {
	static int rounds = 100;

	if(rounds--==0)
		closeWindow(subWindowId);
}

void sgSetup() {
	initWindow(640, 480, "Close", BIT_MAP);
}
void sgLoop() {
	static int first = 1;
	if (first) {
		first = 0;
		subWindowId = createWindow(300, 200, "sub", BIT_MAP, (vect)sgSubSetup, (vect)sgSubLoop);
	}
}