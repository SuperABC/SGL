/*
* Copyright (c) 2016-2020, Super GP Individual.
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


#include "winsgl.h"
#include <windows.h>
#include <shlobj.h>


//Inner macros.

#define SGL_ASSERT(exp, text) {if(!!(exp));else{MessageBox(NULL, _widen(text), _widen("SGL_ASSERT"), 0);}}
#define TIMER_DELTA_HANDLE 1
#define WM_TRAY (WM_USER + 1)


//Char unification.

#ifdef UNICODE
#define _widen(str) String(str).widen()
#define _shorten(str) String(str).shorten()
#define _strcpy(dst, src) String::copy(dst, src)
#define _strlen(str) String::len(str)
#define _strcat(str, suffix) String::connect(str, suffix)
#else
#define _widen(str)  str
#define _shorten(str)  str
#define _strcpy(dst, src) strcpy((char *)dst, src)
#define _strlen(str) strlen(str)
#define _strcat(str, suffix) strcat(str, suffix)
#endif


//Thread lock.

#define SEM_INIT() extern long _windowSem;
#define SEM_P() InterlockedDecrement(&_windowSem);while(_windowSem<0);
#define SEM_V() InterlockedIncrement(&_windowSem);

SEM_INIT()


//Main function declaration.

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SubWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);


//System structures.

struct _text {
	HDC memDC;
	HFONT font;
	HBITMAP hbm;
	BITMAP bm;
	BITMAPINFOHEADER bi;
	unsigned char *bitBuf;
	int bufSize;
	SIZE strRect;
};
struct _mci {
	SGWINSTR buffer;
	int num;

	int length[SG_MCI_MAX_NUM];
};

enum _menuType {
	MT_SUB,
	MT_MAIN,
	MT_TRAY,
	MT_POPUP
};
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


//System functions.

void _startSubWindow(int id);
void _endSubWindow();

void _sgInit();
void _sgSubInit(vect setup);

void widgetCover(int window, int id, int left, int top, int right, int bottom);

#endif