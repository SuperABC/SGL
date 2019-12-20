#include "../../winsgl.h"

void sgSetup() {
	initWindow(640, 480, "Clear", BIT_MAP);
}
void sgLoop() {
	static int r = 0, g = 0, b = 0;

	if (r == g && b < 252)b += 4;
	else if (r + g == 252 && b > 0)b -= 4;
	else if (r == 0 && b == 252 && g < 252)g += 4;
	else if (r == 252 && b == 252 && g > 0)g -= 4;
	else if (b == 0 && g == 252 && r < 252)r += 4;
	else if (b == 0 && g == 0 && r > 0)r -= 4;

	setColor(r, g, b);
	clearScreen();
}