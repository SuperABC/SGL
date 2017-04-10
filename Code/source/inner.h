#ifndef SGL_INNER_H
#define SGL_INNER_H

#include <windows.h>
#include "winsgl.h"

#define INTERUPT_DELTA_HANDLE 1
#define TIMER_DELTA_HANDLE 2

struct _screen {
	bitMap *buffer1, *buffer2;
	textMap *content, *changed;
	byte rgb[3];
	byte bfc;
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
	int coord;
};
struct _vect {
	vect _v8, _v9;
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void  makeBitmap(HDC hdc, LPBYTE lpBits, long width, long height, WORD wBitCount);

void sgInit();
void sgKey(int cAscii, int x, int y);
void sgSpecial(int cAscii, int x, int y);
void sgKeyUp(int cAscii, int x, int y);
void sgSpecialUp(int cAscii, int x, int y);
static void sgMouse(int x, int y);
static void sgDrag(int x, int y);
static void sgClick(int button, int state, int x, int y);

void makeFont(void);
void vectDefault(void);
void textLoop(void);
void textRenew(short c, int x, int y);
LPWSTR widen(const char *src);
RGB bgc(short c);
RGB fgc(short c);

void sgSetup();
void sgLoop();

#endif