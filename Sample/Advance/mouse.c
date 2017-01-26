#include "screen.h"
#define MOUSE_SIZE 16 
extern struct _win *Window;

word mouseOut[] = {
	/* 16 words of screen mask */
	0x3FFF, /*0011111111111111*/
	0x1FFF, /*0001111111111111*/
	0x0FFF, /*0000111111111111*/
	0x07FF, /*0000011111111111*/
	0x03FF, /*0000001111111111*/
	0x01FF, /*0000000111111111*/
	0x00FF, /*0000000011111111*/
	0x007F, /*0000000001111111*/
	0x01FF, /*0000000111111111*/
	0x10FF, /*0001000011111111*/
	0xB0FF, /*1011000011111111*/
	0xF87F, /*1111100001111111*/
	0xF87F, /*1111100001111111*/
	0xFC3F, /*1111110000111111*/
	0xFC3F, /*1111110000111111*/
	0xFE7F, /*1111111001111111*/
};
word mouseIn[] = {
	/* 16 words of cursor mask */
	0x0000, /*0000000000000000*/
	0x4000, /*0100000000000000*/
	0x6000, /*0110000000000000*/
	0x7000, /*0111000000000000*/
	0x7800, /*0111100000000000*/
	0x7C00, /*0111110000000000*/
	0x7E00, /*0111111000000000*/
	0x7F00, /*0111111100000000*/
	0x6C00, /*0110110000000000*/
	0x4600, /*0100011000000000*/
	0x0600, /*0000011000000000*/
	0x0300, /*0000001100000000*/
	0x0300, /*0000001100000000*/
	0x0180, /*0000000110000000*/
	0x0180, /*0000000110000000*/
	0x0000, /*0000000000000000*/
};
struct m{
	bitMap *oldImage;
	bitMap *out, *in;
}*mouse;

char *expand(word mask[16], int w, int h) {
	int i, j;
	char *ret = (char*)malloc(w * h * 3 * sizeof(char));
	for (i = 0; i<h; i++) {
		for (j = 0; j<w; j++) {
			if (mask[i] & 1 << (w - 1 - j)) {
				ret[(i*w + j) * 3] = 255;
				ret[(i*w + j) * 3 + 1] = 255;
				ret[(i*w + j) * 3 + 2] = 255;
			}
			else {
				ret[(i*w + j) * 3] = 0;
				ret[(i*w + j) * 3 + 1] = 0;
				ret[(i*w + j) * 3 + 2] = 0;
			}
		}
	}
	return ret;
}

void sgSetup() {
	Window->winWidth = 160;
	Window->winHeight = 160;
	strcpy(Window->winName, "Rythm Master --SGL Sample Game");

	mouse = (struct m*)malloc(sizeof(struct m));
	mouse->oldImage = (bitMap*)malloc(MOUSE_SIZE*MOUSE_SIZE * 3 * sizeof(char));
	mouse->out = (bitMap*)malloc(MOUSE_SIZE*MOUSE_SIZE * 3 * sizeof(char));
	mouse->in = (bitMap*)malloc(MOUSE_SIZE*MOUSE_SIZE * 3 * sizeof(char));
	mouse->out->sizeX = mouse->out->sizeY = mouse->in->sizeX = mouse->in->sizeY = MOUSE_SIZE;
	mouse->out->data = expand(mouseOut, MOUSE_SIZE, MOUSE_SIZE);
	mouse->in->data = expand(mouseIn, MOUSE_SIZE, MOUSE_SIZE);

	initKey();
	initMouse(SG_COORDINATE);
	hideMouse();
}
void sgLoop() {
	static int first = 1;
	static vecTwo tmp;
	if (first) {
		setColor(255, 255, 255);
		clearScreen();
		first = 0;
	}
	else putImage(tmp.x, tmp.y, mouse->oldImage, COPY_PUT);
	tmp = mousePos();
	getImage(tmp.x, tmp.y, tmp.x + MOUSE_SIZE - 1, tmp.y + MOUSE_SIZE - 1, mouse->oldImage);
	putImage(tmp.x, tmp.y, mouse->out, AND_PUT);
	putImage(tmp.x, tmp.y, mouse->in, OR_PUT);
	if (biosKey(1)) {
		exit(0);
	}
}

