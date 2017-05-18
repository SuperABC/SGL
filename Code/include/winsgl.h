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
#define _SGL_V211

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
#define SG_LINE_DELTA_DEFAULT 20
#define SG_QSIZE 32768

//SG io macros
#define SG_SND_LOOP SND_LOOP
#define SG_SND_NOSTOP SND_NOSTOP
#define SG_SND_PURGE SND_PURGE
#define SG_SND_ONCE NULL

#define SG_ACCURATE 0
#define SG_COORDINATE 1

#define SG_BUTTON_UP 0x80
#define SG_BUTTON_DOWN 0x00
#define SG_KEY_UP 0x80
#define SG_KEY_DOWN 0x00

#define SG_LEFT_BUTTON 0x01
#define SG_RIGHT_BUTTON 0x02
#define SG_MIDDLE_BUTTON 0x04
#define SG_MIDDLE_BUTTON_UP 0x08
#define SG_MIDDLE_BUTTON_DOWN 0x10

#define SG_WINDOW 0
#define SG_SCREEN 1

#define WIDGET_BACK 0
#define WIDGET_FRONT 1

#define WIDGET_DEFAULT 0x00
#define WIDGET_PASS 0x01
#define WIDGET_PRESSED 0x02
#define WIDGET_SELECTED 0x04
#define WIDGET_FOCUSED 0x08

#define INRECT(x, y, xl, yl, xh, yh) (((x)>=(xl))&&((x)<=(xh))&&((y)>=(yl))&&((y)<=(yh)))

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
	SG_LEFT = 0x100,
	SG_UP,
	SG_RIGHT,
	SG_DOWN,
	SG_CTRL,
	SG_ALT,
	SG_SHIFT,
	SG_INSERT,
	SG_DELETE,
	SG_HOME,
	SG_END,
	SG_NUMLOC,
	SG_CAPSLOC,
	SG_PAGEUP,
	SG_PAGEDOWN,
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
enum _line {
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
	DASH_WRAP,
	CROSS_FILL,
	DOT_FILL
};
enum _control {
	SG_BUTTON,
	SG_INPUT,
	SG_DIALOG,
	SG_OUTPUT,
	SG_LIST,
	SG_LABEL,
	SG_CHECK,
	SG_PROCESS,
	SG_OPTION
};
enum _style {
	SG_DESIGN,
	WIN_XP,
	WIN_10,
	LINUX,
	ANDROID,
	WEBSITE,
};
enum _errors {
	SG_NO_ERORR = 0,
	SG_OBJECT_NOT_FOUND = -1,
	SG_NO_LOAD_MEM = -2,
	SG_INVALID_MODE = -3,
	SG_IO_ERROR = -4,
	SG_INVALID_VERSION = -5,
	SG_SIZE_MISMATCH = -6,
	SG_OUT_OF_RANGE = -7,
	SG_NULL_POINTER = -8,
	SG_INCOMPLETE_STRUCT = -9,
	SG_MULTY_VALUE = -10
};

//type defines.
typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned long int dword;
typedef void(*vect)(void);
typedef void(*mouseMoveCall)(void *w, int x, int y);
typedef void(*mouseClickCall)(void *w, int x, int y, int status);
typedef void(*keyCall)(void *w, int key);

typedef void SGvoid;
typedef int SGint;
typedef char SGchar;
typedef unsigned char *SGstring;
typedef float SGfloat;
typedef double SGdouble;
typedef long SGlong;

typedef struct {
	int x, y;
}vecTwo;
typedef struct {
	int x, y, m;
}vecThree;
typedef struct {
	byte r, g, b;
}RGB;
typedef struct {
	int sizeX, sizeY;
	SGstring data;
}bitMap;
typedef struct {
	int width, height;
	short *content;
}textMap;
typedef struct _w{
	enum control type;

	vecTwo pos;
	vecTwo size;

	int visible;
	int priority;
	int status;
	int style;

	int hide;
	int value;
	SGstring name;
	SGstring content;
	SGstring helpMessage;

	bitMap *cover;
	struct _w *associate;

	mouseMoveCall mouseIn, mouseOut;
	mouseClickCall mouseDown, mouseUp, mouseClick;
	keyCall keyDown, keyUp, keyPress;

}widgetObj;

//Frame functions.
#ifdef __cplusplus
extern "C" {
#endif

//Widget callbacks.
void vectDefault(void);
void mouseMoveDefault(widgetObj *w, int x, int y);
void mouseMoveList(widgetObj *w, int x, int y);
void mouseMoveOption(widgetObj *w, int x, int y);
void mouseClickDefault(widgetObj *w, int x, int y, int status);
void mouseClickInput(widgetObj *w, int x, int y, int status);
void mouseClickDialog(widgetObj *w, int x, int y, int status);
void mouseClickList(widgetObj *w, int x, int y, int status);
void mouseClickCheck(widgetObj *w, int x, int y, int status);
void mouseClickProcess(widgetObj *w, int x, int y, int status);
void mouseClickOption(widgetObj *w, int x, int y, int status);
void keyDefault(widgetObj *w, int key);
void keyInput(widgetObj *w, int key);
void keyList(widgetObj *w, int key);

//SG interfaces
SGvoid initWindow(int width, int height, char *title, int mode);
SGvoid setWindow(int left, int up);
SGint getWidth(int obj);
SGint getHeight(int obj);
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
SGvoid showMouse();
SGvoid hideMouse();
SGvoid setMouse(int x, int y);
SGvoid setActivePage(int page);
SGvoid setVisualPage(int page);
widgetObj *newWidget(int type, SGstring name);
SGint registerWidget(widgetObj *obj);
widgetObj *getWidgetByIndex(int index);
widgetObj *getWidgetByName(char *name);
SGvoid showWidget(char *name);
SGvoid ceaseWidget(char *name);
SGint deleteWidgetByIndex(int index);
SGint deleteWidgetByName(char *name);

SGvoid setColor(int r, int g, int b);
SGvoid clearScreen();
SGint putPixel(int x, int y);
RGB getPixel(int x, int y);
SGvoid putLine(int x1, int y1, int x2, int y2, int mode);
SGvoid putQuad(int x1, int y1, int x2, int y2, int mode);
SGvoid putTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int mode);
SGvoid putCircle(int xc, int yc, int r, int mode);
SGvoid putEllipse(int xc, int yc, int a, int b, int mode);
SGint loadBmp(int x, int y, char *filename);
SGvoid putNumber(int n, int x, int y, char lr);
SGvoid putChar(char ch, int x, int y);
SGvoid putChinese(byte *ch, int x, int y);
SGvoid putString(SGstring str, int x, int y);
SGint putStringConstraint(SGstring str, int x, int y, int constraint);
SGint getImage(int left, int top, int right, int bottom, bitMap *bitmap);
SGvoid putImage(int left, int top, bitMap *bitmap, int op);
SGint maskImage(int left, int top, bitMap *mask, bitMap *bitmap);
SGvoid funcMap(int x1, int x2, int y1, int y2, float(*vect)(float x));
SGvoid floodFill(int x, int y, RGB c);

SGvoid setBfc(int bgc, int fgc);
SGvoid clearText();
SGvoid setCharColor(char color, int x, int y);
SGvoid setCharBgc(char color, int x, int y);
SGvoid setCharFgc(char color, int x, int y);
SGint getShort(int x, int y);
SGvoid writeChar(char c, int x, int y);
SGvoid writeString(char *s, int x, int y);
SGint getText(int left, int top, int right, int bottom, textMap *text);
SGvoid putText(int left, int top, textMap *text);

void sgSetup();
void sgLoop();

#ifdef __cplusplus
}
#endif
#endif
