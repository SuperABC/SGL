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
typedef char *SGstring;
typedef void(*vect)(void);
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

void setColor(int r, int g, int b);
void clearScreen();
void putPixel(int x, int y);
RGB getPixel(int x, int y);
void putLine(int x1, int y1, int x2, int y2, int mode);
void putQuad(int x1, int y1, int x2, int y2, int mode);
void putCircle(int xc, int yc, int r, int mode);
void getImage(int left, int top, int right, int bottom, bitMap *bitmap);
void putImage(int left, int top, bitMap *bitmap, int op);
int loadBmp(int x, int y, char *filename);
void putString(char *str, int x, int y);
void floodFill(int x, int y, RGB c);
void initKey();
int biosKey(int cmd);
void clearKeyBuffer();
void initMouse(int mode);
vecTwo mousePos();
int mouseStatus(int b);
vecThree biosMouse(int cmd);
void clearMouseBuffer();
void delay(int t);
void delayBegin();
int delayEnd(int t);
int random(int n);
vect getVect(int intn);
int setVect(int intn, vect v);
void dosInt(int intn, int *ret);
void setFreq(float f);
void fullScreen();
void showMouse();
void hideMouse();
int loadWave(char *filename, int mode);
void setActivePage(int page);
void setVisualPage(int page);
void putNumber(int n, int x, int y, char lr);
void putChar(char ch, int x, int y);

void funcMap(int x1, int x2, int y1, int y2, float(*vect)(int x));
