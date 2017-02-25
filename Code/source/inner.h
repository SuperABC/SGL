#pragma once
#include "sgl.h"

struct _screen {
	bitMap *buffer1, *buffer2;
	textMap *content, *changed;
	GLuint texName;
	GLuint rgb[3];
	GLuint bfc;
};
struct _key {
	word keyBuf[1024];
	int front, rear;
};
struct _mouse {
	vecTwo Pos;
	vecThree mouseBuf[1024];
	int front, rear;
	int left, middle, right;
	int clicked, coord;
};
struct _vect {
	vect _v8, _v9;
};

static void sgInit();
static void sgReshape(int w, int h);
static void sgDisplay(void);
static void sgIdle(void);
static void sgMouse(int x, int y);
static void sgDrag(int x, int y);
static void sgClick(int button, int state, int x, int y);
static void sgWheel(int wheel, int dir, int x, int y);
static void sgKeyUp(unsigned char cAscii, int x, int y);
static void sgKey(unsigned char cAscii, int x, int y);
static void sgSpecialUp(GLint cAscii, int x, int y);
static void sgSpecial(GLint cAscii, int x, int y);

void makeFont(void);
void vectDefault(void);
void textLoop(void);
void textRenew(short c, int x, int y);
RGB bgc(short c);
RGB fgc(short c);