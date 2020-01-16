#include "winsgl.h"

void sgSubSetup() {

}
void sgSubLoop() {

}

void sgSetup() {
	initWindow(640, 480, "Create", BIT_MAP);
}
void sgLoop() {
	static int first = 1;
	if (first) {
		first = 0;
		createWindow(300, 200, "sub1", BIT_MAP, (vect)sgSubSetup, (vect)sgSubLoop);
		createWindow(300, 200, "sub2", BIT_MAP, (vect)sgSubSetup, (vect)sgSubLoop);
	}
}