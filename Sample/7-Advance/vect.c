#include "winsgl.h"

void vectKey(void);
void vectTime(void);

vect vk, vt;
int num = 0, numx = 0, numy = 0;

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);
	//initKey();
	setFreq(1.f);

	//vk = getVect(9);
	//vt = getVect(8);
	//setVect(9, vectKey);
	setVect(8, vectTime);
}
void sgLoop() {
	setColor(255, 255, 255);
	putQuad(0, numy-16, 639, numy+31, SOLID_FILL);
	setColor(0, 0, 0);
	putNumber(num, numx, numy, 'l');
	return;
}

void vectKey(void) {
	int key;
	dosInt(9, &key);

	switch (key) {
	case SG_UP:
		if (numy >= 16)numy -= 16;
		break;
	case SG_DOWN:
		if (numy <= 448)numy += 16;
		break;
	case SG_LEFT:
		if (numx >= 8)numx -= 8;
		break;
	case SG_RIGHT:
		if (numx <= 624)numx += 8;
		break;
	}
}
void vectTime(void) {
	num++;
}