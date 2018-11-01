/*
 * Copyright (c) 2016-2018, Super GP Individual.
 * All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this library for any
 * purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright
 * notice and this permission notice appear in supporting documentation,
 * and that the name of SGL not be used in advertising or publicity
 * pertaining to distribution of the software without specific,
 * written prior permission.
 */



#ifndef SGL_INNER_H
#define SGL_INNER_H


#include <windows.h>
#include <Shlobj.h>
#include "winsgl.h"


#define SCROLL_WIDTH 20
#define MINIMAL_BAR 24

#ifdef UNICODE
#define _widen(str)  _widenStr(str)
#define _shorten(str)  _shortenStr(str)
#define _strcpy(dst, src) wcscpy(dst, src)
#else
#define _widen(str)  str
#define _shorten(str)  str
#define _strcpy(dst, src) strcpy(dst, src)
#endif

#define INTERUPT_DELTA_HANDLE 1
#define TIMER_DELTA_HANDLE 2

#define WM_TRAY (WM_USER + 1)

#define SEM_INIT() extern long windowSem;
#define SEM_P() InterlockedDecrement(&windowSem);while(windowSem<0);
#define SEM_V() InterlockedIncrement(&windowSem);

SEM_INIT()

enum _menuType {
	MT_SUB,
	MT_MAIN,
	MT_TRAY,
	MT_POPUP
};

struct _hash {
	char *content;
	int value;
	struct _hash *next;
};

struct _win {
	int winWidth, winHeight;
	int txtWidth, txtHeight;
	int posLeft, posUp;
	HWND hwnd;
	HICON hIcon;
	char *winName;
};
struct _screen {
	bitMap *buffer1, *buffer2;
	textMap *content, *changed, *textMask;
	byte rgb[3];
	float alpha;
	byte bfc;
};
struct _key {
	word keyBuf[1024];
	int front, rear;
};
struct _mouse {
	vec2 Pos;
	vec3 mouseBuf[1024];
	int front, rear;
	int left, middle, right;
	int coord;
};
struct _vect {
	vect _v8, _v9;
};
struct _text {
	HDC memDC;
	HFONT font;
	HBITMAP hbm;
	BITMAP bm;
	BITMAPINFOHEADER bi;
	char *bitBuf;
	int bufSize;
	SIZE strRect;
};
struct _midi {
	HMIDIOUT hMidiOut;
};
struct _mci {
	SGWINSTR buffer;
	int num;

	int length[SG_MCI_MAX_NUM];
};
struct _widget {
	widgetObj *obj[SG_QSIZE];
	struct _hash *hash[256];
	int count;
	int active;
} *Widget;

struct _item;
struct _menu;
struct _item {
	HMENU hm;
	char *name;
	int id;
	int checked;
	void(*func)();
	struct _menu *sub;
};
struct _menu {
	HMENU hm;
	char *name;
	int id;
	struct _item *sub[SG_MAX_MENU_ITEM_NUM];
};
struct _function;
struct _panel;
struct _function {
	int id;
	char *name;
	vect function;
};
struct _panel {
	int maxId;

	int x, y, outer, inner;
	bitMap *cover;
	struct _function *normalPanel[SG_MAX_PANEL_FUNCTION];
	struct _function *shiftPanel[SG_MAX_PANEL_FUNCTION];
	struct _function *ctrlPanel[SG_MAX_PANEL_FUNCTION];
	struct _function *shiftctrlPanel[SG_MAX_PANEL_FUNCTION];
};

typedef LRESULT(*subWndProc)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
struct _Sub {
	int winWidth, winHeight;
	HWND hwnd;
	HICON hIcon;
	char *winName;
	int scrResizeable;
	int inLoop;
	int drawingWidget;

	void(*resizeFunc)(int x, int y);
	subWndProc wndProc;

	bitMap *buffer1, *buffer2;
	byte rgb[3];
	float alpha;

	font tf;
	struct _text text;

	int visualPage, activePage;
	vect loop;

	struct _key *key;
	struct _mouse *mouse;

	struct _widget *widget;
	void(*background)(int left, int top, int right, int bottom);
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void _makeBitmap(HDC hdc, LPBYTE lpBits, long width, long height, WORD wBitCount);
void _makeSubBitmap(HDC hdc, int id, LPBYTE lpBits, long width, long height, WORD wBitCount);
int _wcharAt(const char *src, int pos);
int _scharAt(const char *src, int pos);
LPWSTR _widenStr(const char *src);
char *_shortenStr(const LPWSTR src);
int _strlenW(const char *str);
void _prepareText(int width);
void _prepareSubText(int width);
void _setFontName(SGWINSTR name);
void _midiout(HMIDIOUT hMidi, int iStatus, int iChannel, int iData1, int iData2);
int _mciSend(const char *cmd);
int _addList(const char *name, struct _menu *super, int id);
int _addItem(const char *name, struct _menu *super, int id, void(*func)());
int _addSeparator(struct _menu *super, int id);
void _checkItem(struct _menu *m, int id, int check, int disable);
void _createMenu(enum _menuType type, struct _item *it, HMENU super);
void _callMenu(struct _menu *m, int id);
int _stringPrintf(const char *format, va_list ap, int x, int y);
void _drawPanel();
void _clearPanel();
int _checkThread();
void _deleteSub(widgetObj *w);
void _bgDrawDefault(int left, int top, int right, int bottom);

void _textLoop(void);
void _textRenew(short c, int x, int y);
void _textAdd(short c, int x, int y);
RGB _bgc(short c);
RGB _fgc(short c);

void sgInit();
void sgKey(int cAscii, int x, int y);
void sgSpecial(int cAscii, int x, int y);
void sgKeyUp(int cAscii, int x, int y);
void sgSpecialUp(int cAscii, int x, int y);
void sgMouse(int x, int y);
void sgDrag(int x, int y);
void sgClick(int button, int state, int x, int y);
void sgWheel(int dir);
void sgSubInit(vect setup);
void sgSubKey(int id, int cAscii, int x, int y);
void sgSubSpecial(int id, int cAscii, int x, int y);
void sgSubKeyUp(int id, int cAscii, int x, int y);
void sgSubSpecialUp(int id, int cAscii, int x, int y);
void sgSubMouse(int id, int x, int y);
void sgSubDrag(int id, int x, int y);
void sgSubClick(int id, int button, int state, int x, int y);
void sgSubWheel(int id, int dir);

void _drawWidget(int fb);
void _drawSubWidget(int id, int fb);
void _drawButton(widgetObj *w);
void _drawInput(widgetObj *w);
void _drawDialog(widgetObj *w);
void _drawOutput(widgetObj *w);
void _drawList(widgetObj *w);
void _drawLable(widgetObj *w);
void _drawPic(widgetObj *w);
void _drawCheck(widgetObj *w);
void _drawProcess(widgetObj *w);
void _drawOption(widgetObj *w);
void _drawDrag(widgetObj *w);
void _drawScrollVert(widgetObj *w);
void _drawScrollHoriz(widgetObj *w);
void _drawCombined(widgetObj *w);


#endif