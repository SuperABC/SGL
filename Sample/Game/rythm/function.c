#include "main.h"

extern int pro;
extern struct set Set;
extern struct archive Archive;
extern struct status Status[8];

char *list[] = { "Canon" };

void start() {
	static int first = 1;
	vecThree mouse;
	if (first) {
		loadBmp(0, 0, "Source\\rythm\\start.bmp");
		first = 0;
	}
	if (biosMouse(1).m) {
		mouse = biosMouse(0);
		if (inRect(mouse.x, mouse.y, 40, 40, 99, 99)) {
			pro = SETTING;
			first = 1;
		}
		if (mouse.x < 240 || mouse.x>439)return;
		if (mouse.y >= 160 && mouse.y <= 199) {
			pro = CHOOSE;
			first = 1;
		}
		if (mouse.y >= 240 && mouse.y <= 279) {
			pro = INSTRUCT;
			first = 1;
		}
		if (mouse.y >= 320 && mouse.y <= 359) {
			exit(0);
		}
	}
}
void instruct() {
	static int first = 1;
	static int t = 0;
	bitMap *oldImage = (bitMap*)malloc(sizeof(bitMap));
	if (first) {
		setColor(0, 0, 0);
		clearScreen();
		first = 0;
	}
	if (++t >= 840) {
		t = 0;
		first = 1;
		pro = START;
	}
	if (t <= 400) {
		delayBegin();
		loadBmp(0, 400 - t, "Source\\rythm\\instruct.bmp");
		putQuad(0, 480, 679, 519, SOLID_FILL);
		delayEnd(20);
	}
	if (t > 400) {
		delayBegin();
		getImage(0, 41, 679, 519, oldImage);
		putImage(0, 40, oldImage, 0);
		delayEnd(20);
	}
	if (biosKey(1)) {
		free(oldImage);
		pro = START;
		first = 1;
	}
}
void setting() {
	static int first = 1;
	static bitMap *oldImage;
	vecThree mouse;
	int key;
	if (first) {
		loadBmp(0, 0, "Source\\rythm\\setting.bmp");
		oldImage = (bitMap*)malloc(sizeof(bitMap));
		first = 0;
	}
	if (biosKey(1)) {
		key = biosKey(0);
		switch (key) {
		case SG_ESC:
			pro = START;
			first = 1;
			return;
		default:
			break;
		}
	}
	if (biosMouse(1).m) {
		mouse = biosMouse(0);

	}
}
void choose() {
	static int first = 1, tmp = 0;
	vecTwo position;
	vecThree mouse;
	int key;
	if (first) {
		loadBmp(0, 0, "Source\\rythm\\choose.bmp");
		first = 0;
	}
	if (biosKey(1)) {
		key = biosKey(0);
		switch (key) {
		case SG_ESC:
			pro = START;
			first = 1;
			return;
		default:
			break;
		}
	}
	position = mousePos();
	if (biosMouse(1).m) {
		mouse = biosMouse(0);

	}
}
void playing() {
	static int first = 1;
	static int t = 0;
	int key;
	if (first) {
		loadBmp(0, 0, "Source\\rythm\\playing.bmp");
		first = 0;
	}
	if (t > 2000) {
		if (biosKey(1)) {
			key = biosKey(0);
			switch (key) {
			case '1' | 0x8000:
			case SG_END|0x8000:
				Status[0].pressed = 1;
				break;
			case '2' | 0x8000:
			case SG_DOWN|0x8000:
				Status[1].pressed = 1;
			case '3' | 0x8000:
			case SG_PAGEDOWN | 0x8000:
				Status[2].pressed = 1;
				break;
			case '6' | 0x8000:
			case SG_RIGHT | 0x8000:
				Status[3].pressed = 1;
				break;
			case '9' | 0x8000:
			case SG_PAGEUP | 0x8000:
				Status[4].pressed = 1;
				break;
			case '8' | 0x8000:
			case SG_UP | 0x8000:
				Status[5].pressed = 1;
				break;
			case '7' | 0x8000:
			case SG_HOME | 0x8000:
				Status[6].pressed = 1;
				break;
			case '4' | 0x8000:
			case SG_LEFT | 0x8000:
				Status[7].pressed = 1;
				break;
			case '1':
			case SG_END:
				Status[0].pressed = 0;
				break;
			case '2':
			case SG_DOWN:
				Status[1].pressed = 0;
			case '3':
			case SG_PAGEDOWN:
				Status[2].pressed = 0;
				break;
			case '6':
			case SG_RIGHT:
				Status[3].pressed = 0;
				break;
			case '9':
			case SG_PAGEUP:
				Status[4].pressed = 0;
				break;
			case '8':
			case SG_UP:
				Status[5].pressed = 0;
				break;
			case '7':
			case SG_HOME:
				Status[6].pressed = 0;
				break;
			case '4':
			case SG_LEFT:
				Status[7].pressed = 0;
				break;
			}
		}
		operate();
	}
}
void score() {
	static int first = 1, tmp = 0;
	vecThree mouse;
	int key;
	if (first) {
		loadBmp(0, 0, "Source\\rythm\\score.bmp");
		first = 0;
	}
	if (biosKey(1)) {
		key = biosKey(0);
		switch (key) {
		case SG_ESC:
			pro = START;
			first = 1;
			return;
		default:
			break;
		}
	}
	if (biosMouse(1).m) {
		mouse = biosMouse(0);

	}
}

void initPro() {

}
void operate() {

}