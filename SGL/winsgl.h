/*
* Copyright (c) 2016-2019, Super GP Individual.
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


#define _SGL_V500
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifdef _DEBUG
#define SG_LIB(name) name "d.lib"
#else
#define SG_LIB(name) name ".lib"
#endif
#define SG_A(name) name ".a"

//#pragma comment(lib, SG_LIB("winsgl"))
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "shlwapi.lib")

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
	SG_OPTION,
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
typedef unsigned char *SGustring;
typedef float SGfloat;
typedef double SGdouble;
#ifdef __cplusplus
typedef bool SGbool;
#else
typedef char SGbool;
#endif

typedef struct {
	byte r, g, b;
}RGB;
typedef struct {
	int sizeX, sizeY;
	unsigned char *data;
}bitMap;
typedef struct {
	int width, height;
	unsigned short *content;
}textMap;
typedef struct {
	RGB color;
	int size;
	SGWINSTR name;
	int coeff;
}font;
typedef struct _w{
	enum _control type;

	vec2i pos;
	vec2i size;

	int style;

	SGstring name;
	void *content;
	int value, extra;

	font tf;
	struct _w *child, *next;

	RGB bgColor, passColor, pressColor, fgColor;
	bitMap bgImg;

	void(*click)(_w *obj);
	void(*move)(_w *obj, int x, int y);
	void(*press)(_w *obj, int key);
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

	char *name;
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

	SGvoid initWindow(int width, int height, const char *title, int mode);
	/* Used in sgSetup(), to set the parameters of the graphic window.*/

	SGvoid initPolarWindow(int cx, int cy, int r);
	/* Use circle window. The centre is (cx, cy) and the radius is r. */

	SGint createWindow(int width, int height, const char *title, int mode, vect setup, vect loop);
	/* Create a new window with SGL canvas. The first three parameters are
	* same as initWindow, while setup and loop are functions similar to sgSetup
	* and sgLoop. */

	SGvoid windowFinish(vect finish);
	/* When the current window is closed, the function finish will be called. */

	SGvoid closeWindow(int id);
	/* Close the sub window with the id given by createWindow. */

	SGvoid resizeFuntion(void(*func)(int x, int y));
	/* The parameter function will be executed when the size of the window
	*changed. */

	SGvoid renameCaption(const char *name);
	/* Set the caption of the window to the given name. */

	SGvoid hideCaption();
	/* Hide the caption bar at the top. */

	SGvoid showCaption();
	/* Show the caption bar at the top. */


	/*
	* SG tool interfaces
	* These functions can be used not only in graphic modes but also in
	* console mode.
	*/

	SGint getWidth(int obj);
	/* Returns the width of the obj. The parameter obj can be SG_WINDOW
	* (the current window) or SG_CANVAS(The screen of the computer). */

	SGint getHeight(int obj);
	/* Returns the height of the obj. The parameter obj can be SG_WINDOW
	* (the current window) or SG_CANVAS(The screen of the computer). */

	vec2i getSize(int obj);
	/* Returns the size of the obj. The parameter obj can be SG_WINDOW
	* (the current window) or SG_CANVAS(The screen of the computer). */

	SGvoid debugf(const char *format, ...);
	/* Print the message with format to the output window when debugging. */

	SGvoid delay(int t);
	/* Wait for t millisecond. During the time the window won't refresh. */

	SGvoid delayBegin();
	/* Used with delayEnd. To set the least time waiting between begin
	* and end. This pair of function is usually used to cotrol the time
	* of some loops. */

	SGint delayEnd(int t);
	/* Used with delayBegin. To set the least time waiting between begin
	* and end. This pair of function is usually used to cotrol the time
	* of some loops. */

	SGint random(int n);
	/* Returns a random integer between 0 and n - 1. */

	SGint selectFile(char name[], char start[], char format[], int idx);
	/* Use Win API Graphic mode to choose one file. Parameter name
	* is used to receive the selected file name with its path, and start
	* is the path to begin with so set it to NULL as default. Parameter
	* format is the probable file format and each format is seperated
	* with \0, and it's set to NULL as default as well. The idx is the
	* default format index which is usually set to 1. */

	SGint selectSave(char name[], char start[], char format[], char def[], int idx);
	/* Use Win API Graphic mode to let user input the file name that
	* they want to save. Parameter name is used to receive the input
	* file name with its path, and start is the path to begin with so set
	* it to NULL as default. Parameter format is the probable file
	* format and each format is seperated with \0, and it's set to NULL
	* as default as well. Parameter def is the default format to append
	* if no format is input. The idx is the default format index which is
	* usually set to 1. */

	SGint selectDir(char name[], char start[]);
	/* Use Win API Graphic mode to choose one directory. Parameter
	* name is used to receive the selected directory name with its
	* path, and start is the path to begin with so set it to NULL as
	* default. */

	SGint makePath(const char path[]);
	/* If the given folder does not exist, then create it. */

	SGint fileExist(const char path[]);
	/* If the given file exists, return 1. Or else return 0. */

	SGvoid initMci();
	/* Initialize the media(mp3) device. */

	SGint loadMciSrc(const char *filename);
	/* Load the file in format mp3 to memory, the return value is its identifier.
	* Then operate the music using this identifier.*/

	SGint playMci(int id);
	/* Start playing the music with the given id. */

	SGint stopMci(int id);
	/* Stop playing the music with the given id, then roll back to its start. */

	SGint pauseMci(int id);
	/* Pause the music with the given id. */

	SGint resumeMci(int id);
	/* Resume playing the music with the given id at the pause point.*/

	SGvoid createThread(thread func, void *param);
	/* Create a thread with function func which means that the new thread
	* will start running with func. */

	SGint copyText(const char *src);
	/* Copy the given text into windows clipboard so that it can be pasted
	* to other programs. */

	SGstring pasteText();
	/* Return the text in clipboard. The return string need to be free by
	* the programmer. */

	SOCKET createServer(int port);
	/* Set up a server(both local and remote), for diy communication, port should
	* be greater than 1023 and less than 65536. The return value is the socket
	* of the server. */

	SOCKET createClient(const char *server, int port);
	/* Set up a client and connect to server(localhost is "127.0.0.1), the port
	* should be equal to the one set by server. The return value is the socket
	* of the client. */

	SOCKET acceptOne(SOCKET server);
	/* Used by a server to accept one request, one acceptOne should answer
	* one createClient. The return value is the socket of the connection. */

	SGint socketSend(SOCKET s, const char *buffer);
	/* Used to send a string via one connection, socket s stands for the socket
	* of the given connection. */

	SGint socketReceive(SOCKET s, char *buffer, int len);
	/* Used to receive a string via one connection, socket s stands for the
	* socket of the given connection. Parameter len is the max length to receive,
	* if more content is sending, the rest will wait. If the connection is stopped,
	* the return value is SG_CONNECTION_FAILED. */

	SGvoid closeSocket(SOCKET s);
	/* When one the connection is cut, we should close the socket of this
	* connection. */

	SGvoid setIcon(const char *ico);
	/* Set the running-time icon for the window. The icon will appear
	* at top_left in the caption bar and in the tray. */

	bitMap *copyPic(bitMap *src);
	/* Copy picture from src to dst. */

	bitMap *grayPic(bitMap *src);
	/* Change source picture to grayscale picture. */

	bitMap *binaryPic(bitMap *src, int threshold);
	/* Change source picture to binary picture. */

	bitMap *zoomPic(bitMap *src, float rate);
	/* Zoom in (rate < 1) or zoom out (rate > 1). */

	bitMap *rotatePic(bitMap *src, bitMap *mask, float angle);
	/* Rotate the source picture and output the mask. */

	bitMap *filterPic(bitMap *src, int mode);
	/* Use different kinds of filter to enhance the quality of the given
	* source picture. */

	bitMap *luminantPic(bitMap *src, int delta);
	/* Change the luminance of the source picture. */

	bitMap *contrastPic(bitMap *src);
	/* Change the contrast of the source picture. */


	/*
	* SG drawing interfaces
	* Funtions below are used to draw something on the screen.
	* The image after drawing will be shown after the loop function.
	*/

	SGvoid setColor(int r, int g, int b);
	/* Set the current rgb color. */

	SGvoid setFontSize(int height);
	/* Set the current font size.*/

	SGvoid setFontName(const char *name);
	/* Set the current font name.*/

	SGvoid setFontStyle(int coeff);
	/* Set the current font style.*/

	SGvoid setAlpha(float alpha);
	/* Set the alpha value when mixing images. */

	SGfloat getAlpha();
	/* Get the alpha value. */

	SGvoid clearScreen();
	/* Fill the whole screen with current color. */

	SGint putPixel(int x, int y);
	/* Set the pixel of coordinate (x, y) of current color. */

	RGB getPixel(int x, int y);
	/* Returns the rgb color of coordinate (x, y). */

	SGvoid putLine(int x1, int y1, int x2, int y2, int mode);
	/* Draw a line from (x1, y1) to (x2, y2) of currrent color with
	* the given mode showed in enum list. */

	SGint putQuad(int x1, int y1, int x2, int y2, int mode);
	/* Draw a rectangle from (x1, y1) to (x2, y2) of currrent color
	* with the given mode showed in enum list. */

	SGvoid putTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int mode);
	/* Draw a triangle from of the given three point of currrent
	* color with the given mode showed in enum list. */

	SGvoid putCircle(int xc, int yc, int r, int mode);
	/* Draw a circle with centre (xc, yc) and radius r of currrent
	* color with the given mode showed in enum list. */

	SGvoid putEllipse(int xc, int yc, int a, int b, int mode);
	/* Draw a circle with centre (xc, yc) and axis a and b of currrent
	* color with the given mode showed in enum list. */

	bitMap loadBmp(const char *filename);
	/* Load bmp to a bitMap struct other than showing it on screen. */

	void putBitmap(int x, int y, bitMap bmp);
	/* Put a bmp picture on the screen with top-left corner (x, y). */

	SGint drawBmp(int x, int y, const char *filename);
	/* Put a bmp picture on the screen with top-left corner (x, y). */

	SGvoid putChar(char ch, int x, int y);
	/* Draw one character ch on the screen of position (x, y). */

	SGvoid putNumber(int n, int x, int y, char lr);
	/* Draw number n on the screen of position (x, y). The parameter
	* lr can be 'l', which means that (x, y) is the top-left corner of the
	* number, or 'r', which means the bottom-right corner. Details are
	* in instruction pdf. */

	SGint putString(const char *str, int x, int y);
	/* Draw one string str on the screen of position (x, y). */

	SGint putStringConstraint(const char *str, int x, int y, int start, int constraint);
	/* Draw one string str on the screen of position (x, y) with maximum
	* length constraint. */

	SGint stringWidth(const char *str, int x);
	/* Get the width on x axis of the given string in current font. */

	char *stringFormat(const char *str, ...);
	/* Build the string with format controling. */

	SGint getImage(int left, int top, int right, int bottom, bitMap *bitmap);
	/* Copy the pixel messages of the given area to parameter bitmap. */

	SGvoid putImage(int left, int top, bitMap *bitmap, int op);
	/* Paste the pixel messages of the given area to (left, top) with mode op. */

	SGint maskImage(int left, int top, bitMap *mask, bitMap *bitmap);
	/* The new pixel */

	SGvoid funcMap(int x1, int x2, int y1, int y2, float(*vect)(float x));
	/* Draw the graph of function vect with border limitation. */

	SGvoid floodFill(int x, int y, RGB c);
	/* Fill from coordinate (x, y) recursively with current color until
	* meeting the given color c. */

	SGint showFps();
	/* Show the fps on the screen for testing. */

	SGint setGraphRegion(int left, int top, int right, int bottom);
	/* Use the region with given parameters to draw 3D graph, the
	* return value is the canvas id.*/

	SGint deleteGraphRegion(int id);
	/* Delete the graph region with given id. */

	SGvoid clearGraphBuffer(int id);
	/* Clear the graph region with black color. */

	SGvoid setPipelineVariable(int id, const char *name, void *var);
	/* Pass a variable from user coding environment to programmable pipeline coding
	* environment. Parameter id is the graph id and pos is the variable index. Parameter
	* name is the unique name of this variable. The last parament is the value pointer
	* that need to be set. It's designed to be a pointer so that this set variable
	* function needn't be called frequently. */

	SGvoid *getPipelineVariable(int id, const char *name);
	/* Get the variable with given graph id and variable name. */

	SGvoid refreshGraph(int id, int elementNum);
	/* When finish sending data strips and uniform variables, draw the
	* whole graph region. The num is the element num.*/

	SGvoid refreshTracer(int id);
	/* When finish sending objects, draw the whole graph region using
	* the tracer defined.*/

	SGint pushDataArray(int id, float *data, int step);
	/* Give one data strip to vertex shader with given graph id. */

	SGvoid vertexShader(int id, int(*vs)(int id, float *datas[], int step[], int dataNum,
		vec3f position[], float *outs[]));
	/* The first part of the programmable pipeline. The times that this function
	* will be executed is equal to the elementNum given in refreshGraph. Every
	* time this function is executed, it calculate the map of an element's vertex
	* from the given data to screen position. Parameter id is the graph id, datas
	* and steps is an array of array which contains the datas and steps given in
	* pushDataArray, dataNum is the number of data strips. It can be refered
	* that datas size is (dataNum, elementNum * step * vertexNum). Parameter
	* position and outs is the output of vertex shader. position is an array with
	* length vertexNum of an element, it saves the map result. outs is an array
	* of array that pass data from vertex shader to fragment shader. The size
	* of outs is (vertexNum, return value) which means the data length for one
	* vertex need to be returned. */

	SGvoid fragmentShader(int id, vec3i(*fs)(int id, int x, int y, float *data));
	/* The second part of the programmable pipeline. The times that this function
	* will be executed is the number of pixels that need to be drawn. The parameter
	* x and y is the screen coordinate when calling this function. The parameter
	* data comes from the ous in vertex shader, and it has been interpolated using
	* temporary position and vertex positions. The return value of fragment shader
	* is the color that need to be drawn in (x, y). */

	vec3f randHemi(vec3f normal);
	/* Randomly select one direction from the hemisphere using the given normal. */

	SGint pushObject(int id, float *data, int length, int vertices,
		float(*intersect)(int id, void *points, vec3f point, vec3f dir, vec3f *norm),
		void(*hit)(int id, float dist, void *prd, vec3f norm), void(*shadow)(int id, void *prd));
	/* An object means a sequence of triangles and their material which contains
	 * intersect function, hit function and shadow function. Parameter id is the graph
	 * id, data is the triangle vertices sequence, vertices is the num of triangles. */

	SGvoid rtTrace(int id, int obj, vec3f light, vec3f dir, int type, float tmin, float tmax, void *prd);
	/* This is the main tracing function. Use the light with starting point light and 
	 * the direction dir to trace the obj that has been pushed with the given obj id.
	 * Parameter id is the graph id, prd is the struct pointer that carries the ray
	 * data for accumulating. */

	SGvoid rtGenerate(int id, vec3i(*generate)(int id, vec2i index, vec2i size));
	/* Set the generate function. It is the first function executed when calling
	 * refresh. the index and size is the position of the drawing pixel and the whole
	 * screen. */

	SGvoid rtMiss(int id, void(*miss)(int id, void *prd));
	/* Set the miss function. That is, when rtTrace function did not hit any objects,
	 the miss function will be called. */

	SGvoid laterDraw(vectInput func, void *param);
	/* The func will be called later in the main thread. */


	/*
	* SG writing interfaces
	* Functions below can be called only in TEXT_MAP mode. Most functions
	* are similar to those in BIT_MAP mode.
	*/

	SGvoid setBfc(int bgc, int fgc);
	/* Set background color and foreground color. */

	SGvoid clearText();
	/* Fill the whole screen to the current background color. */

	SGvoid setCharColor(char color, int x, int y);
	/* Set the background and foreground color of coordinate (x, y). */

	SGvoid setCharBgc(char color, int x, int y);
	/* Set the background color of coordinate (x, y). */

	SGvoid setCharFgc(char color, int x, int y);
	/* Set the foreground color of coordinate (x, y). */

	SGint getShort(int x, int y);
	/* Returns the character and color of coordinate (x, y). */

	SGvoid writeChar(char c, int x, int y);
	/* Put down one character c with current color at (x, y). */

	SGvoid writeString(const char *s, int x, int y);
	/* Put down one string s with current color at (x, y). */

	SGint getText(int left, int top, int right, int bottom, textMap *text);
	/* Copy the text messages of the given area to parameter text. */

	SGvoid putText(int left, int top, textMap *text);
	/* Paste the text messages at (left, top). */


	/*
	* SG system interfaces
	* Basic IO functions to make the program executive. Widget is
	* for advanced graphic programming and for SG Creater to use.
	*/

	SGint biosKey(int cmd);
	/* Main function to deal with key event. The parameter cmd can be
	* 0 or 1. Details of the usage are in instruction pdf. */

	SGvoid clearKeyBuffer();
	/* Delete all key events before current time. */

	vec2i mousePos();
	/* Returns the current position of the cursor. */

	SGint mouseStatus(int b);
	/* Returns whether one of the three mouse button is push down.
	* Parameter b can be SG_LEFT_BUTTON or SG_RIGHT_BUTTON or
	* SG_MIDDLE_BUTTON. */

	vec3i biosMouse(int cmd);
	/* Main function to deal with mouse event. The parameter cmd can be
	* 0 or 1. Details of the usage are in instruction pdf. */

	SGvoid clearMouseBuffer();
	/* Delete all mouse events before current time. */

	SGvoid enablePanel();
	/* Enable right click and move the mouse to select one funtion. */

	SGvoid disablePanel();
	/* Disable right click and move the mouse to select one funtion. */

	SGint addPanelItem(const char *name, vect function, int shift, int ctrl);
	/* Add one function to the input panel, the name is used to shown when
	* moving towards the correspondent direction, and when right button is
	* released, the function will be executed. The parameter shift and ctrl
	* means if this function need to press these two key. The return value
	* is the id of this item.*/

	SGint changePanelItem(int id, const char *name, vect function);
	/* Change the name and function of an input panel item with the given id.
	* The return value is id or SG_OBJECT_NOT_FOUND.*/

	SGint deletePanelItem(int id);
	/* delete one input panel item with the given id. The return value is
	* the error if occured. */

	SGvoid showMouse();
	/* Make the cursor visible. */

	SGvoid hideMouse();
	/* Make the cursor disvisible. */

	SGvoid setMousePos(int x, int y);
	/* Set the cursor position. */

	SGvoid setMouseIcon(SGWINSTR icon);
	/* Set the icon of the cursor. */

	SGint initMainMenu();
	/* Allow this program to use windows main menu. The return value
	* is the main menu id which will be used when add lists or items into
	* main menu. */

	SGint addMainMenuList(const char *title, int id);
	/* Add a new list of name title into the menu with the given id. The
	* return value is the new list id.*/

	SGint addMainMenuItem(const char *title, int id, void(*func)());
	/* Add a new item of name title into the menu with the given id.
	* The parameter func is the callback function which means that it
	* will be called after the user click the item. */

	SGint addMainMenuSeparator(int id);
	/* Add a separate line in the list with given id. */

	SGint enableMainItem(int id);
	/* Make the item or menu of id enabled. That is, it can be clicked. */

	SGint disableMainItem(int id);
	/* Make the item or menu of id disabled. That is, it can't be clicked.*/

	SGvoid checkMainItem(int id);
	/* Make the item of id checked with a tick on the left.*/

	SGvoid uncheckMainItem(int id);
	/* Make the item of id unchecked and clear the tick on the left.*/

	SGvoid addTray();
	/* Add the application icon in the tray bar and enable the program
	* to hide to tray. */

	SGvoid hideToTray();
	/* Hide the whidow, and it will be shown again after clicking the icon
	* in the tray.*/

	SGvoid restoreFromTray();
	/* Show the window again. */

	SGint initTrayMenu();
	/* Enable showing the menu when right click the icon in the tray. The
	* return value is the tray menu id. */

	SGint addTrayMenuList(const char *title, int id);
	/* Add a list in the tray menu. Parameters are same to main menu.*/

	SGint addTrayMenuItem(const char *title, int id, void(*func)());
	/* Add an item in the tray menu. Parameters are same to main menu.*/

	SGint addTrayMenuSeparator(int id);
	/* Add a separator in the tray menu. Parameter is same to main menu.*/

	SGint enableTrayItem(int id);
	/* Make the item or menu of id enabled. That is, it can be clicked. */

	SGint disableTrayItem(int id);
	/* Make the item or menu of id disabled. That is, it can't be clicked.*/

	SGvoid checkTrayItem(int id);
	/* Make the item of id checked with a tick on the left.*/

	SGvoid uncheckTrayItem(int id);
	/* Make the item of id unchecked and clear the tick on the left.*/

	SGint createPopupMenu();
	/* Init one popup menu, the return value is the menu id. */

	SGint addPopupMenuList(const char *title, int id);
	/* Add a list in the popup menu. Parameters are same to main menu.*/

	SGint addPopupMenuItem(const char *title, int id, void(*func)());
	/* Add an item in the popup menu. Parameters are same to main menu.*/

	SGint addPopupMenuSeparator(int id);
	/* Add a separator in the popup menu. Parameter is same to main menu.*/

	SGint enablePopupItem(int id);
	/* Make the item or menu of id enabled. That is, it can be clicked. */

	SGint disablePopupItem(int id);
	/* Make the item or menu of id disabled. That is, it can't be clicked.*/

	SGvoid checkPopupItem(int id);
	/* Make the item of id checked with a tick on the left.*/

	SGvoid uncheckPopupItem(int id);
	/* Make the item of id unchecked and clear the tick on the left.*/

	SGint finishPopupMenu(int id);
	/* The popup menu need to be finished by the programmer which means
	* it is ready to show. */

	SGint showPopupMenu(int menu, int x, int y);
	/* The popup menu with given parameter can be shown with top-left corner at (x, y)
	* by calling this function. */


	/*
	* SG widgets management.
	* Use these functions to build and control widgets.
	*/

	SGvoid setBackgroundRefresh(void(*refresh)(int left, int top, int right, int bottom));
	/* When widgets changes and the background need to be redrew,
	* the function refresh will be called. The parameter of refresh is
	* the area that need to be redrew. */

	SGvoid useBackgroundRefresh(int left, int top, int right, int bottom);
	/* Refresh the background of the given area. */

	widget newWidget(enum _control type, const char *name);
	/* Returns a widget with default parameter. */

	widget newCombinedWidget(int num, const char *name, ...);
	/* Create a combined widget. Pass the sub widget number and
	* then those sub widget pointers.*/

	SGvoid registerWidget(widget obj);
	/* After setting all the parameters, give back the widget to
	* the system. After this function, all callbacks are activated.*/

	SGvoid easyWidget(int type, const char *name,
		int x, int y, int width, int height, const char *content, void(*click)(widget *obj));
	/* Create and register a widget fastly. */

	widget *getWidgetByName(const char *name);
	/* Get the widget structure by the given name. */

	SGvoid refreshWidget(const char *name);
	/* After modifing the structure returned by getWidgetByName, apply the
	 * modification. */

	SGint inWidget(widget *obj, int x, int y);
	/* Judge if coordinate (x, y) is in widget obj. */

	SGint crossWidget(widget *obj, int left, int top, int right, int bottom);
	/* Judge if rectangle (left, top, right, bottom) is crossing widget obj. */

	SGint showWidget(const char *name);
	/* Make the widget of name visible. */

	SGint ceaseWidget(const char *name);
	/* Make the widget of name disvisible. */

	SGint deleteWidget(const char *name);
	/* Delete the widget with the given name. */

	int getSubWidget(const char *parent, const char *sub);
	/* Returns the sub widget with the given name in parent's childs. */

	SGvoid insertSubWidget(const char *parent, int sub, int index);
	/* Insert a child widget. The index decides the display order. */

	SGvoid deleteSubWidget(const char *parent, const char *name);
	/* Delete the child widget of the given name. */

	int moveWidget(const char *name, int xDelta, int yDelta);
	/* Move the widget to (x + xDelta, y + yDelta) with the given name. */

	void setWidgetTop(const char *name);
	/* Set the widget with the given name to the top which means no other
	* widgets can conver it. */

	void setWidgetBottom(const char *name);
	/* Set the widget with the given name to the bottom which means any
	* widget can conver it. */

	void widgetCover(int window, int id, int left, int top, int right, int bottom);
	/* Redraw the widget covered by the widget */

	/*
	* SG data interfaces
	* These functions are used to deal with big data.
	*/

	struct JSON *createJson();
	/* Create an empty json object. */

	struct JSON *createJsonArray();
	/* Create an empty json array.*/

	void freeJson(struct JSON *json);
	/* Safely free the memory of the given structure. */

	struct JSON *readJson(const char *json);
	/* Create a json object or array with the given string. */

	char *writeJson(struct JSON *json);
	/* Give the correspondent string with the given json. */

	struct JSON_Item *getContent(struct JSON *json, const char *name);
	/* Get the item in a json object with the given name. */

	struct JSON_Item *getElement(struct JSON *json, int idx);
	/* Get the item in a json array with the given index. */

	void deleteContent(struct JSON *json, const char *name);
	/* Delete the item in a json object with the given name. */

	void deleteElement(struct JSON *json, int idx);
	/* Delete the item in a json array with the given index. */

	void setIntContent(struct JSON *json, const char *name, SGint i);
	void setFloatContent(struct JSON *json, const char *name, SGfloat f);
	void setCharContent(struct JSON *json, const char *name, SGchar c);
	void setBoolContent(struct JSON *json, const char *name, SGbool b);
	void setStringContent(struct JSON *json, const char *name, const char *s);
	void setObjectContent(struct JSON *json, const char *name, struct JSON *j);
	void setArrayContent(struct JSON *json, const char *name, struct JSON *j);
	/* If there is an item in json object with the given name, then reset its
	* value. Or else, add this item. */

	void setIntElement(struct JSON *json, int idx, SGint i);
	void setFloatElement(struct JSON *json, int idx, SGfloat f);
	void setCharElement(struct JSON *json, int idx, SGchar c);
	void setBoolElement(struct JSON *json, int idx, SGbool b);
	void setStringElement(struct JSON *json, int idx, const char *s);
	void setObjectElement(struct JSON *json, int idx, struct JSON *j);
	void setArrayElement(struct JSON *json, int idx, struct JSON *j);
	/* If there is an item in json array with the given index, then reset its
	* value. Or else, add this item. */

	/*
	* SG encryption interfaces
	* These functions are used to encrypt and decrypt.
	*/

	void DESEncrypt(char *plain, char *key, char *cipher);
	void DESDecrypt(char *cipher, char *key, char *plain);
	void AESEncrypt(char *plain, char *key, char *cipher);
	void AESDecrypt(char *cipher, char *key, char *plain);
	void RSAEncrypt(char *plain, char *key, char *cipher);
	void RSADecrypt(char *cipher, char *key, char *plain);


	/*
	* SG main function
	* These two functions play the same role as main().
	*/

	void sgSetup();
	/* For initializing. */

	void sgLoop();
	/* For main loop. */

#define SGL_GRAPHICS_FRAME
#define SGL_CONSOLE_FRAME \
void sgSetup() {} \
void sgLoop() {}


#ifdef __cplusplus
}
#endif
#endif


