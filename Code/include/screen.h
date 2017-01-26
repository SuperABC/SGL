#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define FREEGLUT_STATIC
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <Windows.h>
#include "freeglut.h"
#define GL_Lib(name) name "d.lib"
#pragma comment(lib, GL_Lib("freeglut_static"))

#define SG_PI 3.14159265358979323846
#define SG_QSIZE 32768

#define SG_LOOP SND_LOOP
#define SG_COORDINATE 1
#define SG_ACCURATE 0

#define SG_BUTTON_UP 0
#define SG_BUTTON_DOWN 1
#define SG_LEFT_BUTTON 1
#define SG_RIGHT_BUTTON 2
#define SG_MIDDLE_BUTTON 3
#define SG_MIDDLE_BUTTON_UP 4
#define SG_MIDDLE_BUTTON_DOWN 5

typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned long int dword;
typedef void(*vect)(void);

typedef void SGvoid;
typedef int SGint;
typedef char SGchar;
typedef char *SGstring;
typedef float SGfloat;
typedef double SGdouble;
typedef long long SGlong;

typedef struct {
	int x, y;
}vecTwo;
typedef struct  {
	int x, y, m;
}vecThree;
typedef struct {
	int r, g, b;
}RGB;
typedef struct {
	GLint sizeX, sizeY;
	unsigned char *data;
}bitMap;

enum _ascii {
	SG_ESC = 0x1b,
	SG_BACKS = '\b',
	SG_ENTER = '\r',
	SG_UP = 0x100,
	SG_DOWN,
	SG_LEFT,
	SG_RIGHT,
	SG_LCTRL,
	SG_RCTRL,
	SG_LALT,
	SG_RALT,
	SG_LSHIFT,
	SG_RSHIFT,
	SG_INSERT,
	SG_DELETE,
	SG_HOME,
	SG_END,
	SG_NUMLOC,
	SG_PAGEUP,
	SG_PAGEDOWN,
	SG_BEGIN,
	SG_F1,
	SG_F2,
	SG_F3,
	SG_F4,
	SG_F5,
	SG_F6,
	SG_F7,
	SG_F8,
	SG_F9,
	SG_F10,
	SG_F11,
	SG_F12
};
enum _style {
	SOLID_LINE = 0,
	DOTTED_LINE = 1,
	CENTER_LINE = 2,
	DASHED_LINE = 3,
	USERBIT_LINE = 4
};
enum _piccpy {
	COPY_PUT,
	XOR_PUT,
	OR_PUT,
	AND_PUT,
	NOT_PUT 
};
enum _fill {
	EMPTY_FILL,
	SOLID_FILL,
	CROSS_FILL,
	DOT_FILL
};
enum _errors {
	SG_NONE = 0,
	SG_FILE_NOT_FOUND = -1,
	SG_NO_LOAD_MEM = -2,
	SG_INVALID_MODE = -3,
	SG_IO_ERROR = -4,
	SG_INVALID_VERSION = -5
};

struct _screen {
	bitMap *buffer1, *buffer2;
	GLuint texName;
	GLuint rgb[3];
};
struct _win {
	int winWidth, winHeight;
	char *winName;
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

void sgSetup();
void sgLoop();

SGvoid setColor(int r, int g, int b);
SGvoid clearScreen();
SGvoid putPixel(int x, int y);
RGB getPixel(int x, int y);
SGvoid putLine(int x1, int y1, int x2, int y2, int mode);
SGvoid putQuad(int x1, int y1, int x2, int y2, int mode);
SGvoid putCircle(int xc, int yc, int r, int mode);
SGvoid getImage(int left, int top, int right, int bottom, bitMap *bitmap);
SGvoid putImage(int left, int top, bitMap *bitmap, int op);
SGint loadBmp(int x, int y, char *filename);
SGvoid putString(char *str, int x, int y);
SGvoid floodFill(int x, int y, RGB c);
SGvoid initKey();
SGint biosKey(int cmd);
SGvoid clearKeyBuffer();
SGvoid initMouse(int mode);
vecTwo mousePos();
SGint mouseStatus(int b);
vecThree biosMouse(int cmd);
SGvoid clearMouseBuffer();
SGvoid delay(int t);
SGvoid delayBegin();
SGint delayEnd(int t);
SGint random(int n);
vect getVect(int intn);
SGint setVect(int intn, vect v);
SGvoid dosInt(int intn, int *ret);
SGvoid setFreq(float f);
SGvoid fullScreen();
SGvoid showMouse();
SGvoid hideMouse();
SGint loadWave(char *filename, int mode);
SGvoid setActivePage(int page);
SGvoid setVisualPage(int page);
SGvoid putNumber(int n, int x, int y, char lr);
SGvoid putChar(char ch, int x, int y);

SGvoid funcMap(int x1, int x2, int y1, int y2, float(*vect)(int x));
