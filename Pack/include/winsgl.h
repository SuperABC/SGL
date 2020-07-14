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



#ifndef SGL_H
#define SGL_H


//SGL standard macros.

#define _SGL_V503
#define _SGL_VERSION_STRING "v5.0.3"
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS


//SGL libraries.

#ifdef _DEBUG
#define SG_LIB(name) name "d.lib"
#else
#define SG_LIB(name) name ".lib"
#endif
#define SG_A(name) name ".a"

#pragma comment(lib, SG_LIB("winsgl"))
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "shlwapi.lib")


//SGL includes.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <winsock.h>

#include "geometry.h"

#ifdef __cplusplus
#include <iostream>
#include <fstream>
#include <string>
#endif


//SG const macros.

#define SG_PI 3.14159265358979323846

#define SG_CHAR_WIDTH 8
#define SG_CHAR_HEIGHT 16
#define SG_LINE_DELTA_DEFAULT 20
#define SG_DEBUGF_MAX_LENGTH 1024
#define SG_MINI_QSIZE 32
#define SG_QSIZE 32768
#define SG_HASH_NUM 256
#define SG_MAX_FONT_SIZE 128
#define SG_MAX_FONT_LENGTH 128
#define SG_MAX_MENU_ITEM_NUM 128
#define SG_MAX_CONNECTION 4096
#define SG_MCI_BUFFER_SIZE 256
#define SG_MCI_MAX_NUM 256
#define SG_MAX_POPUP_NUM 16
#define SG_MAX_FORMAT_FILTERS 256
#define SG_MAX_WINDOW_NUM 256
#define SG_MAX_WIDGET_NUM 256
#define SG_MAX_PANEL_FUNCTION 12
#define SG_MIN_WIDGET_CONTENT 1024


//SG io macros.

#define SG_BUTTON_UP 0x80
#define SG_BUTTON_DOWN 0x00
#define SG_KEY_UP 0x8000
#define SG_KEY_DOWN 0x0000

#define SG_LEFT_BUTTON 0x01
#define SG_RIGHT_BUTTON 0x02
#define SG_MIDDLE_BUTTON 0x04
#define SG_MIDDLE_BUTTON_UP 0x08
#define SG_MIDDLE_BUTTON_DOWN 0x10

#define SG_WINDOW 0
#define SG_CANVAS 1


//SG widget macros.

#define WIDGET_BACK 0
#define WIDGET_FRONT 1

#define WIDGET_DEFAULT 0x00
#define WIDGET_PASS 0x01
#define WIDGET_PRESSED 0x02
#define WIDGET_SELECTED 0x04

#define INRECT(x, y, xl, yl, xh, yh) (((x)>=(xl))&&((x)<=(xh))&&((y)>=(yl))&&((y)<=(yh)))

// Windows string.

#ifdef UNICODE
#define SGWINSTR  LPWSTR
#else
#define SGWINSTR  LPCSTR
#endif


//Multiple thread.

#define NEW_THREAD_FUNC(name) DWORD WINAPI name(LPVOID param)


//SG enums.

enum _colors { //4 bit standard color.
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
enum _mode { //bit mode or text mode.
	BIT_MAP,
	TEXT_MAP
};
enum _ascii { //Extended key code specially for SGL.
	SG_TAB = '\t',
	SG_ESC = 0x1b,
	SG_BACKS = '\b',
	SG_ENTER = '\r',
	SG_LEFT = 0x100,
	SG_UP,
	SG_RIGHT,
	SG_DOWN,
	SG_CTRL,
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
	SG_F12,
	SG_SHIFTBIT = 0x2000,
	SG_CTRLBIT = 0x4000
};
enum _line { //Line modes.
	SOLID_LINE,
	DOTTED_LINE,
	CENTER_LINE,
	DASHED_LINE,
	USERBIT_LINE
};
enum _piccpy { //Bitmap mix modes.
	COPY_PUT,
	XOR_PUT,
	OR_PUT,
	AND_PUT,
	NOT_PUT
};
enum _fill { //Sapes filling modes.
	EMPTY_FILL,
	SOLID_FILL,
	DASH_WRAP,
	CROSS_FILL,
	DOT_FILL
};
enum _control { //Types of widget.
	SG_BUTTON,
	SG_INPUT,
	SG_DIALOG,
	SG_OUTPUT,
	SG_LIST,
	SG_LABEL,
	SG_PIC,
	SG_CHECK,
	SG_PROCESS,
	SG_DRAG,
	SG_SCROLLVERT,
	SG_SCROLLHORIZ,

	SG_COMBINED
};
enum _style { //Styles of widget.
	SG_DESIGN,
	WIN_XP,
	WIN_10,
	LINUX,
	ANDROID,
	WEBSITE,
};
enum _filter {
	SGL_FILTER,
	MEAN_FILTER,
	LAPLACIAN_FILTER,
	BILATERAL_FILTER
};
enum _font {
	FONT_ITALIC = 1,
	FONT_UNDERLINE = 2,
	FONT_STRIKEOUT = 4
};
enum _alert {
	ALERT_BUTTON_ABORTRETRYIGNORE = MB_ABORTRETRYIGNORE,
	ALERT_BUTTON_OK = MB_OK,
	ALERT_BUTTON_OKCANCEL = MB_OKCANCEL,
	ALERT_BUTTON_RETRYCANCEL = MB_RETRYCANCEL,
	ALERT_BUTTON_YESNO = MB_YESNO,
	ALERT_BUTTON_YESNOCANCEL = MB_YESNOCANCEL,

	ALERT_ICON_EXCLAMATION = MB_ICONEXCLAMATION,
	ALERT_ICON_WARNING = MB_ICONWARNING,
	ALERT_ICON_INFORMATION = MB_ICONINFORMATION,
	ALERT_ICON_ASTERISK = MB_ICONASTERISK,
	ALERT_ICON_QUESTION = MB_ICONQUESTION,
	ALERT_ICON_STOP = MB_ICONSTOP,
	ALERT_ICON_ERROR = MB_ICONERROR,
	ALERT_ICON_HAND = MB_ICONHAND,

	ALERT_SYS_APPL = MB_APPLMODAL,
	ALERT_SYS_SYSTEM = MB_SYSTEMMODAL,
	ALERT_SYS_TASK = MB_TASKMODAL,
};
enum _errors { //Different return values when error occurs.
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
	SG_MULTY_VALUE = -10,
	SG_CONNECTION_FAILED = -11,
	SG_MEDIA_ERROR = -12,
	SG_WRONG_THREAD = -13,
};


//type defines.

typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned long int dword;
typedef void(*vect)(void);
typedef void(*vectInput)(void *param);
typedef DWORD(WINAPI *thread)(LPVOID lpThreadParameter);

typedef void SGvoid;
typedef char SGchar;
typedef unsigned char SGuchar;
typedef short SGshort;
typedef unsigned short SGushort;
typedef int SGint;
typedef unsigned int SGuint;
typedef long SGlong;
typedef unsigned long SGulong;
typedef char *SGstring;
typedef const char *SGtext;
typedef unsigned char *SGustring;
typedef unsigned const char *SGutext;
typedef float SGfloat;
typedef double SGdouble;
#ifdef __cplusplus
typedef bool SGbool;
#else
typedef char SGbool;
#endif

typedef struct {
	byte r, g, b;
}rgb;
typedef struct {
	int sizeX, sizeY;
	byte *data;
	byte *alpha;
}bitMap;
typedef struct {
	int width, height;
	unsigned short *content;
}textMap;
typedef struct {
	rgb color;
	int size;
	SGWINSTR name;
	int coeff;
}font;
typedef struct _w {
	enum _control type;

	vec2i pos;
	vec2i size;

	int style;

	SGstring name;
	void *content;
	int value, extra;

	font tf;
	struct _w *child, *next;

	rgb bgColor, passColor, pressColor, fgColor;
	bitMap bgImg;

	void(*click)(struct _w *obj);
	void(*move)(struct _w *obj, int x, int y);
	void(*drag)(struct _w *obj, int x, int y);
	void(*press)(struct _w *obj, int key);
}widget;



//data interfaces.

enum JSON_Type {
	JSON_INT,
	JSON_FLOAT,
	JSON_CHAR,
	JSON_BOOL,
	JSON_STRING,
	JSON_OBJECT,
	JSON_ARRAY
};
union JSON_Data {
	SGint json_int;
	SGfloat json_float;
	SGchar json_char;
	SGbool json_bool;
	SGstring json_string;
	struct JSON *json_object;
	struct JSON *json_array;
};
struct JSON_Item {
	enum JSON_Type t;

	SGstring name;
	union JSON_Data data;

	struct JSON_Item *next;
};

struct JSON {
	SGbool arr;

	union {
		struct JSON_Item *hash[SG_HASH_NUM];
		struct JSON_Item *list;
	};
};


//The active window id when operating the window or canvas.

extern int _currentWindow;


#ifdef __cplusplus
extern "C" {
#endif

	/*
	* SG window management
	* These functions controls the windows of the program and
	* can be used both in graphic and console mode, too.
	*/

	/**
	* Used in sgSetup, to set the parameters of the graphic window.
	* @Param width and height is the window size.
	* @Param title is the string displayed in caption bar.
	* @Param mode can be assigned either BIT_MAP or TEXT_MAP.
	*/
	SGvoid initWindow(int width, int height, SGtext title, int mode);

	/**
	* Used in sgSetup, to set the parameters of the circular graphic window.
	* @Param rx and ry is the x radius and y radius of the window.
	*/
	SGvoid initPolarWindow(int rx, int ry);

	/**
	* Create a new window with SGL canvas.
	* @Param width and height is the window size.
	* @Param title is the string displayed in caption bar.
	* @Param mode can be assigned either BIT_MAP or TEXT_MAP.
	* @Param setup and loop is the logic of the window created.
	* @Return returns the id of the window created.
	*/
	SGint createWindow(int width, int height, SGtext title, int mode, vect setup, vect loop);

	/**
	* Create a new window with SGL canvas.
	* @Param rx and ry is the x radius and y radius of the window.
	*/
	SGint createPolarWindow(int rx, int ry, vect setup, vect loop);

	/**
	* Used in setup or loop function to define what to do when
	* current window closed.
	* @Param finish is the function to be called when window closed.
	*/
	SGvoid windowFinish(vect finish);

	/**
	* Used in setup or loop function to define what to do when
	* current window size changed.
	* @Param func will be executed when the size of the window changed.
	*/
	SGvoid resizeFuntion(void(*func)(int x, int y));

	/**
	* Close the sub window.
	* @Param id determined which window should be closed.
	*/
	SGvoid closeWindow(int id);

	/**
	* Rename the caption of the window.
	* @Param name is the new caption name to be displayed.
	*/
	SGvoid renameCaption(SGtext name);

	/**
	* Hide the caption bar.
	*/
	SGvoid hideCaption();

	/**
	* Show the caption bar.
	*/
	SGvoid showCaption();


	/*
	* SG tool interfaces
	* These functions can be used not only in graphic modes but also in
	* console mode.
	*/

	/**
	* Get the width of one object.
	* @Param obj can be SG_WINDOW (the current window) or
	* SG_CANVAS(The screen of the computer).
	* @Return returns the width.
	*/
	SGint getWidth(int obj);

	/**
	* Get the height of one object.
	* @Param obj can be SG_WINDOW (the current window) or
	* SG_CANVAS(The screen of the computer).
	* @Return returns the height.
	*/
	SGint getHeight(int obj);

	/**
	* Get the size of one object.
	* @Param obj can be SG_WINDOW (the current window) or
	* SG_CANVAS(The screen of the computer).
	* @Return returns the size.
	*/
	vec2i getSize(int obj);

	/**
	* Print the message with format to the output window when debugging.
	* @Param format is the output format.
	* @Param ... is the value for the format.
	*/
	SGvoid debugf(SGtext format, ...);

	/**
	* Wait for t millisecond. During the time the window won't refresh.
	* @Param t is the time period.
	*/
	SGvoid delay(int t);

	/**
	* Used with delayEnd. To set the least time waiting between begin
	* and end. This pair of function is usually used to control the time
	* of some loops.
	*/
	SGvoid delayBegin();

	/**
	* Used with delayBegin. To set the least time waiting between begin
	* and end. This pair of function is usually used to cotrol the time
	* of some loops.
	* @Param t is the time period.
	* @Return return 1 if delayed, or else return 0.
	*/
	SGint delayEnd(int t);

	/**
	* Get a random number.
	* @Param n is the random range.
	* @Return a random integer between 0 and n - 1.
	*/
	SGint random(int n);

	/**
	* Use Win API Graphic mode to choose one file.
	* @Param name name is used to receive the selected file name with its path.
	* @Param start is the path to begin with so set it to NULL as default.
	* @Param format is the probable file format and each format is seperated
	* with \0, and it's set to NULL as default as well.
	* @Param idx is the default format index which is usually set to 1.
	*/
	SGint selectFile(char name[], SGWINSTR start, SGWINSTR format, int idx);

	/**
	* Use Win API Graphic mode to let user input the file name that they
	* want to save.
	* @Param name name is used to receive the selected file name with its path.
	* @Param start is the path to begin with so set it to NULL as default.
	* @Param format is the probable file format and each format is seperated
	* with \0, and it's set to NULL as default as well.
	* @Param def is the default format to append if no format is input.
	* @Param idx is the default format index which is usually set to 1.
	*/
	SGint selectSave(char name[], SGWINSTR start, SGWINSTR format, SGWINSTR def, int idx);

	/**
	* Use Win API Graphic mode to choose one directory.
	* @Param name name is used to receive the selected file name with its path.
	* @Param start is the path to begin with so set it to NULL as default.
	*/
	SGint selectDir(char name[], char start[]);

	/**
	* If the given folder does not exist, then create it.
	* @Param path is the path to find and create.
	*/
	SGint makePath(SGtext path);

	/**
	* Check if one file exists.
	* @Param path is the path of file.
	* @Return returns 1 if the given file exists, or else return 0.
	*/
	SGint fileExist(SGtext path);

	/**
	* Initialize the media(mp3) device.
	*/
	SGvoid initMci();

	/**
	* Load the mp3 file to memory,
	* @Param filename is the mp3 file name.
	* @Return the identifier of this mci.
	*/
	SGint loadMciSrc(SGtext filename);

	/**
	* Start playing music.
	* @Param id is the music identifier.
	*/
	SGint playMci(int id);

	/**
	* Stop playing music.
	* @Param id is the music identifier.
	*/
	SGint stopMci(int id);

	/**
	* Pause playing music.
	* @Param id is the music identifier.
	*/
	SGint pauseMci(int id);

	/**
	* Resume playing music.
	* @Param id is the music identifier.
	*/
	SGint resumeMci(int id);

	/**
	* Create a thread.
	* @Param func will be executed when new thread created.
	* @Param param will be passed to func when new thread created.
	*/
	SGvoid createThread(thread func, void *param);

	/**
	* Copy the given text into windows clipboard so that it can be pasted
	* to other programs.
	* @Param src is the source passed to window clipboard.
	*/
	SGint copyText(SGtext src);

	/**
	* Copy the given text into windows clipboard so that it can be pasted
	* to other programs.
	* @Return string from clipboard and it need to be free by the programmer.
	*/
	SGstring pasteText();

	/**
	* Set up a server(either local and remote).
	* @Param port is the port number. For diy communication, port should
	* be greater than 1023 and less than 65536.
	* @Return the socket of the server.
	*/
	SOCKET createServer(int port);

	/**
	* Set up a client and connect to server(localhost is "127.0.0.1").
	* @Param port is the port number. the port should be equal to the one
	* set by server.
	* @Return the socket of the client.
	*/
	SOCKET createClient(SGtext server, int port);

	/**
	* Used by a server to accept one request, one acceptOne should answer
	* one createClient.
	* @Param server is the listening server.
	* @Return the socket of the connection.
	*/
	SOCKET acceptOne(SOCKET server);

	/**
	* Used to send a string via one connection.
	* @Param s stands for the socket of the given connection.
	* @Param buffer is the sending data.
	*/
	SGint socketSend(SOCKET s, SGtext buffer);

	/**
	* Used to receive a string via one connection.
	* @Param s stands for the socket of the given connection.
	* @Param buffer reveives the data.
	* @Param len is the max buffer size. If more content is sending,
	* the rest will wait.
	* @Return the status of the revceiving. If the connection is stopped,
	* the return value is SG_CONNECTION_FAILED.
	*/
	SGint socketReceive(SOCKET s, SGstring buffer, int len);

	/**
	* When one the connection is cut, we should close the socket of this
	* connection.
	* @Param s is the socket to close.
	*/
	SGvoid closeSocket(SOCKET s);

	/**
	* Set the running-time icon for the window. The icon will appear
	* at top_left in the caption bar and in the tray.
	* @Param ico can be either recource path or pre-defined variables.
	*/
	SGvoid setIcon(SGtext ico);

	/**
	* Copy picture to a new area of memory.
	* @Param src is the copy source.
	* @Return the new area.
	*/
	bitMap copyPic(bitMap src);

	/**
	* Copy picture to a new area of memory and change it to grayscale picture.
	* @Param src is the handle source.
	* @Return the handle result.
	*/
	bitMap grayPic(bitMap src);

	/**
	* Copy picture to a new area of memory and change it to binary picture.
	* @Param src is the handle source.
	* @Param threshold controls the handle result.
	* @Return the handle result.
	*/
	bitMap binaryPic(bitMap src, int threshold);

	/**
	* Copy picture to a new area of memory and zoom.
	* @Param src is the handle source.
	* @Param rate is the zoom rate.
	* @Return the handle result.
	*/
	bitMap zoomPic(bitMap src, float rate);

	/**
	* Copy picture to a new area of memory and rotate.
	* @Param src is the handle source.
	* @Param angle is the rotate rate.
	* @Return the handle result.
	*/
	bitMap rotatePic(bitMap src, float angle);

	/**
	* Copy picture to a new area of memory and filter.
	* @Param src is the handle source.
	* @Param mode can be set to MEAN_FILTER or LAPLACIAN_FILTER
	* or BILATERAL_FILTER.
	* @Return the handle result.
	*/
	bitMap filterPic(bitMap src, int mode);

	/**
	* Copy picture to a new area of memory and change its luminance.
	* @Param src is the handle source.
	* @Param delta is the luminance delta.
	* @Return the handle result.
	*/
	bitMap luminantPic(bitMap src, int delta);

	/**
	* Copy picture to a new area of memory and change its contrast.
	* @Param src is the handle source.
	* @Return the handle result.
	*/
	bitMap contrastPic(bitMap src);

	/**
	* Create a new small window to show the info.
	* @Param caption is the window caption.
	* @Param text is the info showed in new window.
	* @Param mode describes the button in new window.
	* @Param result will be called when window closed or button push.
	*/
	void alertInfo(SGtext caption, SGtext text, int mode, void(*result)(int));


	/*
	* SG drawing interfaces
	* Funtions below are used to draw something on the screen.
	* The image after drawing will be shown after the loop function.
	*/

	/**
	* Set the current rgb color.
	* @Param rgb defines the current color.
	*/
	SGvoid setColor(int r, int g, int b);

	/**
	* Set the font size.
	* @Param height defines the current font size.
	*/
	SGvoid setFontSize(int height);

	/**
	* Set the font name.
	* @Param name defines the current font name.
	*/
	SGvoid setFontName(SGtext name);

	/**
	* Set the font style.
	* @Param coeff defines the current font style.
	*/
	SGvoid setFontStyle(int coeff);

	/**
	* Set the blend alpha.
	* @Param alpha defines the current blend alpha.
	*/
	SGvoid setAlpha(float alpha);

	/**
	* Get the alpha value.
	* @Return the current alpha value.
	*/
	SGfloat getAlpha();

	/**
	* Fill the whole screen with current color.
	*/
	SGvoid clearScreen();

	/**
	* Set the pixel with current color.
	* @Param x and y is the target position of the pixel.
	*/
	SGint putPixel(int x, int y);

	/**
	* Get the rgb color of a pixel.
	* @Param x and y is the target position of the pixel.
	*/
	rgb getPixel(int x, int y);

	/**
	* Draw a line.
	* @Param x1 and y1 is the start point of the line.
	* @Param x2 and y2 is the end point of the line.
	* @Param mode can be choosed in the enum list.
	*/
	SGvoid putLine(int x1, int y1, int x2, int y2, int mode);

	/* Draw a rectangle.
	* @Param x1 and y1 is one vertex of the quad.
	* @Param x2 and y2 is the opposite vertex of the quad.
	* @Param mode can be choosed in the enum list.
	*/
	SGint putQuad(int x1, int y1, int x2, int y2, int mode);

	/**
	* Draw a triangle.
	* @Param x1 and y1 is the first vertex of the triangle.
	* @Param x2 and y2 is the second vertex of the triangle.
	* @Param x3 and y3 is the third vertex of the triangle.
	* @Param mode can be choosed in the enum list.
	*/
	SGvoid putTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int mode);

	/**
	* Draw a circle.
	* @Param x and y is the centre of the circle.
	* @Param r is the radius of the circle.
	* @Param mode can be choosed in the enum list.
	*/
	SGvoid putCircle(int xc, int yc, int r, int mode);

	/**
	* Draw an ellipse.
	* @Param xc and yc is the centre of the ellipse.
	* @Param a and b is the x-radius and y-radius of the ellipse.
	* @Param mode can be choosed in the enum list.
	*/
	SGvoid putEllipse(int xc, int yc, int a, int b, int mode);

	/**
	* Load bmp to a bitMap struct other than showing it on screen.
	* @Param filename is the name of picture file with .bmp format.
	* @Return the memory pointer of the picture.
	*/
	bitMap loadBmp(SGtext filename);

	/**
	* Save bmp to file.
	* @Param filename is the name of picture file with .bmp format.
	* @Param bmp is the picture source.
	*/
	void saveBmp(SGtext filename, bitMap bmp);

	/**
	* Load png to a bitMap struct other than showing it on screen.
	* @Param filename is the name of picture file with .png format.
	* @Return the memory pointer of the picture.
	*/
	bitMap loadPng(SGtext filename);

	/**
	* Save png to file.
	* @Param filename is the name of picture file with .png format.
	* @Param png is the picture source.
	*/
	void savePng(SGtext filename, bitMap png);

	/**
	* Put a bmp picture on the screen.
	* @Param x and y is the top-left corner of the picture.
	* @Param bmp is the memory pointer of the picture.
	*/
	void putBitmap(int x, int y, bitMap bmp);

	/**
	* Draw one character ch on the screen.
	* @Param ch is the ascii of the character.
	* @Param x and y is the top-left corner of the character.
	*/
	SGvoid putChar(char ch, int x, int y);

	/**
	* Draw one number ch on the screen.
	* @Param n is the number value.
	* @Param x and y is the position of the number.
	* @Param lr is the orientation of the number. If lr is 'l' then (x, y) represent
	* the left-top corner, or else 'r' then (x, y) represent the right-top corner.
	*/
	SGvoid putNumber(int n, int x, int y, char lr);

	/**
	* Draw one string on the screen. If there is '\n' in the string, it will
	* be shown in more than one line. The line gap is the current font size.
	* @Param str is the string content.
	* @Param x and y is the top-left corner of the string.
	*/
	SGint putString(SGtext str, int x, int y);

	/**
	* Draw one string on the screen with max x-length.
	* @Param str is the string content.
	* @Param x and y is the top-left corner of the string.
	* @Param start is the index of first character to be shown .
	* @Param constraint is the x pixel length of the constraint .
	*/
	SGint putStringConstraint(SGtext str, int x, int y, int start, int constraint);

	/**
	* Get the width on x axis of the given string in current font.
	* @Param str is the input string.
	* @Param x in the index of the end character.
	* @Return the pixels on x-axis.
	*/
	SGint stringWidth(SGtext str, int x);

	/**
	* Build the string with format controling.
	* @Param str is the format string.
	* @Param ... are the values.
	* @Return the built string.
	*/
	char *stringFormat(SGtext str, ...);

	/**
	* Copy the pixel messages of the given area to parameter bitmap.
	* @Param left and top is the top-left of the quad area.
	* @Param right and bottom is the right-botom of the quad area.
	* @Param bitmap is the memory area to save the image. Note that bitmap->data
	* need not to be allocated.
	*/
	SGint getImage(int left, int top, int right, int bottom, bitMap *bitmap);

	/**
	* Paste the pixel messages.
	* @Param left and top is the left-top vertex of the quad area.
	* @Param bitmap point to the image memory.
	* @Param op can be choosed in the enum list.
	*/
	SGvoid putImage(int left, int top, bitMap *bitmap, int op);

	/**
	* Paste the pixel messages with transparent mask.
	* @Param left and top is the left-top vertex of the quad area.
	* @Param bitmap point to the mask memory. For each pixel, 0 means origin
	* color and 255 means bitmap color.
	* @Param bitmap point to the image memory.
	*/
	SGint maskImage(int left, int top, bitMap *mask, bitMap *bitmap);

	/**
	* Draw the graph of function vect with border limitation.
	* @Param x1 and x2 are x boarders.
	* @Param y1 and y2 are y boarders.
	* @Param vect is the function to be shown.
	*/
	SGvoid funcMap(int x1, int x2, int y1, int y2, float(*vect)(float x));

	/**
	* Fill an area.
	* @Param x and y are the starting point.
	* @Param c is the stopping color which means the area boarder
	* is with this color.
	*/
	SGvoid floodFill(int x, int y, rgb c);

	/**
	* Show the fps on the screen for testing.
	*/
	SGint showFps();

	/**
	* Use the region with given parameters to draw 3D graph.
	* @Param left and top is the top-left of the quad area.
	* @Param right and bottom is the right-botom of the quad area.
	* @Return value is the canvas id.
	*/
	SGint setGraphRegion(int left, int top, int right, int bottom);

	/**
	* Delete the graph region.
	* @Param id is the canvas id.
	*/
	SGint deleteGraphRegion(int id);

	/**
	* Clear the graph region with black color.
	* @Param id is the canvas id.
	*/
	SGvoid clearGraphBuffer(int id);

	/**
	* Pass a variable from user coding environment to programmable pipeline coding
	* environment.
	* @Param id is the graph id.
	* @Param name is the unique name of this variable.
	* @Param var is the value pointer. The canvas will read this pointer each time it
	* is used so just change the content rather than recall this function with another
	* pointer when you want to change this variable.
	*/
	SGvoid setPipelineVariable(int id, SGtext name, void *var);

	/**
	* Get the variable from programmable pipeline coding environment to user coding
	* environment.
	* @Param id is the graph id.
	* @Param name is the unique name of this variable.
	*/
	SGvoid *getPipelineVariable(int id, SGtext name);

	/**
	* When finish sending data strips and uniform variables, draw the
	* whole graph region.
	* @Param id is the canvas id.
	* @Param elementNum is the triangles num to be shown.
	*/
	SGvoid refreshGraph(int id, int elementNum);

	/**
	* When finish sending objects, draw the whole graph region using
	* the tracer defined.
	* @Param id is the canvas id.
	*/
	SGvoid refreshTracer(int id);

	/**
	* Give one data strip to vertex shader.
	* @Param id is the canvas id.
	* @Param data is the data strip pointer.
	* @Param step is the data num for one element.
	*/
	SGint pushDataArray(int id, float *data, int step);

	/**
	* The first part of the programmable pipeline. The times that this function
	* will be executed is equal to the elementNum given in refreshGraph. Every
	* time this function is executed, it calculate the map of an element's vertex
	* from the given data to screen position.
	* @Param id is the graph id.
	* @Param datas for function pointer vs is an array of array which contains
	* the data strips.
	* @Param step for function pointer vs is an array which contains the steps for
	* each data strip.
	* @Param dataNum for function pointer vs is the number of data strips. It can
	* be refered that datas size is (dataNum, elementNum * step * vertexNum).
	* @Param position for function pointer vs is the output of vertex shader which
	* saves the map result.
	* @Param outs for function pointer vs is another output of vertex shader which
	* pass data from vertex shader to fragment shader. The size of outs is
	* (vertexNum, return value) which means the data length for one vertex need
	* to be returned.
	*/
	SGvoid vertexShader(int id, int(*vs)(int id, float *datas[], int step[], int dataNum,
		vec3f position[], float *outs[]));

	/**
	* The second part of the programmable pipeline. The times that this function
	* will be executed is the number of pixels that need to be drawn.
	* @Param x and y is the screen coordinate when calling this function.
	* @Param data comes from the ous in vertex shader, and it has been interpolated
	* using temporary position and vertex positions.The return value of fragment shader
	* is the color that need to be drawn in (x, y).
	*/
	SGvoid fragmentShader(int id, vec3i(*fs)(int id, int x, int y, float *data));

	/**
	* Randomly select one direction from the hemisphere.
	* @Param normal is the hemisphere normal.
	*/
	vec3f randHemi(vec3f normal);

	/**
	* Select one direction from the hemisphere using phone reflection model.
	* @Param normal is the hemisphere normal.
	* @Param wi is the income light.
	* @Param ns is the specular parameter.
	*/
	vec3f phoneSpec(vec3f normal, vec3f wi, float ns);

	/**
	* Return the reflect direction of income light.
	* @Param normal is the hemisphere normal.
	* @Param wi is the income light.
	*/
	vec3f glassSpec(vec3f normal, vec3f wi);

	/**
	* Return the transmit direction of income light.
	* @Param normal is the hemisphere normal.
	* @Param wi is the income light.
	* @Param ni is the refract index.
	*/
	vec3f glassTrans(vec3f normal, vec3f wi, float ni);

	/**
	* An object means a sequence of triangles and their material which contains
	* intersect function, hit function and shadow function.
	* @Param id is the graph id.
	* @Param data is the triangle vertices sequence.
	* @Param length is the num of elements.
	* @Param vertices is the vertex num of one element.
	* @Param intersect is called to judge if current ray intersect with one element.
	* @Param hit is called to calculate the color to be shown.
	* @Param shadow is called to judge if current hit point is in shadow.
	* @Return returns the object id.
	*/
	SGint pushObject(int id, float *pos, float *norm, int length, int vertices,
		float(*intersect)(int id, void *points, void *norms, vec3f point, vec3f dir, vec3f *norm),
		void(*hit)(int id, float dist, void *prd, vec3f norm, void *param),
		void(*shadow)(int id, void *prd), void *param);

	/**
	* This is the main tracing function. Use the light with starting point light and
	* the direction dir to trace the obj that has been pushed with the given obj id.
	* @Param id is the graph id.
	* @Param lighe and dir are the starting point and direction of the light ray.
	* @Param type is the ray type.
	* @Param tmin and tmax is the nearest and fareset distance of intersecting point.
	* @Param prd is the struct pointer that carries the ray data for accumulating.
	*/
	SGvoid rtTrace(int id, int obj, vec3f light, vec3f dir, int type, float tmin, float tmax, void *prd);

	/**
	* Set the generate function. It is the first function executed when calling
	* refresh.
	* @Param id is the graph id.
	* @Param id of function pointer generate is the graph id.
	* @Param index of function pointer generate is the position of the drawing pixel.
	* @Param size of function pointer generate is the size of the whole screen.
	*/
	SGvoid rtGenerate(int id, vec3f(*generate)(int id, vec2i index, vec2i size));

	/**
	* Set the miss function. That is, when rtTrace function did not hit any objects,
	* the miss function will be called.
	* @Param id is the graph id.
	* @Param id of function pointer generate is the graph id.
	* @Param prd is the struct pointer that carries the ray data for accumulating.
	*/
	SGvoid rtMiss(int id, void(*miss)(int id, void *prd));

	/**
	* Get the float color of one pixel.
	* @Param id is the graph id.
	* @Param posX and posY is the pixel coordinate.
	*/
	vec3f getGraphPixel(int id, int posX, int posY);


	/*
	* SG writing interfaces
	* Functions below can be called only in TEXT_MAP mode. Most functions
	* are similar to those in BIT_MAP mode.
	*/

	/**
	* Set background color and foreground color.
	* @Param bgc is the background color.
	* @Param fgc is the foreground color.
	*/
	SGvoid setBfc(int bgc, int fgc);

	/**
	* Fill the whole screen to the current background color.
	*/
	SGvoid clearText();

	/**
	* Set the background and foreground color of one charactor.
	* @Param color is the background and foreground color to be set.
	* @Param x and y is the character position.
	*/
	SGvoid setCharColor(short color, int x, int y);

	/**
	* Set the background color of one charactor.
	* @Param color is the background color to be set.
	* @Param x and y is the character position.
	*/
	SGvoid setCharBgc(char color, int x, int y);

	/**
	* Set the foreground color of one charactor.
	* @Param color is the foreground color to be set.
	* @Param x and y is the character position.
	*/
	SGvoid setCharFgc(char color, int x, int y);

	/**
	* Returns the character and color.
	* @Param x and y is the character position.
	* @Return value combined with character and color. The low 8 bit is
	* character and the high 8 bit is color.
	*/
	SGint getShort(int x, int y);

	/**
	* Put down one character with current color.
	* @Param c is the character.
	* @Param x and y is the character position.
	*/
	SGvoid writeChar(char c, int x, int y);

	/**
	* Put down one string with current color.
	* @Param s is the string.
	* @Param x and y is the character position.
	*/
	SGvoid writeString(SGtext s, int x, int y);

	/**
	* Copy the text messages of the given area to parameter text.
	* @Param left and top is the top-left of the quad area.
	* @Param right and bottom is the right-botom of the quad area.
	* @Param test is the memory area to save the text. Note that text->data
	* need not to be allocated.
	*/
	SGint getText(int left, int top, int right, int bottom, textMap *text);

	/**
	* Paste the text messages.
	* @Param left and top is the left-top vertex of the quad area.
	* @Param text point to the text memory.
	*/
	SGvoid putText(int left, int top, textMap *text);


	/*
	* SG system interfaces
	* Basic IO functions to make the program executive. Widget is
	* for advanced graphic programming and for SG Creater to use.
	*/

	/**
	* Main function to deal with key event.
	* @Param cmd can be 0 or 1.
	* @Return if cmd is 1 then return value is whether there's waiting
	* events, or else if cmd is 0 then return the earliest event key value.
	*/
	SGint biosKey(int cmd);

	/**
	* Delete all key events before current time.
	*/
	SGvoid clearKeyBuffer();

	/**
	* Get the current position of the cursor.
	* @Return value is a struct with mouse position.
	*/
	vec2i mousePos();

	/**
	* Get whether one of the three mouse button is push down.
	* @Param b can be SG_LEFT_BUTTON or SG_RIGHT_BUTTON or
	* SG_MIDDLE_BUTTON.
	* @Return value is whether button b is down.
	*/
	SGint mouseStatus(int b);

	/**
	* Main function to deal with mouse event.
	* @Param cmd can be 0 or 1.
	* @Return if cmd is 1 then the member m of return value is whether
	* there's waiting events, or else if cmd is 0 then return the earliest
	* event mouse contains x, y and which mouse button.
	*/
	vec3i biosMouse(int cmd);

	/**
	* Delete all mouse events before current time.
	*/
	SGvoid clearMouseBuffer();

	/**
	* Enable right click and move the mouse to select one funtion.
	*/
	SGvoid enablePanel();

	/**
	* Disable right click and move the mouse to select one funtion.
	*/
	SGvoid disablePanel();

	/**
	* Add one function to the input panel.
	* @Param name is used to shown when moving towards the correspondent
	* direction.
	* @Param function is called when right button is released.
	* @Parameter shift and ctrl means if this function need to press these two key.
	* @Return value is the id of this item.
	*/
	SGint addPanelItem(SGtext name, vect function, int shift, int ctrl);

	/**
	* Change the name and function of an input panel item.
	* @Param id is the item id.
	* @Param name is used to shown when moving towards the correspondent
	* direction.
	* @Param function is called when right button is released.
	* @Return value is id or SG_OBJECT_NOT_FOUND
	*/
	SGint changePanelItem(int id, SGtext name, vect function);

	/**
	* Delete one input panel item.
	* @Param id is the item id.
	*/
	SGint deletePanelItem(int id);

	/**
	* Make the cursor visible.
	*/
	SGvoid showMouse();

	/**
	* Make the cursor disvisible.
	*/
	SGvoid hideMouse();

	/*
	* Set the cursor position.
	* @Param x and y describe the position to be set.
	*/
	SGvoid setMousePos(int x, int y);

	/**
	* Set the icon of the cursor.
	* @Param icon can be either preset enums or icon resource path.
	*/
	SGvoid setMouseIcon(SGWINSTR icon);

	/**
	* Allow this program to use windows main menu.
	* @Return value is the main menu id which will be used when add lists or
	* items into main menu.
	*/
	SGint initMainMenu();

	/**
	* Add a new list of name title into the menu.
	* @Param title is the string to be shown.
	* @Param id is the item id.
	* @Return value is the new list id.
	*/
	SGint addMainMenuList(SGtext title, int id);

	/**
	* Add a new item of name title into the menu.
	* @Param title is the string to be shown.
	* @Param id is the item id.
	* @Param func will be called when this item is clicked.
	* @Return value is the new list id.
	*/
	SGint addMainMenuItem(SGtext title, int id, void(*func)());

	/**
	* Add a separate line in the list.
	* @Param id is the item id.
	*/
	SGint addMainMenuSeparator(int id);

	/**
	* Make the item or menu of id enabled. That is, it can be clicked.
	* @Param id is the item id.
	*/
	SGint enableMainItem(int id);

	/**
	* Make the item or menu of id disabled. That is, it can't be clicked.
	* @Param id is the item id.
	*/
	SGint disableMainItem(int id);

	/**
	* Make the item of id checked with a tick on the left.
	* @Param id is the item id.
	*/
	SGvoid checkMainItem(int id);

	/**
	* Make the item of id unchecked and clear the tick on the left.
	* @Param id is the item id.
	*/
	SGvoid uncheckMainItem(int id);

	/**
	* Add the application icon in the tray bar and enable the program
	* to hide to tray.
	*/
	SGvoid addTray();

	/**
	* Hide the whidow, and it will be shown again after clicking the icon
	* in the tray.
	*/
	SGvoid hideToTray();

	/**
	* Show the window again.
	*/
	SGvoid restoreFromTray();

	/**
	* Enable showing the menu when right click the icon in the tray. The
	* return value is the tray menu id.
	*/
	SGint initTrayMenu();

	/**
	* Add a new list of name title into the tray menu.
	* @Param title is the string to be shown.
	* @Param id is the item id.
	* @Return value is the new list id.
	*/
	SGint addTrayMenuList(SGtext title, int id);

	/**
	* Add a new item of name title into the tray menu.
	* @Param title is the string to be shown.
	* @Param id is the item id.
	* @Param func will be called when this item is clicked.
	* @Return value is the new list id.
	*/
	SGint addTrayMenuItem(SGtext title, int id, void(*func)());

	/**
	* Add a separate line in the list.
	* @Param id is the item id.
	*/
	SGint addTrayMenuSeparator(int id);

	/**
	* Make the item or menu of id enabled. That is, it can be clicked.
	* @Param id is the item id.
	*/
	SGint enableTrayItem(int id);

	/**
	* Make the item or menu of id disabled. That is, it can't be clicked.
	* @Param id is the item id.
	*/
	SGint disableTrayItem(int id);

	/**
	* Make the item of id checked with a tick on the left.
	* @Param id is the item id.
	*/
	SGvoid checkTrayItem(int id);

	/**
	* Make the item of id unchecked and clear the tick on the left.
	* @Param id is the item id.
	*/
	SGvoid uncheckTrayItem(int id);

	/*
	* Init one popup menu, the return value is the menu id.
	*/
	SGint createPopupMenu();

	/**
	* Add a new list of name title into the popup menu.
	* @Param title is the string to be shown.
	* @Param id is the item id.
	* @Return value is the new list id.
	*/
	SGint addPopupMenuList(SGtext title, int id);

	/**
	* Add a new item of name title into the popup menu.
	* @Param title is the string to be shown.
	* @Param id is the item id.
	* @Param func will be called when this item is clicked.
	* @Return value is the new list id.
	*/
	SGint addPopupMenuItem(SGtext title, int id, void(*func)());

	/**
	* Add a separate line in the list.
	* @Param id is the item id.
	*/
	SGint addPopupMenuSeparator(int id);

	/**
	* Make the item or menu of id enabled. That is, it can be clicked.
	* @Param id is the item id.
	*/
	SGint enablePopupItem(int id);

	/**
	* Make the item or menu of id disabled. That is, it can't be clicked.
	* @Param id is the item id.
	*/
	SGint disablePopupItem(int id);

	/**
	* Make the item of id checked with a tick on the left.
	* @Param id is the item id.
	*/
	SGvoid checkPopupItem(int id);

	/**
	* Make the item of id unchecked and clear the tick on the left.
	* @Param id is the item id.
	*/
	SGvoid uncheckPopupItem(int id);

	/**
	* The popup menu need to be finished by the programmer which means
	* it is ready to show.
	* @Param id is the item id.
	*/
	SGint finishPopupMenu(int id);

	/**
	* The popup menu with given parameter can be shown.
	* @Param menu is the popup menu id.
	* @Param x and y describe the top-left position of the popup menu.
	*/
	SGint showPopupMenu(int menu, int x, int y);


	/*
	* SG widgets management.
	* Use these functions to build and control widgets.
	*/

	/**
	* When widgets changes and the background need to be redrew,
	* the function refresh will be called. The parameter of refresh is
	* the area that need to be redrew.
	* @Param refresh is the function to be called when refreshing.
	*/
	SGvoid setBackgroundRefresh(void(*refresh)(int left, int top, int right, int bottom));

	/**
	* Refresh the background of the given area using the function set
	* by setBackgroundRefresh.
	* @Param left, top, right and bottom define the quad area.
	*/
	SGvoid useBackgroundRefresh(int left, int top, int right, int bottom);

	/**
	* Create a widget with default parameter.
	* @Param type is the widget type.
	* @Param name is the widget name.
	* @Return struct is the mid-object of widget.
	*/
	widget newWidget(enum _control type, SGtext name);

	/**
	* After setting all the parameters, give back the mid-object to
	* the system. After this function, all callbacks are activated.
	* @Param obj is the mid-object sending to the system.
	*/
	SGvoid registerWidget(widget obj);

	/**
	* Create and register a widget fastly.
	* @Param type is the widget type.
	* @Param name is the widget name.
	* @Param x and y defines the top-left position of the widget.
	* @Param width and height defines the widget size.
	* @Param content is the string to be shown.
	* @Param click will be called when widget is pressed.
	* @Return struct is the mid-object of widget.
	*/
	widget easyWidget(int type, SGtext name,
		int x, int y, int width, int height, SGtext content, void(*click)(widget *obj));

	/**
	* Create a combined widget. Pass the sub widget number and
	* then those sub widget pointers.
	* @Param name is the widget name.
	* @Param x and y defines the top-left position of the widget.
	* @Param width and height defines the widget size.
	* @Param num is the number of sub widgets.
	* @Param ... are the sub widgets.
	*/
	widget easyCombinedWidget(SGtext name, int x, int y, int width, int height, int num, ...);

	/**
	* Get the widget structure by the given name.
	* @Param name is the widget name.
	* @Return the poiter of the widget's mid-object.
	*/
	widget *getWidgetByName(SGtext name);

	/**
	* After modifing the structure returned by getWidgetByName, apply the
	* modification.
	* @Param name is the widget name.
	*/
	SGvoid refreshWidget(SGtext name);

	/**
	* Judge if coordinate (x, y) is in widget obj.
	* @Param obj is the mid-object of widget.
	* @Param x and y is the position to be judged.
	*/
	SGint inWidget(widget *obj, int x, int y);

	/**
	* Judge if rectangle (left, top, right, bottom) is crossing widget obj.
	* @Param obj is the mid-object of widget.
	* @Param left, top, right and bottom are the quad to be judged.
	*/
	SGint crossWidget(widget *obj, int left, int top, int right, int bottom);

	/**
	* Make the widget of name visible.
	* @Param name is the widget name.
	*/
	SGint showWidget(SGtext name);

	/**
	* Make the widget of name disvisible.
	* @Param name is the widget name.
	*/
	SGint ceaseWidget(SGtext name);

	/**
	* Delete the widget with the given name.
	* @Param name is the widget name.
	*/
	SGint deleteWidget(SGtext name);

	/**
	* Returns the sub widget with the given name in parent's childs.
	* @Param parent is the parent widget name.
	* @Param sub is the sub widget name.
	*/
	widget *getSubWidget(SGtext parent, SGtext sub);

	/**
	* Insert a child widget. The index decides the display order.
	* @Param parent is the parent widget name.
	* @Param sub is the sub widget.
	* @Param index defines the insert position.
	*/
	SGvoid insertSubWidget(SGtext parent, widget sub, int index);

	/**
	* Delete the child widget of the given name.
	* @Param parent is the parent widget name.
	* @Param sub is the sub widget name.
	*/
	SGvoid deleteSubWidget(SGtext parent, SGtext name);

	/**
	* Move the widget to (x + xDelta, y + yDelta) with the given name.
	* @Param name is the widget name.
	* @Param xDelta and yDelta define the moving delta.
	*/
	SGint moveWidget(SGtext name, int xDelta, int yDelta);

	/**
	* Set the widget with the given name to the top which means no other
	* widgets can conver it.
	* @Param name is the widget name.
	*/
	SGvoid setWidgetTop(SGtext name);

	/**
	* Set the widget with the given name to the bottom which means any
	* widget can conver it.
	* @Param name is the widget name.
	*/
	SGvoid setWidgetBottom(SGtext name);

	/**
	* Set the sub widget with the given name to the top in combined widget
	* which means no other widgets can conver it.
	* @Param parent is the combined widget name.
	* @Param name is the sub widget name.
	*/
	SGvoid setSubWidgetTop(SGtext parent, SGtext name);

	/**
	* Set the sub widget with the given name to the bottom in combined widget
	* which means any widget can conver it.
	* @Param parent is the combined widget name.
	* @Param name is the sub widget name.
	*/
	SGvoid setSubWidgetBottom(SGtext parent, SGtext name);


	/*
	* SG data interfaces
	* These functions are used to deal with big data.
	*/

	/**
	* Create an empty json object.
	* @Return struct is an empty json object.
	*/
	struct JSON *createJson();

	/**
	* Create an empty json array.
	* @Return struct is an empty json array.
	*/
	struct JSON *createJsonArray();

	/**
	* Safely free the memory of the given structure.
	* @Param json is the object or array to be free.
	*/
	void freeJson(struct JSON *json);

	/**
	* Create a json object or array with the given string.
	* @Param json is the json string.
	* @Retrun struct is the correspondent json structure.
	*/
	struct JSON *readJson(SGtext json);

	/**
	* Give the correspondent string with the given json.
	* @Param json is the json structure.
	* @Return string is the correspondent json string.
	*/
	char *writeJson(struct JSON *json);

	/**
	* Get the item in a json object with the given name.
	* @Param json is the json structure.
	* @Param name is the content name.
	* @Return struct is the item with the given name.
	*/
	struct JSON_Item *getContent(struct JSON *json, SGtext name);

	/**
	* Get the item in a json array with the given index.
	* @Param json is the json structure.
	* @Param idex is the element index.
	* @Return struct is the item with the given index.
	*/
	struct JSON_Item *getElement(struct JSON *json, int idx);

	/**
	* Delete the item in a json object with the given name.
	* @Param json is the json structure.
	* @Param name is the content name.
	*/
	void deleteContent(struct JSON *json, SGtext name);

	/**
	* Delete the item in a json array with the given index.
	* @Param json is the json structure.
	* @Param idex is the element index.
	*/
	void deleteElement(struct JSON *json, int idx);

	/**
	* If there is an item in json object with the given name, then reset its
	* value. Or else, add this item.
	*/
	void setIntContent(struct JSON *json, SGtext name, SGint i);
	void setFloatContent(struct JSON *json, SGtext name, SGfloat f);
	void setCharContent(struct JSON *json, SGtext name, SGchar c);
	void setBoolContent(struct JSON *json, SGtext name, SGbool b);
	void setStringContent(struct JSON *json, SGtext name, const char *s);
	void setObjectContent(struct JSON *json, SGtext name, struct JSON *j);
	void setArrayContent(struct JSON *json, SGtext name, struct JSON *j);

	/**
	* If there is an item in json array with the given index, then reset its
	* value. Or else, add this item.
	*/
	void setIntElement(struct JSON *json, int idx, SGint i);
	void setFloatElement(struct JSON *json, int idx, SGfloat f);
	void setCharElement(struct JSON *json, int idx, SGchar c);
	void setBoolElement(struct JSON *json, int idx, SGbool b);
	void setStringElement(struct JSON *json, int idx, const char *s);
	void setObjectElement(struct JSON *json, int idx, struct JSON *j);
	void setArrayElement(struct JSON *json, int idx, struct JSON *j);

	/**
	* Create zip file if the zip file isn't exist.
	* @Param zip is the path of the zip file.
	*/
	HANDLE createZip(SGstring zip);

	/**
	* Add file into zip file.
	* @Param h is the zip handle from createZip.
	* @Param src is the path of the origin file.
	* @Param dst is the path where origin file should be put in the zip file.
	*/
	void zipFile(HANDLE h, SGstring src, SGstring dst);

	/**
	* Add file into zip file.
	* @Param h is the zip handle from createZip.
	* @Param src is the memory pointer.
	* @Param len is the memory size(bytes).
	* @Param dst is the path where origin file should be put in the zip file.
	*/
	void zipMemory(HANDLE h, void *src, int len, SGstring dst);

	/**
	* Create a folder in the zip file.
	* @Param h is the zip handle from createZip.
	* @Param dst is the folder in the zip file.
	*/
	void zipFolder(HANDLE h, SGstring dst);

	/**
	* Close zip file.
	* @Param h is the zip handle from createZip.
	*/
	void zipFinish(HANDLE h);

	/**
	* Open zip file for unzip.
	* @Param zip is the path of the zip file.
	*/
	HANDLE createUnzip(SGstring zip);

	/**
	* Read zip file and get the idxth file in the zip file.
	* @Param h is the zip handle from createUnzip.
	* @Param src is the idxth file in the zip file..
	* @Param dst reveives the result and should be allocated first.
	*/
	void readUnzip(HANDLE h, int idx, SGstring path);

	/**
	* Open and unzip the file int the zip file.
	* @Param h is the zip handle from createUnzip.
	* @Param src is the path where the file is in the zip file.
	* @Param dst is the path where the unzip file should be put.
	*/
	void unzipFile(HANDLE h, SGstring src, SGstring dst);

	/**
	* Open and unzip the file int the zip file.
	* @Param h is the zip handle from createUnzip.
	* @Param src is the path where the unzip file should be put.
	* @Param dst is the memory pointer.
	* @Param len is the memory size(bytes).
	*/
	void unzipMemory(HANDLE h, SGstring src, void *dst, int len);


	/*
	* SG encryption interfaces
	* These functions are used to encrypt and decrypt.
	*/

	/* DES key length 8. */
	int DESEncrypt(const char *plain, int len, const char *key, char *cipher);
	int DESDecrypt(const char *cipher, int len, const char *key, char *plain);

	/* AES key length 16. */
	void AESEncrypt(const char *plain, int len, const char *key, char *cipher);
	void AESDecrypt(const char *cipher, int len, const char *key, char *plain);

	void RSAProduceKey(unsigned int p, unsigned int q, unsigned int *e, unsigned int *d, unsigned int *n);
	void RSAEncrypt(unsigned int plain, int key, int N, unsigned int *cipher);
	void RSADecrypt(unsigned int cipher, int key, int N, unsigned int *plain);


	/*
	* SG main function
	* These two functions play the same role as main().
	*/

	/**
	* For initializing.
	*/
	void sgSetup();

	/**
	* For main loop.
	*/
	void sgLoop();

#define SGL_GRAPHICS_FRAME
#define SGL_CONSOLE_FRAME \
void sgSetup() {} \
void sgLoop() {}


#ifdef __cplusplus
}
#endif
#endif


