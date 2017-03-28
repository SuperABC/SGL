#ifndef SGL_H
#define SGL_H

#ifdef _DEBUG
#define SG_LIB(name) name "d.lib"
#else
#define SG_LIB(name) name ".lib"
#endif
#define SG_A(name) name ".a"

//#pragma comment(lib, SG_LIB("winsgl"))
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#ifdef __cplusplus
#include <iostream>
#include <fstream>
#endif

//SG const macros
#define SG_PI 3.14159265358979323846
#define SG_CHAR_WIDTH 8
#define SG_CHAR_HEIGHT 16
#define SG_QSIZE 32768

//SG io macros
#define SG_SND_LOOP SND_LOOP
#define SG_SND_NOSTOP SND_NOSTOP
#define SG_SND_PURGE SND_PURGE
#define SG_SND_ONCE NULL
#define SG_COORDINATE 1
#define SG_ACCURATE 0
#define SG_BUTTON_UP 0
#define SG_BUTTON_DOWN 1
#define SG_LEFT_BUTTON 1
#define SG_RIGHT_BUTTON 2
#define SG_MIDDLE_BUTTON 3
#define SG_MIDDLE_BUTTON_UP 4
#define SG_MIDDLE_BUTTON_DOWN 5

//type defines.
typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned long int dword;
typedef void(*vect)(void);
typedef void(*func)(double, double);

typedef void SGvoid;
typedef int SGint;
typedef char SGchar;
typedef char *SGstring;
typedef float SGfloat;
typedef double SGdouble;
typedef long SGlong;

struct _win {
	int winWidth, winHeight;
	int txtWidth, txtHeight;
	int posLeft, posUp;
	int timeFreq;
	HWND hwnd;
	char *winName;
};

typedef struct {
	int x, y;
}vecTwo;
typedef struct {
	int x, y, m;
}vecThree;
typedef struct {
	int r, g, b;
}RGB;
typedef struct {
	int sizeX, sizeY;
	unsigned char *data;
}bitMap;
typedef struct {
	int width, height;
	short *content;
}textMap;

//SG enums.
enum _colors {
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	DARKGRAY,
	LIGHTGRAY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTCYAN,
	LIGHTRED,
	LIGHTMAGENTA,
	YELLOW,
	WHITE
};
enum _mode {
	BIT_MAP,
	TEXT_MAP
};
enum _ascii {
	SG_TAB = '\t',
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
	SOLID_LINE,
	DOTTED_LINE,
	CENTER_LINE,
	DASHED_LINE,
	USERBIT_LINE
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
	SG_NO_ERORR = 0,
	SG_FILE_NOT_FOUND = -1,
	SG_NO_LOAD_MEM = -2,
	SG_INVALID_MODE = -3,
	SG_IO_ERROR = -4,
	SG_INVALID_VERSION = -5,
	SG_SIZE_MISMATCH = -6,
	SG_OUT_OF_RANGE = -7
};

//Frame functions.
#ifdef __cplusplus
extern "C" {
#endif

//SG interfaces
SGvoid initWindow(int width, int height, char *title, int mode);
SGvoid setWindow(int left, int up);
SGvoid setColor(int r, int g, int b);
SGvoid setBfc(int bgc, int fgc);
SGvoid setCharColor(char color, int x, int y);
SGvoid setCharBgc(char color, int x, int y);
SGvoid setCharFgc(char color, int x, int y);
SGvoid clearScreen();
SGvoid clearText();
SGint putPixel(int x, int y);
RGB getPixel(int x, int y);
SGint getShort(int x, int y);
SGvoid putLine(int x1, int y1, int x2, int y2, int mode);
SGvoid putQuad(int x1, int y1, int x2, int y2, int mode);
SGvoid putCircle(int xc, int yc, int r, int mode);
SGvoid putEllipse(int xc, int yc, int a, int b, int mode);
SGint getImage(int left, int top, int right, int bottom, bitMap *bitmap);
SGvoid putImage(int left, int top, bitMap *bitmap, int op);
SGint getText(int left, int top, int right, int bottom, textMap *text);
SGvoid putText(int left, int top, textMap *text);
SGint loadBmp(int x, int y, char *filename);
SGvoid putString(char *str, int x, int y);
SGvoid writeString(char *s, int x, int y);
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
//SGvoid fullScreen();
SGvoid showMouse();
SGvoid hideMouse();
SGvoid setMouse(int x, int y);
SGvoid setActivePage(int page);
SGvoid setVisualPage(int page);
SGvoid putNumber(int n, int x, int y, char lr);
SGvoid putChar(char ch, int x, int y);
SGvoid writeChar(char c, int x, int y);
SGint maskImage(int left, int top, bitMap *mask, bitMap *bitmap);
SGvoid funcMap(int x1, int x2, int y1, int y2, float(*vect)(int x));

#ifdef __cplusplus
}
#endif
#endif
