#include "screen.h"
void sgSetup() {
	initWindow(640, 480, "SGL sample");
}
void sgLoop() {
	setColor(0, 0, 0);
	putString("Hello world!", 0, 0);
	putString("SGL is the best graphic library in the world!", 100, 100);
	putChar('a', 200, 200);
	putNumber(123456789, 300, 300, 'l');
	putNumber(987654321, 400, 400, 'r');
}
