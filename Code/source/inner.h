#ifndef SGL_INNER_H
#define SGL_INNER_H

#include <windows.h>
#include "..\include\winsgl.h"

#define INTERUPT_DELTA_HANDLE 1
#define TIMER_DELTA_HANDLE 2

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
	vecTwo Pos;
	vecThree mouseBuf[1024];
	int front, rear;
	int left, middle, right;
	int coord;
};
struct _vect {
	vect _v8, _v9;
};
struct _widget {
	widgetObj *obj[SG_QSIZE];
	struct _hash *hash[256];
	int count;
	int active;
} *Widget;
struct _text {
	HDC memDC;
	HFONT font;
	HBITMAP hbm;
	BITMAP bm;
	BITMAPINFOHEADER bi;
	char *bitBuf;
	int bufSize;
};
typedef struct {
	RGB color;
	int size;
	char *name;
	int coeff;
}font;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void _makeBitmap(HDC hdc, LPBYTE lpBits, long width, long height, WORD wBitCount);
LPWSTR _widen(const char *src);
int _strlenW(const char *str);
void _prepareText(int width);

void sgInit();
void sgKey(int cAscii, int x, int y);
void sgSpecial(int cAscii, int x, int y);
void sgKeyUp(int cAscii, int x, int y);
void sgSpecialUp(int cAscii, int x, int y);
void sgMouse(int x, int y);
void sgDrag(int x, int y);
void sgClick(int button, int state, int x, int y);

void _textLoop(void);
void _textRenew(short c, int x, int y);
void _textAdd(short c, int x, int y);
RGB _bgc(short c);
RGB _fgc(short c);

void _drawWidget(int fb);
void _drawButton(widgetObj *w);
void _drawInput(widgetObj *w);
void _drawDialog(widgetObj *w);
void _drawOutput(widgetObj *w);
void _drawList(widgetObj *w);
void _drawLable(widgetObj *w);
void _drawCheck(widgetObj *w);
void _drawProcess(widgetObj *w);
void _drawOption(widgetObj *w);
void _drawDrag(widgetObj *w);


#endif