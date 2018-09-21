#define _CRT_SECURE_NO_WARNINGS
#include "..\include\inner.h"
#include "..\include\winsgl.h"

/*
* Global struct variables.
* Started in v0.0.0
*/
struct _win *_Window;
static struct _screen *_Screen;
static struct _key *_Key;
static struct _mouse *_Mouse;
static struct _vect *_Vector;

/*
* Global status variables.
* Started in v0.0.0
*/
int _sglMode, _innerFunc;
int _scrResizeable = 0;
void(*_resizeFunc)(int x, int y) = NULL;
int _delaySE = 0, _inDelay = 0;
clock_t _delayBegin, _delayEnd, _vectDelta = 50;
int _vectKey = 0;
int _enMouse = 0, _enKey = 0;
int _activePage = 0, _visualPage = 0;
int _mainMenu = 0, _tmpItem = 0;
int _useTray = 0, _trayMenu = 0, _tmCreated = 0;
int _inLoop = 0;
int _enOpenGL = 0;

int currentWindow = -1;

/*
* Lattice font library for 256 asciis.
* Started in v0.0.0
*/
byte letters[256][8] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x00
{ 0x7E, 0x81, 0xA5, 0x81, 0xBD, 0x99, 0x81, 0x7E }, //0x01
{ 0x7E, 0xFF, 0xDB, 0xFF, 0xC3, 0xE7, 0xFF, 0x7E }, //0x02
{ 0x6C, 0xFE, 0xFE, 0xFE, 0x7C, 0x38, 0x10, 0x00 }, //0x03
{ 0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x10, 0x00 }, //0x04
{ 0x38, 0x6C, 0x6C, 0xEE, 0xC6, 0x7C, 0x38, 0x7C }, //0x05
{ 0x00, 0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x7C }, //0x06
{ 0x00, 0x00, 0x18, 0x3C, 0x3C, 0x18, 0x00, 0x00 }, //0x07
{ 0xFF, 0xFF, 0xE7, 0xC3, 0xC3, 0xE7, 0xFF, 0xFF }, //0x08
{ 0x00, 0x3C, 0x66, 0x42, 0x42, 0x66, 0x3C, 0x00 }, //0x09
{ 0xFF, 0xC3, 0x99, 0xBD, 0xBD, 0x99, 0xC3, 0xFF }, //0x0A
{ 0x0F, 0x07, 0x0D, 0x7C, 0xCC, 0xCC, 0xCC, 0x78 }, //0x0B
{ 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x7E, 0x18 }, //0x0C
{ 0x3F, 0x30, 0x3F, 0x30, 0x30, 0x70, 0xF0, 0xE0 }, //0x0D
{ 0x7F, 0x63, 0x7F, 0x63, 0x63, 0x67, 0xE6, 0xC0 }, //0x0E
{ 0x99, 0x5A, 0x3C, 0xE7, 0xE7, 0x3C, 0x5A, 0x99 }, //0x0F
{ 0x80, 0xE0, 0xF8, 0xFE, 0xF8, 0xE0, 0x80, 0x00 }, //0x10
{ 0x02, 0x0E, 0x3E, 0xFE, 0x3E, 0x0E, 0x02, 0x00 }, //0x11
{ 0x18, 0x3C, 0x7E, 0x18, 0x7E, 0x3C, 0x18, 0x00 }, //0x12
{ 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x66, 0x00 }, //0x13
{ 0x7F, 0xDB, 0xDB, 0x7B, 0x1B, 0x1B, 0x1B, 0x00 }, //0x14
{ 0x3E, 0x63, 0x3C, 0x66, 0x66, 0x3C, 0xC6, 0x7C }, //0x15
{ 0x00, 0x00, 0x00, 0x00, 0x7E, 0x7E, 0x7E, 0x00 }, //0x16
{ 0x18, 0x3C, 0x7E, 0x18, 0x7E, 0x3C, 0x18, 0xFF }, //0x17
{ 0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x00 }, //0x18
{ 0x18, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x00 }, //0x19
{ 0x00, 0x18, 0x0C, 0xFE, 0x0C, 0x18, 0x00, 0x00 }, //0x1A
{ 0x00, 0x30, 0x60, 0xFE, 0x60, 0x30, 0x00, 0x00 }, //0x1B
{ 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xFE, 0x00, 0x00 }, //0x1C
{ 0x00, 0x24, 0x66, 0xFF, 0x66, 0x24, 0x00, 0x00 }, //0x1D
{ 0x00, 0x18, 0x3C, 0x7E, 0xFF, 0xFF, 0x00, 0x00 }, //0x1E
{ 0x00, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00, 0x00 }, //0x1F
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //Space'
{ 0x30, 0x78, 0x78, 0x30, 0x30, 0x00, 0x30, 0x00 }, //!
{ 0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00 }, //"
{ 0x6C, 0x6C, 0xFE, 0x6C, 0xFE, 0x6C, 0x6C, 0x00 }, //#
{ 0x30, 0x7C, 0xC0, 0x78, 0x0C, 0xF8, 0x30, 0x00 }, // $
{ 0x00, 0xC6, 0xCC, 0x18, 0x30, 0x66, 0xC6, 0x00 }, // %
{ 0x38, 0x6C, 0x38, 0x76, 0xDC, 0xCC, 0x76, 0x00 }, // &
{ 0x60, 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '
{ 0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x00 }, // (
{ 0x60, 0x30, 0x18, 0x18, 0x18, 0x30, 0x60, 0x00 }, // )
{ 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00 }, // *
{ 0x00, 0x30, 0x30, 0xFC, 0x30, 0x30, 0x00, 0x00 }, // +
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x60 }, // ,
{ 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00 }, // -
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00 }, // .
{ 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00 }, // / 
{ 0x7C, 0xC6, 0xC6, 0xD6, 0xC6, 0xC6, 0x7C, 0x00 }, //0 
{ 0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xFC, 0x00 }, //1
{ 0x78, 0xCC, 0x0C, 0x38, 0x60, 0xCC, 0xFC, 0x00 }, //2
{ 0x78, 0xCC, 0x0C, 0x38, 0x0C, 0xCC, 0x78, 0x00 }, //3
{ 0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x1E, 0x00 }, //4
{ 0xFC, 0xC0, 0xF8, 0x0C, 0x0C, 0xCC, 0x78, 0x00 }, //5
{ 0x38, 0x60, 0xC0, 0xF8, 0xCC, 0xCC, 0x78, 0x00 }, //6
{ 0xFC, 0xCC, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x00 }, //7
{ 0x78, 0xCC, 0xCC, 0x78, 0xCC, 0xCC, 0x78, 0x00 }, //8
{ 0x78, 0xCC, 0xCC, 0x7C, 0x0C, 0x18, 0x70, 0x00 }, //9
{ 0x00, 0x30, 0x30, 0x00, 0x30, 0x30, 0x00, 0x00 }, //:
{ 0x00, 0x30, 0x30, 0x00, 0x30, 0x30, 0x60, 0x00 }, //;
{ 0x18, 0x30, 0x60, 0xC0, 0x60, 0x30, 0x18, 0x00 }, //<
{ 0x00, 0x00, 0xFC, 0x00, 0x00, 0xFC, 0x00, 0x00 }, //=
{ 0x60, 0x30, 0x18, 0x0C, 0x18, 0x30, 0x60, 0x00 }, //>
{ 0x78, 0xCC, 0xCC, 0x18, 0x30, 0x00, 0x30, 0x00 }, //?
{ 0x7C, 0xC6, 0xDE, 0xDE, 0xDE, 0xC0, 0x78, 0x00 }, //@
{ 0x30, 0x78, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0x00 }, //A
{ 0xFC, 0x66, 0x66, 0x7C, 0x66, 0x66, 0xFC, 0x00 }, //B
{ 0x3C, 0x66, 0xC0, 0xC0, 0xC0, 0x66, 0x3C, 0x00 }, //C
{ 0xF8, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00 }, //D
{ 0xFE, 0x62, 0x68, 0x78, 0x68, 0x62, 0xFE, 0x00 }, //E
{ 0xFE, 0x62, 0x68, 0x78, 0x68, 0x60, 0xF0, 0x00 }, //F
{ 0x3C, 0x66, 0xC0, 0xC0, 0xCE, 0x66, 0x3E, 0x00 }, //G
{ 0xCC, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0xCC, 0x00 }, //H
{ 0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00 }, //I
{ 0x1E, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78, 0x00 }, //J
{ 0xC6, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0xC6, 0x00 }, //K
{ 0xF0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xFE, 0x00 }, //L
{ 0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0x00 }, //M
{ 0xC6, 0xE6, 0xF6, 0xDE, 0xCE, 0xC6, 0xC6, 0x00 }, //N
{ 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00 }, //O
{ 0xFC, 0x66, 0x66, 0x7C, 0x60, 0x60, 0xF0, 0x00 }, //P
{ 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xD6, 0x7C, 0x06 }, //Q
{ 0xFC, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0xE6, 0x00 }, //R
{ 0x7C, 0xC6, 0x60, 0x38, 0x0C, 0xC6, 0x7C, 0x00 }, //S
{ 0xFF, 0x99, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00 }, //T
{ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x00 }, //U
{ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00 }, //V
{ 0xC6, 0xC6, 0xD6, 0xFE, 0xFE, 0xEE, 0xC6, 0x00 }, //W
{ 0xC6, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0xC6, 0x00 }, //X
{ 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x30, 0x78, 0x00 }, //Y
{ 0xFE, 0xC6, 0x8C, 0x18, 0x32, 0x66, 0xFE, 0x00 }, //Z
{ 0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x00 }, //[
{ 0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x02, 0x00 }, //\/
{ 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00 }, //]
{ 0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00 }, //^
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF }, //_
{ 0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00 }, //`
{ 0x00, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00 }, //a
{ 0xE0, 0x60, 0x7C, 0x66, 0x66, 0x66, 0xDC, 0x00 }, //b
{ 0x00, 0x00, 0x78, 0xCC, 0xC0, 0xCC, 0x78, 0x00 }, //c
{ 0x1C, 0x0C, 0x7C, 0xCC, 0xCC, 0xCC, 0x76, 0x00 }, //d
{ 0x00, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00 }, //e
{ 0x38, 0x6C, 0x60, 0xF0, 0x60, 0x60, 0xF0, 0x00 }, //f
{ 0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8 }, //g
{ 0xE0, 0x60, 0x6C, 0x76, 0x66, 0x66, 0xE6, 0x00 }, //h
{ 0x30, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00 }, //i
{ 0x0C, 0x00, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78 }, //j
{ 0xE0, 0x60, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0x00 }, //k
{ 0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00 }, //l
{ 0x00, 0x00, 0xCC, 0xFE, 0xFE, 0xD6, 0xD6, 0x00 }, //m
{ 0x00, 0x00, 0xF8, 0xCC, 0xCC, 0xCC, 0xCC, 0x00 }, //n
{ 0x00, 0x00, 0x78, 0xCC, 0xCC, 0xCC, 0x78, 0x00 }, //o
{ 0x00, 0x00, 0xDC, 0x66, 0x66, 0x7C, 0x60, 0xF0 }, //p
{ 0x00, 0x00, 0x76, 0x6C, 0x6C, 0x7C, 0x0C, 0x1E }, //q
{ 0x00, 0x00, 0xDC, 0x76, 0x60, 0x60, 0xF0, 0x00 }, //r
{ 0x00, 0x00, 0x7C, 0xC0, 0x78, 0x0C, 0xF8, 0x00 }, //s
{ 0x10, 0x30, 0x7C, 0x30, 0x30, 0x36, 0x1C, 0x00 }, //t
{ 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00 }, //u
{ 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00 }, //v
{ 0x00, 0x00, 0xC6, 0xD6, 0xFE, 0xFE, 0x6C, 0x00 }, //w
{ 0x00, 0x00, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0x00 }, //x
{ 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8 }, //y
{ 0x00, 0x00, 0xFC, 0x98, 0x30, 0x64, 0xFC, 0x00 }, //z
{ 0x1C, 0x30, 0x30, 0xE0, 0x30, 0x30, 0x1C, 0x00 }, //{
{ 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00 }, //|
{ 0xE0, 0x30, 0x30, 0x1C, 0x30, 0x30, 0xE0, 0x00 }, //}
{ 0x00, 0x00, 0x72, 0xD6, 0x9C, 0x00, 0x00, 0x00 }, //~
{ 0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0x00 }, //0x7F
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x80
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x81
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x82
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x83
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x84
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x85
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x86
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x87
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x88
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x89
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x8A
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x8B
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x8C
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x8D
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x8E
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x8F
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x90
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x91
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x92
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x93
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x94
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x95
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x96
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x97
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x98
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x99
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x9A
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x9B
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x9C
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x9D
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x9E
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0x9F
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xA0
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xA1
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xA2
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xA3
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xA4
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xA5
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xA6
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xA7
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xA8
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xA9
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xAA
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xAB
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xAC
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xAD
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xAE
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xAF
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xB0
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xB1
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xB2
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xB3
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xB4
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xB5
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xB6
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xB7
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xB8
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xB9
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xBA
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xBB
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xBC
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xBD
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xBE
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xBF
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xC0
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xC1
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xC2
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xC3
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xC4
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xC5
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xC6
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xC7
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xC8
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xC9
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xCA
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xCB
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xCC
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xCD
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xCE
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xCF
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xD0
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xD1
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xD2
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xD3
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xD4
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xD5
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xD6
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xD7
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xD8
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xD9
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xDA
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xDB
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xDC
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xDD
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xDE
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xDF
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xE0
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xE1
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xE2
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xE3
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xE4
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xE5
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xE6
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xE7
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xE8
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xE9
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xEA
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xEB
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xEC
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xED
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xEE
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xEF
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xF0
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xF1
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xF2
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xF3
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xF4
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xF5
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xF6
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xF7
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xF8
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xF9
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xFA
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xFB
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xFC
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xFD
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xFE
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0xFF
};

/*
* Global font variables.
* Started in v2.2.0
*/
struct _text text;
font tf;

/*
* Global midi records.
* Started in v3.0.0
*/
struct _midi midi;
struct _mci mci;

/*
* Global menu list.
* Started in v3.0.1
*/
struct _menu mainMenu;
struct _menu trayMenu;
struct _menu popupMenu[SG_MAX_POPUP_NUM];
int _popupNum = 0;

/*
* Multiple thread programming.
* Started in v3.0.1
*/
void vectDefault(void) {
	return;
}
int tmpThread;
vect threadFunc = vectDefault;
int interval = 0, times = 0;
long windowSem = 1;

/*
* Global input panel info.
* Started in v4.0.1
*/
struct _panel panel;
int _enPanel = 0;

/*
 * Sub window info.
 * Started in v4.0.0.
 */
typedef LRESULT(*subWndProc)
	(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
struct _Sub {
	int winWidth, winHeight;
	HWND hwnd;
	HICON hIcon;
	char *winName;
	int scrResizeable;
	int inLoop;

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
	int passWidget;
	int tipShowing;
} _wndList[SG_MAX_WINDOW_NUM];
int subNum = 0;

/*
* Main functions.
* Started in v2.0.0
*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static int  cxClient, cyClient, cxSource, cySource;
	static HBITMAP hBitmap;
	LPPOINT lPoint;
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message) {
	case WM_CREATE:
		cxSource = _Window->winWidth;
		cySource = _Window->winHeight;
		SetTimer(hwnd, INTERUPT_DELTA_HANDLE, _vectDelta, NULL);
		SetTimer(hwnd, TIMER_DELTA_HANDLE, 10, NULL);
		_Window->hwnd = hwnd;

		if (_mainMenu) {
			_createMenu(MT_MAIN, NULL, mainMenu.hm);
			SetMenu(_Window->hwnd, mainMenu.hm);
		}
		return 0;

	case WM_COMMAND:
		_callMenu(&mainMenu, LOWORD(wParam));
		_callMenu(&trayMenu, LOWORD(wParam));
		return 0;

	case WM_SIZE:
		_Window->winWidth = cxClient = LOWORD(lParam);
		_Window->winHeight = cyClient = HIWORD(lParam);
		if (_resizeFunc)_resizeFunc(cxClient, cyClient);
		return 0;

	case WM_PAINT:
		//_drawWidget(WIDGET_FRONT);
		_drawPanel();

		hdc = BeginPaint(hwnd, &ps);
		if (_visualPage == 0) _makeBitmap(hdc, _Screen->buffer1->data, _Screen->buffer1->sizeX, _Screen->buffer1->sizeY, 24);
		else _makeBitmap(hdc, _Screen->buffer2->data, _Screen->buffer2->sizeX, _Screen->buffer2->sizeY, 24);
		EndPaint(hwnd, &ps);

		_clearPanel();
		//_drawWidget(WIDGET_BACK);
		return 0;

	case WM_TIMER:
		if (wParam == TIMER_DELTA_HANDLE) {
			if (!_inDelay) {
				startSubWindow(-1);
				sgLoop();
				endSubWindow();
			}
			if (_sglMode == TEXT_MAP)_textLoop();
		}
		if (wParam == INTERUPT_DELTA_HANDLE) {
			_Vector->_v8();
		}
		InvalidateRect(hwnd, NULL, FALSE);
		return 0;

	case WM_TRAY:
		switch (lParam) {
		case WM_LBUTTONDOWN:
			ShowWindow(hwnd, SW_SHOW);
			SetForegroundWindow(hwnd);
			break;
		case WM_LBUTTONDBLCLK:
			ShowWindow(hwnd, SW_RESTORE);
			SetForegroundWindow(hwnd);
			break;
		case WM_RBUTTONDOWN:
			lPoint = (LPPOINT)malloc(sizeof(LPPOINT));
			GetCursorPos(lPoint);
			if (!_tmCreated) {
				_tmCreated = 1;
				_createMenu(MT_TRAY, NULL, trayMenu.hm);
			}
			TrackPopupMenu(trayMenu.hm, TPM_LEFTALIGN, lPoint->x, lPoint->y, 0, hwnd, NULL);
			break;
		}
		break;

	case	WM_SETCURSOR:
		break;

	case WM_KEYDOWN:
		sgSpecial((int)wParam, 0, 0);
		return 0;

	case WM_CHAR:
		sgKey((int)wParam |
			((GetKeyState(VK_CONTROL) & 0x8000) >> 1) |
			((GetKeyState(VK_SHIFT) & 0x8000) >> 2), 0, 0);
		return 0;

	case WM_KEYUP:
		sgKeyUp((int)wParam, 0, 0);
		sgSpecialUp((int)wParam, 0, 0);
		return 0;

	case WM_MOUSEMOVE:
		if (!wParam)sgMouse(LOWORD(lParam), HIWORD(lParam));
		else sgDrag(LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_MOUSEWHEEL:
		sgWheel((short)HIWORD(wParam));
		break;

	case WM_LBUTTONDOWN:
		sgClick(SG_LEFT_BUTTON, SG_BUTTON_DOWN, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_MBUTTONDOWN:
		sgClick(SG_MIDDLE_BUTTON, SG_BUTTON_DOWN, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_RBUTTONDOWN:
		sgClick(SG_RIGHT_BUTTON, SG_BUTTON_DOWN, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_LBUTTONUP:
		sgClick(SG_LEFT_BUTTON, SG_BUTTON_UP, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_MBUTTONUP:
		sgClick(SG_MIDDLE_BUTTON, SG_BUTTON_UP, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_RBUTTONUP:
		sgClick(SG_RIGHT_BUTTON, SG_BUTTON_UP, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
LRESULT CALLBACK SubWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static int  cxClient, cyClient;
	HDC hdc;
	PAINTSTRUCT ps;

	int index;
	for (index = 0; index < SG_MAX_WINDOW_NUM; index++) {
		if (hwnd == _wndList[index].hwnd)break;
	}
	if (index == SG_MAX_WINDOW_NUM)
		return DefWindowProc(hwnd, message, wParam, lParam);
	if (!_wndList[index].inLoop) {
		_wndList[index].inLoop = 1;
		SetTimer(hwnd, TIMER_DELTA_HANDLE, 10, NULL);
	}
	switch (message) {
	case WM_CREATE:
		return 0;

	case WM_COMMAND:
		return 0;

	case WM_SIZE:
		_wndList[index].winWidth = cxClient = LOWORD(lParam);
		_wndList[index].winHeight = cyClient = HIWORD(lParam);
		if (_wndList[index].resizeFunc)_wndList[index].resizeFunc(cxClient, cyClient);
		return 0;

	case WM_PAINT:
		//_drawSubWidget(index, WIDGET_FRONT);
		hdc = BeginPaint(hwnd, &ps);
		if (_wndList[index].visualPage == 0) _makeSubBitmap(hdc, index, _wndList[index].buffer1->data,
			_wndList[index].buffer1->sizeX, _wndList[index].buffer1->sizeY, 24);
		else _makeSubBitmap(hdc, index, _wndList[index].buffer2->data,
			_wndList[index].buffer2->sizeX, _wndList[index].buffer2->sizeY, 24);
		EndPaint(hwnd, &ps);
		//_drawSubWidget(index, WIDGET_BACK);
		return 0;

	case WM_TIMER:
		_wndList[index].loop();
		InvalidateRect(hwnd, NULL, FALSE);
		return 0;

	case WM_TRAY:
		return 0;

	case	WM_SETCURSOR:
		return 0;

	case WM_KEYDOWN:
		sgSubSpecial(index, (int)wParam, 0, 0);
		return 0;

	case WM_CHAR:
		sgSubKey(index, (int)wParam |
			((GetKeyState(VK_CONTROL) & 0x8000) >> 1) |
			((GetKeyState(VK_SHIFT) & 0x8000) >> 2), 0, 0);
		return 0;

	case WM_KEYUP:
		sgSubKeyUp(index, (int)wParam, 0, 0);
		sgSubSpecialUp(index, (int)wParam, 0, 0);
		return 0;

	case WM_MOUSEMOVE:
		if (!wParam)sgSubMouse(index, LOWORD(lParam), HIWORD(lParam));
		else sgSubDrag(index, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_MOUSEWHEEL:
		sgSubWheel(index, (short)HIWORD(wParam));
		return 0;

	case WM_LBUTTONDOWN:
		sgSubClick(index, SG_LEFT_BUTTON, SG_BUTTON_DOWN, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_MBUTTONDOWN:
		sgSubClick(index, SG_MIDDLE_BUTTON, SG_BUTTON_DOWN, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_RBUTTONDOWN:
		sgSubClick(index, SG_RIGHT_BUTTON, SG_BUTTON_DOWN, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_LBUTTONUP:
		sgSubClick(index, SG_LEFT_BUTTON, SG_BUTTON_UP, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_MBUTTONUP:
		sgSubClick(index, SG_MIDDLE_BUTTON, SG_BUTTON_UP, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_RBUTTONUP:
		sgSubClick(index, SG_RIGHT_BUTTON, SG_BUTTON_UP, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_DESTROY:
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	HWND hwnd;
	MSG msg;
	int mh = GetSystemMetrics(SM_CYMENU);
	SGWINSTR _wd = NULL;

	sgInit();

	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = TEXT("WindowClass");
	if (_Window->hIcon == NULL)
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	else
		wc.hIcon = _Window->hIcon;
	if (_Window->hIcon == NULL)
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	else
		wc.hIconSm = _Window->hIcon;

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindow(wc.lpszClassName, _wd = _widen(_Window->winName),
		WS_OVERLAPPEDWINDOW,
		_Window->posLeft, _Window->posUp,
		_Window->winWidth + 16, _Window->winHeight + 16 + GetSystemMetrics(SM_CYCAPTION) +
		(_mainMenu ? GetSystemMetrics(SM_CYMENU) : 0),
		NULL, NULL, hInstance, NULL);
	free((void *)_wd);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	_inLoop = 1;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

/*
* Remained OpenGL functions.
* Origin OpenGL functions start in v0.0.0.
* These OpenGL callback functions start in v2.0.0
*/
void sgInit() {
	int i, winPx, winPy;

	//init the global structs.
	_Screen = (struct _screen*)malloc(sizeof(struct _screen));
	_Window = (struct _win*)malloc(sizeof(struct _win));
	_Key = (struct _key*)malloc(sizeof(struct _key));
	_Mouse = (struct _mouse*)malloc(sizeof(struct _mouse));
	_Vector = (struct _vect*)malloc(sizeof(struct _vect));
	Widget = (struct _widget*)malloc(sizeof(struct _widget));

	_Key->front = -1;
	_Key->rear = 0;

	_Mouse->front = -1;
	_Mouse->rear = 0;
	_Mouse->left = SG_BUTTON_UP;
	_Mouse->right = SG_BUTTON_UP;
	_Mouse->middle = SG_BUTTON_UP;
	_Mouse->coord = 0;

	_Vector->_v8 = (vect)vectDefault;
	_Vector->_v9 = (vect)vectDefault;

	Widget->count = 0;
	Widget->active = -1;
	for (i = 0; i < SG_QSIZE; i++) {
		Widget->obj[i] = NULL;
	}
	for (i = 0; i < 256; i++) {
		Widget->hash[i] = NULL;
	}

	_Window->winName = (char *)malloc(256 * sizeof(char));
	_Window->winWidth = 640;
	_Window->winHeight = 480;
	_Window->txtWidth = 80;
	_Window->txtHeight = 25;
	_Window->hIcon = NULL;

	_sglMode = BIT_MAP;
	_innerFunc = 0;
	strcpy(_Window->winName, "SGpro");

	panel.maxId = 0;
	panel.cover = (bitMap *)malloc(sizeof(bitMap));
	panel.cover->data = NULL;
	panel.outer = 40;
	panel.inner = 20;
	for (i = 0; i < SG_MAX_PANEL_FUNCTION; i++) {
		panel.normalPanel[i] = NULL;
		panel.shiftPanel[i] = NULL;
		panel.ctrlPanel[i] = NULL;
		panel.shiftctrlPanel[i] = NULL;
	}

	//users' setup.
	sgSetup();

	_prepareText(GetSystemMetrics(SM_CXFULLSCREEN) / 8 * 8 + 8);

	//build the buffer.
	_Screen->content = (textMap *)malloc(sizeof(textMap));
	_Screen->content->width = _Window->txtWidth;
	_Screen->content->height = _Window->txtHeight;
	_Screen->content->content = (unsigned short *)malloc(_Window->txtWidth * _Window->txtHeight * sizeof(unsigned short));
	for (i = 0; i < _Window->txtWidth*_Window->txtHeight; i++)_Screen->content->content[i] = 0xF000;

	_Screen->changed = (textMap *)malloc(sizeof(textMap));
	_Screen->changed->width = _Window->txtWidth;
	_Screen->changed->height = _Window->txtHeight;
	_Screen->changed->content = (unsigned short *)malloc(_Window->txtWidth * _Window->txtHeight * sizeof(unsigned short));
	for (i = 0; i < _Window->txtWidth*_Window->txtHeight; i++)_Screen->changed->content[i] = 0xF000;

	_Screen->textMask = (textMap *)malloc(sizeof(textMap));
	_Screen->textMask->width = _Window->txtWidth;
	_Screen->textMask->height = _Window->txtHeight;
	_Screen->textMask->content = (unsigned short *)malloc(_Window->txtWidth * _Window->txtHeight * sizeof(unsigned short));
	for (i = 0; i < _Window->txtWidth*_Window->txtHeight; i++)_Screen->textMask->content[i] = 0x0000;

	_Screen->buffer1 = (bitMap *)malloc(sizeof(bitMap));
	if (_scrResizeable) {
		_Screen->buffer1->sizeX = GetSystemMetrics(SM_CXFULLSCREEN) / 8 * 8 + 16;
		_Screen->buffer1->sizeY = GetSystemMetrics(SM_CYFULLSCREEN) / 8 * 8 + 16;
	}
	else {
		_Screen->buffer1->sizeX = _sglMode == TEXT_MAP ? _Window->txtWidth*SG_CHAR_WIDTH : _Window->winWidth;
		_Screen->buffer1->sizeY = _sglMode == TEXT_MAP ? _Window->txtHeight*SG_CHAR_HEIGHT : _Window->winHeight;
	}
	_Screen->buffer1->data = (unsigned char *)malloc(_Screen->buffer1->sizeX * _Screen->buffer1->sizeY * 3 * sizeof(unsigned char));
	memset(_Screen->buffer1->data, -1, _Screen->buffer1->sizeX * _Screen->buffer1->sizeY * 3 * sizeof(unsigned char));

	_Screen->buffer2 = (bitMap *)malloc(sizeof(bitMap));
	if (_scrResizeable) {
		_Screen->buffer2->sizeX = GetSystemMetrics(SM_CXFULLSCREEN) / 8 * 8 + 16;
		_Screen->buffer2->sizeY = GetSystemMetrics(SM_CYFULLSCREEN) / 8 * 8 + 16;
	}
	else {
		_Screen->buffer2->sizeX = _sglMode == TEXT_MAP ? _Window->txtWidth*SG_CHAR_WIDTH : _Window->winWidth;
		_Screen->buffer2->sizeY = _sglMode == TEXT_MAP ? _Window->txtHeight*SG_CHAR_HEIGHT : _Window->winHeight;
	}
	_Screen->buffer2->data = (unsigned char *)malloc(_Screen->buffer2->sizeX * _Screen->buffer2->sizeY * 3 * sizeof(char));
	memset(_Screen->buffer2->data, -1, _Screen->buffer2->sizeX * _Screen->buffer2->sizeY * 3 * sizeof(char));

	_Window->winWidth = _sglMode == TEXT_MAP ? _Window->txtWidth*SG_CHAR_WIDTH : _Window->winWidth;
	_Window->winHeight = _sglMode == TEXT_MAP ? _Window->txtHeight*SG_CHAR_HEIGHT : _Window->winHeight;

	_Screen->rgb[0] = 0;
	_Screen->rgb[1] = 0;
	_Screen->rgb[2] = 0;
	_Screen->alpha = 1.f;
	_Screen->bfc = WHITE << 4 | BLACK;

	//create a window.
	winPx = GetSystemMetrics(SM_CXFULLSCREEN);
	winPy = GetSystemMetrics(SM_CYFULLSCREEN);

	if (_Window->winWidth >= winPx)_Window->posLeft = 0;
	else _Window->posLeft = (winPx - _Window->winWidth) / 2;
	if (_Window->winHeight >= winPy)_Window->posUp = 0;
	else _Window->posUp = (winPy - _Window->winHeight) / 2;

	tmpThread = GetThreadId(GetCurrentThread());
}
void sgKey(int cAscii, int x, int y) {
	widgetObj *tmp;
	int ctrl, shift;
	int i;

	ctrl = cAscii & SG_CTRLBIT;
	shift = cAscii & SG_SHIFTBIT;
	if (ctrl)cAscii += 'a' - 1;
	cAscii &= 0xff;
	if (!_enKey || cAscii >= 0x80 || cAscii < 0x20)return;

	_Key->front = (_Key->front + 1) % 1024;
	_Key->keyBuf[_Key->front] = (word)(cAscii | (ctrl | shift));
	_Key->keyBuf[_Key->front] &= (word)0x7fff;
	_vectKey = cAscii;

	if (_sglMode == BIT_MAP) {
		for (i = 0; i < Widget->count; i++) {
			tmp = Widget->obj[i];
			if (tmp->status&WIDGET_SELECTED) {
				tmp->keyDown(tmp, cAscii);
				tmp->keyUser(tmp, cAscii);
			}
		}
	}

	(*_Vector->_v9)();
}
void sgSpecial(int cAscii, int x, int y) {
	widgetObj *tmp;
	int i;

	if (!_enKey)return;

	switch (cAscii) {
	case VK_UP:
		cAscii = SG_UP;
		break;
	case VK_DOWN:
		cAscii = SG_DOWN;
		break;
	case VK_LEFT:
		cAscii = SG_LEFT;
		break;
	case VK_RIGHT:
		cAscii = SG_RIGHT;
		break;
	case 0x11:
		cAscii = SG_CTRL;
		break;
	case 0x10:
		cAscii = SG_SHIFT;
		break;
	case SG_ESC:
	case SG_BACKS:
	case SG_TAB:
	case SG_ENTER:
		break;
	case VK_INSERT:
		cAscii = SG_INSERT;
		break;
	case VK_DELETE:
		cAscii = SG_DELETE;
		break;
	case VK_HOME:
		cAscii = SG_HOME;
		break;
	case VK_END:
		cAscii = SG_END;
		break;
	case VK_NUMLOCK:
		cAscii = SG_NUMLOC;
		break;
	case VK_CAPITAL:
		cAscii = SG_CAPSLOC;
		break;
	case VK_PRIOR:
		cAscii = SG_PAGEUP;
		break;
	case VK_NEXT:
		cAscii = SG_PAGEDOWN;
		break;
	case VK_F1:
		cAscii = SG_F1;
		break;
	case VK_F2:
		cAscii = SG_F2;
		break;
	case VK_F3:
		cAscii = SG_F3;
		break;
	case VK_F4:
		cAscii = SG_F4;
		break;
	case VK_F5:
		cAscii = SG_F5;
		break;
	case VK_F6:
		cAscii = SG_F6;
		break;
	case VK_F7:
		cAscii = SG_F7;
		break;
	case VK_F8:
		cAscii = SG_F8;
		break;
	case VK_F9:
		cAscii = SG_F9;
		break;
	case VK_F10:
		cAscii = SG_F10;
		break;
	case VK_F11:
		cAscii = SG_F11;
		break;
	case VK_F12:
		cAscii = SG_F12;
		break;
	default:
		return;
	}

	_Key->front = (_Key->front + 1) % 1024;
	_Key->keyBuf[_Key->front] = (word)cAscii;
	_Key->keyBuf[_Key->front] &= (word)0x7fff;
	_vectKey = cAscii;

	if (_sglMode == BIT_MAP) {
		for (i = 0; i < Widget->count; i++) {
			tmp = Widget->obj[i];
			if (tmp->status&WIDGET_SELECTED) {
				tmp->keyDown(tmp, cAscii);
				tmp->keyUser(tmp, cAscii);
			}
		}
	}

	(*_Vector->_v9)();
}
void sgKeyUp(int cAscii, int x, int y) {
	widgetObj *tmp;
	int i;

	if (!_enKey || cAscii >= 0x100 || cAscii < 0x20)return;
	if (cAscii >= 0x21 && cAscii <= 0x2F)return;
	if (cAscii >= 0x70 && cAscii <= 0x7B)return;

	_Key->front = (_Key->front + 1) % 1024;
	_Key->keyBuf[_Key->front] = (word)cAscii;
	_Key->keyBuf[_Key->front] |= (word)0x8000;

	if (_sglMode == BIT_MAP) {
		for (i = 0; i < Widget->count; i++) {
			tmp = Widget->obj[i];
			if (tmp->status&WIDGET_SELECTED)
				tmp->keyUp(tmp, cAscii | 0x8000);
		}
	}
}
void sgSpecialUp(int cAscii, int x, int y) {
	widgetObj *tmp;
	int i;

	if (!_enKey)return;

	switch (cAscii) {
	case VK_UP:
		cAscii = SG_UP;
		break;
	case VK_DOWN:
		cAscii = SG_DOWN;
		break;
	case VK_LEFT:
		cAscii = SG_LEFT;
		break;
	case VK_RIGHT:
		cAscii = SG_RIGHT;
		break;
	case 0x11:
		cAscii = SG_CTRL;
		break;
	case 0x10:
		cAscii = SG_SHIFT;
		break;
	case SG_ESC:
	case SG_BACKS:
	case SG_TAB:
	case SG_ENTER:
		break;
	case VK_INSERT:
		cAscii = SG_INSERT;
		break;
	case VK_DELETE:
		cAscii = SG_DELETE;
		break;
	case VK_HOME:
		cAscii = SG_HOME;
		break;
	case VK_END:
		cAscii = SG_END;
		break;
	case VK_NUMLOCK:
		cAscii = SG_NUMLOC;
		break;
	case VK_CAPITAL:
		cAscii = SG_CAPSLOC;
		break;
	case VK_PRIOR:
		cAscii = SG_PAGEUP;
		break;
	case VK_NEXT:
		cAscii = SG_PAGEDOWN;
		break;
	case VK_F1:
		cAscii = SG_F1;
		break;
	case VK_F2:
		cAscii = SG_F2;
		break;
	case VK_F3:
		cAscii = SG_F3;
		break;
	case VK_F4:
		cAscii = SG_F4;
		break;
	case VK_F5:
		cAscii = SG_F5;
		break;
	case VK_F6:
		cAscii = SG_F6;
		break;
	case VK_F7:
		cAscii = SG_F7;
		break;
	case VK_F8:
		cAscii = SG_F8;
		break;
	case VK_F9:
		cAscii = SG_F9;
		break;
	case VK_F10:
		cAscii = SG_F10;
		break;
	case VK_F11:
		cAscii = SG_F11;
		break;
	case VK_F12:
		cAscii = SG_F12;
		break;
	default:
		return;
	}
	_Key->front = (_Key->front + 1) % 1024;
	_Key->keyBuf[_Key->front] = (word)cAscii;
	_Key->keyBuf[_Key->front] |= (word)0x8000;

	if (_sglMode == BIT_MAP) {
		for (i = 0; i < Widget->count; i++) {
			tmp = Widget->obj[i];
			if (tmp->status&WIDGET_SELECTED)
				tmp->keyUp(tmp, cAscii | 0x8000);
		}
	}
}
void sgMouse(int x, int y) {
	bitMap *buf;
	int i;
	widgetObj *tmp;

	if (!_enMouse)return;

	if (_activePage == 0) buf = _Screen->buffer1;
	else buf = _Screen->buffer2;

	if (_scrResizeable) {
		_Mouse->Pos.x = x;
		_Mouse->Pos.y = y;
	}
	else {
		_Mouse->Pos.x = x * buf->sizeX / _Window->winWidth;
		_Mouse->Pos.y = y * buf->sizeY / _Window->winHeight;
	}

	if (_sglMode == BIT_MAP) {
		if (Widget->active != -1) {
			if (inWidget(Widget->obj[Widget->active], x, y))
				Widget->obj[Widget->active]->mouseIn(Widget->obj[Widget->active],
					_Mouse->Pos.x, _Mouse->Pos.y);
			else
				Widget->obj[Widget->active]->mouseOut(Widget->obj[Widget->active],
					_Mouse->Pos.x, _Mouse->Pos.y);
			for (i = 0; i < Widget->count; i++) {
				if (i == Widget->active)continue;

				tmp = Widget->obj[i];
				if (tmp->visible == FALSE)continue;
				if (_Mouse->Pos.x >= tmp->pos.x&&_Mouse->Pos.y >= tmp->pos.y&&
					_Mouse->Pos.x <= tmp->pos.x + tmp->size.x&&_Mouse->Pos.y <= tmp->pos.y + tmp->size.y) {
					tmp->mouseIn(Widget->obj[i], _Mouse->Pos.x, _Mouse->Pos.y);
				}
				else
					tmp->mouseOut(Widget->obj[i], _Mouse->Pos.x, _Mouse->Pos.y);
			}
		}
		else {
			for (i = 0; i < Widget->count; i++) {
				tmp = Widget->obj[i];
				if (tmp->visible == FALSE)continue;
				if (_Mouse->Pos.x >= tmp->pos.x&&_Mouse->Pos.y >= tmp->pos.y&&
					_Mouse->Pos.x <= tmp->pos.x + tmp->size.x&&_Mouse->Pos.y <= tmp->pos.y + tmp->size.y) {
					tmp->mouseIn(Widget->obj[i], _Mouse->Pos.x, _Mouse->Pos.y);
				}
				else
					tmp->mouseOut(Widget->obj[i], _Mouse->Pos.x, _Mouse->Pos.y);
			}
		}
	}
}
void sgDrag(int x, int y) {
	bitMap *buf;
	widgetObj *tmp;
	int i;

	if (!_enMouse)return;

	if (_activePage == 0) buf = _Screen->buffer1;
	else buf = _Screen->buffer2;

	if (_scrResizeable) {
		_Mouse->Pos.x = x;
		_Mouse->Pos.y = y;
	}
	else {
		_Mouse->Pos.x = x * buf->sizeX / _Window->winWidth;
		_Mouse->Pos.y = y * buf->sizeY / _Window->winHeight;
	}

	if (_sglMode == BIT_MAP) {
		if (Widget->active != -1) {
			if (inWidget(Widget->obj[Widget->active], x, y))
				Widget->obj[Widget->active]->mouseIn(Widget->obj[Widget->active],
					_Mouse->Pos.x, _Mouse->Pos.y);
			else
				Widget->obj[Widget->active]->mouseOut(Widget->obj[Widget->active],
					_Mouse->Pos.x, _Mouse->Pos.y);
			for (i = 0; i < Widget->count; i++) {
				if (i == Widget->active)continue;

				tmp = Widget->obj[i];
				if (tmp->visible == FALSE)continue;
				if (inWidget(tmp, x, y))continue;
				if (_Mouse->Pos.x >= tmp->pos.x&&_Mouse->Pos.y >= tmp->pos.y&&
					_Mouse->Pos.x <= tmp->pos.x + tmp->size.x&&_Mouse->Pos.y <= tmp->pos.y + tmp->size.y) {
					if (!(tmp->status&WIDGET_PASS))
						tmp->mouseIn(Widget->obj[i], _Mouse->Pos.x, _Mouse->Pos.y);
				}
				else
					if (tmp->status&WIDGET_PASS)
						tmp->mouseOut(Widget->obj[i], _Mouse->Pos.x, _Mouse->Pos.y);
			}
		}
		else {
			for (i = 0; i < Widget->count; i++) {
				tmp = Widget->obj[i];
				if (tmp->visible == FALSE)continue;
				if (_Mouse->Pos.x >= tmp->pos.x&&_Mouse->Pos.y >= tmp->pos.y&&
					_Mouse->Pos.x <= tmp->pos.x + tmp->size.x&&_Mouse->Pos.y <= tmp->pos.y + tmp->size.y) {
					if (!(tmp->status&WIDGET_PASS))
						tmp->mouseIn(Widget->obj[i], _Mouse->Pos.x, _Mouse->Pos.y);
				}
				else
					if (tmp->status&WIDGET_PASS)
						tmp->mouseOut(Widget->obj[i], _Mouse->Pos.x, _Mouse->Pos.y);
			}
		}
	}
}
void sgClick(int button, int state, int x, int y) {
	static vec2 pos[3];
	bitMap *buf;
	widgetObj *tmp;
	int i;

	if (!_enMouse)return;

	if (_activePage == 0) buf = _Screen->buffer1;
	else buf = _Screen->buffer2;

	if (button == SG_LEFT_BUTTON && state == SG_BUTTON_DOWN) {
		_Mouse->left = SG_BUTTON_DOWN;
		pos[0].x = x;
		pos[0].y = y;
		if (_scrResizeable) {
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].x = x;
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].y = y;
		}
		else {
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].x = x * buf->sizeX / _Window->winWidth;
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].y = y * buf->sizeY / _Window->winHeight;
		}
		_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].z = SG_BUTTON_DOWN | SG_LEFT_BUTTON;
		_Mouse->front = (_Mouse->front + 1) % 1024;

		if (_sglMode == BIT_MAP) {
			if (Widget->active != -1 && inWidget(Widget->obj[Widget->active], x, y)) {
				Widget->obj[Widget->active]->mouseDown(Widget->obj[Widget->active],
					_Mouse->Pos.x, _Mouse->Pos.y, SG_BUTTON_DOWN | SG_LEFT_BUTTON);
				for (i = 0; i < Widget->count; i++) {
					if (i == Widget->active)continue;

					tmp = Widget->obj[i];
					if (tmp->visible == FALSE)continue;
					if (inWidget(tmp, x, y))continue;
					tmp->mouseDown(Widget->obj[i], _Mouse->Pos.x, _Mouse->Pos.y, SG_BUTTON_DOWN | SG_LEFT_BUTTON);
				}
			}
			else {
				for (i = 0; i < Widget->count; i++) {
					tmp = Widget->obj[i];
					if (tmp->visible == FALSE)continue;
					tmp->mouseDown(Widget->obj[i], _Mouse->Pos.x, _Mouse->Pos.y, SG_BUTTON_DOWN | SG_LEFT_BUTTON);
				}
			}
		}
	}
	if (button == SG_LEFT_BUTTON && state == SG_BUTTON_UP) {
		_Mouse->left = SG_BUTTON_UP;
		if (!_Mouse->coord)
			if (pos[0].x != x || pos[0].y != y)return;
		if (_scrResizeable) {
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].x = x;
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].y = y;
		}
		else {
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].x = x * buf->sizeX / _Window->winWidth;
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].y = y * buf->sizeY / _Window->winHeight;
		}
		_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].z = SG_BUTTON_UP | SG_LEFT_BUTTON;
		_Mouse->front = (_Mouse->front + 1) % 1024;

		if (_sglMode == BIT_MAP) {
			if (Widget->active != -1 && inWidget(Widget->obj[Widget->active], x, y)) {
				Widget->obj[Widget->active]->mouseUser(Widget->obj[Widget->active]);
				Widget->obj[Widget->active]->mouseUp(Widget->obj[Widget->active],
					_Mouse->Pos.x, _Mouse->Pos.y, SG_BUTTON_UP | SG_LEFT_BUTTON);
				for (i = 0; i < Widget->count; i++) {
					tmp = Widget->obj[i];
					if (tmp->visible == FALSE)continue;
					if (inWidget(tmp, x, y))continue;
					if (Widget->active == i)continue;
					tmp->mouseUser(Widget->obj[i]);
					tmp->mouseUp(Widget->obj[i], _Mouse->Pos.x, _Mouse->Pos.y, SG_BUTTON_UP | SG_LEFT_BUTTON);
				}
			}
			else {
				for (i = 0; i < Widget->count; i++) {
					tmp = Widget->obj[i];
					if (tmp->visible == FALSE)continue;
					tmp->mouseUser(Widget->obj[i]);
					tmp->mouseUp(Widget->obj[i], _Mouse->Pos.x, _Mouse->Pos.y, SG_BUTTON_UP | SG_LEFT_BUTTON);
				}
			}
		}
	}
	if (button == SG_RIGHT_BUTTON && state == SG_BUTTON_DOWN) {
		_Mouse->right = SG_BUTTON_DOWN;
		pos[1].x = x;
		pos[1].y = y;

		panel.x = x;
		panel.y = y;
		if (_scrResizeable) {
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].x = x;
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].y = y;
		}
		else {
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].x = x * buf->sizeX / _Window->winWidth;
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].y = y * buf->sizeY / _Window->winHeight;
		}
		_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].z = SG_BUTTON_DOWN | SG_RIGHT_BUTTON;
		_Mouse->front = (_Mouse->front + 1) % 1024;

		if (_sglMode == BIT_MAP) {
			if (Widget->active != -1 && inWidget(Widget->obj[Widget->active], x, y)) {
				Widget->obj[Widget->active]->mouseDown(Widget->obj[Widget->active],
					_Mouse->Pos.x, _Mouse->Pos.y, SG_BUTTON_DOWN | SG_RIGHT_BUTTON);
				for (i = 0; i < Widget->count; i++) {
					tmp = Widget->obj[i];
					if (tmp->visible == FALSE)continue;
					if (inWidget(tmp, x, y))continue;
					tmp->mouseDown(Widget->obj[i], _Mouse->Pos.x, _Mouse->Pos.y, SG_BUTTON_DOWN | SG_RIGHT_BUTTON);
				}
			}
			else {
				for (i = 0; i < Widget->count; i++) {
					tmp = Widget->obj[i];
					if (tmp->visible == FALSE)continue;
					tmp->mouseDown(Widget->obj[i], _Mouse->Pos.x, _Mouse->Pos.y, SG_BUTTON_DOWN | SG_RIGHT_BUTTON);
				}
			}
		}
	}
	if (button == SG_RIGHT_BUTTON && state == SG_BUTTON_UP) {
		_Mouse->right = SG_BUTTON_UP;
		if (!_Mouse->coord)
			if (pos[1].x != x || pos[1].y != y)return;
		if (_scrResizeable) {
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].x = x;
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].y = y;
		}
		else {
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].x = x * buf->sizeX / _Window->winWidth;
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].y = y * buf->sizeY / _Window->winHeight;
		}
		_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].z = SG_BUTTON_UP | SG_RIGHT_BUTTON;
		_Mouse->front = (_Mouse->front + 1) % 1024;

		if (_sglMode == BIT_MAP) {
			if (Widget->active != -1 && inWidget(Widget->obj[Widget->active], x, y)) {
				Widget->obj[Widget->active]->mouseUser(Widget->obj[Widget->active]);
				Widget->obj[Widget->active]->mouseUp(Widget->obj[Widget->active],
					_Mouse->Pos.x, _Mouse->Pos.y, SG_BUTTON_DOWN | SG_RIGHT_BUTTON);
				for (i = 0; i < Widget->count; i++) {
					tmp = Widget->obj[i];
					if (tmp->visible == FALSE)continue;
					if (inWidget(tmp, x, y))continue;
					tmp->mouseUser(Widget->obj[i]);
					tmp->mouseUp(Widget->obj[i], _Mouse->Pos.x, _Mouse->Pos.y, SG_BUTTON_UP | SG_RIGHT_BUTTON);
				}
			}
			else {
				for (i = 0; i < Widget->count; i++) {
					tmp = Widget->obj[i];
					if (tmp->visible == FALSE)continue;
					tmp->mouseUser(Widget->obj[i]);
					tmp->mouseUp(Widget->obj[i], _Mouse->Pos.x, _Mouse->Pos.y, SG_BUTTON_UP | SG_RIGHT_BUTTON);
				}
			}
		}
	}
	if (button == SG_MIDDLE_BUTTON && state == SG_BUTTON_DOWN) {
		_Mouse->middle = SG_BUTTON_DOWN;
		pos[2].x = x;
		pos[2].y = y;
		if (_scrResizeable) {
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].x = x;
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].y = y;
		}
		else {
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].x = x * buf->sizeX / _Window->winWidth;
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].y = y * buf->sizeY / _Window->winHeight;
		}
		_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].z = SG_BUTTON_DOWN | SG_MIDDLE_BUTTON;
		_Mouse->front = (_Mouse->front + 1) % 1024;
	}
	if (button == SG_MIDDLE_BUTTON && state == SG_BUTTON_UP) {
		_Mouse->middle = SG_BUTTON_UP;
		if (!_Mouse->coord)
			if (pos[2].x != x || pos[2].y != y)return;
		if (_scrResizeable) {
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].x = x;
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].y = y;
		}
		else {
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].x = x * buf->sizeX / _Window->winWidth;
			_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].y = y * buf->sizeY / _Window->winHeight;
		}
		_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].z = SG_BUTTON_UP | SG_MIDDLE_BUTTON;
		_Mouse->front = (_Mouse->front + 1) % 1024;
	}
}
void sgWheel(int dir) {
	int x = _Mouse->Pos.x, y = _Mouse->Pos.y, i;
	bitMap *buf;
	widgetObj *tmp;

	if (!_enMouse)return;

	if (_activePage == 0) buf = _Screen->buffer1;
	else buf = _Screen->buffer2;

	if (_scrResizeable) {
		_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].x = x;
		_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].y = y;
	}
	else {
		_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].x = x * buf->sizeX / _Window->winWidth;
		_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].y = y * buf->sizeY / _Window->winHeight;
	}
	_Mouse->mouseBuf[(_Mouse->front + 1) % 1024].z = dir > 0 ?
		SG_MIDDLE_BUTTON_UP : SG_MIDDLE_BUTTON_DOWN;
	_Mouse->front = (_Mouse->front + 1) % 1024;

	if (_sglMode == BIT_MAP) {
		if (Widget->active != -1 && inWidget(Widget->obj[Widget->active], x, y)) {
			Widget->obj[Widget->active]->mouseDown(Widget->obj[Widget->active],
				_Mouse->Pos.x, _Mouse->Pos.y, dir > 0 ?
				SG_MIDDLE_BUTTON_UP : SG_MIDDLE_BUTTON_DOWN);
			for (i = 0; i < Widget->count; i++) {
				if (i == Widget->active)continue;

				tmp = Widget->obj[i];
				if (tmp->visible == FALSE)continue;
				if (inWidget(tmp, x, y))continue;
				tmp->mouseDown(Widget->obj[i], _Mouse->Pos.x, _Mouse->Pos.y, dir > 0 ?
					SG_MIDDLE_BUTTON_UP : SG_MIDDLE_BUTTON_DOWN);
			}
		}
		else {
			for (i = 0; i < Widget->count; i++) {
				tmp = Widget->obj[i];
				if (tmp->visible == FALSE)continue;
				tmp->mouseDown(Widget->obj[i], _Mouse->Pos.x, _Mouse->Pos.y, dir > 0 ?
					SG_MIDDLE_BUTTON_UP : SG_MIDDLE_BUTTON_DOWN);
			}
		}
	}
}
void sgSubInit(vect setup) {
	int i;

	_wndList[subNum].key = (struct _key*)malloc(sizeof(struct _key));
	_wndList[subNum].mouse = (struct _mouse*)malloc(sizeof(struct _mouse));
	_wndList[subNum].widget = (struct _widget*)malloc(sizeof(struct _widget));

	_wndList[subNum].widget->count = 0;
	_wndList[subNum].widget->active = -1;
	for (i = 0; i < SG_QSIZE; i++) {
		_wndList[subNum].widget->obj[i] = NULL;
	}
	for (i = 0; i < 256; i++) {
		_wndList[subNum].widget->hash[i] = NULL;
	}

	setup();

	_prepareSubText(GetSystemMetrics(SM_CXFULLSCREEN) / 8 * 8 + 8);

	_wndList[subNum].buffer1 = (bitMap *)malloc(sizeof(bitMap));
	if (_wndList[subNum].scrResizeable) {
		_wndList[subNum].buffer1->sizeX = GetSystemMetrics(SM_CXFULLSCREEN) / 8 * 8 + 16;
		_wndList[subNum].buffer1->sizeY = GetSystemMetrics(SM_CYFULLSCREEN) / 8 * 8 + 16;
	}
	else {
		_wndList[subNum].buffer1->sizeX = _wndList[subNum].winWidth;
		_wndList[subNum].buffer1->sizeY = _wndList[subNum].winHeight;
	}
	_wndList[subNum].buffer1->data = (unsigned char *)malloc(
		_wndList[subNum].buffer1->sizeX * _wndList[subNum].buffer1->sizeY * 3 * sizeof(unsigned char));
	memset(_wndList[subNum].buffer1->data, -1,
		_wndList[subNum].buffer1->sizeX * _wndList[subNum].buffer1->sizeY * 3 * sizeof(unsigned char));

	_wndList[subNum].buffer2 = (bitMap *)malloc(sizeof(bitMap));
	if (_wndList[subNum].scrResizeable) {
		_wndList[subNum].buffer2->sizeX = GetSystemMetrics(SM_CXFULLSCREEN) / 8 * 8 + 16;
		_wndList[subNum].buffer2->sizeY = GetSystemMetrics(SM_CYFULLSCREEN) / 8 * 8 + 16;
	}
	else {
		_wndList[subNum].buffer2->sizeX = _wndList[subNum].winWidth;
		_wndList[subNum].buffer2->sizeY = _wndList[subNum].winHeight;
	}
	_wndList[subNum].buffer2->data = (unsigned char *)malloc(
		_wndList[subNum].buffer2->sizeX * _wndList[subNum].buffer2->sizeY * 3 * sizeof(char));
	memset(_wndList[subNum].buffer2->data, -1,
		_wndList[subNum].buffer2->sizeX * _wndList[subNum].buffer2->sizeY * 3 * sizeof(char));

	_wndList[subNum].rgb[0] = 0;
	_wndList[subNum].rgb[1] = 0;
	_wndList[subNum].rgb[2] = 0;
	_wndList[subNum].alpha = 1.f;
	_wndList[subNum].tipShowing = -1;
	_wndList[subNum].passWidget = -1;
}
void sgSubKey(int id, int cAscii, int x, int y) {
	widgetObj *tmp;
	int ctrl, shift;
	int i;

	ctrl = cAscii & SG_CTRLBIT;
	shift = cAscii & SG_SHIFTBIT;
	if (ctrl)cAscii += 'a' - 1;
	cAscii &= 0xff;
	if (cAscii >= 0x80 || cAscii < 0x20)return;

	_wndList[id].key->front = (_wndList[id].key->front + 1) % 1024;
	_wndList[id].key->keyBuf[_wndList[id].key->front] = (word)(cAscii | (ctrl | shift));
	_wndList[id].key->keyBuf[_wndList[id].key->front] &= (word)0x7fff;

	for (i = 0; i < _wndList[id].widget->count; i++) {
		tmp = _wndList[id].widget->obj[i];
		if (tmp->status&WIDGET_SELECTED) {
			tmp->keyDown(tmp, cAscii);
			tmp->keyUser(tmp, cAscii);
		}
	}
}
void sgSubSpecial(int id, int cAscii, int x, int y) {
	widgetObj *tmp;
	int i;

	switch (cAscii) {
	case VK_UP:
		cAscii = SG_UP;
		break;
	case VK_DOWN:
		cAscii = SG_DOWN;
		break;
	case VK_LEFT:
		cAscii = SG_LEFT;
		break;
	case VK_RIGHT:
		cAscii = SG_RIGHT;
		break;
	case 0x11:
		cAscii = SG_CTRL;
		break;
	case 0x10:
		cAscii = SG_SHIFT;
		break;
	case SG_ESC:
	case SG_BACKS:
	case SG_TAB:
	case SG_ENTER:
		break;
	case VK_INSERT:
		cAscii = SG_INSERT;
		break;
	case VK_DELETE:
		cAscii = SG_DELETE;
		break;
	case VK_HOME:
		cAscii = SG_HOME;
		break;
	case VK_END:
		cAscii = SG_END;
		break;
	case VK_NUMLOCK:
		cAscii = SG_NUMLOC;
		break;
	case VK_CAPITAL:
		cAscii = SG_CAPSLOC;
		break;
	case VK_PRIOR:
		cAscii = SG_PAGEUP;
		break;
	case VK_NEXT:
		cAscii = SG_PAGEDOWN;
		break;
	case VK_F1:
		cAscii = SG_F1;
		break;
	case VK_F2:
		cAscii = SG_F2;
		break;
	case VK_F3:
		cAscii = SG_F3;
		break;
	case VK_F4:
		cAscii = SG_F4;
		break;
	case VK_F5:
		cAscii = SG_F5;
		break;
	case VK_F6:
		cAscii = SG_F6;
		break;
	case VK_F7:
		cAscii = SG_F7;
		break;
	case VK_F8:
		cAscii = SG_F8;
		break;
	case VK_F9:
		cAscii = SG_F9;
		break;
	case VK_F10:
		cAscii = SG_F10;
		break;
	case VK_F11:
		cAscii = SG_F11;
		break;
	case VK_F12:
		cAscii = SG_F12;
		break;
	default:
		return;
	}

	_wndList[id].key->front = (_wndList[id].key->front + 1) % 1024;
	_wndList[id].key->keyBuf[_wndList[id].key->front] = (word)cAscii;
	_wndList[id].key->keyBuf[_wndList[id].key->front] &= (word)0x7fff;

	for (i = 0; i < _wndList[id].widget->count; i++) {
		tmp = _wndList[id].widget->obj[i];
		if (tmp->status&WIDGET_SELECTED) {
			tmp->keyDown(tmp, cAscii);
			tmp->keyUser(tmp, cAscii);
		}
	}
}
void sgSubKeyUp(int id, int cAscii, int x, int y) {
	widgetObj *tmp;
	int i;

	if (cAscii >= 0x100 || cAscii < 0x20)return;
	if (cAscii >= 0x21 && cAscii <= 0x2F)return;
	if (cAscii >= 0x70 && cAscii <= 0x7B)return;

	_wndList[id].key->front = (_wndList[id].key->front + 1) % 1024;
	_wndList[id].key->keyBuf[_wndList[id].key->front] = (word)cAscii;
	_wndList[id].key->keyBuf[_wndList[id].key->front] |= (word)0x8000;

	for (i = 0; i < _wndList[id].widget->count; i++) {
		tmp = _wndList[id].widget->obj[i];
		if (tmp->status&WIDGET_SELECTED)
			tmp->keyUp(tmp, cAscii | 0x8000);
	}
}
void sgSubSpecialUp(int id, int cAscii, int x, int y) {
	widgetObj *tmp;
	int i;

	switch (cAscii) {
	case VK_UP:
		cAscii = SG_UP;
		break;
	case VK_DOWN:
		cAscii = SG_DOWN;
		break;
	case VK_LEFT:
		cAscii = SG_LEFT;
		break;
	case VK_RIGHT:
		cAscii = SG_RIGHT;
		break;
	case 0x11:
		cAscii = SG_CTRL;
		break;
	case 0x10:
		cAscii = SG_SHIFT;
		break;
	case SG_ESC:
	case SG_BACKS:
	case SG_TAB:
	case SG_ENTER:
		break;
	case VK_INSERT:
		cAscii = SG_INSERT;
		break;
	case VK_DELETE:
		cAscii = SG_DELETE;
		break;
	case VK_HOME:
		cAscii = SG_HOME;
		break;
	case VK_END:
		cAscii = SG_END;
		break;
	case VK_NUMLOCK:
		cAscii = SG_NUMLOC;
		break;
	case VK_CAPITAL:
		cAscii = SG_CAPSLOC;
		break;
	case VK_PRIOR:
		cAscii = SG_PAGEUP;
		break;
	case VK_NEXT:
		cAscii = SG_PAGEDOWN;
		break;
	case VK_F1:
		cAscii = SG_F1;
		break;
	case VK_F2:
		cAscii = SG_F2;
		break;
	case VK_F3:
		cAscii = SG_F3;
		break;
	case VK_F4:
		cAscii = SG_F4;
		break;
	case VK_F5:
		cAscii = SG_F5;
		break;
	case VK_F6:
		cAscii = SG_F6;
		break;
	case VK_F7:
		cAscii = SG_F7;
		break;
	case VK_F8:
		cAscii = SG_F8;
		break;
	case VK_F9:
		cAscii = SG_F9;
		break;
	case VK_F10:
		cAscii = SG_F10;
		break;
	case VK_F11:
		cAscii = SG_F11;
		break;
	case VK_F12:
		cAscii = SG_F12;
		break;
	default:
		return;
	}
	_wndList[id].key->front = (_wndList[id].key->front + 1) % 1024;
	_wndList[id].key->keyBuf[_wndList[id].key->front] = (word)cAscii;
	_wndList[id].key->keyBuf[_wndList[id].key->front] |= (word)0x8000;

	for (i = 0; i < _wndList[id].widget->count; i++) {
		tmp = _wndList[id].widget->obj[i];
		if (tmp->status&WIDGET_SELECTED)
			tmp->keyUp(tmp, cAscii | 0x8000);
	}
}
void sgSubMouse(int id, int x, int y) {
	bitMap *buf;
	int i;
	widgetObj *tmp;

	if (_wndList[id].activePage == 0) buf = _wndList[id].buffer1;
	else buf = _wndList[id].buffer2;

	if (_wndList[id].scrResizeable) {
		_wndList[id].mouse->Pos.x = x;
		_wndList[id].mouse->Pos.y = y;
	}
	else {
		_wndList[id].mouse->Pos.x = x * buf->sizeX / _wndList[id].winWidth;
		_wndList[id].mouse->Pos.y = y * buf->sizeY / _wndList[id].winHeight;
	}

	if (_wndList[id].widget->active != -1) {
		if (inWidget(_wndList[id].widget->obj[_wndList[id].widget->active], x, y))
			_wndList[id].widget->obj[_wndList[id].widget->active]->mouseIn(
				_wndList[id].widget->obj[_wndList[id].widget->active],
				_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y);
		else
			_wndList[id].widget->obj[_wndList[id].widget->active]->mouseOut(
				_wndList[id].widget->obj[_wndList[id].widget->active],
				_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y);
		for (i = 0; i < _wndList[id].widget->count; i++) {
			if (i == _wndList[id].widget->active)continue;

			tmp = _wndList[id].widget->obj[i];
			if (tmp->visible == FALSE)continue;
			if (_wndList[id].mouse->Pos.x >= tmp->pos.x&&_wndList[id].mouse->Pos.y >= tmp->pos.y&&
				_wndList[id].mouse->Pos.x <= tmp->pos.x + tmp->size.x&&
				_wndList[id].mouse->Pos.y <= tmp->pos.y + tmp->size.y) {
				tmp->mouseIn(_wndList[id].widget->obj[i],
					_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y);
			}
			else
				tmp->mouseOut(_wndList[id].widget->obj[i],
					_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y);
		}
	}
	else {
		for (i = 0; i < _wndList[id].widget->count; i++) {
			tmp = _wndList[id].widget->obj[i];
			if (tmp->visible == FALSE)continue;
			if (_wndList[id].mouse->Pos.x >= tmp->pos.x&&_wndList[id].mouse->Pos.y >= tmp->pos.y&&
				_wndList[id].mouse->Pos.x <= tmp->pos.x + tmp->size.x&&
				_wndList[id].mouse->Pos.y <= tmp->pos.y + tmp->size.y) {
				tmp->mouseIn(_wndList[id].widget->obj[i],
					_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y);
			}
			else
				tmp->mouseOut(_wndList[id].widget->obj[i],
					_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y);
		}
	}
}
void sgSubDrag(int id, int x, int y) {
	bitMap *buf;
	widgetObj *tmp;
	int i;

	if (_wndList[id].activePage == 0) buf = _wndList[id].buffer1;
	else buf = _wndList[id].buffer2;

	if (_wndList[id].scrResizeable) {
		_wndList[id].mouse->Pos.x = x;
		_wndList[id].mouse->Pos.y = y;
	}
	else {
		_wndList[id].mouse->Pos.x = x * buf->sizeX / _wndList[id].winWidth;
		_wndList[id].mouse->Pos.y = y * buf->sizeY / _wndList[id].winHeight;
	}

	if (_wndList[id].widget->active != -1) {
		if (inWidget(_wndList[id].widget->obj[_wndList[id].widget->active], x, y))
			_wndList[id].widget->obj[_wndList[id].widget->active]->mouseIn(
				_wndList[id].widget->obj[_wndList[id].widget->active],
				_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y);
		else
			_wndList[id].widget->obj[_wndList[id].widget->active]->mouseOut(
				_wndList[id].widget->obj[_wndList[id].widget->active],
				_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y);
		for (i = 0; i < _wndList[id].widget->count; i++) {
			if (i == _wndList[id].widget->active)continue;

			tmp = _wndList[id].widget->obj[i];
			if (tmp->visible == FALSE)continue;
			if (inWidget(tmp, x, y))continue;
			if (_wndList[id].mouse->Pos.x >= tmp->pos.x&&_wndList[id].mouse->Pos.y >= tmp->pos.y&&
				_wndList[id].mouse->Pos.x <= tmp->pos.x + tmp->size.x&&
				_wndList[id].mouse->Pos.y <= tmp->pos.y + tmp->size.y) {
				if (!(tmp->status&WIDGET_PASS))
					tmp->mouseIn(_wndList[id].widget->obj[i],
						_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y);
			}
			else
				if (tmp->status&WIDGET_PASS)
					tmp->mouseOut(_wndList[id].widget->obj[i], _wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y);
		}
	}
	else {
		for (i = 0; i < _wndList[id].widget->count; i++) {
			tmp = _wndList[id].widget->obj[i];
			if (tmp->visible == FALSE)continue;
			if (_wndList[id].mouse->Pos.x >= tmp->pos.x&&_wndList[id].mouse->Pos.y >= tmp->pos.y&&
				_wndList[id].mouse->Pos.x <= tmp->pos.x + tmp->size.x&&
				_wndList[id].mouse->Pos.y <= tmp->pos.y + tmp->size.y) {
				if (!(tmp->status&WIDGET_PASS))
					tmp->mouseIn(_wndList[id].widget->obj[i],
						_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y);
			}
			else
				if (tmp->status&WIDGET_PASS)
					tmp->mouseOut(_wndList[id].widget->obj[i],
						_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y);
		}
	}
}
void sgSubClick(int id, int button, int state, int x, int y) {
	static vec2 pos[3];
	bitMap *buf;
	widgetObj *tmp;
	int i;

	if (_wndList[id].activePage == 0) buf = _wndList[id].buffer1;
	else buf = _wndList[id].buffer2;

	if (button == SG_LEFT_BUTTON && state == SG_BUTTON_DOWN) {
		_wndList[id].mouse->left = SG_BUTTON_DOWN;
		pos[0].x = x;
		pos[0].y = y;
		if (_wndList[id].scrResizeable) {
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].x = x;
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].y = y;
		}
		else {
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].x =
				x * buf->sizeX / _wndList[id].winWidth;
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].y =
				y * buf->sizeY / _wndList[id].winHeight;
		}
		_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].z =
			SG_BUTTON_DOWN | SG_LEFT_BUTTON;
		_wndList[id].mouse->front = (_wndList[id].mouse->front + 1) % 1024;

		if (_wndList[id].widget->active != -1 &&
			inWidget(_wndList[id].widget->obj[_wndList[id].widget->active], x, y)) {
			_wndList[id].widget->obj[_wndList[id].widget->active]->mouseDown(
				_wndList[id].widget->obj[_wndList[id].widget->active],
				_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y,
				SG_BUTTON_DOWN | SG_LEFT_BUTTON);
			for (i = 0; i < _wndList[id].widget->count; i++) {
				if (i == _wndList[id].widget->active)continue;

				tmp = _wndList[id].widget->obj[i];
				if (tmp->visible == FALSE)continue;
				if (inWidget(tmp, x, y))continue;
				tmp->mouseDown(_wndList[id].widget->obj[i],
					_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y,
					SG_BUTTON_DOWN | SG_LEFT_BUTTON);
			}
		}
		else {
			for (i = 0; i < _wndList[id].widget->count; i++) {
				tmp = _wndList[id].widget->obj[i];
				if (tmp->visible == FALSE)continue;
				tmp->mouseDown(_wndList[id].widget->obj[i],
					_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y,
					SG_BUTTON_DOWN | SG_LEFT_BUTTON);
			}
		}
	}
	if (button == SG_LEFT_BUTTON && state == SG_BUTTON_UP) {
		_wndList[id].mouse->left = SG_BUTTON_UP;
		if (!_wndList[id].mouse->coord)
			if (pos[0].x != x || pos[0].y != y)return;
		if (_wndList[id].scrResizeable) {
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].x = x;
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].y = y;
		}
		else {
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].x =
				x * buf->sizeX / _wndList[id].winWidth;
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].y =
				y * buf->sizeY / _wndList[id].winHeight;
		}
		_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].z =
			SG_BUTTON_UP | SG_LEFT_BUTTON;
		_wndList[id].mouse->front = (_wndList[id].mouse->front + 1) % 1024;

		if (_wndList[id].widget->active != -1 &&
			inWidget(_wndList[id].widget->obj[_wndList[id].widget->active], x, y)) {
			_wndList[id].widget->obj[_wndList[id].widget->active]->mouseDown(
				_wndList[id].widget->obj[_wndList[id].widget->active],
				_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y,
				SG_BUTTON_UP | SG_LEFT_BUTTON);
			for (i = 0; i < _wndList[id].widget->count; i++) {
				if (i == _wndList[id].widget->active)continue;

				tmp = _wndList[id].widget->obj[i];
				if (tmp->visible == FALSE)continue;
				if (inWidget(tmp, x, y))continue;
				tmp->mouseDown(_wndList[id].widget->obj[i],
					_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y,
					SG_BUTTON_UP | SG_LEFT_BUTTON);
			}
		}
		else {
			for (i = 0; i < _wndList[id].widget->count; i++) {
				tmp = _wndList[id].widget->obj[i];
				if (tmp->visible == FALSE)continue;
				tmp->mouseDown(_wndList[id].widget->obj[i],
					_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y,
					SG_BUTTON_UP | SG_LEFT_BUTTON);
			}
		}
	}
	if (button == SG_RIGHT_BUTTON && state == SG_BUTTON_DOWN) {
		_wndList[id].mouse->right = SG_BUTTON_DOWN;
		pos[1].x = x;
		pos[1].y = y;
		if (_wndList[id].scrResizeable) {
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].x = x;
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].y = y;
		}
		else {
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].x =
				x * buf->sizeX / _wndList[id].winWidth;
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].y =
				y * buf->sizeY / _wndList[id].winHeight;
		}
		_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].z =
			SG_BUTTON_DOWN | SG_RIGHT_BUTTON;
		_wndList[id].mouse->front = (_wndList[id].mouse->front + 1) % 1024;

		if (_wndList[id].widget->active != -1 &&
			inWidget(_wndList[id].widget->obj[_wndList[id].widget->active], x, y)) {
			_wndList[id].widget->obj[_wndList[id].widget->active]->mouseDown(
				_wndList[id].widget->obj[_wndList[id].widget->active],
				_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y,
				SG_BUTTON_DOWN | SG_RIGHT_BUTTON);
			for (i = 0; i < _wndList[id].widget->count; i++) {
				if (i == _wndList[id].widget->active)continue;

				tmp = _wndList[id].widget->obj[i];
				if (tmp->visible == FALSE)continue;
				if (inWidget(tmp, x, y))continue;
				tmp->mouseDown(_wndList[id].widget->obj[i],
					_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y,
					SG_BUTTON_DOWN | SG_RIGHT_BUTTON);
			}
		}
		else {
			for (i = 0; i < _wndList[id].widget->count; i++) {
				tmp = _wndList[id].widget->obj[i];
				if (tmp->visible == FALSE)continue;
				tmp->mouseDown(_wndList[id].widget->obj[i],
					_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y,
					SG_BUTTON_DOWN | SG_RIGHT_BUTTON);
			}
		}
	}
	if (button == SG_RIGHT_BUTTON && state == SG_BUTTON_UP) {
		_wndList[id].mouse->right = SG_BUTTON_UP;
		if (!_wndList[id].mouse->coord)
			if (pos[1].x != x || pos[1].y != y)return;
		if (_wndList[id].scrResizeable) {
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].x = x;
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].y = y;
		}
		else {
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].x =
				x * buf->sizeX / _wndList[id].winWidth;
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].y =
				y * buf->sizeY / _wndList[id].winHeight;
		}
		_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].z =
			SG_BUTTON_UP | SG_RIGHT_BUTTON;
		_wndList[id].mouse->front = (_wndList[id].mouse->front + 1) % 1024;

		if (_wndList[id].widget->active != -1 &&
			inWidget(_wndList[id].widget->obj[_wndList[id].widget->active], x, y)) {
			_wndList[id].widget->obj[_wndList[id].widget->active]->mouseDown(
				_wndList[id].widget->obj[_wndList[id].widget->active],
				_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y,
				SG_BUTTON_UP | SG_RIGHT_BUTTON);
			for (i = 0; i < _wndList[id].widget->count; i++) {
				if (i == _wndList[id].widget->active)continue;

				tmp = _wndList[id].widget->obj[i];
				if (tmp->visible == FALSE)continue;
				if (inWidget(tmp, x, y))continue;
				tmp->mouseDown(_wndList[id].widget->obj[i],
					_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y,
					SG_BUTTON_UP | SG_RIGHT_BUTTON);
			}
		}
		else {
			for (i = 0; i < _wndList[id].widget->count; i++) {
				tmp = _wndList[id].widget->obj[i];
				if (tmp->visible == FALSE)continue;
				tmp->mouseDown(_wndList[id].widget->obj[i],
					_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y,
					SG_BUTTON_UP | SG_RIGHT_BUTTON);
			}
		}
	}
	if (button == SG_MIDDLE_BUTTON && state == SG_BUTTON_DOWN) {
		_wndList[id].mouse->middle = SG_BUTTON_DOWN;
		pos[2].x = x;
		pos[2].y = y;
		if (_wndList[id].scrResizeable) {
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].x = x;
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].y = y;
		}
		else {
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].x =
				x * buf->sizeX / _wndList[id].winWidth;
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].y =
				y * buf->sizeY / _wndList[id].winHeight;
		}
		_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].z =
			SG_BUTTON_DOWN | SG_MIDDLE_BUTTON;
		_wndList[id].mouse->front = (_wndList[id].mouse->front + 1) % 1024;
	}
	if (button == SG_MIDDLE_BUTTON && state == SG_BUTTON_UP) {
		_wndList[id].mouse->middle = SG_BUTTON_UP;
		if (!_wndList[id].mouse->coord)
			if (pos[2].x != x || pos[2].y != y)return;
		if (_wndList[id].scrResizeable) {
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].x = x;
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].y = y;
		}
		else {
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].x =
				x * buf->sizeX / _wndList[id].winWidth;
			_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].y =
				y * buf->sizeY / _wndList[id].winHeight;
		}
		_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].z =
			SG_BUTTON_DOWN | SG_BUTTON_UP;
		_wndList[id].mouse->front = (_wndList[id].mouse->front + 1) % 1024;
	}
}
void sgSubWheel(int id, int dir) {
	int x = _wndList[id].mouse->Pos.x, y = _wndList[id].mouse->Pos.y, i;
	bitMap *buf;
	widgetObj *tmp;

	if (_wndList[id].activePage == 0) buf = _wndList[id].buffer1;
	else buf = _wndList[id].buffer2;

	if (_wndList[id].scrResizeable) {
		_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].x = x;
		_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].y = y;
	}
	else {
		_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].x =
			x * buf->sizeX / _wndList[id].winWidth;
		_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].y =
			y * buf->sizeY / _wndList[id].winHeight;
	}
	_wndList[id].mouse->mouseBuf[(_wndList[id].mouse->front + 1) % 1024].z = dir > 0 ?
		SG_MIDDLE_BUTTON_UP : SG_MIDDLE_BUTTON_DOWN;
	_wndList[id].mouse->front = (_wndList[id].mouse->front + 1) % 1024;

	if (_wndList[id].widget->active != -1 &&
		inWidget(_wndList[id].widget->obj[_wndList[id].widget->active], x, y)) {
		_wndList[id].widget->obj[_wndList[id].widget->active]->mouseDown(
			_wndList[id].widget->obj[_wndList[id].widget->active],
			_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y, dir > 0 ?
			SG_MIDDLE_BUTTON_UP : SG_MIDDLE_BUTTON_DOWN);
		for (i = 0; i < _wndList[id].widget->count; i++) {
			if (i == _wndList[id].widget->active)continue;

			tmp = _wndList[id].widget->obj[i];
			if (tmp->visible == FALSE)continue;
			if (inWidget(tmp, x, y))continue;
			tmp->mouseDown(_wndList[id].widget->obj[i],
				_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y, dir > 0 ?
				SG_MIDDLE_BUTTON_UP : SG_MIDDLE_BUTTON_DOWN);
		}
	}
	else {
		for (i = 0; i < _wndList[id].widget->count; i++) {
			tmp = _wndList[id].widget->obj[i];
			if (tmp->visible == FALSE)continue;
			tmp->mouseDown(_wndList[id].widget->obj[i],
				_wndList[id].mouse->Pos.x, _wndList[id].mouse->Pos.y, dir > 0 ?
				SG_MIDDLE_BUTTON_UP : SG_MIDDLE_BUTTON_DOWN);
		}
	}
}

/*
* Find the index of the character position.
* Start in v2.2.0
*/
int _wcharAt(const char *src, int pos) {
	int ret = 0;
	while (pos-- > 0) {
		if (src[ret] == 0)return ret;
		if (src[ret] < 0)ret++;
		ret++;
	}
	return ret;
}
int _scharAt(const char *src, int pos) {
	int ret = 0, i = 0;
	while (i < pos) {
		if (src[i] == 0)return ret;
		if (src[i] < 0)i++;
		ret++;
		i++;
	}
	return ret;
}

/*
* Inner functions.
* Text mode start in v1.0.0
* String drawing start in v2.2.0
*/
void _makeBitmap(HDC hdc, LPBYTE lpBits, long width, long height, WORD wBitCount) {
	BITMAPINFO bitinfo;
	char *buffer;
	int i;

	memset(&bitinfo, 0, sizeof(BITMAPINFO));

	bitinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitinfo.bmiHeader.biWidth = width;
	bitinfo.bmiHeader.biHeight = height;
	bitinfo.bmiHeader.biPlanes = 1;
	bitinfo.bmiHeader.biBitCount = wBitCount;
	bitinfo.bmiHeader.biCompression = BI_RGB;
	bitinfo.bmiHeader.biSizeImage = width * height*(wBitCount / 8);
	bitinfo.bmiHeader.biXPelsPerMeter = 0;
	bitinfo.bmiHeader.biYPelsPerMeter = 0;
	bitinfo.bmiHeader.biClrUsed = 0;
	bitinfo.bmiHeader.biClrImportant = 0;

	buffer = (char *)malloc(bitinfo.bmiHeader.biSizeImage);
	for (i = 0; i < height; i++) {
		memcpy(buffer + i * width*(wBitCount / 8), lpBits + (height - 1 - i)*width*(wBitCount / 8), width*(wBitCount / 8));
	}

	if (_scrResizeable && _sglMode == BIT_MAP) {
		StretchDIBits(hdc, 0, 0, bitinfo.bmiHeader.biWidth,
			bitinfo.bmiHeader.biHeight, 0, 0, bitinfo.bmiHeader.biWidth,
			bitinfo.bmiHeader.biHeight, buffer, (BITMAPINFO*)&bitinfo.bmiHeader,
			DIB_RGB_COLORS, SRCCOPY);
	}
	else {
		StretchDIBits(hdc, 0, 0, _Window->winWidth,
			_Window->winHeight, 0, 0, bitinfo.bmiHeader.biWidth,
			bitinfo.bmiHeader.biHeight, buffer, (BITMAPINFO*)&bitinfo.bmiHeader,
			DIB_RGB_COLORS, SRCCOPY);
	}
	free(buffer);
}
void _makeSubBitmap(HDC hdc, int id, LPBYTE lpBits, long width, long height, WORD wBitCount) {
	BITMAPINFO bitinfo;
	char *buffer;
	int i;

	memset(&bitinfo, 0, sizeof(BITMAPINFO));

	bitinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitinfo.bmiHeader.biWidth = width;
	bitinfo.bmiHeader.biHeight = height;
	bitinfo.bmiHeader.biPlanes = 1;
	bitinfo.bmiHeader.biBitCount = wBitCount;
	bitinfo.bmiHeader.biCompression = BI_RGB;
	bitinfo.bmiHeader.biSizeImage = width * height*(wBitCount / 8);
	bitinfo.bmiHeader.biXPelsPerMeter = 0;
	bitinfo.bmiHeader.biYPelsPerMeter = 0;
	bitinfo.bmiHeader.biClrUsed = 0;
	bitinfo.bmiHeader.biClrImportant = 0;

	buffer = (char *)malloc(bitinfo.bmiHeader.biSizeImage);
	for (i = 0; i < height; i++) {
		memcpy(buffer + i * width*(wBitCount / 8), lpBits + (height - 1 - i)*width*(wBitCount / 8), width*(wBitCount / 8));
	}

	if (_wndList[id].scrResizeable) {
		StretchDIBits(hdc, 0, 0, bitinfo.bmiHeader.biWidth,
			bitinfo.bmiHeader.biHeight, 0, 0, bitinfo.bmiHeader.biWidth,
			bitinfo.bmiHeader.biHeight, buffer, (BITMAPINFO*)&bitinfo.bmiHeader,
			DIB_RGB_COLORS, SRCCOPY);
	}
	else {
		StretchDIBits(hdc, 0, 0, _wndList[id].winWidth,
			_wndList[id].winHeight, 0, 0, bitinfo.bmiHeader.biWidth,
			bitinfo.bmiHeader.biHeight, buffer, (BITMAPINFO*)&bitinfo.bmiHeader,
			DIB_RGB_COLORS, SRCCOPY);
	}
	free(buffer);
}
void _textLoop() {
	int i;

	for (i = 0; i < _Screen->changed->width * _Screen->changed->height; i++) {
		if (_Screen->changed->content[i] != _Screen->content->content[i]) {
			_Screen->content->content[i] = _Screen->changed->content[i];
			_textRenew(_Screen->content->content[i], i % _Screen->changed->width, i / _Screen->changed->width);
		}
		if (_Screen->textMask->content[i] & 0xFF) {
			_textAdd(_Screen->content->content[i], i % _Screen->changed->width, i / _Screen->changed->width);
		}
	}
}
void _textRenew(short c, int x, int y) {
	RGB b, f;

	_innerFunc = 1;
	b = _bgc(c), f = _fgc(c);
	setColor(b.r, b.g, b.b);
	putQuad(x * 8, y * 16, x * 8 + 7, y * 16 + 15, SOLID_FILL);
	setColor(f.r, f.g, f.b);
	putChar((char)c, x * 8, y * 16);
	_innerFunc = 0;
}
void _textAdd(short c, int x, int y) {
	RGB f;

	_innerFunc = 1;
	f = _fgc(c);
	setColor(f.r, f.g, f.b);
	putChar((char)c, x * 8, y * 16);
	_innerFunc = 0;
}
RGB _bgc(short c) {
	RGB ret = { 0, 0, 0 };

	c >>= 12;
	c &= 0x000F;

	switch (c) {
	case BLACK:
		ret.r = 0;
		ret.g = 0;
		ret.b = 0;
		break;
	case BLUE:
		ret.r = 0;
		ret.g = 0;
		ret.b = 127;
		break;
	case GREEN:
		ret.r = 0;
		ret.g = 127;
		ret.b = 0;
		break;
	case CYAN:
		ret.r = 0;
		ret.g = 127;
		ret.b = 127;
		break;
	case RED:
		ret.r = 127;
		ret.g = 0;
		ret.b = 0;
		break;
	case MAGENTA:
		ret.r = 127;
		ret.g = 0;
		ret.b = 127;
		break;
	case BROWN:
		ret.r = 127;
		ret.g = 127;
		ret.b = 0;
		break;
	case DARKGRAY:
		ret.r = 127;
		ret.g = 127;
		ret.b = 127;
		break;
	case LIGHTGRAY:
		ret.r = 191;
		ret.g = 191;
		ret.b = 191;
		break;
	case LIGHTBLUE:
		ret.r = 0;
		ret.g = 0;
		ret.b = 255;
		break;
	case LIGHTGREEN:
		ret.r = 0;
		ret.g = 255;
		ret.b = 0;
		break;
	case LIGHTCYAN:
		ret.r = 0;
		ret.g = 255;
		ret.b = 255;
		break;
	case LIGHTRED:
		ret.r = 255;
		ret.g = 0;
		ret.b = 0;
		break;
	case LIGHTMAGENTA:
		ret.r = 255;
		ret.g = 0;
		ret.b = 255;
		break;
	case YELLOW:
		ret.r = 255;
		ret.g = 255;
		ret.b = 0;
		break;
	case WHITE:
		ret.r = 255;
		ret.g = 255;
		ret.b = 255;
		break;
	}
	return ret;
}
RGB _fgc(short c) {
	RGB ret = { 0, 0, 0 };

	c >>= 8;
	c &= 0x000F;

	switch (c) {
	case BLACK:
		ret.r = 0;
		ret.g = 0;
		ret.b = 0;
		break;
	case BLUE:
		ret.r = 0;
		ret.g = 0;
		ret.b = 127;
		break;
	case GREEN:
		ret.r = 0;
		ret.g = 127;
		ret.b = 0;
		break;
	case CYAN:
		ret.r = 0;
		ret.g = 127;
		ret.b = 127;
		break;
	case RED:
		ret.r = 127;
		ret.g = 0;
		ret.b = 0;
		break;
	case MAGENTA:
		ret.r = 127;
		ret.g = 0;
		ret.b = 127;
		break;
	case BROWN:
		ret.r = 127;
		ret.g = 127;
		ret.b = 0;
		break;
	case DARKGRAY:
		ret.r = 127;
		ret.g = 127;
		ret.b = 127;
		break;
	case LIGHTGRAY:
		ret.r = 191;
		ret.g = 191;
		ret.b = 191;
		break;
	case LIGHTBLUE:
		ret.r = 0;
		ret.g = 0;
		ret.b = 255;
		break;
	case LIGHTGREEN:
		ret.r = 0;
		ret.g = 255;
		ret.b = 0;
		break;
	case LIGHTCYAN:
		ret.r = 0;
		ret.g = 255;
		ret.b = 255;
		break;
	case LIGHTRED:
		ret.r = 255;
		ret.g = 0;
		ret.b = 0;
		break;
	case LIGHTMAGENTA:
		ret.r = 255;
		ret.g = 0;
		ret.b = 255;
		break;
	case YELLOW:
		ret.r = 255;
		ret.g = 255;
		ret.b = 0;
		break;
	case WHITE:
		ret.r = 255;
		ret.g = 255;
		ret.b = 255;
		break;
	}
	return ret;
}
LPWSTR _widenStr(const char *src) {
	int rt;
	LPWSTR rs;

	if (src == NULL)return NULL;

	rt = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
	rs = (LPWSTR)malloc(rt * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, src, -1, rs, rt * sizeof(wchar_t));
	return rs;
}
char *_shortenStr(const LPWSTR src) {
	int rt;
	char* rs;

	rt = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0, NULL, NULL);
	rs = (char*)malloc((rt + 1));
	WideCharToMultiByte(CP_ACP, 0, src, -1, rs, rt, NULL, NULL);
	return rs;
}
int _strlenW(const char *str) {
	return MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0) - 1;
}
void _putImageRev(int left, int top, bitMap *bitmap) {
	int x1, x2, y1, y2, i, j, rate;
	bitMap *buf;

	if (_activePage == 0) buf = _Screen->buffer1;
	else buf = _Screen->buffer2;

	if (left >= buf->sizeX || top >= buf->sizeY)return;
	if (left + bitmap->sizeX <= 0 || top + bitmap->sizeY <= 0)return;

	if (left < 0)x1 = 0;
	else x1 = left;
	if (top < 0)y1 = 0;
	else y1 = top;

	if (left + text.strRect.cx >= buf->sizeX)x2 = buf->sizeX - 1;
	else x2 = left + text.strRect.cx;
	y2 = top + bitmap->sizeY - 1;

	int tmp;
	for (i = 0; i < y2 - y1; i++) {
		if (top + i >= buf->sizeY)break;
		for (j = 0; j < x2 - x1; j++) {
			if (rate = bitmap->data[(bitmap->sizeX*(y2 - y1 - i) + j) * 3]) {
				tmp = buf->data[(buf->sizeX * (i + top) + left + j) * 3] * (255 - rate);
				tmp += tf.color.b*rate;
				buf->data[(buf->sizeX * (i + top) + left + j) * 3] = tmp >> 8;
			}
			if (rate = bitmap->data[(bitmap->sizeX*(y2 - y1 - i) + j) * 3 + 1]) {
				tmp = buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 1] * (255 - rate);
				tmp += tf.color.g*rate;
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 1] = tmp >> 8;
			}
			if (rate = bitmap->data[(bitmap->sizeX*(y2 - y1 - i) + j) * 3 + 2]) {
				tmp = buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 2] * (255 - rate);
				tmp += tf.color.r*rate;
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 2] = tmp >> 8;
			}
		}
	}
}
void _putSubImageRev(int id, int left, int top, bitMap *bitmap) {
	int x1, x2, y1, y2, i, j, rate;
	bitMap *buf;

	if (_wndList[id].activePage == 0) buf = _wndList[id].buffer1;
	else buf = _wndList[id].buffer2;

	if (left >= buf->sizeX || top >= buf->sizeY)return;
	if (left + bitmap->sizeX <= 0 || top + bitmap->sizeY <= 0)return;

	if (left < 0)x1 = 0;
	else x1 = left;
	if (top < 0)y1 = 0;
	else y1 = top;

	if (left + _wndList[id].text.strRect.cx >= buf->sizeX)x2 = buf->sizeX - 1;
	else x2 = left + _wndList[id].text.strRect.cx;
	y2 = top + bitmap->sizeY - 1;

	int tmp;
	for (i = 0; i < y2 - y1; i++) {
		if (top + i >= buf->sizeY)break;
		for (j = 0; j < x2 - x1; j++) {
			if (rate = bitmap->data[(bitmap->sizeX*(y2 - y1 - i) + j) * 3]) {
				tmp = buf->data[(buf->sizeX * (i + top) + left + j) * 3] * (255 - rate);
				tmp += _wndList[id].tf.color.b*rate;
				buf->data[(buf->sizeX * (i + top) + left + j) * 3] = tmp >> 8;
			}
			if (rate = bitmap->data[(bitmap->sizeX*(y2 - y1 - i) + j) * 3 + 1]) {
				tmp = buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 1] * (255 - rate);
				tmp += _wndList[id].tf.color.g*rate;
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 1] = tmp >> 8;
			}
			if (rate = bitmap->data[(bitmap->sizeX*(y2 - y1 - i) + j) * 3 + 2]) {
				tmp = buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 2] * (255 - rate);
				tmp += _wndList[id].tf.color.r*rate;
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 2] = tmp >> 8;
			}
		}
	}
}
void _hbmImage(HDC hdc, HBITMAP hbm, int x, int y, const char *str) {
	GetDIBits(hdc, hbm, 0, text.bm.bmHeight, text.bitBuf, (BITMAPINFO*)(&text.bi), DIB_RGB_COLORS);

	bitMap tmp;
	tmp.sizeX = text.bi.biWidth;
	tmp.sizeY = tf.size;
	tmp.data = text.bitBuf + text.bi.biWidth*(text.bi.biHeight - tmp.sizeY) * 3;
	_putImageRev(x, y, &tmp);
}
void _hbmSubImage(int id, HDC hdc, HBITMAP hbm, int x, int y, const char *str) {
	GetDIBits(hdc, hbm, 0, _wndList[id].text.bm.bmHeight, _wndList[id].text.bitBuf,
		(BITMAPINFO*)(&_wndList[id].text.bi), DIB_RGB_COLORS);

	bitMap tmp;
	tmp.sizeX = _wndList[id].text.bi.biWidth;
	tmp.sizeY = _wndList[id].tf.size;
	tmp.data = _wndList[id].text.bitBuf +
		_wndList[id].text.bi.biWidth*(_wndList[id].text.bi.biHeight - tmp.sizeY) * 3;
	_putSubImageRev(id, x, y, &tmp);
}
void _prepareText(int width) {
	text.memDC = CreateCompatibleDC(NULL);
	text.hbm = CreateBitmap(width, SG_MAX_FONT_SIZE, 1, 32, NULL);

	GetObject(text.hbm, sizeof(text.bm), &text.bm);
	int bmRowCount = ((text.bm.bmWidth * 24 + 31) / 32) * 4;
	text.bufSize = bmRowCount * text.bm.bmHeight;
	text.bitBuf = (char *)malloc(text.bufSize);
	memset(text.bitBuf, 0, text.bufSize);

	text.bi.biSize = sizeof(BITMAPINFOHEADER);
	text.bi.biWidth = text.bm.bmWidth;
	text.bi.biHeight = text.bm.bmHeight;
	text.bi.biPlanes = 1;
	text.bi.biBitCount = 24;
	text.bi.biCompression = BI_RGB;
	text.bi.biSizeImage = 0;
	text.bi.biXPelsPerMeter = 0;
	text.bi.biYPelsPerMeter = 0;
	text.bi.biClrImportant = 0;
	text.bi.biClrUsed = 0;

	SelectObject(text.memDC, text.hbm);

	tf.color.r = tf.color.g = tf.color.b = 0;
	tf.size = 20;
	tf.name = _widen("微软雅黑");
	tf.coeff = 0;

	SetTextColor(text.memDC, RGB(255, 255, 255));
	SetBkMode(text.memDC, TRANSPARENT);

	text.font = CreateFont(tf.size, 0, 0, 0, FW_THIN, tf.coeff & FONT_ITALIC,
		tf.coeff & FONT_UNDERLINE, tf.coeff & FONT_STRIKEOUT,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, FF_MODERN, tf.name);
	SelectObject(text.memDC, text.font);
}
void _prepareSubText(int width) {
	_wndList[subNum].text.memDC = CreateCompatibleDC(NULL);
	_wndList[subNum].text.hbm = CreateBitmap(width, SG_MAX_FONT_SIZE, 1, 32, NULL);

	GetObject(_wndList[subNum].text.hbm,
		sizeof(_wndList[subNum].text.bm), &_wndList[subNum].text.bm);
	int bmRowCount = ((_wndList[subNum].text.bm.bmWidth * 24 + 31) / 32) * 4;
	_wndList[subNum].text.bufSize = bmRowCount * _wndList[subNum].text.bm.bmHeight;
	_wndList[subNum].text.bitBuf = (char *)malloc(_wndList[subNum].text.bufSize);
	memset(_wndList[subNum].text.bitBuf, 0, _wndList[subNum].text.bufSize);

	_wndList[subNum].text.bi.biSize = sizeof(BITMAPINFOHEADER);
	_wndList[subNum].text.bi.biWidth = _wndList[subNum].text.bm.bmWidth;
	_wndList[subNum].text.bi.biHeight = _wndList[subNum].text.bm.bmHeight;
	_wndList[subNum].text.bi.biPlanes = 1;
	_wndList[subNum].text.bi.biBitCount = 24;
	_wndList[subNum].text.bi.biCompression = BI_RGB;
	_wndList[subNum].text.bi.biSizeImage = 0;
	_wndList[subNum].text.bi.biXPelsPerMeter = 0;
	_wndList[subNum].text.bi.biYPelsPerMeter = 0;
	_wndList[subNum].text.bi.biClrImportant = 0;
	_wndList[subNum].text.bi.biClrUsed = 0;

	SelectObject(_wndList[subNum].text.memDC, _wndList[subNum].text.hbm);

	_wndList[subNum].tf.color.r = _wndList[subNum].tf.color.g = _wndList[subNum].tf.color.b = 0;
	_wndList[subNum].tf.size = 20;
	_wndList[subNum].tf.name = _widen("??????");
	_wndList[subNum].tf.coeff = 0;

	SetTextColor(_wndList[subNum].text.memDC, RGB(255, 255, 255));
	SetBkMode(_wndList[subNum].text.memDC, TRANSPARENT);

	_wndList[subNum].text.font = CreateFont(_wndList[subNum].tf.size, 0, 0, 0, FW_THIN,
		_wndList[subNum].tf.coeff & FONT_ITALIC,
		_wndList[subNum].tf.coeff & FONT_UNDERLINE,
		_wndList[subNum].tf.coeff & FONT_STRIKEOUT,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, FF_MODERN, _wndList[subNum].tf.name);
	SelectObject(_wndList[subNum].text.memDC, _wndList[subNum].text.font);
}
void _midiout(HMIDIOUT hMidi, int iStatus, int iChannel, int iData1, int iData2) {
	DWORD dwMessage = iStatus | iChannel | (iData1 << 8) | (iData2 << 16);
	midiOutShortMsg(hMidi, dwMessage);
}
int _mciSend(const char *cmd) {
	int ret;
	SGWINSTR _wd = NULL;
	ret = !mciSendString(_wd = _widen(cmd), (void *)mci.buffer, SG_MCI_BUFFER_SIZE, 0);
	free((void *)_wd);
	return ret;
}
int _addList(const char *name, struct _menu *super, int id) {
	int i, j, ret;

	if (super == NULL)return SG_OBJECT_NOT_FOUND;
	if (super->id == id) {
		i = 0;
		while (super->sub[i++]);
		i--;
		super->sub[i] = (struct _item *)malloc(sizeof(struct _item));
		super->sub[i]->name = (char *)malloc(strlen(name) + 1);
		strcpy(super->sub[i]->name, name);
		super->sub[i]->id = _tmpItem;
		super->sub[i]->sub = (struct _menu *)malloc(sizeof(struct _menu));
		super->sub[i]->sub->name = (char *)malloc(strlen(name) + 1);
		strcpy(super->sub[i]->sub->name, name);
		super->sub[i]->sub->id = _tmpItem;
		super->sub[i]->checked = 0;
		super->sub[i]->func = NULL;
		for (j = 0; j < SG_MAX_MENU_ITEM_NUM; j++) {
			super->sub[i]->sub->sub[j] = NULL;
		}
		return _tmpItem++;
	}
	else {
		for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
			if (super->sub[i] == NULL)return SG_OBJECT_NOT_FOUND;
			if ((int)super->sub[i] == -1)continue;
			if ((ret = _addList(name, super->sub[i]->sub, id)) !=
				SG_OBJECT_NOT_FOUND)return ret;
		}
		return SG_OBJECT_NOT_FOUND;
	}
}
int _addItem(const char *name, struct _menu *super, int id, void(*func)()) {
	int i, ret;

	if (super == NULL)return SG_OBJECT_NOT_FOUND;
	if (super->id == id) {
		i = 0;
		while (super->sub[i++]);
		i--;
		super->sub[i] = (struct _item *)malloc(sizeof(struct _item));
		super->sub[i]->name = (char *)malloc(strlen(name) + 1);
		strcpy(super->sub[i]->name, name);
		super->sub[i]->id = _tmpItem;
		super->sub[i]->checked = 0;
		super->sub[i]->func = func;
		super->sub[i]->sub = NULL;
		return _tmpItem++;
	}
	else {
		for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
			if (super->sub[i] == NULL)return SG_OBJECT_NOT_FOUND;
			if ((int)super->sub[i] == -1)continue;
			if ((ret = _addItem(name, super->sub[i]->sub, id, func)) !=
				SG_OBJECT_NOT_FOUND)return ret;
		}
		return SG_OBJECT_NOT_FOUND;
	}
}
int _addSeparator(struct _menu *super, int id) {
	int i, ret;

	if (super == NULL)return SG_OBJECT_NOT_FOUND;
	if (super->id == id) {
		i = 0;
		while (super->sub[i++]);
		i--;
		super->sub[i] = (struct _item *) - 1;
		return SG_NO_ERORR;
	}
	else {
		for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
			if (super->sub[i] == NULL)return SG_OBJECT_NOT_FOUND;
			if (super->sub[i] == (struct _item *) - 1)continue;
			if ((ret = _addSeparator(super->sub[i]->sub, id)) !=
				SG_OBJECT_NOT_FOUND)return ret;
		}
		return SG_OBJECT_NOT_FOUND;
	}
}
void _checkItem(struct _menu *m, int id, int check, int disable) {
	int i, s;
	for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
		if (m->sub[i]) {
			if ((int)m->sub[i] == -1)continue;
			if (m->sub[i]->id != id) {
				if (m->sub[i]->sub != NULL)_checkItem(m->sub[i]->sub, id, check, disable);
			}
			else {
				s = GetMenuState(m->hm, id, MF_BYCOMMAND | MF_CHECKED | MF_GRAYED);
				if (check == 1)s |= MF_CHECKED;
				if (check == 0)s &= ~MF_CHECKED;
				if (disable == 1)s |= MF_GRAYED;
				if (disable == 0)s &= ~MF_GRAYED;
				CheckMenuItem(m->hm, id, s);
				EnableMenuItem(m->hm, id, s);
			}
		}
		else break;
	}

}
void _createMenu(enum _menuType type, struct _item *it, HMENU super) {
	int i;
	HMENU tmp;

	if (it == NULL) {
		if (type == MT_MAIN) {
			for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
				if (mainMenu.sub[i]) {
					if ((int)mainMenu.sub[i] == -1) {
						AppendMenu(super, MF_SEPARATOR, 0, NULL);
					}
					else
						_createMenu(MT_SUB, mainMenu.sub[i], mainMenu.hm);
				}
				else break;
			}
		}
		if (type == MT_TRAY) {
			for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
				if (trayMenu.sub[i]) {
					if ((int)trayMenu.sub[i] == -1) {
						AppendMenu(super, MF_SEPARATOR, 0, NULL);
					}
					else
						_createMenu(MT_SUB, trayMenu.sub[i], trayMenu.hm);
				}
				else break;
			}
		}
		if (type >= MT_POPUP) {
			type -= MT_POPUP;
			for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
				if (popupMenu[type].sub[i]) {
					if ((int)popupMenu[type].sub[i] == -1) {
						AppendMenu(super, MF_SEPARATOR, 0, NULL);
					}
					else
						_createMenu(MT_SUB, popupMenu[type].sub[i], popupMenu[type].hm);
				}
				else break;
			}
		}
		return;
	}

	if (it->sub == NULL) {
		SGWINSTR _wd = NULL;
		AppendMenu(super, MF_STRING, it->id, _wd = _widen(it->name));
		free((void *)_wd);
		return;
	}
	tmp = CreateMenu();
	it->sub->hm = it->hm = tmp;
	for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
		if (it->sub->sub[i]) {
			if ((int)it->sub->sub[i] == -1) {
				AppendMenu(it->hm, MF_SEPARATOR, 0, NULL);
			}
			else
				_createMenu(MT_SUB, it->sub->sub[i], tmp);
		}
		else break;
	}
	SGWINSTR _wd = NULL;
	AppendMenu(super, MF_STRING | MF_POPUP, (UINT)tmp, _wd = _widen(it->name));
	free((void *)_wd);
}
void _callMenu(struct _menu *m, int id) {
	int i;
	for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
		if (m->sub[i]) {
			if ((int)m->sub[i] == -1)continue;
			if (m->sub[i]->id != id) {
				if (m->sub[i]->sub != NULL)_callMenu(m->sub[i]->sub, id);
			}
			else {
				m->sub[i]->func();
			}
		}
		else break;
	}
}
int _stringPrintf(const char *format, va_list ap, int x, int y) {
	int idx = 0;
	char *tmp = (char *)malloc(2 * strlen(format) + 16);
	while (*format) {
		if (*format == '\n') {
			format++;
			tmp[idx] = '\0';
			putString(tmp, x, y);
			y += tf.size;
			idx = 0;
		}
		else if (*format == '\t') {
			tmp[idx++] = ' ';
			tmp[idx++] = ' ';
			tmp[idx++] = ' ';
			tmp[idx++] = ' ';
		}
		else if (*format == '%') {
			format++;
			switch (*format) {
			case 'c': {
				char valch = va_arg(ap, int);
				tmp[idx++] = valch;
				format++;
				break;
			}
			case 'd': {
				int valint = va_arg(ap, int);
				_itoa(valint, tmp + idx, 10);
				while (tmp[idx++]);
				idx--;
				format++;
				break;
			}
			case 'o': {
				int valint = va_arg(ap, int);
				_itoa(valint, tmp + idx, 8);
				while (tmp[idx++]);
				idx--;
				format++;
				break;
			}
			case 'x': {
				int valint = va_arg(ap, int);
				_itoa(valint, tmp + idx, 16);
				while (tmp[idx++]);
				idx--;
				format++;
				break;
			}
			case 's': {
				char *valstr = va_arg(ap, char *);
				tmp[idx] = '\0';
				strcat(tmp, valstr);
				while (tmp[idx++]);
				idx--;
				format++;
				break;
			}
			case '%': {
				tmp[idx++] = *format++;
			}
			}
		}
		else {
			tmp[idx++] = *format++;
		}
	}

	tmp[idx] = '\0';
	putString(tmp, x, y);

	free(tmp);
	return SG_NO_ERORR;
}
void _setThreadFunc(vect func) {
	threadFunc = func;
}
DWORD WINAPI _threadFunc(LPVOID pM) {
	threadFunc();
	return 0;
}
DWORD WINAPI _timerFunc(LPVOID pM) {
	int count = times;
	int millis = interval;
	if (count > 0) {
		while (count--) {
			threadFunc();
			delay(millis);
		}
	}
	else if (count == 0) {
		while (1) {
			threadFunc();
			delay(millis);
		}
	}
	return 0;
}
void _drawPanel() {
	float alpha;
	int x, y, square, i, total;
	char *names[SG_MAX_PANEL_FUNCTION];

	if (_enPanel == 0)return;

	if (mouseStatus(SG_RIGHT_BUTTON) == SG_BUTTON_DOWN) {
		getImage(panel.x - panel.outer - panel.inner, panel.y - panel.outer - panel.inner,
			panel.x + panel.outer + panel.inner, panel.y + panel.outer + panel.inner, panel.cover);

		alpha = getAlpha();
		setAlpha(.5f);
		setColor(255, 255, 255);
		putCircle(panel.x, panel.y, panel.outer, SOLID_FILL);

		x = mousePos().x;
		y = mousePos().y;
		x -= panel.x;
		y -= panel.y;
		square = x * x + y * y;
		if (sqrt(square) > panel.outer) {
			x = (int)(x / (sqrt(square) / panel.outer));
			y = (int)(y / (sqrt(square) / panel.outer));
		}
		putCircle(panel.x + x, panel.y + y, panel.inner, SOLID_FILL);

		if (square > 400) {
			setAlpha(1.f);
			setColor(0, 0, 0);

			total = 0;
			for (i = 0; i < SG_MAX_PANEL_FUNCTION; i++) {
				if (panel.normalPanel[i]) {
					names[total] = panel.normalPanel[i]->name;
					total++;
				}
			}
			switch (total) {
			case 0:
				break;
			case 1:
				putString(names[0],
					panel.x - stringWidth(names[0], strlen(names[0])) / 2,
					panel.y - 10);
				break;
			case 2:
				if (x > 0)
					putString(names[0],
						panel.x - stringWidth(names[0], strlen(names[0])) / 2,
						panel.y - 10);
				else
					putString(names[1],
						panel.x - stringWidth(names[1], strlen(names[1])) / 2,
						panel.y - 10);
				break;
			case 3:
				if (y < 0 && (x == 0 ||
					(x > 0 && (-y) > x*tanf((float)PI / 6)) ||
					(x < 0 && (-y) > x*(-tanf((float)PI / 6)))))
					putString(names[0],
						panel.x - stringWidth(names[0], strlen(names[0])) / 2,
						panel.y - 10);
				else if (x > 0)
					putString(names[1],
						panel.x - stringWidth(names[1], strlen(names[1])) / 2,
						panel.y - 10);
				else
					putString(names[2],
						panel.x - stringWidth(names[2], strlen(names[2])) / 2,
						panel.y - 10);
				break;
			case 4:
				if (x > y)
					if (x < -y)
						putString(names[0],
							panel.x - stringWidth(names[0], strlen(names[0])) / 2,
							panel.y - 10);
					else
						putString(names[1],
							panel.x - stringWidth(names[1], strlen(names[1])) / 2,
							panel.y - 10);
				else
					if (x > -y)
						putString(names[2],
							panel.x - stringWidth(names[2], strlen(names[2])) / 2,
							panel.y - 10);
					else
						putString(names[3],
							panel.x - stringWidth(names[3], strlen(names[3])) / 2,
							panel.y - 10);
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				break;
			case 8:
				break;
			case 9:
				break;
			case 10:
				break;
			case 11:
				break;
			case 12:
				break;
			default:
				break;
			}
		}

		setAlpha(alpha);
	}
}
void _clearPanel() {
	if (_enPanel == 0)return;

	if (panel.cover->data) {
		putImage((panel.x - panel.outer - panel.inner) < 0 ? 0 : (panel.x - panel.outer - panel.inner),
			(panel.y - panel.outer - panel.inner) < 0 ? 0 : (panel.y - panel.outer - panel.inner),
			panel.cover, COPY_PUT);
		free(panel.cover->data);
		panel.cover->data = NULL;
	}

}

/*
* Debug interfaces.
* Started in v3.1.0
*/
SGvoid debugf(const char *format, ...) {
	va_list ap;
	int idx = 0;
	char *tmp;
	SGWINSTR _wd;

	va_start(ap, format);

	tmp = (char *)malloc(2 * strlen(format) + 16);
	while (*format) {
		if (*format == '%') {
			format++;
			switch (*format) {
			case 'c': {
				char valch = va_arg(ap, int);
				tmp[idx++] = valch;
				format++;
				break;
			}
			case 'd': {
				int valint = va_arg(ap, int);
				_itoa(valint, tmp + idx, 10);
				while (tmp[idx++]);
				idx--;
				format++;
				break;
			}
			case 'o': {
				int valint = va_arg(ap, int);
				_itoa(valint, tmp + idx, 8);
				while (tmp[idx++]);
				idx--;
				format++;
				break;
			}
			case 'x': {
				int valint = va_arg(ap, int);
				_itoa(valint, tmp + idx, 16);
				while (tmp[idx++]);
				idx--;
				format++;
				break;
			}
			case 's': {
				char *valstr = va_arg(ap, char *);
				tmp[idx] = '\0';
				strcat(tmp, valstr);
				while (tmp[idx++]);
				idx--;
				format++;
				break;
			}
			case '%': {
				tmp[idx++] = *format++;
			}
			}
		}
		else {
			tmp[idx++] = *format++;
		}
	}

	tmp[idx] = '\0';
	OutputDebugString(_wd = _widen(tmp));
	free((void *)_wd);

	free(tmp);
	va_end(ap);
}
SGint checkThread() {
	int tmp = GetThreadId(GetCurrentThread());
	if (tmp != tmpThread) {
		alertInfo("系统拒绝在非主线程中使用绘图函数。", "SGL警告", ALERT_BUTTON_OK);
		return -1;
	}
	else return 0;
}

/*
* System interfaces.
* Started in v0.0.0
*/
void initWindow(int width, int height, const char *title, int mode) {
	if (mode == BIT_MAP || mode == TEXT_MAP)_sglMode = mode;
	else exit(SG_INVALID_MODE);

	if (mode == BIT_MAP) {
		if (width == 0 || height == 0) {
			RECT rect;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
			_Window->winWidth = rect.right - rect.left - 16;
			_Window->winHeight = rect.bottom - rect.top - 20;
			_Window->winWidth = _Window->winWidth - _Window->winWidth % 8;
			_Window->posLeft = _Window->posUp = 0;
		}
		else {
			_Window->winWidth = width;
			_Window->winHeight = height;
		}
	}
	if (mode == TEXT_MAP) {
		_Window->txtWidth = width;
		_Window->txtHeight = height;
		_Window->winWidth = width * SG_CHAR_WIDTH;
		_Window->winHeight = height * SG_CHAR_HEIGHT;
	}

	strcpy(_Window->winName, title);
	strcat(_Window->winName, "  --powered by Super GP");
}
void setWindow(int left, int up) {
	_Window->posLeft = left;
	_Window->posUp = up;
}
void setResizeable() {
	if (currentWindow == -1) {
		if (!_inLoop)
			_scrResizeable = 1;
	}
	else {
		if (!_wndList[currentWindow].inLoop)
			_wndList[currentWindow].scrResizeable = 1;
	}
}
void resizeFuntion(void(*func)(int x, int y)) {
	if (currentWindow == -1) {
		_resizeFunc = func;
	}
	else {
		_wndList[currentWindow].resizeFunc = func;
	}
}
int getWidth(int obj) {
	if (currentWindow == -1) {
		switch (obj) {
		case SG_WINDOW:
			return _Window->winWidth;
		case SG_SCREEN:
			if (_inLoop)
				return _Screen->buffer1->sizeX == _Screen->buffer2->sizeX ?
				_Screen->buffer1->sizeX : SG_SIZE_MISMATCH;
			else return _Window->winWidth;
		}
	}
	else {
		switch (obj) {
		case SG_WINDOW:
			return _wndList[currentWindow].winWidth;
		case SG_SCREEN:
			if (_wndList[currentWindow].inLoop)
				return _wndList[currentWindow].buffer1->sizeX == 
					_wndList[currentWindow].buffer2->sizeX ?
					_wndList[currentWindow].buffer1->sizeX : SG_SIZE_MISMATCH;
			else return _wndList[currentWindow].winWidth;
		}
	}
	return 0;
}
int getHeight(int obj) {
	if (currentWindow == -1) {
		switch (obj) {
		case SG_WINDOW:
			return _Window->winHeight;
		case SG_SCREEN:
			if (_inLoop)
				return _Screen->buffer1->sizeY == _Screen->buffer2->sizeY ?
				_Screen->buffer1->sizeY : SG_SIZE_MISMATCH;
			else return _Window->winHeight;
		}
	}
	else {
		switch (obj) {
		case SG_WINDOW:
			return _wndList[currentWindow].winHeight;
		case SG_SCREEN:
			if (_wndList[currentWindow].inLoop)
				return _wndList[currentWindow].buffer1->sizeY ==
					_wndList[currentWindow].buffer2->sizeY ?
					_wndList[currentWindow].buffer1->sizeY : SG_SIZE_MISMATCH;
			else return _wndList[currentWindow].winHeight;
		}
	}
	return 0;
}
int createWindow(int width, int height, const char *title, vect setup, vect loop) {
	WNDCLASSEX wc;
	SGWINSTR _wd = NULL, _wn = NULL;
	char tmpStr[20];

	if (_sglMode == TEXT_MAP)return SG_INVALID_MODE;
	_wndList[subNum].winWidth = width;
	_wndList[subNum].winHeight = height;
	_wndList[subNum].winName = (char *)malloc(strlen(title) + 1);
	strcpy(_wndList[subNum].winName, title);

	sgSubInit(setup);

	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = SubWndProc;
	wc.hInstance = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	sprintf(tmpStr, "SubClass%d", subNum);
	wc.lpszClassName = _wn = _widen(tmpStr);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	_wndList[subNum].loop = loop;
	_wndList[subNum].inLoop = 0;
	_wndList[subNum].scrResizeable = 0;
	_wndList[subNum].resizeFunc = NULL;
	_wndList[subNum].visualPage = 0;
	_wndList[subNum].activePage = 0;
	_wndList[subNum].hwnd = CreateWindow(wc.lpszClassName, _wd = _widen(title),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		_Window->hwnd, NULL, NULL, NULL);
	free((void *)_wd);
	free((void *)_wn);

	ShowWindow(_wndList[subNum].hwnd, SW_NORMAL);
	UpdateWindow(_wndList[subNum].hwnd);

	return subNum++;
}
void closeWindow(int id) {
	PostMessage(_wndList[id].hwnd, WM_CLOSE, 0, 0);
}
void startSubWindow(int id) {
	SEM_P();
	currentWindow = id;
}
void endSubWindow() {
	SEM_V();
}
void initKey() {
	_enKey = 1;
}
int biosKey(int cmd) {
	word ret;

	if (currentWindow == -1) {
		if (cmd == 1)return (_Key->front + 1) % 1024 != _Key->rear;
		if (cmd != 0)return 0;

		while ((_Key->front + 1) % 1024 == _Key->rear);
		ret = _Key->keyBuf[_Key->rear++];
		_Key->rear %= 1024;
	}
	else {
		if (cmd == 1)
			return (_wndList[currentWindow].key->front + 1) % 1024 !=
			_wndList[currentWindow].key->rear;
		if (cmd != 0)return 0;

		while ((_wndList[currentWindow].key->front + 1) % 1024 ==
			_wndList[currentWindow].key->rear);
		ret = _wndList[currentWindow].key->
			keyBuf[_wndList[currentWindow].key->rear++];
		_wndList[currentWindow].key->rear %= 1024;
	}

	return ret;
}
void clearKeyBuffer() {
	if (currentWindow == -1) {
		_Key->rear = (_Key->front + 1) % 1024;
	}
	else {
		_wndList[currentWindow].key->rear =
			(_wndList[currentWindow].key->front + 1) % 1024;
	}
}
void enablePanel() {
	_enPanel = 1;
}
void disablePanel() {
	_enPanel = 0;
}
int addPanelItem(const char *name, vect function, int shift, int ctrl) {
	int i = 0;
	if (shift == 0 && ctrl == 0) {
		for (i = 0; i < SG_MAX_PANEL_FUNCTION; i++) {
			if (panel.normalPanel[i] == NULL) {
				panel.normalPanel[i] = (struct _function *)malloc(sizeof(struct _function));
				panel.normalPanel[i]->id = panel.maxId++;
				panel.normalPanel[i]->name = (char *)malloc(strlen(name) + 1);
				strcpy(panel.normalPanel[i]->name, name);
				panel.normalPanel[i]->function = function;
				return panel.normalPanel[i]->id;
			}
		}
		if (i == SG_MAX_PANEL_FUNCTION)return SG_OUT_OF_RANGE;
	}
	if (shift == 1 && ctrl == 0) {
		for (i = 0; i < SG_MAX_PANEL_FUNCTION; i++) {
			if (panel.shiftPanel[i] == NULL) {
				panel.shiftPanel[i] = (struct _function *)malloc(sizeof(struct _function));
				panel.shiftPanel[i]->id = panel.maxId++;
				panel.shiftPanel[i]->name = (char *)malloc(strlen(name) + 1);
				strcpy(panel.shiftPanel[i]->name, name);
				panel.shiftPanel[i]->function = function;
				return panel.shiftPanel[i]->id;
			}
		}
		if (i == SG_MAX_PANEL_FUNCTION)return SG_OUT_OF_RANGE;
	}
	if (shift == 0 && ctrl == 1) {
		for (i = 0; i < SG_MAX_PANEL_FUNCTION; i++) {
			if (panel.ctrlPanel[i] == NULL) {
				panel.ctrlPanel[i] = (struct _function *)malloc(sizeof(struct _function));
				panel.ctrlPanel[i]->id = panel.maxId++;
				panel.ctrlPanel[i]->name = (char *)malloc(strlen(name) + 1);
				strcpy(panel.ctrlPanel[i]->name, name);
				panel.ctrlPanel[i]->function = function;
				return panel.ctrlPanel[i]->id;
			}
		}
		if (i == SG_MAX_PANEL_FUNCTION)return SG_OUT_OF_RANGE;
	}
	if (shift == 1 && ctrl == 1) {
		for (i = 0; i < SG_MAX_PANEL_FUNCTION; i++) {
			if (panel.shiftctrlPanel[i] == NULL) {
				panel.shiftctrlPanel[i] = (struct _function *)malloc(sizeof(struct _function));
				panel.shiftctrlPanel[i]->id = panel.maxId++;
				panel.shiftctrlPanel[i]->name = (char *)malloc(strlen(name) + 1);
				strcpy(panel.shiftctrlPanel[i]->name, name);
				panel.shiftctrlPanel[i]->function = function;
				return panel.shiftctrlPanel[i]->id;
			}
		}
		if (i == SG_MAX_PANEL_FUNCTION)return SG_OUT_OF_RANGE;
	}
	return SG_OUT_OF_RANGE;
}
int changePanelItem(int id, const char *name, vect function) {
	int i;
	for (i = 0; i < SG_MAX_PANEL_FUNCTION; i++) {
		if (panel.normalPanel[i]->id == id) {
			strcpy(panel.normalPanel[i]->name, name);
			panel.normalPanel[i]->function = function;
			return panel.normalPanel[i]->id;
		}
		if (panel.shiftPanel[i]->id == id) {
			strcpy(panel.shiftPanel[i]->name, name);
			panel.shiftPanel[i]->function = function;
			return panel.shiftPanel[i]->id;
		}
		if (panel.ctrlPanel[i]->id == id) {
			strcpy(panel.ctrlPanel[i]->name, name);
			panel.ctrlPanel[i]->function = function;
			return panel.ctrlPanel[i]->id;
		}
		if (panel.shiftctrlPanel[i]->id == id) {
			strcpy(panel.shiftctrlPanel[i]->name, name);
			panel.shiftctrlPanel[i]->function = function;
			return panel.shiftctrlPanel[i]->id;
		}
	}
	return SG_OBJECT_NOT_FOUND;
}
int deletePanelItem(int id) {
	int i;
	for (i = 0; i < SG_MAX_PANEL_FUNCTION; i++) {
		if (panel.normalPanel[i]->id == id) {
			free(panel.normalPanel[i]);
			panel.normalPanel[i] = NULL;
			return SG_NO_ERORR;
		}
		if (panel.shiftPanel[i]->id == id) {
			free(panel.shiftPanel[i]);
			panel.shiftPanel[i] = NULL;
			return SG_NO_ERORR;
		}
		if (panel.ctrlPanel[i]->id == id) {
			free(panel.ctrlPanel[i]);
			panel.ctrlPanel[i] = NULL;
			return SG_NO_ERORR;
		}
		if (panel.shiftctrlPanel[i]->id == id) {
			free(panel.shiftctrlPanel[i]);
			panel.shiftctrlPanel[i] = NULL;
			return SG_NO_ERORR;
		}
	}
	return SG_OBJECT_NOT_FOUND;
}
void initMouse(int mode) {
	if (mode&SG_COORDINATE)_Mouse->coord = 1;
	else _Mouse->coord = 0;

	_enMouse = 1;
}
int mouseStatus(int b) {
	if (currentWindow == -1) {
		switch (b) {
		case SG_LEFT_BUTTON:
			return _Mouse->left;
		case SG_RIGHT_BUTTON:
			return _Mouse->right;
		case SG_MIDDLE_BUTTON:
			return _Mouse->middle;
		default:
			return 0;
		}
	}
	else {
		switch (b) {
		case SG_LEFT_BUTTON:
			return _wndList[currentWindow].mouse->left;
		case SG_RIGHT_BUTTON:
			return _wndList[currentWindow].mouse->right;
		case SG_MIDDLE_BUTTON:
			return _wndList[currentWindow].mouse->middle;
		default:
			return 0;
		}
	}
}
vec2 mousePos() {
	vec2 ret;

	if (currentWindow == -1) {
		ret.x = _Mouse->Pos.x;
		ret.y = _Mouse->Pos.y;
	}
	else {
		ret.x = _wndList[currentWindow].mouse->Pos.x;
		ret.y = _wndList[currentWindow].mouse->Pos.y;
	}
	return ret;
}
vec3 biosMouse(int cmd) {
	vec3 ret;

	if (currentWindow == -1) {
		if (cmd == 1) {
			ret.z = (_Mouse->front + 1) % 1024 != _Mouse->rear;
			return ret;
		}
		if (cmd != 0) {
			ret.z = 0;
			return ret;
		}

		while ((_Mouse->front + 1) % 1024 == _Mouse->rear);
		ret = _Mouse->mouseBuf[_Mouse->rear++];
		_Mouse->rear %= 1024;
	}
	else {
		if (cmd == 1) {
			ret.z =(_wndList[currentWindow].mouse->front + 1) % 1024 !=
				_wndList[currentWindow].mouse->rear;
			return ret;
		}
		if (cmd != 0) {
			ret.z = 0;
			return ret;
		}

		while ((_wndList[currentWindow].mouse->front + 1) % 1024 ==
			_wndList[currentWindow].mouse->rear);
		ret = _wndList[currentWindow].mouse->
			mouseBuf[_wndList[currentWindow].mouse->rear++];
		_wndList[currentWindow].mouse->rear %= 1024;
	}

	return ret;
}
void clearMouseBuffer() {
	if (currentWindow == -1) {
		_Mouse->rear = (_Mouse->front + 1) % 1024;
	}
	else {
		_wndList[currentWindow].mouse->rear =
			(_wndList[currentWindow].mouse->front + 1) % 1024;
	}
}
void delay(int t) {
	clock_t st, en;

	_inDelay = 1;
	st = clock();
	en = clock();

	if (_inLoop) InvalidateRect(_Window->hwnd, NULL, FALSE);
	/*while (en - st < t) {
	en = clock();
	}*/
	Sleep(t);

	_inDelay = 0;
}
void delayBegin() {
	_delayBegin = clock();
	_delaySE = 1;
}
int delayEnd(int t) {
	if (!_delaySE)return 0;

	_inDelay = 1;

	_delayEnd = clock();
	if (t > _delayEnd - _delayBegin)
		Sleep(t - _delayEnd + _delayBegin);

	_delaySE = 0;
	_inDelay = 0;

	return 1;
}
vect getVect(int intn) {
	if (intn == 8)return _Vector->_v8;
	if (intn == 9)return _Vector->_v9;

	return (vect)NULL;
}
int setVect(int intn, vect v) {
	if (intn == 8) {
		_Vector->_v8 = v;
		return 1;
	}
	if (intn == 9) {
		_Vector->_v9 = v;
		return 1;
	}

	return 0;
}
void dosInt(int intn, int *ret) {
	if (intn == 9)*ret = _vectKey;
}
int random(int n) {
	return rand() % n;
}
void setFreq(float f) {
	_vectDelta = (clock_t)(1000 / f);
}
void showMouse() {
	while (ShowCursor(TRUE) < 0)
		ShowCursor(TRUE);
}
void hideMouse() {
	while (ShowCursor(FALSE) >= 0)
		ShowCursor(FALSE);
}
void setMousePos(int x, int y) {
	LPPOINT point;

	if (currentWindow == -1) {
		point = (LPPOINT)malloc(sizeof(POINT));
		point->x = x;
		point->y = y;

		ClientToScreen(_Window->hwnd, point);
		SetCursorPos(point->x, point->y);
		free(point);
	}
	else {
		point = (LPPOINT)malloc(sizeof(POINT));
		point->x = x;
		point->y = y;

		ClientToScreen(_wndList[currentWindow].hwnd, point);
		SetCursorPos(point->x, point->y);
		free(point);
	}
}
void setMouseIcon(SGWINSTR icon) {
	SetSystemCursor(LoadCursor(NULL, icon), 0);
}
void setActivePage(int page) {
	if (currentWindow == -1) {
		if (page != 0 && page != 1)return;
		_activePage = page;
	}
	else {
		if (page != 0 && page != 1)return;
		_wndList[currentWindow].activePage = page;
	}
}
void setVisualPage(int page) {
	if (currentWindow == -1) {
		if (page != 0 && page != 1)return;
		_visualPage = page;
	}
	else {
		if (page != 0 && page != 1)return;
		_wndList[currentWindow].visualPage = page;
	}
}
int selectFile(char name[], char start[], char format[], int idx) {
	OPENFILENAME ofn = { 0 };
	TCHAR strFilename[MAX_PATH] = { 0 };
	SGWINSTR _wd1 = NULL, _wd2;
	char *_ws, *filters[SG_MAX_FORMAT_FILTERS];
	int filterNum = 0, shortTotal = 0, wideTotal = 0, filterIdx,
		shortLen[SG_MAX_FORMAT_FILTERS], wideLen[SG_MAX_FORMAT_FILTERS];

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;

#ifdef UNICODE
	if (format) {
		while (format[shortTotal]) {
			filters[filterNum] = format + shortTotal;
			shortTotal += (shortLen[filterNum] = strlen(format + shortTotal)) + 1;
			wideTotal += (wideLen[filterNum] = _strlenW(format + wideTotal)) + 1;
			filterNum++;
		}
		_wd1 = (SGWINSTR)malloc(2 * (wideTotal + 1));
		for (filterIdx = 0, shortTotal = 0, wideTotal = 0; filterIdx < filterNum; filterIdx++) {
			MultiByteToWideChar(CP_ACP, 0, format + shortTotal, -1, _wd1 + wideTotal, wideLen[filterIdx]);
			wideTotal += wideLen[filterIdx] + 1;
			_wd1[wideTotal - 1] = 0;
			shortTotal += shortLen[filterIdx] + 1;
		}
		_wd1[wideTotal] = 0;
	}
	ofn.lpstrFilter = _wd1;
#else
	ofn.lpstrFilter = format;
#endif

	ofn.nFilterIndex = idx;
	ofn.lpstrFile = strFilename;
	ofn.nMaxFile = sizeof(strFilename);
	ofn.lpstrInitialDir = _wd2 = _widen(start);
	ofn.lpstrTitle = TEXT("请选择一个文件");
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	GetOpenFileName(&ofn);
	free((void *)_wd1);
	free((void *)_wd2);
	if (strFilename[0]) {
		strcpy(name, _ws = _shorten(strFilename));
		free(_ws);
		return SG_NO_ERORR;
	}
	else {
		return SG_OBJECT_NOT_FOUND;
	}
}
int selectSave(char name[], char start[], char format[], char def[], int idx) {
	OPENFILENAME ofn = { 0 };
	TCHAR strFilename[MAX_PATH] = { 0 };
	SGWINSTR _wd1 = NULL, _wd2, _wd3;
	char *_ws, *filters[SG_MAX_FORMAT_FILTERS];
	int filterNum = 0, shortTotal = 0, wideTotal = 0, filterIdx,
		shortLen[SG_MAX_FORMAT_FILTERS], wideLen[SG_MAX_FORMAT_FILTERS];

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;

#ifdef UNICODE
	if (format) {
		while (format[shortTotal]) {
			filters[filterNum] = format + shortTotal;
			shortTotal += (shortLen[filterNum] = strlen(format + shortTotal)) + 1;
			wideTotal += (wideLen[filterNum] = _strlenW(format + wideTotal)) + 1;
			filterNum++;
		}
		_wd1 = (SGWINSTR)malloc(2 * (wideTotal + 1));
		for (filterIdx = 0, shortTotal = 0, wideTotal = 0; filterIdx < filterNum; filterIdx++) {
			MultiByteToWideChar(CP_ACP, 0, format + shortTotal, -1, _wd1 + wideTotal, wideLen[filterIdx]);
			wideTotal += wideLen[filterIdx] + 1;
			_wd1[wideTotal - 1] = 0;
			shortTotal += shortLen[filterIdx] + 1;
		}
		_wd1[wideTotal] = 0;
	}
	ofn.lpstrFilter = _wd1;
#else
	ofn.lpstrFilter = format;
#endif

	ofn.nFilterIndex = idx;
	ofn.lpstrFile = strFilename;
	ofn.nMaxFile = sizeof(strFilename);
	ofn.lpstrInitialDir = _wd2 = _widen(start);
	ofn.lpstrTitle = TEXT("请选择一个文件");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrTitle = TEXT("保存到");
	ofn.lpstrDefExt = _wd3 = _widen(def);
	GetSaveFileName(&ofn);
	free((void *)_wd1);
	free((void *)_wd2);
	free((void *)_wd3);
	if (strFilename[0]) {
		strcpy(name, _ws = _shorten(strFilename));
		free(_ws);
		return SG_NO_ERORR;
	}
	else {
		return SG_OBJECT_NOT_FOUND;
	}
}
int selectDir(char name[], char start[]) {
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi = { 0 };
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = TEXT("选择一个文件夹");
	bi.ulFlags = BIF_NEWDIALOGSTYLE;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(idl, szBuffer);
	if (szBuffer[0]) {
		char *_ws;
		strcpy(name, _ws = _shorten(szBuffer));
		free(_ws);
		return SG_NO_ERORR;
	}
	else {
		return SG_OBJECT_NOT_FOUND;
	}
}
void alertInfo(const char *info, const char *title, int mode) {
	SGWINSTR _wd = NULL;
	MessageBox(NULL, _wd = _widen(info), _widen(title), mode);
	free((void *)_wd);
}
int initMidi() {
	return midiOutOpen(&midi.hMidiOut, MIDIMAPPER, 0, 0, 0);
}
void changeInstrument(int in) {
	_midiout(midi.hMidiOut, 0xC0, 0, in, 0);
}
void playMidi(int tune, int volume, int sw) {
	_midiout(midi.hMidiOut, sw ? 0x90 : 0x80, 0, tune, volume);
}
int playMidiFile(const char *filename) {
	return 0;
}
void stopMidiFile(int id) {

}
void pauseMidiFile(int id) {

}
void resumeMidiFile(int id) {

}
void initMci() {
	mci.buffer = (SGWINSTR)malloc(sizeof(SG_MCI_BUFFER_SIZE));
	mci.num = 0;
}
int loadMciSrc(const char *filename) {
	unsigned int i;
	char randstr[6];
	char *cmd = (char *)malloc(256);

	_itoa(mci.num, randstr, 10);

	strcpy(cmd, "open ");
	strcat(cmd, filename);
	for (i = 0; i < strlen(cmd); i++) {
		if (cmd[i] == '/')cmd[i] = '\\';
	}
	strcat(cmd, " alias ");
	strcat(cmd, "mp3_");
	strcat(cmd, randstr);
	if (!_mciSend(cmd)) {
		free(cmd);
		return SG_MEDIA_ERROR;
	}
	strcpy(cmd, "set ");
	strcat(cmd, "mp3_");
	strcat(cmd, randstr);
	strcat(cmd, " time format milliseconds");
	if (!_mciSend(cmd)) {
		free(cmd);
		return SG_MEDIA_ERROR;
	}
	strcpy(cmd, "status ");
	strcat(cmd, "mp3_");
	strcat(cmd, randstr);
	strcat(cmd, " length");
	if (!_mciSend(cmd)) {
		free(cmd);
		return SG_MEDIA_ERROR;
	}
	char *_ws;
	mci.length[mci.num] = atoi(_ws = _shorten((void *)mci.buffer));
	free(_ws);
	free(cmd);
	return mci.num++;
}
int playMci(int id) {
	char startTime[16], endTime[16];
	char randstr[6];
	char *cmd = (char *)malloc(256);

	_itoa(0, startTime, 10);
	_itoa(mci.length[id], endTime, 10);
	_itoa(id, randstr, 10);
	strcpy(cmd, "play ");
	strcat(cmd, "mp3_");
	strcat(cmd, randstr);
	strcat(cmd, " from ");
	strcat(cmd, startTime);
	strcat(cmd, " to ");
	strcat(cmd, endTime);
	if (_mciSend(cmd)) {
		free(cmd);
		return SG_NO_ERORR;
	}
	free(cmd);
	return SG_MEDIA_ERROR;
}
int stopMci(int id) {
	char randstr[6];
	char *cmd = (char *)malloc(256);

	_itoa(id, randstr, 10);
	strcpy(cmd, "stop ");
	strcat(cmd, "mp3_");
	strcat(cmd, randstr);
	if (!_mciSend(cmd)) {
		free(cmd);
		return SG_MEDIA_ERROR;
	}
	strcpy(cmd, "seek ");
	strcat(cmd, "mp3_");
	strcat(cmd, randstr);
	strcat(cmd, " to start");
	if (!_mciSend(cmd)) {
		free(cmd);
		return SG_MEDIA_ERROR;
	}
	free(cmd);
	return SG_NO_ERORR;
}
int pauseMci(int id) {
	char randstr[6];
	char *cmd = (char *)malloc(256);

	_itoa(id, randstr, 10);
	strcpy(cmd, "pause ");
	strcat(cmd, "mp3_");
	strcat(cmd, randstr);
	if (_mciSend(cmd)) {
		free(cmd);
		return SG_NO_ERORR;
	}
	free(cmd);
	return SG_MEDIA_ERROR;
}
int resumeMci(int id) {
	char randstr[6];
	char *cmd = (char *)malloc(256);

	_itoa(id, randstr, 10);
	strcpy(cmd, "resume ");
	strcat(cmd, "mp3_");
	strcat(cmd, randstr);
	if (_mciSend(cmd)) {
		free(cmd);
		return SG_NO_ERORR;
	}
	free(cmd);
	return SG_MEDIA_ERROR;
}
void createThread(vect func) {
	_setThreadFunc(func);
	HANDLE handle = CreateThread(NULL, 0, _threadFunc, NULL, 0, NULL);
}
void timerThread(vect func, int millis, int time) {
	_setThreadFunc(func);
	interval = millis;
	times = time;
	HANDLE handle = CreateThread(NULL, 0, _timerFunc, NULL, 0, NULL);
}
int initMenu() {
	int i;

	_mainMenu = 1;
	mainMenu.id = _tmpItem++;
	mainMenu.name = "main";
	mainMenu.hm = CreateMenu();
	for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
		mainMenu.sub[i] = NULL;
	}
	return mainMenu.id;
}
int addMenuList(const char *title, int id) {
	if (_mainMenu == 0)return SG_INVALID_MODE;
	return _addList(title, &mainMenu, id);
}
int addMenuItem(const char *title, int id, void(*func)()) {
	if (_mainMenu == 0)return SG_INVALID_MODE;
	return _addItem(title, &mainMenu, id, func);
}
int addMenuSeparator(int id) {
	if (_mainMenu == 0)return SG_INVALID_MODE;
	return _addSeparator(&mainMenu, id);
}
int copyText(const char *src) {
	HGLOBAL hg;
	char *pt;

	hg = GlobalAlloc(GHND | GMEM_SHARE,
		(strlen(src) + 1) * sizeof(TCHAR));
	pt = GlobalLock(hg);
	strcpy(pt, src);
	GlobalUnlock(hg);

	OpenClipboard(_Window->hwnd);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();

	return SG_NO_ERORR;
}
char *pasteText() {
	HGLOBAL hg;
	char *pt, *ret;

	OpenClipboard(_Window->hwnd);

	if (hg = GetClipboardData(CF_TEXT)) {
		pt = GlobalLock(hg);

		ret = malloc(GlobalSize(hg));
		strcpy(ret, pt);
	}
	else return NULL;
	CloseClipboard();
	return ret;
}
int enableItem(int id) {
	_checkItem(&mainMenu, id, -1, 0);
	return 0;
}
int disableItem(int id) {
	_checkItem(&mainMenu, id, -1, 1);
	return 0;
}
void checkItem(int id) {
	_checkItem(&mainMenu, id, 1, -1);
}
void uncheckItem(int id) {
	_checkItem(&mainMenu, id, 0, -1);
}
SOCKET createServer(int port) {
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return 0;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return 0;
	}
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);

	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	listen(sockSrv, SG_MAX_CONNECTION);
	return sockSrv;
}
SOCKET createClient(const char *server, int port) {
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return 0;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return 0;
	}
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(server);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);
	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	return sockClient;
}
SOCKET acceptOne(SOCKET server) {
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	return accept(server, (SOCKADDR*)&addrClient, &len);
}
int socketSend(SOCKET s, const char *buffer) {
	if (send(s, buffer, strlen(buffer), 0) == SOCKET_ERROR)return SG_CONNECTION_FAILED;
	return SG_NO_ERORR;
}
int socketReceive(SOCKET s, char *buffer, int len) {
	if (recv(s, buffer, len, 0) == SOCKET_ERROR)return SG_CONNECTION_FAILED;
	return SG_NO_ERORR;
}
void closeSocket(SOCKET s) {
	closesocket(s);
}
void hideCaption() {
	if (currentWindow == -1) {
		LONG lStyle = GetWindowLong(_Window->hwnd, GWL_STYLE);
		SetWindowLong(_Window->hwnd, GWL_STYLE, lStyle & ~WS_CAPTION);
	}
	else {
		LONG lStyle = GetWindowLong(_wndList[currentWindow].hwnd, GWL_STYLE);
		SetWindowLong(_wndList[currentWindow].hwnd, GWL_STYLE, lStyle & ~WS_CAPTION);
	}
}
void showCaption() {
	if (currentWindow == -1) {
		LONG lStyle = GetWindowLong(_Window->hwnd, GWL_STYLE);
		SetWindowLong(_Window->hwnd, GWL_STYLE, lStyle | WS_CAPTION);
	}
	else {
		LONG lStyle = GetWindowLong(_wndList[currentWindow].hwnd, GWL_STYLE);
		SetWindowLong(_wndList[currentWindow].hwnd, GWL_STYLE, lStyle | WS_CAPTION);
	}
}
void addTray() {
	NOTIFYICONDATA nid;

	if (_useTray)return;
	_useTray = 1;

	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = _Window->hwnd;
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.uCallbackMessage = WM_TRAY;
	if (_Window->hIcon == NULL)
		nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	else
		nid.hIcon = _Window->hIcon;
	SGWINSTR _wd = NULL;
	_strcpy(nid.szTip, _wd = _widen(_Window->winName));
	Shell_NotifyIcon(NIM_ADD, &nid);
	free((void *)_wd);
}
void hideToTray() {
	addTray();
	ShowWindow(_Window->hwnd, SW_HIDE);
}
void restoreFromTray() {
	ShowWindow(_Window->hwnd, SW_RESTORE);
}
int initTrayMenu() {
	int i;

	_trayMenu = 1;
	trayMenu.id = _tmpItem++;
	trayMenu.name = "tray";
	trayMenu.hm = CreatePopupMenu();
	for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
		trayMenu.sub[i] = NULL;
	}
	return trayMenu.id;
}
int addTrayMenuList(const char *title, int id) {
	if (_trayMenu == 0)return SG_INVALID_MODE;
	return _addList(title, &trayMenu, id);
}
int addTrayMenuItem(const char *title, int id, void(*func)()) {
	if (_trayMenu == 0)return SG_INVALID_MODE;
	return _addItem(title, &trayMenu, id, func);
}
int addTrayMenuSeparator(int id) {
	if (_trayMenu == 0)return SG_INVALID_MODE;
	return _addSeparator(&trayMenu, id);
}
int createPopupMenu() {
	int i;

	popupMenu[_popupNum].id = _tmpItem++;
	popupMenu[_popupNum].name = "popup";
	popupMenu[_popupNum].hm = CreatePopupMenu();
	for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
		popupMenu[_popupNum].sub[i] = NULL;
	}
	return popupMenu[_popupNum++].id;
}
int addPopupMenuList(const char *title, int id) {
	int i, ret;
	for (i = 0; i < _popupNum; i++) {
		if ((ret = _addList(title, &popupMenu[i], id)) < 0)continue;
		break;
	}
	if (i == _popupNum)return SG_OBJECT_NOT_FOUND;
	return ret;
}
int addPopupMenuItem(const char *title, int id, void(*func)()) {
	int i, ret;
	for (i = 0; i < _popupNum; i++) {
		if ((ret = _addItem(title, &popupMenu[i], id, func)) < 0)continue;
		break;
	}
	if (i == _popupNum)return SG_OBJECT_NOT_FOUND;
	return ret;
}
int addPopupMenuSeparator(int id) {
	int i, ret;
	for (i = 0; i < _popupNum; i++) {
		if ((ret = _addSeparator(&popupMenu[i], id)) < 0)continue;
		break;
	}
	if (i == _popupNum)return SG_OBJECT_NOT_FOUND;
	return ret;
}
int finishPopupMenu(int id) {
	int i;
	for (i = 0; i < _popupNum; i++) {
		if (popupMenu[i].id == id)break;
	}
	if (i == _popupNum)return SG_OBJECT_NOT_FOUND;
	_createMenu(MT_POPUP, NULL, popupMenu[i].hm);
	return SG_NO_ERORR;
}
int showPopupMenu(int menu, int x, int y) {
	int i;
	struct tagPOINT p;

	if (currentWindow == -1) {
		for (i = 0; i < _popupNum; i++) {
			if (popupMenu[i].id == menu)break;
		}
		if (i == _popupNum)return SG_OBJECT_NOT_FOUND;
		if (!_scrResizeable) {
			p.x = x * _Window->winWidth / _Screen->buffer1->sizeX;
			p.y = y * _Window->winHeight / _Screen->buffer1->sizeY;
		}
		else {
			p.x = x;
			p.y = y;
		}
		ClientToScreen(_Window->hwnd, &p);
		TrackPopupMenu(popupMenu[i].hm, TPM_LEFTALIGN,
			p.x, p.y, 0, _Window->hwnd, NULL);
	}
	else {
		for (i = 0; i < _popupNum; i++) {
			if (popupMenu[i].id == menu)break;
		}
		if (i == _popupNum)return SG_OBJECT_NOT_FOUND;
		if (_wndList[currentWindow].scrResizeable) {
			p.x = x * _wndList[currentWindow].winWidth /
				_wndList[currentWindow].buffer1->sizeX;
			p.y = y * _wndList[currentWindow].winHeight /
				_wndList[currentWindow].buffer1->sizeY;
		}
		else {
			p.x = x;
			p.y = y;
		}
		ClientToScreen(_wndList[currentWindow].hwnd, &p);
		TrackPopupMenu(popupMenu[i].hm, TPM_LEFTALIGN,
			p.x, p.y, 0, _wndList[currentWindow].hwnd, NULL);
	}

	return SG_NO_ERORR;
}
void setIcon(const char *ico) {
	SGWINSTR _wd = NULL;
	if (currentWindow == -1) {
		_Window->hIcon = LoadImage(NULL, _wd = _widen(ico), IMAGE_ICON, 0, 0,
			LR_LOADFROMFILE | LR_DEFAULTSIZE);
		free((void *)_wd);
	}
	else {
		_wndList[currentWindow].hIcon =
			LoadImage(NULL, _wd = _widen(ico), IMAGE_ICON, 0, 0,
			LR_LOADFROMFILE | LR_DEFAULTSIZE);
		free((void *)_wd);
	}
}
bitMap *copyPic(bitMap *src) {
	bitMap *dst = (bitMap*)malloc(sizeof(bitMap));
	dst->data = malloc(src->sizeX*src->sizeY * 3 * sizeof(char));

	dst->sizeX = src->sizeX;
	dst->sizeY = src->sizeY;
	memcpy(dst->data, src->data, src->sizeX*src->sizeY * 3 * sizeof(char));
	return dst;
}
bitMap *grayPic(bitMap *src) {
	int i, j, v;
	bitMap *dst = (bitMap*)malloc(sizeof(bitMap));
	dst->data = malloc(src->sizeX*src->sizeY * 3 * sizeof(char));

	dst->sizeX = src->sizeX;
	dst->sizeY = src->sizeY;
	for (i = 0; i < src->sizeY; i++) {
		for (j = 0; j < src->sizeX; j++) {
			v = (int)(src->data[(i*src->sizeX + j) * 3] * 0.299f +
				src->data[(i*src->sizeX + j) * 3 + 1] * 0.587f +
				src->data[(i*src->sizeX + j) * 3 + 2] * 0.114f);
			dst->data[(i*src->sizeX + j) * 3] =
				dst->data[(i*src->sizeX + j) * 3 + 1] =
				dst->data[(i*src->sizeX + j) * 3 + 2] = v;
		}
	}
	return dst;
}
bitMap *binaryPic(bitMap *src, int threshold) {
	int i, j, v;
	bitMap *dst = (bitMap*)malloc(sizeof(bitMap));
	dst->data = malloc(src->sizeX*src->sizeY * 3 * sizeof(char));

	dst->sizeX = src->sizeX;
	dst->sizeY = src->sizeY;
	for (i = 0; i < src->sizeY; i++) {
		for (j = 0; j < src->sizeX; j++) {
			v = (int)(src->data[(i*src->sizeX + j) * 3] * 0.299f +
				src->data[(i*src->sizeX + j) * 3 + 1] * 0.587f +
				src->data[(i*src->sizeX + j) * 3 + 2] * 0.114f);
			if (v < threshold)
				dst->data[(i*src->sizeX + j) * 3] =
				dst->data[(i*src->sizeX + j) * 3 + 1] =
				dst->data[(i*src->sizeX + j) * 3 + 2] = 0;
			else
				dst->data[(i*src->sizeX + j) * 3] =
				dst->data[(i*src->sizeX + j) * 3 + 1] =
				dst->data[(i*src->sizeX + j) * 3 + 2] = 255;
		}
	}
	return dst;
}
bitMap *zoomPic(bitMap *src, float rate) {
	int i, j;
	float tmpx, tmpy, deltax, deltay;
	bitMap *dst = (bitMap*)malloc(sizeof(bitMap));
	dst->data = malloc((int)(src->sizeX * rate) * (int)(src->sizeY * rate) * 3 * sizeof(char));
	dst->sizeX = (int)(src->sizeX * rate);
	dst->sizeY = (int)(src->sizeY * rate);
	for (i = 0; i < dst->sizeY; i++) {
		for (j = 0; j < dst->sizeX; j++) {
			tmpx = j / rate;
			tmpy = i / rate;
			deltax = tmpx - (int)tmpx;
			deltay = tmpy - (int)tmpy;
			dst->data[(i*dst->sizeX + j) * 3] = (unsigned char)
				((src->data[((int)(i / rate + 1)*src->sizeX + (int)(j / rate)) * 3] * (1 - deltay) +
					src->data[((int)(i / rate)*src->sizeX + (int)(j / rate)) * 3] * deltay)*deltax +
					(src->data[((int)(i / rate + 1)*src->sizeX + (int)(j / rate + 1)) * 3] * (1 - deltay) +
						src->data[((int)(i / rate)*src->sizeX + (int)(j / rate + 1)) * 3] * deltay)*(1 - deltax));
			dst->data[(i*dst->sizeX + j) * 3 + 1] = (unsigned char)
				((src->data[((int)(i / rate + 1)*src->sizeX + (int)(j / rate)) * 3 + 1] * (1 - deltay) +
					src->data[((int)(i / rate)*src->sizeX + (int)(j / rate)) * 3 + 1] * deltay)*deltax +
					(src->data[((int)(i / rate + 1)*src->sizeX + (int)(j / rate + 1)) * 3 + 1] * (1 - deltay) +
						src->data[((int)(i / rate)*src->sizeX + (int)(j / rate + 1)) * 3 + 1] * deltay)*(1 - deltax));
			dst->data[(i*dst->sizeX + j) * 3 + 2] = (unsigned char)
				((src->data[((int)(i / rate + 1)*src->sizeX + (int)(j / rate)) * 3 + 2] * (1 - deltay) +
					src->data[((int)(i / rate)*src->sizeX + (int)(j / rate)) * 3 + 2] * deltay)*deltax +
					(src->data[((int)(i / rate + 1)*src->sizeX + (int)(j / rate + 1)) * 3 + 2] * (1 - deltay) +
						src->data[((int)(i / rate)*src->sizeX + (int)(j / rate + 1)) * 3 + 2] * deltay)*(1 - deltax));
		}
	}
	return dst;
}
bitMap *rotatePic(bitMap *src, bitMap *mask, float angle) {
	return NULL;
}
bitMap *filterPic(bitMap *src, int mode) {
	int i, j, v, x, y, z, pos;
	float weight, sum;
	bitMap *dst = (bitMap*)malloc(sizeof(bitMap));
	dst->data = malloc(src->sizeX*src->sizeY * 3 * sizeof(char));

	dst->sizeX = src->sizeX;
	dst->sizeY = src->sizeY;
	switch (mode) {
	case MEAN_FILTER:
		for (i = 0; i < src->sizeY; i++) {
			for (j = 0; j < src->sizeX; j++) {
				pos = (i*src->sizeX + j) * 3;
				if (i == 0 || j == 0 || i == src->sizeY - 1 || j == src->sizeX - 1)
					memcpy(dst->data + pos, src->data + pos, 3 * sizeof(char));
				else {
					for (z = 0; z < 3; z++) {
						v = 0;
						for (y = 0; y < 3; y++) {
							for (x = 0; x < 3; x++) {
								v += src->data[pos + ((y - 1)*src->sizeX + x - 1) * 3 + z];
							}
						}
						dst->data[pos + z] = v / 9;;
					}
				}
			}
		}
		break;
	case LAPLACIAN_FILTER:
		for (i = 0; i < src->sizeY; i++) {
			for (j = 0; j < src->sizeX; j++) {
				pos = (i*src->sizeX + j) * 3;
				if (i == 0 || j == 0 || i == src->sizeY - 1 || j == src->sizeX - 1)
					memcpy(dst->data + pos, src->data + pos, 3 * sizeof(char));
				else {
					for (z = 0; z < 3; z++) {
						v = 0;
						v += src->data[pos - 3 + z];
						v += src->data[pos + 3 + z];
						v += src->data[pos - src->sizeX * 3 + z];
						v += src->data[pos + src->sizeX * 3 + z];
						v -= 4 * src->data[pos + z];
						dst->data[pos + z] = src->data[pos + z] + v / 9;
					}
				}
			}
		}
		break;
	case BILATERAL_FILTER:
		for (i = 0; i < src->sizeY; i++) {
			for (j = 0; j < src->sizeX; j++) {
				int pos = (i*src->sizeX + j) * 3;
				if (i == 0 || j == 0 || i == src->sizeY - 1 || j == src->sizeX - 1)
					memcpy(dst->data + pos, src->data + pos, 3 * sizeof(char));
				else {
					for (z = 0; z < 3; z++) {
						sum = 0;
						weight = 0;
						for (y = 0; y < 3; y++) {
							for (x = 0; x < 3; x++) {
								weight += (float)exp(-((x - 1)*(x - 1) + (y - 1)*(y - 1)) -
									((src->data[pos + z] - src->data[pos + ((y - 1)*src->sizeX + x - 1) * 3 + z])*
									(src->data[pos + z] - src->data[pos + ((y - 1)*src->sizeX + x - 1) * 3 + z])) / 1024);
								sum += src->data[pos + ((y - 1)*src->sizeX + x - 1) * 3 + z] *
									(float)exp(-((x - 1)*(x - 1) + (y - 1)*(y - 1)) -
									((src->data[pos + z] - src->data[pos + ((y - 1)*src->sizeX + x - 1) * 3 + z])*
										(src->data[pos + z] - src->data[pos + ((y - 1)*src->sizeX + x - 1) * 3 + z])) / 1024);
							}
						}
						dst->data[pos + z] = (int)(sum / weight);
					}
				}
			}
		}
		break;
	default:
		free(dst);
		return NULL;
	}
	return dst;
}
bitMap *luminantPic(bitMap *src, int delta) {
	int i, j, y, u, v;
	bitMap *dst = (bitMap*)malloc(sizeof(bitMap));
	dst->data = malloc(src->sizeX*src->sizeY * 3 * sizeof(char));

	dst->sizeX = src->sizeX;
	dst->sizeY = src->sizeY;
	for (i = 0; i < src->sizeY; i++) {
		for (j = 0; j < src->sizeX; j++) {
			y = (int)(src->data[(i*src->sizeX + j) * 3] * 0.299f +
				src->data[(i*src->sizeX + j) * 3 + 1] * 0.587f +
				src->data[(i*src->sizeX + j) * 3 + 2] * 0.114f);
			u = -(int)(src->data[(i*src->sizeX + j) * 3] * 0.417f -
				src->data[(i*src->sizeX + j) * 3 + 1] * 0.289f +
				src->data[(i*src->sizeX + j) * 3 + 2] * 0.436f);
			v = (int)(src->data[(i*src->sizeX + j) * 3] * 0.615f -
				src->data[(i*src->sizeX + j) * 3 + 1] * 0.515f -
				src->data[(i*src->sizeX + j) * 3 + 2] * 0.100f);

			y += delta;
			if (y > 255)y = 255;
			if (y < 0)y = 0;

			dst->data[(i*src->sizeX + j) * 3] = (char)(y + 1.14f * v);
			dst->data[(i*src->sizeX + j) * 3 + 1] = (char)(y - 0.39f * u - 0.58f * v);
			dst->data[(i*src->sizeX + j) * 3 + 2] = (char)(y + 2.03f * u);
		}
	}
	return dst;
}
bitMap *contrastPic(bitMap *src, int delta) {
	return NULL;
}


/*
* Bitmap interfaces.
* Started in v0.0.0
*/
void setColor(int r, int g, int b) {
	if (_sglMode != BIT_MAP && !_innerFunc)return;

	if (currentWindow == -1) {
		if (checkThread())return;

		_Screen->rgb[0] = r % 256;
		_Screen->rgb[1] = g % 256;
		_Screen->rgb[2] = b % 256;

		tf.color.r = r;
		tf.color.g = g;
		tf.color.b = b;
	}
	else {
		_wndList[currentWindow].rgb[0] = r % 256;
		_wndList[currentWindow].rgb[1] = g % 256;
		_wndList[currentWindow].rgb[2] = b % 256;

		_wndList[currentWindow].tf.color.r = r;
		_wndList[currentWindow].tf.color.g = g;
		_wndList[currentWindow].tf.color.b = b;
	}
}
void setFontSize(int height) {
	if (_sglMode != BIT_MAP && !_innerFunc)return;

	if (currentWindow == -1) {
		if (checkThread())return;

		if (height<SG_MAX_FONT_LENGTH)tf.size = height;
		else tf.size = SG_MAX_FONT_LENGTH;

		DeleteObject(text.font);
		text.font = CreateFont(tf.size, 0, 0, 0, FW_THIN, tf.coeff & FONT_ITALIC,
			tf.coeff & FONT_UNDERLINE, tf.coeff & FONT_STRIKEOUT,
			DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
			DEFAULT_QUALITY, FF_MODERN, tf.name);
		SelectObject(text.memDC, text.font);
	}
	else {
		if (height<SG_MAX_FONT_LENGTH)_wndList[currentWindow].tf.size = height;
		else _wndList[currentWindow].tf.size = SG_MAX_FONT_LENGTH;

		DeleteObject(_wndList[currentWindow].text.font);
		_wndList[currentWindow].text.font = 
			CreateFont(_wndList[currentWindow].tf.size, 0, 0, 0, FW_THIN,
			_wndList[currentWindow].tf.coeff & FONT_ITALIC,
			_wndList[currentWindow].tf.coeff & FONT_UNDERLINE,
			_wndList[currentWindow].tf.coeff & FONT_STRIKEOUT,
			DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
			DEFAULT_QUALITY, FF_MODERN, _wndList[currentWindow].tf.name);
		SelectObject(_wndList[currentWindow].text.memDC,
			_wndList[currentWindow].text.font);
	}
}
void setFontName(const char *name) {
	if (_sglMode != BIT_MAP && !_innerFunc)return;

	if (currentWindow == -1) {
		if (checkThread())return;

		if (tf.name)free((void *)tf.name);
		tf.name = _widen(name);

		DeleteObject(text.font);
		text.font = CreateFont(tf.size, 0, 0, 0, FW_THIN, tf.coeff & FONT_ITALIC,
			tf.coeff & FONT_UNDERLINE, tf.coeff & FONT_STRIKEOUT,
			DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
			DEFAULT_QUALITY, FF_MODERN, tf.name);
		SelectObject(text.memDC, text.font);
	}
	else {
		if (_wndList[currentWindow].tf.name)
			free((void *)_wndList[currentWindow].tf.name);
		_wndList[currentWindow].tf.name = _widen(name);

		DeleteObject(_wndList[currentWindow].text.font);
		_wndList[currentWindow].text.font =
			CreateFont(_wndList[currentWindow].tf.size, 0, 0, 0, FW_THIN,
			_wndList[currentWindow].tf.coeff & FONT_ITALIC,
			_wndList[currentWindow].tf.coeff & FONT_UNDERLINE,
			_wndList[currentWindow].tf.coeff & FONT_STRIKEOUT,
			DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
			DEFAULT_QUALITY, FF_MODERN, _wndList[currentWindow].tf.name);
		SelectObject(_wndList[currentWindow].text.memDC,
			_wndList[currentWindow].text.font);
	}
}
void setFontStyle(int coeff) {
	if (_sglMode != BIT_MAP && !_innerFunc)return;

	if (currentWindow == -1) {
		if (checkThread())return;

		tf.coeff = coeff;

		DeleteObject(text.font);
		text.font = CreateFont(tf.size, 0, 0, 0, FW_THIN, tf.coeff & FONT_ITALIC,
			tf.coeff & FONT_UNDERLINE, tf.coeff & FONT_STRIKEOUT,
			DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
			DEFAULT_QUALITY, FF_MODERN, tf.name);
		SelectObject(text.memDC, text.font);
	}
	else {
		_wndList[currentWindow].tf.coeff = coeff;

		DeleteObject(_wndList[currentWindow].text.font);
		_wndList[currentWindow].text.font =
			CreateFont(_wndList[currentWindow].tf.size, 0, 0, 0, FW_THIN,
			_wndList[currentWindow].tf.coeff & FONT_ITALIC,
			_wndList[currentWindow].tf.coeff & FONT_UNDERLINE,
			_wndList[currentWindow].tf.coeff & FONT_STRIKEOUT,
			DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
			DEFAULT_QUALITY, FF_MODERN, _wndList[currentWindow].tf.name);
		SelectObject(_wndList[currentWindow].text.memDC,
			_wndList[currentWindow].text.font);
	}
}
void setAlpha(float alpha) {
	if (_sglMode != BIT_MAP && !_innerFunc)return;

	if (currentWindow == -1) {
		if (checkThread())return;

		_Screen->alpha = alpha;
	}
	else {
		_wndList[currentWindow].alpha = alpha;
	}
}
float getAlpha() {
	if (_sglMode != BIT_MAP && !_innerFunc)return 0.f;

	if (currentWindow == -1) {
		return _Screen->alpha;
	}
	else {
		return _wndList[currentWindow].alpha;
	}
}
void clearScreen() {
	int i;
	bitMap *buf;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

	if (currentWindow == -1) {
		if (checkThread())return;

		if (_activePage == 0) buf = _Screen->buffer1;
		else buf = _Screen->buffer2;

		if ((_Screen->rgb[0] & _Screen->rgb[1] & _Screen->rgb[2]) == 255) {
			memset(buf->data, -1, 3 * buf->sizeX*buf->sizeY);
			return;
		}
		if ((_Screen->rgb[0] | _Screen->rgb[1] | _Screen->rgb[2]) == 0) {
			memset(buf->data, 0, 3 * buf->sizeX*buf->sizeY);
			return;
		}

		for (i = 0; i < 3 * buf->sizeX*buf->sizeY; i += 3) {
			buf->data[i] = _Screen->rgb[2];
			buf->data[i + 1] = _Screen->rgb[1];
			buf->data[i + 2] = _Screen->rgb[0];
		}
	}
	else {
		if (_wndList[currentWindow].activePage == 0) buf = _wndList[currentWindow].buffer1;
		else buf = _wndList[currentWindow].buffer2;

		if ((_wndList[currentWindow].rgb[0] &
			_wndList[currentWindow].rgb[1] &
			_wndList[currentWindow].rgb[2]) == 255) {
			memset(buf->data, -1, 3 * buf->sizeX*buf->sizeY);
			return;
		}
		if ((_wndList[currentWindow].rgb[0] |
			_wndList[currentWindow].rgb[1] |
			_wndList[currentWindow].rgb[2]) == 0) {
			memset(buf->data, 0, 3 * buf->sizeX*buf->sizeY);
			return;
		}

		for (i = 0; i < 3 * buf->sizeX*buf->sizeY; i += 3) {
			buf->data[i] = _wndList[currentWindow].rgb[2];
			buf->data[i + 1] = _wndList[currentWindow].rgb[1];
			buf->data[i + 2] = _wndList[currentWindow].rgb[0];
		}
	}
}
int putPixel(int x, int y) {
	bitMap *buf;
	int p;

	if (_sglMode != BIT_MAP && !_innerFunc)return SG_INVALID_MODE;

	if (currentWindow == -1) {
		if (checkThread())return SG_WRONG_THREAD;

		if (_activePage == 0) buf = _Screen->buffer1;
		else buf = _Screen->buffer2;

		if (x < 0 || x >= buf->sizeX || y < 0 || y >= buf->sizeY)return SG_OUT_OF_RANGE;

		p = (y * buf->sizeX + x) * 3;
		if (_Screen->alpha == 1.f) {
			buf->data[p] = _Screen->rgb[2];
			buf->data[p + 1] = _Screen->rgb[1];
			buf->data[p + 2] = _Screen->rgb[0];
		}
		else {
			buf->data[p] = (int)(buf->data[p] * (1.f - _Screen->alpha));
			buf->data[p] += (int)(_Screen->rgb[2] * (_Screen->alpha));
			buf->data[p + 1] = (int)(buf->data[p + 1] * (1.f - _Screen->alpha));
			buf->data[p + 1] += (int)(_Screen->rgb[1] * (_Screen->alpha));
			buf->data[p + 2] = (int)(buf->data[p + 2] * (1.f - _Screen->alpha));
			buf->data[p + 2] += (int)(_Screen->rgb[0] * (_Screen->alpha));
		}
	}
	else {
		if (_wndList[currentWindow].activePage == 0)
			buf = _wndList[currentWindow].buffer1;
		else buf = _wndList[currentWindow].buffer2;

		if (x < 0 || x >= buf->sizeX || y < 0 || y >= buf->sizeY)return SG_OUT_OF_RANGE;

		p = (y * buf->sizeX + x) * 3;
		if (_wndList[currentWindow].alpha == 1.f) {
			buf->data[p] = _wndList[currentWindow].rgb[2];
			buf->data[p + 1] = _wndList[currentWindow].rgb[1];
			buf->data[p + 2] = _wndList[currentWindow].rgb[0];
		}
		else {
			buf->data[p] = (int)(buf->data[p] * (1.f - _wndList[currentWindow].alpha));
			buf->data[p] +=
				(int)(_wndList[currentWindow].rgb[2] * (_wndList[currentWindow].alpha));
			buf->data[p + 1] = (int)(buf->data[p + 1] * (1.f - _wndList[currentWindow].alpha));
			buf->data[p + 1] +=
				(int)(_wndList[currentWindow].rgb[1] * (_wndList[currentWindow].alpha));
			buf->data[p + 2] = (int)(buf->data[p + 2] * (1.f - _wndList[currentWindow].alpha));
			buf->data[p + 2] +=
				(int)(_wndList[currentWindow].rgb[0] * (_wndList[currentWindow].alpha));
		}
	}

	return SG_NO_ERORR;
}
RGB getPixel(int x, int y) {
	RGB im, ret;
	bitMap *buf;
	int p;

	if (_sglMode != BIT_MAP && !_innerFunc) {
		im.r = im.g = im.b = SG_INVALID_MODE;
		return im;
	}

	if (currentWindow == -1) {
		if (checkThread()) {
			im.r = im.g = im.b = SG_WRONG_THREAD;
			return im;
		}

		if (_activePage == 0) buf = _Screen->buffer1;
		else buf = _Screen->buffer2;

		ret.r = ret.g = ret.b = SG_OUT_OF_RANGE;

		if (x < 0 || x >= buf->sizeX || y < 0 || y >= buf->sizeY)return ret;
		p = (y*buf->sizeX + x) * 3;
		ret.r = buf->data[p++];
		ret.g = buf->data[p++];
		ret.b = buf->data[p++];
	}
	else {
		if (_wndList[currentWindow].activePage == 0)
			buf = _wndList[currentWindow].buffer1;
		else buf = _wndList[currentWindow].buffer2;

		ret.r = ret.g = ret.b = SG_OUT_OF_RANGE;

		if (x < 0 || x >= buf->sizeX || y < 0 || y >= buf->sizeY)return ret;
		p = (y*buf->sizeX + x) * 3;
		ret.r = buf->data[p++];
		ret.g = buf->data[p++];
		ret.b = buf->data[p++];
	}

	return ret;
}
void putLine(int x1, int y1, int x2, int y2, int mode) {
	int dx, dy, ux, uy, x, y, eps;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

#define ABS(x) ((x > 0) ? (x) : (-x))

	if (currentWindow == -1) {
		if (checkThread())return;

		dx = x2 - x1;
		dy = y2 - y1;

		ux = ((dx > 0) << 1) - 1;
		uy = ((dy > 0) << 1) - 1;

		x = x1;
		y = y1;

		eps = 0;
		dx = ABS(dx);
		dy = ABS(dy);

		if (dx > dy) {
			for (x = x1; x != x2 + ux; x += ux) {
				putPixel(x, y);
				eps += dy;
				if ((eps << 1) >= dx) {
					y += uy;
					eps -= dx;
				}
			}
		}
		else {
			for (y = y1; y != y2 + uy; y += uy) {
				putPixel(x, y);
				eps += dx;
				if ((eps << 1) >= dy) {
					x += ux;
					eps -= dy;
				}
			}
		}
	}
	else {
		dx = x2 - x1;
		dy = y2 - y1;

		ux = ((dx > 0) << 1) - 1;
		uy = ((dy > 0) << 1) - 1;

		x = x1;
		y = y1;

		eps = 0;
		dx = ABS(dx);
		dy = ABS(dy);

		if (dx > dy) {
			for (x = x1; x != x2 + ux; x += ux) {
				putPixel(x, y);
				eps += dy;
				if ((eps << 1) >= dx) {
					y += uy;
					eps -= dx;
				}
			}
		}
		else {
			for (y = y1; y != y2 + uy; y += uy) {
				putPixel(x, y);
				eps += dx;
				if ((eps << 1) >= dy) {
					x += ux;
					eps -= dy;
				}
			}
		}
	}

#undef ABS
}
void putQuad(int x1, int y1, int x2, int y2, int mode) {
	int i, j;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

	if (currentWindow == -1) {
		if (checkThread())return;

		if (mode == SOLID_FILL)
			for (i = x1; i <= x2; i++)
				for (j = y1; j <= y2; j++)
					putPixel(i, j);

		if (mode == EMPTY_FILL) {
			for (i = x1; i <= x2; i++) {
				putPixel(i, y1);
				putPixel(i, y2);
			}
			for (j = y1; j <= y2; j++) {
				putPixel(x1, j);
				putPixel(x2, j);
			}
		}
	}
	else {
		if (mode == SOLID_FILL)
			for (i = x1; i <= x2; i++)
				for (j = y1; j <= y2; j++)
					putPixel(i, j);

		if (mode == EMPTY_FILL) {
			for (i = x1; i <= x2; i++) {
				putPixel(i, y1);
				putPixel(i, y2);
			}
			for (j = y1; j <= y2; j++) {
				putPixel(x1, j);
				putPixel(x2, j);
			}
		}
	}
}
void putTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int mode) {
	RGB c;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

	if (currentWindow == -1) {
		if (checkThread())return;

		c.r = _Screen->rgb[0];
		c.g = _Screen->rgb[1];
		c.b = _Screen->rgb[2];
		putLine(x1, y1, x2, y2, SOLID_LINE);
		putLine(x1, y1, x3, y3, SOLID_LINE);
		putLine(x2, y2, x3, y3, SOLID_LINE);
		if (mode == SOLID_FILL)floodFill((x1 + x2 + x3) / 3, (y1 + y2 + y3) / 3, c);
	}
	else {
		c.r = _wndList[currentWindow].rgb[0];
		c.g = _wndList[currentWindow].rgb[1];
		c.b = _wndList[currentWindow].rgb[2];
		putLine(x1, y1, x2, y2, SOLID_LINE);
		putLine(x1, y1, x3, y3, SOLID_LINE);
		putLine(x2, y2, x3, y3, SOLID_LINE);
		if (mode == SOLID_FILL)floodFill((x1 + x2 + x3) / 3, (y1 + y2 + y3) / 3, c);
	}
}
void putCircle(int xc, int yc, int r, int mode) {
	int x, y, yi, d;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

	if (currentWindow == -1) {
		if (checkThread())return;

		x = 0;
		y = r;
		d = 3 - 2 * r;

		if (mode == SOLID_FILL) {
			putPixel(xc, yc);
			while (x <= y) {
				if (x == 0) {
					for (yi = x + 1; yi <= y; yi++) {
						putPixel(xc + x, yc + yi);
						putPixel(xc + x, yc - yi);
						putPixel(xc + yi, yc + x);
						putPixel(xc - yi, yc + x);
					}
				}
				else {
					putPixel(xc + x, yc + x);
					putPixel(xc + x, yc - x);
					putPixel(xc - x, yc - x);
					putPixel(xc - x, yc + x);
					for (yi = x + 1; yi <= y; yi++) {
						putPixel(xc + x, yc + yi);
						putPixel(xc + x, yc - yi);
						putPixel(xc - x, yc - yi);
						putPixel(xc - x, yc + yi);
						putPixel(xc + yi, yc + x);
						putPixel(xc + yi, yc - x);
						putPixel(xc - yi, yc - x);
						putPixel(xc - yi, yc + x);
					}
				}

				if (d < 0)d = d + 4 * x + 6;
				else {
					d = d + 4 * (x - y) + 10;
					y--;
				}
				x++;
			}
		}
		if (mode == EMPTY_FILL) {
			while (x <= y) {
				if (x == 0) {
					putPixel(xc + x, yc + y);
					putPixel(xc + x, yc - y);
					putPixel(xc + y, yc + x);
					putPixel(xc - y, yc + x);
				}
				else {
					putPixel(xc + x, yc + y);
					putPixel(xc + x, yc - y);
					putPixel(xc - x, yc - y);
					putPixel(xc - x, yc + y);
					putPixel(xc + y, yc + x);
					putPixel(xc + y, yc - x);
					putPixel(xc - y, yc - x);
					putPixel(xc - y, yc + x);
				}

				if (d < 0)d = d + 4 * x + 6;
				else {
					d = d + 4 * (x - y) + 10;
					y--;
				}
				x++;
			}
		}
	}
	else {
		x = 0;
		y = r;
		d = 3 - 2 * r;

		if (mode == SOLID_FILL) {
			putPixel(xc, yc);
			while (x <= y) {
				if (x == 0) {
					for (yi = x + 1; yi <= y; yi++) {
						putPixel(xc + x, yc + yi);
						putPixel(xc + x, yc - yi);
						putPixel(xc + yi, yc + x);
						putPixel(xc - yi, yc + x);
					}
				}
				else {
					putPixel(xc + x, yc + x);
					putPixel(xc + x, yc - x);
					putPixel(xc - x, yc - x);
					putPixel(xc - x, yc + x);
					for (yi = x + 1; yi <= y; yi++) {
						putPixel(xc + x, yc + yi);
						putPixel(xc + x, yc - yi);
						putPixel(xc - x, yc - yi);
						putPixel(xc - x, yc + yi);
						putPixel(xc + yi, yc + x);
						putPixel(xc + yi, yc - x);
						putPixel(xc - yi, yc - x);
						putPixel(xc - yi, yc + x);
					}
				}

				if (d < 0)d = d + 4 * x + 6;
				else {
					d = d + 4 * (x - y) + 10;
					y--;
				}
				x++;
			}
		}
		if (mode == EMPTY_FILL) {
			while (x <= y) {
				if (x == 0) {
					putPixel(xc + x, yc + y);
					putPixel(xc + x, yc - y);
					putPixel(xc + y, yc + x);
					putPixel(xc - y, yc + x);
				}
				else {
					putPixel(xc + x, yc + y);
					putPixel(xc + x, yc - y);
					putPixel(xc - x, yc - y);
					putPixel(xc - x, yc + y);
					putPixel(xc + y, yc + x);
					putPixel(xc + y, yc - x);
					putPixel(xc - y, yc - x);
					putPixel(xc - y, yc + x);
				}

				if (d < 0)d = d + 4 * x + 6;
				else {
					d = d + 4 * (x - y) + 10;
					y--;
				}
				x++;
			}
		}
	}
}
void putEllipse(int xc, int yc, int a, int b, int mode) {
	int sqa, sqb, x, y, d, P_x, xi, jmp;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

	if (currentWindow == -1) {
		if (checkThread())return;

		sqa = a * a;
		sqb = b * b;
		x = 0;
		y = b;
		d = 2 * sqb - 2 * b * sqa + sqa;
		P_x = (int)((double)sqa / sqrt((double)(sqa + sqb)));

		if (mode == SOLID_FILL) {
			putPixel(xc, yc);
			for (xi = 1; xi < b; xi++) {
				putPixel(xc, yc + xi);
				putPixel(xc, yc - xi);
			}
			for (xi = 1; xi < a; xi++) {
				putPixel(xc + xi, yc);
				putPixel(xc - xi, yc);
			}
			while (x <= P_x) {
				if (d < 0) {
					d += 2 * sqb * (2 * x + 3);
					x++;
					continue;
				}
				else {
					d += 2 * sqb * (2 * x + 3) - 4 * sqa * (y - 1);
					y--;
				}
				x++;

				for (xi = 1; xi <= x; xi++) {
					putPixel(xc + xi, yc + y);
					putPixel(xc - xi, yc + y);
					putPixel(xc + xi, yc - y);
					putPixel(xc - xi, yc - y);
				}
			}
			d = sqb * (x * x + x) + sqa * (y * y - y) - sqa * sqb;
			jmp = y;
			while (y > 0) {
				if (jmp != y) {
					for (xi = 1; xi <= x; xi++) {
						putPixel(xc + xi, yc + y);
						putPixel(xc - xi, yc + y);
						putPixel(xc + xi, yc - y);
						putPixel(xc - xi, yc - y);
					}
				}
				y--;

				if (d < 0) {
					x++;
					d = d - 2 * sqa * y - sqa + 2 * sqb * x + 2 * sqb;
				}
				else d = d - 2 * sqa * y - sqa;
			}
		}
		if (mode == EMPTY_FILL) {
			putPixel(xc + x, yc + y);
			putPixel(xc - x, yc - y);
			while (x <= P_x) {
				if (d < 0) d += 2 * sqb * (2 * x + 3);
				else {
					d += 2 * sqb * (2 * x + 3) - 4 * sqa * (y - 1);
					y--;
				}
				x++;
				putPixel(xc + x, yc + y);
				putPixel(xc - x, yc + y);
				putPixel(xc + x, yc - y);
				putPixel(xc - x, yc - y);
			}
			d = sqb * (x * x + x) + sqa * (y * y - y) - sqa * sqb;
			jmp = y;
			while (y > 0) {
				if (jmp != y) {
					putPixel(xc + x, yc + y);
					putPixel(xc - x, yc + y);
					putPixel(xc + x, yc - y);
					putPixel(xc - x, yc - y);
				}
				y--;
				if (d < 0) {
					x++;
					d = d - 2 * sqa * y - sqa + 2 * sqb * x + 2 * sqb;
				}
				else d = d - 2 * sqa * y - sqa;
			}
			putPixel(xc + x, yc + y);
			putPixel(xc - x, yc - y);
		}
	}
	else {
		sqa = a * a;
		sqb = b * b;
		x = 0;
		y = b;
		d = 2 * sqb - 2 * b * sqa + sqa;
		P_x = (int)((double)sqa / sqrt((double)(sqa + sqb)));

		if (mode == SOLID_FILL) {
			putPixel(xc, yc);
			for (xi = 1; xi < b; xi++) {
				putPixel(xc, yc + xi);
				putPixel(xc, yc - xi);
			}
			for (xi = 1; xi < a; xi++) {
				putPixel(xc + xi, yc);
				putPixel(xc - xi, yc);
			}
			while (x <= P_x) {
				if (d < 0) {
					d += 2 * sqb * (2 * x + 3);
					x++;
					continue;
				}
				else {
					d += 2 * sqb * (2 * x + 3) - 4 * sqa * (y - 1);
					y--;
				}
				x++;

				for (xi = 1; xi <= x; xi++) {
					putPixel(xc + xi, yc + y);
					putPixel(xc - xi, yc + y);
					putPixel(xc + xi, yc - y);
					putPixel(xc - xi, yc - y);
				}
			}
			d = sqb * (x * x + x) + sqa * (y * y - y) - sqa * sqb;
			jmp = y;
			while (y > 0) {
				if (jmp != y) {
					for (xi = 1; xi <= x; xi++) {
						putPixel(xc + xi, yc + y);
						putPixel(xc - xi, yc + y);
						putPixel(xc + xi, yc - y);
						putPixel(xc - xi, yc - y);
					}
				}
				y--;

				if (d < 0) {
					x++;
					d = d - 2 * sqa * y - sqa + 2 * sqb * x + 2 * sqb;
				}
				else d = d - 2 * sqa * y - sqa;
			}
		}
		if (mode == EMPTY_FILL) {
			putPixel(xc + x, yc + y);
			putPixel(xc - x, yc - y);
			while (x <= P_x) {
				if (d < 0) d += 2 * sqb * (2 * x + 3);
				else {
					d += 2 * sqb * (2 * x + 3) - 4 * sqa * (y - 1);
					y--;
				}
				x++;
				putPixel(xc + x, yc + y);
				putPixel(xc - x, yc + y);
				putPixel(xc + x, yc - y);
				putPixel(xc - x, yc - y);
			}
			d = sqb * (x * x + x) + sqa * (y * y - y) - sqa * sqb;
			jmp = y;
			while (y > 0) {
				if (jmp != y) {
					putPixel(xc + x, yc + y);
					putPixel(xc - x, yc + y);
					putPixel(xc + x, yc - y);
					putPixel(xc - x, yc - y);
				}
				y--;
				if (d < 0) {
					x++;
					d = d - 2 * sqa * y - sqa + 2 * sqb * x + 2 * sqb;
				}
				else d = d - 2 * sqa * y - sqa;
			}
			putPixel(xc + x, yc + y);
			putPixel(xc - x, yc - y);
		}
	}
}
void putBitmap(int x, int y, bitMap bmp) {
	int i, width, height, lines;
	bitMap *buf;
	byte *vp;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

	if (currentWindow == -1) {
		if (checkThread())return;

		if (_activePage == 0) buf = _Screen->buffer1;
		else buf = _Screen->buffer2;
		vp = buf->data + (y*buf->sizeX + x) * 3;

		width = bmp.sizeX;
		height = bmp.sizeY;
		for (i = height - 1; i >= 0; i--) {
			lines = i * buf->sizeX * 3;
			if (x + width > buf->sizeX)
				memcpy(vp + lines, bmp.data + i * width * 3, (buf->sizeX - x) * 3);
			else memcpy(vp + lines, bmp.data + i * width * 3, width * 3);
		}
	}
	else{
		if (_wndList[currentWindow].activePage == 0)
			buf = _wndList[currentWindow].buffer1;
		else buf = _wndList[currentWindow].buffer2;
		vp = buf->data + (y*buf->sizeX + x) * 3;

		width = bmp.sizeX;
		height = bmp.sizeY;
		for (i = height - 1; i >= 0; i--) {
			lines = i * buf->sizeX * 3;
			if (x + width > buf->sizeX)
				memcpy(vp + lines, bmp.data + i * width * 3, (buf->sizeX - x) * 3);
			else memcpy(vp + lines, bmp.data + i * width * 3, width * 3);
		}
	}
}
int drawBmp(int x, int y, const char *filename) {
	FILE *fp = NULL;
	SGstring p = NULL, tmp = NULL;
	int width, height, i;
	dword dataOffset, lineBytes;
	dword lines;
	bitMap *buf;
	byte *vp;
	char buffer = 0;

	if (_sglMode != BIT_MAP && !_innerFunc)return SG_INVALID_MODE;

	if (currentWindow == -1) {
		if (checkThread())return SG_WRONG_THREAD;

		if (_activePage == 0) buf = _Screen->buffer1;
		else buf = _Screen->buffer2;
		vp = buf->data + (y*buf->sizeX + x) * 3;

		if (x < 0 || y < 0)goto displayError;
		p = (SGstring)malloc(2048 * 3 * sizeof(char));
		if (p == NULL)goto displayError;
		fp = fopen(filename, "rb");
		if (fp == NULL)goto displayError;

		fread(p, 1, 0x36, fp);
		if (*(word *)p != 0x4D42)goto displayError;
		if (*(word *)(p + 0x1C) != 24)goto displayError;

		width = *(dword *)(p + 0x12);
		height = *(dword *)(p + 0x16);
		dataOffset = *(dword *)(p + 0x0A);
		lineBytes = (width * 3 + 3) / 4 * 4;

		if (y + height > buf->sizeY) {
			fseek(fp, dataOffset + (y + height - buf->sizeY)*lineBytes, SEEK_SET);
			height = buf->sizeY - y;
		}
		else fseek(fp, dataOffset, SEEK_SET);

		for (i = height - 1; i >= 0; i--) {
			fread(p, 1, lineBytes, fp);
			lines = i * buf->sizeX * 3;
			tmp = p;
			if (x + width > buf->sizeX)
				memcpy(vp + lines, p, (buf->sizeX - x) * 3);
			else memcpy(vp + lines, p, width * 3);
		}
	}
	else {
		if (_wndList[currentWindow].activePage == 0)
			buf = _wndList[currentWindow].buffer1;
		else buf = _wndList[currentWindow].buffer2;
		vp = buf->data + (y*buf->sizeX + x) * 3;

		if (x < 0 || y < 0)goto displayError;
		p = (SGstring)malloc(2048 * 3 * sizeof(char));
		if (p == NULL)goto displayError;
		fp = fopen(filename, "rb");
		if (fp == NULL)goto displayError;

		fread(p, 1, 0x36, fp);
		if (*(word *)p != 0x4D42)goto displayError;
		if (*(word *)(p + 0x1C) != 24)goto displayError;

		width = *(dword *)(p + 0x12);
		height = *(dword *)(p + 0x16);
		dataOffset = *(dword *)(p + 0x0A);
		lineBytes = (width * 3 + 3) / 4 * 4;

		if (y + height > buf->sizeY) {
			fseek(fp, dataOffset + (y + height - buf->sizeY)*lineBytes, SEEK_SET);
			height = buf->sizeY - y;
		}
		else fseek(fp, dataOffset, SEEK_SET);

		for (i = height - 1; i >= 0; i--) {
			fread(p, 1, lineBytes, fp);
			lines = i * buf->sizeX * 3;
			tmp = p;
			if (x + width > buf->sizeX)
				memcpy(vp + lines, p, (buf->sizeX - x) * 3);
			else memcpy(vp + lines, p, width * 3);
		}
	}

	free(p);
	fclose(fp);
	return SG_NO_ERORR;

displayError:
	if (p != NULL)free(p);
	if (fp != NULL)fclose(fp);
	return SG_IO_ERROR;
}
bitMap loadBmp(const char *filename) {
	FILE *fp = NULL;
	SGstring p = NULL;
	int width, height, i;
	dword dataOffset, lineBytes;
	dword lines;
	bitMap ret;

	p = (SGstring)malloc(2048 * 3 * sizeof(char));
	if (p == NULL)goto displayError;
	fp = fopen(filename, "rb");
	if (fp == NULL)goto displayError;

	fread(p, 1, 0x36, fp);
	if (*(word *)p != 0x4D42)goto displayError;
	if (*(word *)(p + 0x1C) != 24)goto displayError;

	width = *(dword *)(p + 0x12);
	height = *(dword *)(p + 0x16);
	dataOffset = *(dword *)(p + 0x0A);
	lineBytes = (width * 3 + 3) / 4 * 4;

	ret.data = malloc(width * height * 3 * sizeof(char));
	if (ret.data == NULL)goto displayError;
	fseek(fp, dataOffset, SEEK_SET);

	ret.sizeX = width;
	ret.sizeY = height;
	for (i = height - 1; i >= 0; i--) {
		fread(p, 1, lineBytes, fp);
		lines = i * width * 3;
		memcpy(ret.data + lines, p, width * 3);
	}

	free(p);
	fclose(fp);
	return ret;

displayError:
	if (p != NULL)free(p);
	if (fp != NULL)fclose(fp);
	ret.data = NULL;
	ret.sizeX = ret.sizeY = SG_IO_ERROR;
	return ret;
}
void putChar(char ch, int x, int y) {
	int j, k;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

	if (currentWindow == -1) {
		if (checkThread())return;

		for (j = 0; j < 8; j++)
			for (k = 7; k >= 0; k--)
				if ((letters[ch][j] >> k) & 1) {
					putPixel(x + 7 - k, y + 2 * j);
					putPixel(x + 7 - k, y + 2 * j + 1);
				}
	}
	else {
		for (j = 0; j < 8; j++)
			for (k = 7; k >= 0; k--)
				if ((letters[ch][j] >> k) & 1) {
					putPixel(x + 7 - k, y + 2 * j);
					putPixel(x + 7 - k, y + 2 * j + 1);
				}
	}
}
void putString(const char *str, int x, int y) {
	unsigned int tab = 0, i, j;
	char *tmp;

	if (_sglMode != BIT_MAP && !_innerFunc)return;
	if (str == NULL)return;
	if (strlen(str) == 0)return;

	for (i = 0; i < strlen(str); i++) {
		if (str[i] == '\t')tab++;
	}
	tmp = (char *)malloc(strlen(str) + 3 * tab + 1);
	memset(tmp, 0, strlen(str) + 3 * tab + 1);
	for (i = 0, j = 0; i < strlen(str); i++) {
		if (str[i] == '\t') {
			tmp[j] = tmp[j + 1] = tmp[j + 2] = tmp[j + 3] = ' ';
			j += 4;
		}
		else tmp[j++] = str[i];
	}

	if (currentWindow == -1) {
		if (checkThread())return;

		SGWINSTR _wd = NULL;
		GetTextExtentPoint32(text.memDC, _wd = _widen(tmp), _strlenW(tmp), &text.strRect);
		free((void *)_wd);
		memset(text.bitBuf, 0, text.bufSize);
		RECT imgRect = { 0, 0, text.strRect.cx, text.strRect.cy };
		FillRect(text.memDC, &imgRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

		TextOut(text.memDC, 0, 0, _wd = _widen(tmp), _strlenW(tmp));
		free((void *)_wd);
		_hbmImage(text.memDC, text.hbm, x, y, tmp);
		free(tmp);
	}
	else {
		SGWINSTR _wd = NULL;
		GetTextExtentPoint32(_wndList[currentWindow].text.memDC, _wd = _widen(tmp),
			_strlenW(tmp), &_wndList[currentWindow].text.strRect);
		free((void *)_wd);
		memset(_wndList[currentWindow].text.bitBuf, 0,
			_wndList[currentWindow].text.bufSize);
		RECT imgRect = { 0, 0, _wndList[currentWindow].text.strRect.cx,
			_wndList[currentWindow].text.strRect.cy };
		FillRect(_wndList[currentWindow].text.memDC, &imgRect,
			(HBRUSH)GetStockObject(BLACK_BRUSH));

		TextOut(_wndList[currentWindow].text.memDC, 0, 0,
			_wd = _widen(tmp), _strlenW(tmp));
		free((void *)_wd);
		_hbmSubImage(currentWindow, _wndList[currentWindow].text.memDC,
			_wndList[currentWindow].text.hbm, x, y, tmp);
		free(tmp);
	}
}
void putNumber(int n, int x, int y, char lr) {
	int s[20], sn = 0;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

	if (currentWindow == -1) {
		if (checkThread())return;

		if (lr == 'l') {
			if (n == 0)putChar('0', x, y);
			while (n > 0) {
				s[sn++] = n % 10;
				n /= 10;
			}

			while (sn > 0) {
				putChar((char)(s[--sn] + '0'), x, y);
				x += 8;
			}
		}
		else if (lr == 'r') {
			if (n == 0)putChar('0', x - 8, y);
			while (n > 0) {
				x -= 8;
				putChar((char)(n % 10 + '0'), x, y);
				n /= 10;
			}
		}
	}
	else {
		if (lr == 'l') {
			if (n == 0)putChar('0', x, y);
			while (n > 0) {
				s[sn++] = n % 10;
				n /= 10;
			}

			while (sn > 0) {
				putChar((char)(s[--sn] + '0'), x, y);
				x += 8;
			}
		}
		else if (lr == 'r') {
			if (n == 0)putChar('0', x - 8, y);
			while (n > 0) {
				x -= 8;
				putChar((char)(n % 10 + '0'), x, y);
				n /= 10;
			}
		}
	}
}
int stringWidth(const char *str, int x) {
	SIZE ret;
	int tab = 0, i, j;
	char *tmp;

	if (_sglMode != BIT_MAP && !_innerFunc)return SG_INVALID_MODE;
	if (checkThread())return SG_WRONG_THREAD;

	if (x < 0)return 0;

	for (i = 0; i < x; i++) {
		if (str[i] == '\t')tab++;
	}
	tmp = (char *)malloc(x + 3 * tab + 1);
	memset(tmp, 0, x + 3 * tab + 1);
	for (i = 0, j = 0; i < x; i++) {
		if (str[i] == '\t') {
			tmp[j] = tmp[j + 1] = tmp[j + 2] = tmp[j + 3] = ' ';
			j += 4;
		}
		else tmp[j++] = str[i];
	}
	x += 3 * tab;

	SGWINSTR _wd = NULL;
	GetTextExtentPoint32(text.memDC, _wd = _widen(tmp), x, &ret);
	free((void *)_wd);
	free(tmp);
	return ret.cx;
}
void putStringFormat(const char *str, int x, int y, ...) {
	int cnt = 0;
	va_list ap;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

	if (currentWindow == -1) {
		if (checkThread())return;

		va_start(ap, y);
		cnt = _stringPrintf(str, ap, x, y);
		va_end(ap);
	}
	else {
		va_start(ap, y);
		cnt = _stringPrintf(str, ap, x, y);
		va_end(ap);
	}
}
int putStringConstraint(const char *str, int x, int y, int start, int constraint) {
	int len, i;

	if (_sglMode != BIT_MAP && !_innerFunc)return SG_INVALID_MODE;
	if (str == NULL)return SG_NULL_POINTER;

	if (currentWindow == -1) {
		if (checkThread())return SG_WRONG_THREAD;

		len = _strlenW(str) - start > constraint / 5 ? constraint / 5 : _strlenW(str) - start;
		if (len <= 0)return 0;

		SGWINSTR _wd = NULL;
		GetTextExtentPoint32(text.memDC, (_wd = _widen(str)) + start, len, &text.strRect);
		free((void *)_wd);
		while (text.strRect.cx > constraint) {
			len--;
			GetTextExtentPoint32(text.memDC, (_wd = _widen(str)) + start, len, &text.strRect);
			free((void *)_wd);
		}
		len = _wcharAt(str, len);
		if (len <= 1)len = 1;
		i = 1;
		while (str[i++] != '\n')
			if (i >= len)break;
		len = i;

		memset(text.bitBuf, 0, text.bufSize);
		RECT imgRect = { 0, 0, text.strRect.cx, text.strRect.cy };
		FillRect(text.memDC, &imgRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

		TextOut(text.memDC, 0, 0, (_wd = _widen(str)) + start, _scharAt(str, len));
		free((void *)_wd);
		_hbmImage(text.memDC, text.hbm, x, y, str);
	}
	else {
		len = _strlenW(str) - start > constraint / 5 ? constraint / 5 : _strlenW(str) - start;
		if (len <= 0)return 0;

		SGWINSTR _wd = NULL;
		GetTextExtentPoint32(_wndList[currentWindow].text.memDC,
			(_wd = _widen(str)) + start, len, &_wndList[currentWindow].text.strRect);
		free((void *)_wd);
		while (_wndList[currentWindow].text.strRect.cx > constraint) {
			len--;
			GetTextExtentPoint32(_wndList[currentWindow].text.memDC,
				(_wd = _widen(str)) + start, len, &_wndList[currentWindow].text.strRect);
			free((void *)_wd);
		}
		len = _wcharAt(str, len);
		if (len <= 1)len = 1;
		i = 1;
		while (str[i++] != '\n')
			if (i >= len)break;
		len = i;

		memset(_wndList[currentWindow].text.bitBuf, 0,
			_wndList[currentWindow].text.bufSize);
		RECT imgRect = { 0, 0, _wndList[currentWindow].text.strRect.cx,
			_wndList[currentWindow].text.strRect.cy };
		FillRect(_wndList[currentWindow].text.memDC, &imgRect,
			(HBRUSH)GetStockObject(BLACK_BRUSH));

		TextOut(_wndList[currentWindow].text.memDC, 0, 0,
			(_wd = _widen(str)) + start, _scharAt(str, len));
		free((void *)_wd);
		_hbmSubImage(currentWindow, _wndList[currentWindow].text.memDC,
			_wndList[currentWindow].text.hbm, x, y, str);
	}

	return len;
}
int getImage(int left, int top, int right, int bottom, bitMap *bitmap) {
	int deltaX, deltaY, i;
	bitMap *buf;

	if (_sglMode != BIT_MAP && !_innerFunc)return SG_INVALID_MODE;

	if (currentWindow == -1) {
		if (checkThread())return SG_WRONG_THREAD;

		if (_activePage == 0) buf = _Screen->buffer1;
		else buf = _Screen->buffer2;

		if (right < left)left ^= right ^= left ^= right;
		if (bottom < top)top ^= bottom ^= top ^= bottom;

		if (right >= buf->sizeX)right = buf->sizeX - 1;
		if (left < 0)left = 0;
		if (bottom >= buf->sizeY)bottom = buf->sizeY - 1;
		if (top < 0)top = 0;

		deltaX = right - left + 1;
		deltaY = bottom - top + 1;

		bitmap->sizeX = deltaX;
		bitmap->sizeY = deltaY;
		bitmap->data = (byte*)malloc(deltaX * deltaY * 3 * sizeof(byte));
		if (bitmap->data == NULL)return SG_NO_LOAD_MEM;

		for (i = 0; i < deltaY; i++) {
			memcpy(bitmap->data + bitmap->sizeX * i * 3, buf->data + (buf->sizeX * (i + top) + left) * 3, deltaX * 3 * sizeof(char));
		}
	}
	else {
		if (_wndList[currentWindow].activePage == 0)
			buf = _wndList[currentWindow].buffer1;
		else buf = _wndList[currentWindow].buffer2;

		if (right <left)left ^= right ^= left ^= right;
		if (bottom < top)top ^= bottom ^= top ^= bottom;

		if (right >= buf->sizeX)right = buf->sizeX - 1;
		if (left < 0)left = 0;
		if (bottom >= buf->sizeY)bottom = buf->sizeY - 1;
		if (top < 0)top = 0;

		deltaX = right - left + 1;
		deltaY = bottom - top + 1;

		bitmap->sizeX = deltaX;
		bitmap->sizeY = deltaY;
		bitmap->data = (byte*)malloc(deltaX * deltaY * 3 * sizeof(byte));
		if (bitmap->data == NULL)return SG_NO_LOAD_MEM;

		for (i = 0; i < deltaY; i++) {
			memcpy(bitmap->data + bitmap->sizeX * i * 3, buf->data + (buf->sizeX * (i + top) + left) * 3, deltaX * 3 * sizeof(char));
		}
	}

	return SG_NO_ERORR;
}
void putImage(int left, int top, bitMap *bitmap, int op) {
	int x1, x2, y1, y2, i, j;
	bitMap *buf;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

	if (currentWindow == -1) {
		if (checkThread())return;

		if (_activePage == 0) buf = _Screen->buffer1;
		else buf = _Screen->buffer2;

		if (left >= buf->sizeX || top >= buf->sizeY)return;
		if (left + bitmap->sizeX <= 0 || top + bitmap->sizeY <= 0)return;

		if (left < 0)x1 = 0;
		else x1 = left;
		if (top < 0)y1 = 0;
		else y1 = top;

		if (left + bitmap->sizeX >= buf->sizeX)x2 = buf->sizeX - 1;
		else x2 = left + bitmap->sizeX - 1;
		if (top + bitmap->sizeY >= buf->sizeY)y2 = buf->sizeY - 1;
		else y2 = top + bitmap->sizeY - 1;

		switch (op) {
		case COPY_PUT:
			for (i = 0; i < y2 - y1 + 1; i++) {
				memcpy(buf->data + (buf->sizeX * (i + y1) + x1) * 3,
					bitmap->data + (bitmap->sizeX * (i + y1 - top) + x1 - left) * 3,
					(x2 - x1 + 1) * 3 * sizeof(char));
			}
			break;

		case AND_PUT:
			for (i = 0; i < y2 - y1 + 1; i++) {
				for (j = 0; j < x2 - x1 + 1; j++) {
					buf->data[(buf->sizeX * (i + y1) + x1 + j) * 3] &=
						bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3];
					buf->data[(buf->sizeX * (i + y1) + x1 + j) * 3 + 1] &=
						bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3 + 1];
					buf->data[(buf->sizeX * (i + y1) + x1 + j) * 3 + 2] &=
						bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3 + 2];
				}
			}
			break;

		case OR_PUT:
			for (i = 0; i < y2 - y1 + 1; i++) {
				for (j = 0; j < x2 - x1 + 1; j++) {
					buf->data[(buf->sizeX * (i + y1) + x1 + j) * 3] |=
						bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3];
					buf->data[(buf->sizeX * (i + y1) + x1 + j) * 3 + 1] |=
						bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3 + 1];
					buf->data[(buf->sizeX * (i + y1) + x1 + j) * 3 + 2] |=
						bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3 + 2];
				}
			}
			break;

		case XOR_PUT:
			for (i = 0; i < y2 - y1 + 1; i++) {
				for (j = 0; j < x2 - x1 + 1; j++) {
					buf->data[(buf->sizeX * (i + y1) + x1 + j) * 3] ^=
						bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3];
					buf->data[(buf->sizeX * (i + y1) + x1 + j) * 3 + 1] ^=
						bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3 + 1];
					buf->data[(buf->sizeX * (i + y1) + x1 + j) * 3 + 2] ^=
						bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3 + 2];
				}
			}
			break;

		case NOT_PUT:
			for (i = 0; i < y2 - y1 + 1; i++) {
				for (j = 0; j < x2 - x1 + 1; j++) {
					buf->data[(buf->sizeX * (i + y1) + x1 + j) * 3] =
						~bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3];
					buf->data[(buf->sizeX * (i + y1) + x1 + j) * 3 + 1] =
						~bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3 + 1];
					buf->data[(buf->sizeX * (i + y1) + x1 + j) * 3 + 2] =
						~bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3 + 2];
				}
			}
			break;
		}
	}
	else {
		if (_wndList[currentWindow].activePage == 0)
			buf = _wndList[currentWindow].buffer1;
		else buf = _wndList[currentWindow].buffer2;

		if (left >= buf->sizeX || top >= buf->sizeY)return;
		if (left + bitmap->sizeX <= 0 || top + bitmap->sizeY <= 0)return;

		if (left < 0)x1 = 0;
		else x1 = left;
		if (top < 0)y1 = 0;
		else y1 = top;

		if (left + bitmap->sizeX >= buf->sizeX)x2 = buf->sizeX - 1;
		else x2 = left + bitmap->sizeX - 1;
		if (top + bitmap->sizeY >= buf->sizeY)y2 = buf->sizeY - 1;
		else y2 = top + bitmap->sizeY - 1;

		switch (op) {
		case COPY_PUT:
			for (i = 0; i < y2 - y1 + 1; i++) {
				memcpy(buf->data + (buf->sizeX * (i + top) + left) * 3, bitmap->data + bitmap->sizeX * i * 3, (x2 - x1 + 1) * 3 * sizeof(char));
			}
			break;

		case AND_PUT:
			for (i = 0; i < y2 - y1 + 1; i++) {
				for (j = 0; j < x2 - x1 + 1; j++) {
					buf->data[(buf->sizeX * (i + top) + left + j) * 3] &= bitmap->data[(bitmap->sizeX*i + j) * 3];
					buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 1] &= bitmap->data[(bitmap->sizeX*i + j) * 3 + 1];
					buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 2] &= bitmap->data[(bitmap->sizeX*i + j) * 3 + 2];
				}
			}
			break;

		case OR_PUT:
			for (i = 0; i < y2 - y1 + 1; i++) {
				for (j = 0; j < x2 - x1 + 1; j++) {
					buf->data[(buf->sizeX * (i + top) + left + j) * 3] |= bitmap->data[(bitmap->sizeX*i + j) * 3];
					buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 1] |= bitmap->data[(bitmap->sizeX*i + j) * 3 + 1];
					buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 2] |= bitmap->data[(bitmap->sizeX*i + j) * 3 + 2];
				}
			}
			break;

		case XOR_PUT:
			for (i = 0; i < y2 - y1 + 1; i++) {
				for (j = 0; j < x2 - x1 + 1; j++) {
					buf->data[(buf->sizeX * (i + top) + left + j) * 3] ^= bitmap->data[(bitmap->sizeX*i + j) * 3];
					buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 1] ^= bitmap->data[(bitmap->sizeX*i + j) * 3 + 1];
					buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 2] ^= bitmap->data[(bitmap->sizeX*i + j) * 3 + 2];
				}
			}
			break;

		case NOT_PUT:
			for (i = 0; i < y2 - y1 + 1; i++) {
				for (j = 0; j < x2 - x1 + 1; j++) {
					buf->data[(buf->sizeX * (i + top) + left + j) * 3] = ~bitmap->data[(bitmap->sizeX*i + j) * 3];
					buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 1] = ~bitmap->data[(bitmap->sizeX*i + j) * 3 + 1];
					buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 2] = ~bitmap->data[(bitmap->sizeX*i + j) * 3 + 2];
				}
			}
			break;
		}
	}
}
void funcMap(int x1, int x2, int y1, int y2, float(*vect)(float x)) {
	int tmp = 0, pre, i, j;
	int addition;
	float y;

	if (_sglMode != BIT_MAP && !_innerFunc)return;
	if (checkThread())return;

#define XCHG(a, b) {tmp = a; a = b; b = tmp;}

	if (currentWindow == -1) {
		if (x1 > x2)XCHG(x1, x2);
		if (y1 > y2)XCHG(y1, y2);

		pre = (int)vect((float)x1);
		for (i = x1; i <= x2; i++) {
			y = vect((float)i);
			if (y < y1) {
				pre = y1;
				continue;
			}
			if (y > y2) {
				pre = y2;
				continue;
			}
			if (y - (int)y > 0.5) {
				putPixel(i, (int)(y + 1));
				addition = 1;
			}
			else {
				putPixel(i, (int)y);
				addition = 0;
			}
			if (y > pre) {
				if ((addition && y - pre > 1) || (!addition && y - pre > 2)) {
					for (j = pre + 1; j < (y + pre) / 2; j++) {
						putPixel(i - 1, j);
					}
					for (; j < y; j++) {
						putPixel(i, j);
					}
				}
				pre = addition ? (int)(y + 1) : (int)y;
			}
			else {
				if ((addition && pre - y > 2) || (!addition && pre - y > 1)) {
					for (j = pre - 1; j > (y + pre) / 2; j--) {
						putPixel(i - 1, j);
					}
					for (; j >= y; j--) {
						putPixel(i, j);
					}
				}
				pre = addition ? (int)(y + 1) : (int)y;
			}
		}
	}
	else {
		if (x1 > x2)XCHG(x1, x2);
		if (y1 > y2)XCHG(y1, y2);

		pre = (int)vect((float)x1);
		for (i = x1; i <= x2; i++) {
			y = vect((float)i);
			if (y < y1) {
				pre = y1;
				continue;
			}
			if (y > y2) {
				pre = y2;
				continue;
			}
			if (y - (int)y > 0.5) {
				putPixel(i, (int)(y + 1));
				addition = 1;
			}
			else {
				putPixel(i, (int)y);
				addition = 0;
			}
			if (y > pre) {
				if ((addition && y - pre > 1) || (!addition && y - pre > 2)) {
					for (j = pre + 1; j < (y + pre) / 2; j++) {
						putPixel(i - 1, j);
					}
					for (; j < y; j++) {
						putPixel(i, j);
					}
				}
				pre = addition ? (int)(y + 1) : (int)y;
			}
			else {
				if ((addition && pre - y > 2) || (!addition && pre - y > 1)) {
					for (j = pre - 1; j > (y + pre) / 2; j--) {
						putPixel(i - 1, j);
					}
					for (; j >= y; j--) {
						putPixel(i, j);
					}
				}
				pre = addition ? (int)(y + 1) : (int)y;
			}
		}
	}

#undef XCHG

}
void floodFill(int x, int y, RGB c) {
	vec2 Q[SG_QSIZE];
	vec2 tmp;
	int front = -1, rear = 0, p;
	bitMap *buf;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

#define ENQUE(x) Q[(++front)%SG_QSIZE] = x
#define DEQUE() Q[(rear++)%SG_QSIZE]
#define ISEMPTY() ((front+1)%SG_QSIZE==(rear%SG_QSIZE))

	if (currentWindow == -1) {
		if (checkThread())return;

		if (_activePage == 0) buf = _Screen->buffer1;
		else buf = _Screen->buffer2;

		tmp.x = x;
		tmp.y = y;
		ENQUE(tmp);

		while (!ISEMPTY()) {
			tmp = DEQUE();
			p = (tmp.y*buf->sizeX + tmp.x) * 3;
			if ((buf->data[p] == c.b&&buf->data[p + 1] == c.g&&buf->data[p + 2] == c.r) || (buf->data[p] == _Screen->rgb[2] && buf->data[p + 1] == _Screen->rgb[1] && buf->data[p + 2] == _Screen->rgb[0]))continue;
			if (tmp.x < 0 || tmp.x >= buf->sizeX)continue;
			if (tmp.y < 0 || tmp.y >= buf->sizeY)continue;
			putPixel(tmp.x, tmp.y);
			tmp.x--;
			ENQUE(tmp);
			tmp.x += 2;
			ENQUE(tmp);
			tmp.x--;
			tmp.y++;
			ENQUE(tmp);
			tmp.y -= 2;
			ENQUE(tmp);
		}
	}
	else {
		if (_wndList[currentWindow].activePage == 0)
			buf = _wndList[currentWindow].buffer1;
		else buf = _wndList[currentWindow].buffer2;

		tmp.x = x;
		tmp.y = y;
		ENQUE(tmp);

		while (!ISEMPTY()) {
			tmp = DEQUE();
			p = (tmp.y*buf->sizeX + tmp.x) * 3;
			if ((buf->data[p] == c.b&&buf->data[p + 1] == c.g&&buf->data[p + 2] == c.r) ||
				(buf->data[p] == _wndList[currentWindow].rgb[2] &&
					buf->data[p + 1] == _wndList[currentWindow].rgb[1] &&
					buf->data[p + 2] == _wndList[currentWindow].rgb[0]))continue;
			if (tmp.x < 0 || tmp.x >= buf->sizeX)continue;
			if (tmp.y < 0 || tmp.y >= buf->sizeY)continue;
			putPixel(tmp.x, tmp.y);
			tmp.x--;
			ENQUE(tmp);
			tmp.x += 2;
			ENQUE(tmp);
			tmp.x--;
			tmp.y++;
			ENQUE(tmp);
			tmp.y -= 2;
			ENQUE(tmp);
		}
	}

#undef ENQUE
#undef DEQUE
#undef ISEMPTY

}
int maskImage(int left, int top, bitMap *mask, bitMap *bitmap) {
	int x1, x2, y1, y2, i, j;
	int w1, w2, w3;
	bitMap *buf;

	if (_sglMode != BIT_MAP && !_innerFunc)return SG_INVALID_MODE;
	if (checkThread())return SG_WRONG_THREAD;

	if (_activePage == 0) buf = _Screen->buffer1;
	else buf = _Screen->buffer2;

	if (mask->sizeX != bitmap->sizeX || mask->sizeY != bitmap->sizeY)return SG_SIZE_MISMATCH;
	if (left >= buf->sizeX || top >= buf->sizeY)return SG_OUT_OF_RANGE;
	if (left + bitmap->sizeX <= 0 || top + bitmap->sizeY <= 0)return SG_OUT_OF_RANGE;

	if (left < 0)x1 = 0;
	else x1 = left;
	if (top < 0)y1 = 0;
	else y1 = top;

	if (left + bitmap->sizeX > buf->sizeX)x2 = buf->sizeX - 1;
	else x2 = left + bitmap->sizeX - 1;
	if (top + bitmap->sizeY > buf->sizeY)y2 = buf->sizeY - 1;
	else y2 = top + bitmap->sizeY - 1;

	for (i = 0; i < y2 - y1 + 1; i++) {
		for (j = 0; j < x2 - x1 + 1; j++) {
			w1 = mask->data[(mask->sizeX*i + j) * 3];
			buf->data[(buf->sizeX * (i + top) + left + j) * 3] =
				buf->data[(buf->sizeX * (i + top) + left + j) * 3] * (255 - w1) / 255 +
				bitmap->data[(bitmap->sizeX*i + j) * 3] * w1 / 255;
			w2 = mask->data[(mask->sizeX*i + j) * 3 + 1];
			buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 1] =
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 1] * (255 - w2) / 255 +
				bitmap->data[(bitmap->sizeX*i + j) * 3 + 1] * w2 / 255;
			w3 = mask->data[(mask->sizeX*i + j) * 3 + 2];
			buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 2] =
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 2] * (255 - w3) / 255 +
				bitmap->data[(bitmap->sizeX*i + j) * 3 + 2] * w3 / 255;
		}
	}

	return SG_NO_ERORR;
}
void initOpenGL() {
	_enOpenGL = 1;
}


/*
* Text interfaces.
* Started in v1.0.0
*/
void setBfc(int bgc, int fgc) {
	_Screen->bfc = (bgc << 4) | fgc;
}
void setCharColor(char color, int x, int y) {
	int pos;

	pos = y * _Screen->changed->width + x;
	_Screen->changed->content[pos] &= 0x00FF;
	_Screen->changed->content[pos] |= (color << 8);
}
void setCharBgc(char color, int x, int y) {
	int pos;

	pos = y * _Screen->changed->width + x;
	_Screen->changed->content[pos] &= 0x0FFF;
	_Screen->changed->content[pos] |= ((color & 0x0F) << 12);
}
void setCharFgc(char color, int x, int y) {
	int pos;

	pos = y * _Screen->changed->width + x;
	_Screen->changed->content[pos] &= 0xF0FF;
	_Screen->changed->content[pos] |= ((color & 0x0F) << 8);
}
void clearText() {
	int i;

	for (i = 0; i < _Screen->changed->width*_Screen->changed->height; i++) {
		writeChar(0, i%_Screen->changed->width, i / _Screen->changed->width);
	}
}
void writeChar(char c, int x, int y) {
	int pos;

	pos = y * _Screen->changed->width + x;
	_Screen->changed->content[pos] = ((short)_Screen->bfc << 8) | (short)c;
}
void writeString(const char *s, int x, int y) {
	int i;

	for (i = 0; s[i] != '\0'; i++) {
		writeChar(s[i], x++, y);
	}
}
int getShort(int x, int y) {
	return _Screen->changed->content[y * _Screen->changed->width + x];
}
int getText(int left, int top, int right, int bottom, textMap *text) {
	int i;

	if (right <left)left ^= right ^= left ^= right;
	if (bottom < top)top ^= bottom ^= top ^= bottom;

	if (right > _Screen->changed->width - 1)right = _Screen->changed->width - 1;
	if (left < 0)right = 0;
	if (bottom > _Screen->changed->height - 1)right = _Screen->changed->height - 1;
	if (top < 0)top = 0;

	text->width = right - left + 1;
	text->height = bottom - top + 1;
	text->content = (short*)malloc(text->width * text->height * sizeof(short));
	if (text->content == NULL)return SG_NO_LOAD_MEM;

	for (i = 0; i < text->height; i++) {
		memcpy(text->content + (i*text->width), _Screen->changed->content + (top + i) * _Screen->changed->width + left, text->width * sizeof(short));
	}

	return SG_NO_ERORR;
}
void putText(int left, int top, textMap *text) {
	int x1, x2, y1, y2, i;

	if (left >= _Screen->changed->width)return;
	if (top >= _Screen->changed->height)return;
	if (left + text->width < 0)return;
	if (top + text->height < 0)return;

	if (left < 0)x1 = 0;
	else x1 = left;
	if (top < 0)y1 = 0;
	else y1 = top;

	if (left + text->width >= _Screen->changed->width)x2 = _Screen->changed->width - 1;
	else x2 = left + text->width - 1;
	if (top + text->height >= _Screen->changed->height)y2 = _Screen->changed->height - 1;
	else y2 = top + text->height - 1;

	for (i = 0; i < text->height; i++) {
		memcpy(_Screen->changed->content + (top + i) * _Screen->changed->width + left, text->content + (i*text->width), text->width * sizeof(short));
	}
}
