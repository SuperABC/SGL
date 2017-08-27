#define _CRT_SECURE_NO_WARNINGS
#include "inner.h"
#include "..\include\winsgl.h"

struct _win *Window;
static struct _screen *Screen;
static struct _key *Key;
static struct _mouse *Mouse;
static struct _vect *Vector;

int _sglMode, _innerFunc;
int _delaySE = 0, _inDelay = 0;
clock_t _delayBegin, _delayEnd, _vectDelta = 50;
int _vectKey = 0;
int _enMouse = 0, _enKey = 0;
int _activePage = 0, _visualPage = 0;
int _inLoop = 0;

struct _text text;
font tf;

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

//Main functions.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	HWND hwnd;
	MSG msg;

	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = TEXT("WindowClass");
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	sgInit();

	hwnd = CreateWindow(wc.lpszClassName, _widen(Window->winName),
		WS_OVERLAPPEDWINDOW,
		Window->posLeft, Window->posUp,
		Window->winWidth + 16, Window->winHeight + GetSystemMetrics(SM_CYCAPTION) + 16,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	_inLoop = 1;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static int  cxClient, cyClient, cxSource, cySource;
	static HBITMAP hBitmap;
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message) {
	case WM_CREATE:
		cxSource = Window->winWidth;
		cySource = Window->winHeight;
		SetTimer(hwnd, INTERUPT_DELTA_HANDLE, _vectDelta, NULL);
		SetTimer(hwnd, TIMER_DELTA_HANDLE, 10, NULL);
		Window->hwnd = hwnd;

		//HIMC himc = ImmAssociateContext(Window->hwnd, NULL);
		//ImmAssociateContext(Window->hwnd, himc);
		return 0;

	case WM_SIZE:
		Window->winWidth = cxClient = LOWORD(lParam);
		Window->winHeight = cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		_drawWidget(WIDGET_FRONT);
		hdc = BeginPaint(hwnd, &ps);
		if (_visualPage == 0) _makeBitmap(hdc, Screen->buffer1->data, Screen->buffer1->sizeX, Screen->buffer1->sizeY, 24);
		else _makeBitmap(hdc, Screen->buffer2->data, Screen->buffer2->sizeX, Screen->buffer2->sizeY, 24);

		EndPaint(hwnd, &ps);
		_drawWidget(WIDGET_BACK);
		return 0;

	case WM_TIMER:
		if (wParam == TIMER_DELTA_HANDLE) {
			if (!_inDelay)sgLoop();
			if (_sglMode == TEXT_MAP)_textLoop();
		}
		if (wParam == INTERUPT_DELTA_HANDLE) {
			Vector->_v8();
		}
		InvalidateRect(hwnd, NULL, FALSE);
		return 0;

	case WM_KEYDOWN:
		sgSpecial(wParam, 0, 0);
		return 0;

	case WM_CHAR:
		sgKey(wParam, 0, 0);
		return 0;

	case WM_KEYUP:
		sgKeyUp(wParam, 0, 0);
		sgSpecialUp(wParam, 0, 0);
		return 0;

	case WM_MOUSEMOVE:
		if(!wParam)sgMouse(LOWORD(lParam), HIWORD(lParam));
		else sgDrag(LOWORD(lParam), HIWORD(lParam));
		return 0;

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

//Remained OpenGL functions.
void sgInit() {
	int i, winPx, winPy;

	//init the global structs.
	Screen = (struct _screen*)malloc(sizeof(struct _screen));
	Window = (struct _win*)malloc(sizeof(struct _win));
	Key = (struct _key*)malloc(sizeof(struct _key));
	Mouse = (struct _mouse*)malloc(sizeof(struct _mouse));
	Vector = (struct _vect*)malloc(sizeof(struct _vect));
	Widget = (struct _widget*)malloc(sizeof(struct _widget));

	Key->front = -1;
	Key->rear = 0;

	Mouse->front = -1;
	Mouse->rear = 0;
	Mouse->left = 0;
	Mouse->right = 0;
	Mouse->middle = 0;
	Mouse->coord = 0;

	Vector->_v8 = (vect)vectDefault;
	Vector->_v9 = (vect)vectDefault;

	Widget->count = 0;
	Widget->active = -1;
	for (i = 0; i < SG_QSIZE; i++) {
		Widget->obj[i] = NULL;
	}
	for (i = 0; i < 256; i++) {
		Widget->hash[i] = NULL;
	}

	Window->winName = (char *)malloc(256 * sizeof(char));
	Window->winWidth = 640;
	Window->winHeight = 480;
	Window->txtWidth = 80;
	Window->txtHeight = 25;

	_sglMode = BIT_MAP;
	_innerFunc = 0;
	strcpy(Window->winName, "SGpro");

	_prepareText(Window->winWidth);

	//users' setup.
	sgSetup();

	//build the buffer.
	Screen->content = (textMap *)malloc(sizeof(textMap));
	Screen->content->width = Window->txtWidth;
	Screen->content->height = Window->txtHeight;
	Screen->content->content = (unsigned short *)malloc(Window->txtWidth * Window->txtHeight * sizeof(unsigned short));
	for (i = 0; i < Window->txtWidth*Window->txtHeight; i++)Screen->content->content[i] = 0xF000;

	Screen->changed = (textMap *)malloc(sizeof(textMap));
	Screen->changed->width = Window->txtWidth;
	Screen->changed->height = Window->txtHeight;
	Screen->changed->content = (unsigned short *)malloc(Window->txtWidth * Window->txtHeight * sizeof(unsigned short));
	for (i = 0; i < Window->txtWidth*Window->txtHeight; i++)Screen->changed->content[i] = 0xF000;

	Screen->textMask = (textMap *)malloc(sizeof(textMap));
	Screen->textMask->width = Window->txtWidth;
	Screen->textMask->height = Window->txtHeight;
	Screen->textMask->content = (unsigned short *)malloc(Window->txtWidth * Window->txtHeight * sizeof(unsigned short));
	for (i = 0; i < Window->txtWidth*Window->txtHeight; i++)Screen->textMask->content[i] = 0x0000;

	Screen->buffer1 = (bitMap *)malloc(sizeof(bitMap));
	Screen->buffer1->sizeX = _sglMode == TEXT_MAP ? Window->txtWidth*SG_CHAR_WIDTH : Window->winWidth;
	Screen->buffer1->sizeY = _sglMode == TEXT_MAP ? Window->txtHeight*SG_CHAR_HEIGHT : Window->winHeight;
	Screen->buffer1->data = (unsigned char *)malloc(Screen->buffer1->sizeX * Screen->buffer1->sizeY * 3 * sizeof(unsigned char));
	memset(Screen->buffer1->data, -1, Screen->buffer1->sizeX * Screen->buffer1->sizeY * 3 * sizeof(unsigned char));
	
	Screen->buffer2 = (bitMap *)malloc(sizeof(bitMap));
	Screen->buffer2->sizeX = _sglMode == TEXT_MAP ? Window->txtWidth*SG_CHAR_WIDTH : Window->winWidth;
	Screen->buffer2->sizeY = _sglMode == TEXT_MAP ? Window->txtHeight*SG_CHAR_HEIGHT : Window->winHeight;
	Screen->buffer2->data = (unsigned char *)malloc(Screen->buffer2->sizeX * Screen->buffer2->sizeY * 3 * sizeof(char));
	memset(Screen->buffer2->data, -1, Screen->buffer2->sizeX * Screen->buffer2->sizeY * 3 * sizeof(char));

	Window->winWidth = _sglMode == TEXT_MAP ? Window->txtWidth*SG_CHAR_WIDTH : Window->winWidth;
	Window->winHeight = _sglMode == TEXT_MAP ? Window->txtHeight*SG_CHAR_HEIGHT : Window->winHeight;

	Screen->rgb[0] = 0;
	Screen->rgb[1] = 0;
	Screen->rgb[2] = 0;
	Screen->alpha = 1.f;
	Screen->bfc = WHITE << 4 | BLACK;

	for (i = 0; i < Widget->count; i++) {
		if(Widget->obj[i]->visible)
			getImage(Widget->obj[i]->pos.x, Widget->obj[i]->pos.y,
				Widget->obj[i]->pos.x + Widget->obj[i]->size.x, Widget->obj[i]->pos.y + Widget->obj[i]->size.y,
				Widget->obj[i]->cover);
	}

	//create a window.
	winPx = GetSystemMetrics(SM_CXFULLSCREEN);
	winPy = GetSystemMetrics(SM_CYFULLSCREEN);

	if (Window->winWidth >= winPx)Window->posLeft = 0;
	else Window->posLeft = (winPx - Window->winWidth) / 2;
	if (Window->winHeight >= winPy)Window->posUp = 0;
	else Window->posUp = (winPy - Window->winHeight) / 2;

}
void sgKey(int cAscii, int x, int y) {
	widgetObj *tmp;
	int i;

	if (!_enKey || cAscii >= 0x80 || cAscii < 0x20)return;

	Key->front = (Key->front + 1) % 1024;
	Key->keyBuf[Key->front] = (word)cAscii;
	Key->keyBuf[Key->front] &= (word)0x7fff;
	_vectKey = cAscii;

	if (_sglMode == BIT_MAP) {
		for (i = 0; i < Widget->count; i++) {
			tmp = Widget->obj[i];
			if (tmp->status&WIDGET_SELECTED) {
				tmp->keyDown(tmp, cAscii);
				tmp->keyPress(tmp, cAscii);
			}
		}
	}

	(*Vector->_v9)();
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

	Key->front = (Key->front + 1) % 1024;
	Key->keyBuf[Key->front] = (word)cAscii;
	Key->keyBuf[Key->front] &= (word)0x7fff;
	_vectKey = cAscii;

	if (_sglMode == BIT_MAP) {
		for (i = 0; i < Widget->count; i++) {
			tmp = Widget->obj[i];
			if (tmp->status&WIDGET_SELECTED) {
				tmp->keyDown(tmp, cAscii);
				tmp->keyPress(tmp, cAscii);
			}
		}
	}

	(*Vector->_v9)();
}
void sgKeyUp(int cAscii, int x, int y) {
	widgetObj *tmp;
	int i;

	if (!_enKey || cAscii >= 0x100 || cAscii < 0x20)return;
	if (cAscii >= 0x21 && cAscii <= 0x2F)return;
	if (cAscii >= 0x70 && cAscii <= 0x7B)return;

	Key->front = (Key->front + 1) % 1024;
	Key->keyBuf[Key->front] = (word)cAscii;
	Key->keyBuf[Key->front] |= (word)0x8000;

	if (_sglMode == BIT_MAP) {
		for (i = 0; i < Widget->count; i++) {
			tmp = Widget->obj[i];
			if (tmp->status&WIDGET_SELECTED)
				tmp->keyUp(tmp, cAscii|0x8000);
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
	Key->front = (Key->front + 1) % 1024;
	Key->keyBuf[Key->front] = (word)cAscii;
	Key->keyBuf[Key->front] |= (word)0x8000;

	if (_sglMode == BIT_MAP) {
		for (i = 0; i < Widget->count; i++) {
			tmp = Widget->obj[i];
			if (tmp->status&WIDGET_SELECTED)
				tmp->keyUp(tmp, cAscii|0x8000);
		}
	}
}
void sgMouse(int x, int y) {
	bitMap *buf;
	int i;
	widgetObj *tmp;

	if (!_enMouse)return;

	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;

	Mouse->Pos.x = x*buf->sizeX / Window->winWidth;
	Mouse->Pos.y = y*buf->sizeY / Window->winHeight;

	if (_sglMode == BIT_MAP) {
		if (Widget->active != -1) {
			if(inWidget(Widget->obj[Widget->active], x, y))
				Widget->obj[Widget->active]->mouseIn(Widget->obj[Widget->active],
					Mouse->Pos.x, Mouse->Pos.y);
			else
				Widget->obj[Widget->active]->mouseOut(Widget->obj[Widget->active],
					Mouse->Pos.x, Mouse->Pos.y);
			for (i = 0; i < Widget->count; i++) {
				tmp = Widget->obj[i];
				if (tmp->visible == FALSE)continue;
				if (inWidget(tmp, x, y))continue;
				if (Mouse->Pos.x >= tmp->pos.x&&Mouse->Pos.y >= tmp->pos.y&&
					Mouse->Pos.x <= tmp->pos.x + tmp->size.x&&Mouse->Pos.y <= tmp->pos.y + tmp->size.y) {
					tmp->mouseIn(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y);
				}
				else
					tmp->mouseOut(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y);
			}
		}
		else {
			for (i = 0; i < Widget->count; i++) {
				tmp = Widget->obj[i];
				if (tmp->visible == FALSE)continue;
				if (Mouse->Pos.x >= tmp->pos.x&&Mouse->Pos.y >= tmp->pos.y&&
					Mouse->Pos.x <= tmp->pos.x + tmp->size.x&&Mouse->Pos.y <= tmp->pos.y + tmp->size.y) {
					tmp->mouseIn(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y);
				}
				else
					tmp->mouseOut(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y);
			}
		}
	}
}
void sgDrag(int x, int y) {
	bitMap *buf;
	widgetObj *tmp;
	int i;

	if (!_enMouse)return;

	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;

	Mouse->Pos.x = x*buf->sizeX / Window->winWidth;
	Mouse->Pos.y = y*buf->sizeY / Window->winHeight;

	if (_sglMode == BIT_MAP) {
		if (Widget->active != -1) {
			if (inWidget(Widget->obj[Widget->active], x, y))
				Widget->obj[Widget->active]->mouseIn(Widget->obj[Widget->active],
					Mouse->Pos.x, Mouse->Pos.y);
			else
				Widget->obj[Widget->active]->mouseOut(Widget->obj[Widget->active],
					Mouse->Pos.x, Mouse->Pos.y);
			for (i = 0; i < Widget->count; i++) {
				tmp = Widget->obj[i];
				if (tmp->visible == FALSE)continue;
				if (inWidget(tmp, x, y))continue;
				if (Mouse->Pos.x >= tmp->pos.x&&Mouse->Pos.y >= tmp->pos.y&&
					Mouse->Pos.x <= tmp->pos.x + tmp->size.x&&Mouse->Pos.y <= tmp->pos.y + tmp->size.y) {
					if (!(tmp->status&WIDGET_PASS))
						tmp->mouseIn(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y);
				}
				else
					if (tmp->status&WIDGET_PASS)
						tmp->mouseOut(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y);
			}
		}
		else {
			for (i = 0; i < Widget->count; i++) {
				tmp = Widget->obj[i];
				if (tmp->visible == FALSE)continue;
				if (Mouse->Pos.x >= tmp->pos.x&&Mouse->Pos.y >= tmp->pos.y&&
					Mouse->Pos.x <= tmp->pos.x + tmp->size.x&&Mouse->Pos.y <= tmp->pos.y + tmp->size.y) {
					if (!(tmp->status&WIDGET_PASS))
						tmp->mouseIn(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y);
				}
				else
					if (tmp->status&WIDGET_PASS)
						tmp->mouseOut(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y);
			}
		}
	}
}
void sgClick(int button, int state, int x, int y) {
	static vecTwo pos[3];
	bitMap *buf;
	widgetObj *tmp;
	int i;

	if (!_enMouse)return;

	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;

	if (button == SG_LEFT_BUTTON && state == SG_BUTTON_DOWN) {
		Mouse->left = SG_BUTTON_DOWN;
		pos[0].x = x;
		pos[0].y = y;

		if (_sglMode == BIT_MAP) {
			if (Widget->active != -1 && inWidget(Widget->obj[Widget->active], x, y)) {
				Widget->obj[Widget->active]->mouseDown(Widget->obj[Widget->active],
					Mouse->Pos.x, Mouse->Pos.y, SG_BUTTON_DOWN | SG_LEFT_BUTTON);
				for (i = 0; i < Widget->count; i++) {
					tmp = Widget->obj[i];
					if (tmp->visible == FALSE)continue;
					if (inWidget(tmp, x, y))continue;
					tmp->mouseDown(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y, SG_BUTTON_DOWN | SG_LEFT_BUTTON);
				}
			}
			else {
				for (i = 0; i < Widget->count; i++) {
					tmp = Widget->obj[i];
					if (tmp->visible == FALSE)continue;
					tmp->mouseDown(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y, SG_BUTTON_DOWN | SG_LEFT_BUTTON);
				}
			}
		}
	}
	if (button == SG_LEFT_BUTTON && state == SG_BUTTON_UP) {
		Mouse->left = SG_BUTTON_UP;
		if (!Mouse->coord)
			if (pos[0].x != x || pos[0].y != y)return;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].x = x*buf->sizeX / Window->winWidth;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].y = y*buf->sizeY / Window->winHeight;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].m = SG_LEFT_BUTTON;
		Mouse->front = (Mouse->front + 1) % 1024;

		if (_sglMode == BIT_MAP) {
			if (Widget->active != -1 && inWidget(Widget->obj[Widget->active], x, y)) {
				Widget->obj[Widget->active]->mouseClick(Widget->obj[Widget->active],
					Mouse->Pos.x, Mouse->Pos.y, SG_BUTTON_UP | SG_LEFT_BUTTON);
				if (Widget->active != -1)
					Widget->obj[Widget->active]->mouseUp(Widget->obj[Widget->active],
						Mouse->Pos.x, Mouse->Pos.y, SG_BUTTON_UP | SG_LEFT_BUTTON);
				for (i = 0; i < Widget->count; i++) {
					tmp = Widget->obj[i];
					if (tmp->visible == FALSE)continue;
					if (inWidget(tmp, x, y))continue;
					tmp->mouseClick(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y, SG_BUTTON_UP | SG_LEFT_BUTTON);
					tmp->mouseUp(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y, SG_BUTTON_UP | SG_LEFT_BUTTON);
				}
			}
			else {
				for (i = 0; i < Widget->count; i++) {
					tmp = Widget->obj[i];
					if (tmp->visible == FALSE)continue;
					tmp->mouseClick(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y, SG_BUTTON_UP | SG_LEFT_BUTTON);
					tmp->mouseUp(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y, SG_BUTTON_UP | SG_LEFT_BUTTON);
				}
			}
		}
	}
	if (button == SG_RIGHT_BUTTON && state == SG_BUTTON_DOWN) {
		Mouse->right = SG_BUTTON_DOWN;
		pos[1].x = x;
		pos[1].y = y;

		if (_sglMode == BIT_MAP) {
			if (Widget->active != -1 && inWidget(Widget->obj[Widget->active], x, y)) {
				Widget->obj[Widget->active]->mouseDown(Widget->obj[Widget->active],
					Mouse->Pos.x, Mouse->Pos.y, SG_BUTTON_DOWN | SG_RIGHT_BUTTON);
				for (i = 0; i < Widget->count; i++) {
					tmp = Widget->obj[i];
					if (tmp->visible == FALSE)continue;
					if (inWidget(tmp, x, y))continue;
					tmp->mouseDown(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y, SG_BUTTON_DOWN | SG_RIGHT_BUTTON);
				}
			}
			else {
				for (i = 0; i < Widget->count; i++) {
					tmp = Widget->obj[i];
					if (tmp->visible == FALSE)continue;
					tmp->mouseDown(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y, SG_BUTTON_DOWN | SG_RIGHT_BUTTON);
				}
			}
		}
	}
	if (button == SG_RIGHT_BUTTON && state == SG_BUTTON_UP) {
		Mouse->right = SG_BUTTON_UP;
		if (!Mouse->coord)
			if (pos[1].x != x || pos[1].y != y)return;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].x = x*buf->sizeX / Window->winWidth;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].y = y*buf->sizeY / Window->winHeight;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].m = SG_RIGHT_BUTTON;
		Mouse->front = (Mouse->front + 1) % 1024;

		if (_sglMode == BIT_MAP) {
			if (Widget->active != -1 && inWidget(Widget->obj[Widget->active], x, y)) {
				Widget->obj[Widget->active]->mouseClick(Widget->obj[Widget->active],
					Mouse->Pos.x, Mouse->Pos.y, SG_BUTTON_DOWN | SG_RIGHT_BUTTON);
				if (Widget->active != -1)
					Widget->obj[Widget->active]->mouseUp(Widget->obj[Widget->active],
						Mouse->Pos.x, Mouse->Pos.y, SG_BUTTON_DOWN | SG_RIGHT_BUTTON);
				for (i = 0; i < Widget->count; i++) {
					tmp = Widget->obj[i];
					if (tmp->visible == FALSE)continue;
					if (inWidget(tmp, x, y))continue;
					tmp->mouseClick(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y, SG_BUTTON_UP | SG_RIGHT_BUTTON);
					tmp->mouseUp(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y, SG_BUTTON_UP | SG_RIGHT_BUTTON);
				}
			}
			else {
				for (i = 0; i < Widget->count; i++) {
					tmp = Widget->obj[i];
					if (tmp->visible == FALSE)continue;
					tmp->mouseClick(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y, SG_BUTTON_UP | SG_RIGHT_BUTTON);
					tmp->mouseUp(Widget->obj[i], Mouse->Pos.x, Mouse->Pos.y, SG_BUTTON_UP | SG_RIGHT_BUTTON);
				}
			}
		}
	}
	if (button == SG_MIDDLE_BUTTON && state == SG_BUTTON_DOWN) {
		Mouse->middle = SG_BUTTON_DOWN;
		pos[2].x = x;
		pos[2].y = y;
	}
	if (button == SG_MIDDLE_BUTTON && state == SG_BUTTON_UP) {
		Mouse->middle = SG_BUTTON_UP;
		if (!Mouse->coord)
			if (pos[2].x != x || pos[2].y != y)return;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].x = x*buf->sizeX / Window->winWidth;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].y = y*buf->sizeY / Window->winHeight;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].m = SG_MIDDLE_BUTTON;
		Mouse->front = (Mouse->front + 1) % 1024;
	}
}

//Inner functions.
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
	bitinfo.bmiHeader.biSizeImage = width*height*(wBitCount / 8);
	bitinfo.bmiHeader.biXPelsPerMeter = 0;
	bitinfo.bmiHeader.biYPelsPerMeter = 0;
	bitinfo.bmiHeader.biClrUsed = 0;
	bitinfo.bmiHeader.biClrImportant = 0;

	buffer = (char *)malloc(bitinfo.bmiHeader.biSizeImage);
	for (i = 0; i < height; i++) {
		memcpy(buffer + i*width*(wBitCount / 8), lpBits + (height - 1 - i)*width*(wBitCount / 8), width*(wBitCount / 8));
	}

	StretchDIBits(hdc, 0, 0, Window->winWidth,
		Window->winHeight, 0, 0, bitinfo.bmiHeader.biWidth,
		bitinfo.bmiHeader.biHeight, buffer, (BITMAPINFO*)&bitinfo.bmiHeader,
		DIB_RGB_COLORS, SRCCOPY);
	free(buffer);
}
void vectDefault(void) {
	return;
}

void _textLoop() {
	int i;

	for (i = 0; i < Screen->changed->width * Screen->changed->height; i++) {
		if (Screen->changed->content[i] != Screen->content->content[i]) {
			Screen->content->content[i] = Screen->changed->content[i];
			_textRenew(Screen->content->content[i], i % Screen->changed->width, i / Screen->changed->width);
		}
		if (Screen->textMask->content[i] & 0xFF) {
			_textAdd(Screen->content->content[i], i % Screen->changed->width, i / Screen->changed->width);
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
LPWSTR _widen(const char *src) {
	int rt;
	LPWSTR rs;
	rt = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
	rs = (LPWSTR)malloc(rt * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, src, -1, rs, rt * sizeof(wchar_t));
	return rs;
}
int _strlenW(const char *str) {
	return MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
}
void _putImageRev(int left, int top, bitMap *bitmap, int op) {
	int x1, x2, y1, y2, i, j;
	float rate;
	bitMap *buf;

	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;

	if (left >= buf->sizeX || top >= buf->sizeY)return;
	if (left + bitmap->sizeX <= 0 || top + bitmap->sizeY <= 0)return;

	if (left < 0)x1 = 0;
	else x1 = left;
	if (top < 0)y1 = 0;
	else y1 = top;

	if (left + bitmap->sizeX >= buf->sizeX)x2 = buf->sizeX - 1;
	else x2 = left + bitmap->sizeX - 1;
	y2 = top + bitmap->sizeY - 1;

	for (i = 0; i < y2 - y1 + 1; i++) {
		if (top + i >= buf->sizeY)break;
		for (j = 0; j < x2 - x1 + 1; j++) {
			if (rate = bitmap->data[(bitmap->sizeX*(y2 - y1 - i) + j) * 3]){
				buf->data[(buf->sizeX * (i + top) + left + j) * 3] *= (1 - rate / 255);
				buf->data[(buf->sizeX * (i + top) + left + j) * 3] += rate / 255 * tf.color.r;
			}
			if (rate = bitmap->data[(bitmap->sizeX*(y2 - y1 - i) + j) * 3 + 1]) {
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 1] *= (1 - rate / 255);
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 1] += rate / 255 * tf.color.g;
			}
			if (rate = bitmap->data[(bitmap->sizeX*(y2 - y1 - i) + j) * 3 + 2]) {
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 2] *= (1 - rate / 255);
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 2] += rate / 255 * tf.color.b;
			}
		}
	}
}
void _hbmImage(HDC hdc, HBITMAP hbm, int x, int y) {
	GetDIBits(hdc, hbm, 0, text.bm.bmHeight, text.bitBuf, (BITMAPINFO*)(&text.bi), DIB_RGB_COLORS);

	bitMap tmp;
	tmp.sizeX = text.bi.biWidth;
	tmp.sizeY = text.bi.biHeight;
	tmp.data = text.bitBuf;
	_putImageRev(x, y, &tmp, COPY_PUT);
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
	tf.name = (Cstring)malloc(SG_MAX_FONT_LENGTH);
	strcpy(tf.name, "Î¢ÈíÑÅºÚ");
	tf.coeff = 0;
}


//System interfaces.
void initWindow(int width, int height, char *title, int mode) {
	if (mode == BIT_MAP || mode == TEXT_MAP)_sglMode = mode;
	else exit(SG_INVALID_MODE);

	if (mode == BIT_MAP) {
		Window->winWidth = width;
		Window->winHeight = height;
	}
	if (mode == TEXT_MAP) {
		Window->txtWidth = width;
		Window->txtHeight = height;
		Window->winWidth = width*SG_CHAR_WIDTH;
		Window->winHeight = height*SG_CHAR_HEIGHT;
	}

	strcpy(Window->winName, title);
	strcat(Window->winName, "  --powered by Super GP");
}
void setWindow(int left, int up) {
	Window->posLeft = left;
	Window->posUp = up;
}
int getWidth(int obj) {
	switch (obj) {
	case SG_WINDOW:
		return Window->winWidth;
	case SG_SCREEN:
		return Screen->buffer1->sizeX == Screen->buffer2->sizeX ?
			Screen->buffer1->sizeX : SG_SIZE_MISMATCH;
	}
	return 0;
}
int getHeight(int obj) {
	switch (obj) {
	case SG_WINDOW:
		return Window->winHeight;
	case SG_SCREEN:
		return Screen->buffer1->sizeY == Screen->buffer2->sizeY ?
			Screen->buffer1->sizeY : SG_SIZE_MISMATCH;
	}
	return 0;
}
void initMouse(int mode) {
	if (mode&SG_COORDINATE)Mouse->coord = 1;
	else Mouse->coord = 0;

	_enMouse = 1;
}
int mouseStatus(int b) {
	switch (b) {
	case SG_LEFT_BUTTON:
		return Mouse->left;
	case SG_RIGHT_BUTTON:
		return Mouse->right;
	case SG_MIDDLE_BUTTON:
		return Mouse->middle;
	default:
		return 0;
	}
}
vecTwo mousePos() {
	vecTwo ret;

	ret.x = Mouse->Pos.x;
	ret.y = Mouse->Pos.y;
	return ret;
}
vecThree biosMouse(int cmd) {
	vecThree ret;

	if (cmd == 1) {
		ret.m = (Mouse->front + 1) % 1024 != Mouse->rear;
		return ret;
	}
	if (cmd != 0) {
		ret.m = 0;
		return ret;
	}

	while ((Mouse->front + 1) % 1024 == Mouse->rear);
	ret = Mouse->mouseBuf[Mouse->rear++];
	Mouse->rear %= 1024;

	return ret;
}
void clearMouseBuffer() {
	Mouse->rear = (Mouse->front + 1) % 1024;
}
void initKey() {
	_enKey = 1;
}
int biosKey(int cmd) {
	word ret;

	if (cmd == 1)return (Key->front + 1) % 1024 != Key->rear;
	if (cmd != 0)return 0;

	while ((Key->front + 1) % 1024 == Key->rear);
	ret = Key->keyBuf[Key->rear++];
	Key->rear %= 1024;

	return ret;
}
void clearKeyBuffer() {
	Key->rear = (Key->front + 1) % 1024;
}
void delay(int t) {
	clock_t st, en;

	_inDelay = 1;
	st = clock();
	en = clock();

	if (_inLoop) InvalidateRect(Window->hwnd, NULL, FALSE);
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
	/*if (_inLoop) InvalidateRect(Window->hwnd, NULL, FALSE);
	while (_delayEnd - _delayBegin < t) {
		_delayEnd = clock();
	}*/
	if(t > _delayEnd - _delayBegin)
		Sleep(t - _delayEnd + _delayBegin);

	_delaySE = 0;
	_inDelay = 0;

	return 1;
}
vect getVect(int intn) {
	if (intn == 8)return Vector->_v8;
	if (intn == 9)return Vector->_v9;

	return (vect)NULL;
}
int setVect(int intn, vect v) {
	if (intn == 8) {
		Vector->_v8 = v;
		return 1;
	}
	if (intn == 9) {
		Vector->_v9 = v;
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
void setMouse(int x, int y) {
	LPPOINT point;

	point = (LPPOINT)malloc(sizeof(POINT));
	point->x = x;
	point->y = y;

	ClientToScreen(Window->hwnd, point);
	SetCursorPos(point->x, point->y);
	free(point);
}
void setActivePage(int page) {
	if (page != 0 && page != 1)return;
	_activePage = page;
}
void setVisualPage(int page) {
	if (page != 0 && page != 1)return;
	_visualPage = page;
}
int bmpMemory(bitMap *obj, char *filename) {
	FILE *fp = NULL;
	SGstring p = NULL;
	int width, height, i;
	dword dataOffset, lineBytes;
	dword lines;

	if (_sglMode != BIT_MAP && !_innerFunc)return SG_INVALID_MODE;

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

	obj->data = malloc(width * height * 3 * sizeof(char));
	if (obj->data == NULL)goto displayError;
	fseek(fp, dataOffset, SEEK_SET);

	for (i = height - 1; i >= 0; i--) {
		fread(p, 1, lineBytes, fp);
		lines = i * width * 3;
		memcpy(obj->data + lines, p, width * 3);
	}

	free(p);
	fclose(fp);
	return SG_NO_ERORR;

displayError:
	if (p != NULL)free(p);
	if (fp != NULL)fclose(fp);
	return SG_IO_ERROR;
}

//Bitmap interfaces.
void setColor(int r, int g, int b) {
	if (_sglMode != BIT_MAP && !_innerFunc)return;

	Screen->rgb[0] = r % 256;
	Screen->rgb[1] = g % 256;
	Screen->rgb[2] = b % 256;

	tf.color.r = r;
	tf.color.g = g;
	tf.color.b = b;
}
void setFontSize(int height) {
	if(height<SG_MAX_FONT_LENGTH)tf.size = height;
	else tf.size = SG_MAX_FONT_LENGTH;
}
void setFontName(Cstring name) {
	strcpy(tf.name, name);
}
void setFontStyle(int coeff) {
	tf.coeff = coeff;
}
void setAlpha(float alpha) {
	Screen->alpha = alpha;
}
void clearScreen() {
	int i;
	bitMap *buf;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;

	if ((Screen->rgb[0] & Screen->rgb[1] & Screen->rgb[2]) == 255) {
		memset(buf->data, -1, 3 * buf->sizeX*buf->sizeY);
		return;
	}
	if ((Screen->rgb[0] | Screen->rgb[1] | Screen->rgb[2]) == 0) {
		memset(buf->data, 0, 3 * buf->sizeX*buf->sizeY);
		return;
	}

	for (i = 0; i < 3 * buf->sizeX*buf->sizeY; i += 3) {
		buf->data[i] = Screen->rgb[2];
		buf->data[i + 1] = Screen->rgb[1];
		buf->data[i + 2] = Screen->rgb[0];
	}
}
int putPixel(int x, int y) {
	bitMap *buf;
	int p;

	if (_sglMode != BIT_MAP && !_innerFunc)return SG_INVALID_MODE;

	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;

	if (x < 0 || x >= buf->sizeX || y < 0 || y >= buf->sizeY)return SG_OUT_OF_RANGE;

	p = (y * buf->sizeX + x) * 3;
	if (Screen->alpha == 1.f) {
		buf->data[p] = Screen->rgb[2];
		buf->data[p + 1] = Screen->rgb[1];
		buf->data[p + 2] = Screen->rgb[0];
	}
	else {
		buf->data[p] = (int)(buf->data[p]*(1 - Screen->alpha));
		buf->data[p] += (int)(Screen->rgb[2] * (Screen->alpha));
		buf->data[p + 1] *= (int)(buf->data[p + 1] * (1 - Screen->alpha));
		buf->data[p + 1] += (int)(Screen->rgb[1] * (Screen->alpha));
		buf->data[p + 2] *= (int)(buf->data[p + 2] * (1 - Screen->alpha));
		buf->data[p + 2] += (int)(Screen->rgb[0] * (Screen->alpha));
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

	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;

	ret.r = ret.g = ret.b = SG_OUT_OF_RANGE;

	if (x < 0 || x >= buf->sizeX || y < 0 || y >= buf->sizeY)return ret;
	p = (y*buf->sizeX + x) * 3;
	ret.r = buf->data[p++];
	ret.g = buf->data[p++];
	ret.b = buf->data[p++];

	return ret;
}
void putQuad(int x1, int y1, int x2, int y2, int mode) {
	int i, j;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

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
void putTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int mode) {
	RGB c;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

	c.r = Screen->rgb[0];
	c.g = Screen->rgb[1];
	c.b = Screen->rgb[2];
	putLine(x1, y1, x2, y2, SOLID_LINE);
	putLine(x1, y1, x3, y3, SOLID_LINE);
	putLine(x2, y2, x3, y3, SOLID_LINE);
	if(mode == SOLID_FILL)floodFill((x1 + x2 + x3) / 3, (y1 + y2 + y3) / 3, c);
}
int loadBmp(int x, int y, char *filename) {
	FILE *fp = NULL;
	SGstring p = NULL, tmp = NULL;
	int width, height, i;
	dword dataOffset, lineBytes;
	dword lines;
	bitMap *buf;
	byte *vp;
	char buffer = 0;

	if (_sglMode != BIT_MAP && !_innerFunc)return SG_INVALID_MODE;

	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;
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

	free(p);
	fclose(fp);
	return SG_NO_ERORR;

displayError:
	if (p != NULL)free(p);
	if (fp != NULL)fclose(fp);
	return SG_IO_ERROR;
}
void putChar(char ch, int x, int y) {
	int j, k;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

	for (j = 0; j < 8; j++)
		for (k = 7; k >= 0; k--)
			if ((letters[ch][j] >> k) & 1) {
				putPixel(x + 7 - k, y + 2 * j);
				putPixel(x + 7 - k, y + 2 * j + 1);
			}
}
void putString(Cstring str, int x, int y) {
	if (_sglMode != BIT_MAP && !_innerFunc)return;
	if (str == NULL)return;

	memset(text.bitBuf, 0, text.bufSize);
	RECT imgRect = { 0, 0, text.bm.bmWidth, text.bm.bmHeight };
	FillRect(text.memDC, &imgRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

	text.font = CreateFont(tf.size, 0, 0, 0, FW_THIN, tf.coeff & FONT_ITALIC,
		tf.coeff & FONT_UNDERLINE, tf.coeff & FONT_STRIKEOUT,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, FF_MODERN, _widen(tf.name));
	SelectObject(text.memDC, text.font);

	SetTextColor(text.memDC, RGB(255, 255, 255));
	SetBkMode(text.memDC, TRANSPARENT);

	TextOut(text.memDC, 0, 0, _widen(str), _strlenW(str));
	_hbmImage(text.memDC, text.hbm, x, y);

	DeleteObject(text.font);
}
int putStringConstraint(Cstring str, int x, int y, int constraint) {
	int len;

	if (_sglMode != BIT_MAP && !_innerFunc)return SG_INVALID_MODE;
	if (str == NULL)return SG_NULL_POINTER;

	if (constraint < (int)_strlenW(str))len = constraint;
	else len = _strlenW(str) - 1;

	memset(text.bitBuf, 0, text.bufSize);
	RECT imgRect = { 0, 0, text.bm.bmWidth, text.bm.bmHeight };
	FillRect(text.memDC, &imgRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

	text.font = CreateFont(tf.size, 0, 0, 0, FW_THIN, tf.coeff & FONT_ITALIC,
		tf.coeff & FONT_UNDERLINE, tf.coeff & FONT_STRIKEOUT,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, FF_MODERN, _widen(tf.name));
	SelectObject(text.memDC, text.font);

	SetTextColor(text.memDC, RGB(255, 255, 255));
	SetBkMode(text.memDC, TRANSPARENT);

	TextOut(text.memDC, 0, 0, _widen(str), len);
	_hbmImage(text.memDC, text.hbm, x, y);

	DeleteObject(text.font);
	return len;
}
int getImage(int left, int top, int right, int bottom, bitMap *bitmap) {
	int deltaX, deltaY, i;
	bitMap *buf;

	if (_sglMode != BIT_MAP && !_innerFunc)return SG_INVALID_MODE;

	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;

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

	return SG_NO_ERORR;
}
void putImage(int left, int top, bitMap *bitmap, int op) {
	int x1, x2, y1, y2, i, j;
	bitMap *buf;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;

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
void floodFill(int x, int y, RGB c) {
	vecTwo Q[SG_QSIZE];
	vecTwo tmp;
	int front = -1, rear = 0, p;
	bitMap *buf;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

#define ENQUE(x) Q[(++front)%SG_QSIZE] = x
#define DEQUE() Q[(rear++)%SG_QSIZE]
#define ISEMPTY() ((front+1)%SG_QSIZE==(rear%SG_QSIZE))

	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;

	tmp.x = x;
	tmp.y = y;
	ENQUE(tmp);

	while (!ISEMPTY()) {
		tmp = DEQUE();
		p = (tmp.y*buf->sizeX + tmp.x) * 3;
		if ((buf->data[p] == c.b&&buf->data[p + 1] == c.g&&buf->data[p + 2] == c.r) || (buf->data[p] == Screen->rgb[2] && buf->data[p + 1] == Screen->rgb[1] && buf->data[p + 2] == Screen->rgb[0]))continue;
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

#undef ENQUE
#undef DEQUE
#undef ISEMPTY

}
void putLine(int x1, int y1, int x2, int y2, int mode) {
	int dx, dy, ux, uy, x, y, eps;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

#define ABS(x) ((x > 0) ? (x) : (-x))

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
#undef ABS
}
void putCircle(int xc, int yc, int r, int mode) {
	int x, y, yi, d;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

	x = 0;
	y = r;
	d = 3 - 2 * r;

	if (mode == SOLID_FILL) {
		while (x <= y) {
			for (yi = x; yi <= y; yi++) {
				putPixel(xc + x, yc + yi);
				putPixel(xc + x, yc - yi);
				putPixel(xc - x, yc - yi);
				putPixel(xc - x, yc + yi);
				putPixel(xc + yi, yc + x);
				putPixel(xc + yi, yc - x);
				putPixel(xc - yi, yc - x);
				putPixel(xc - yi, yc + x);
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
			putPixel(xc + x, yc + y);
			putPixel(xc + x, yc - y);
			putPixel(xc - x, yc - y);
			putPixel(xc - x, yc + y);
			putPixel(xc + y, yc + x);
			putPixel(xc + y, yc - x);
			putPixel(xc - y, yc - x);
			putPixel(xc - y, yc + x);

			if (d < 0)d = d + 4 * x + 6;
			else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
}
void putEllipse(int xc, int yc, int a, int b, int mode) {
	int sqa, sqb, x, y, d, P_x, xi;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

	sqa = a * a;
	sqb = b * b;
	x = 0;
	y = b;
	d = 2 * sqb - 2 * b * sqa + sqa;
	P_x = (int)((double)sqa / sqrt((double)(sqa + sqb)));

	putPixel(xc + x, yc + y);
	putPixel(xc - x, yc - y);

	if (mode == SOLID_FILL) {
		while (x <= P_x) {
			if (d < 0) d += 2 * sqb * (2 * x + 3);
			else {
				d += 2 * sqb * (2 * x + 3) - 4 * sqa * (y - 1);
				y--;
			}
			x++;

			for (xi = 0; xi <= x; xi++) {
				putPixel(xc + xi, yc + y);
				putPixel(xc - xi, yc + y);
				putPixel(xc + xi, yc - y);
				putPixel(xc - xi, yc - y);
			}
		}
		d = sqb * (x * x + x) + sqa * (y * y - y) - sqa * sqb;
		while (y >= 0) {
			for (xi = 0; xi <= x; xi++) {
				putPixel(xc + xi, yc + y);
				putPixel(xc - xi, yc + y);
				putPixel(xc + xi, yc - y);
				putPixel(xc - xi, yc - y);
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
		while (y >= 0) {
			putPixel(xc + x, yc + y);
			putPixel(xc - x, yc + y);
			putPixel(xc + x, yc - y);
			putPixel(xc - x, yc - y);
			y--;
			if (d < 0) {
				x++;
				d = d - 2 * sqa * y - sqa + 2 * sqb * x + 2 * sqb;
			}
			else d = d - 2 * sqa * y - sqa;
		}
	}
}
void putNumber(int n, int x, int y, char lr) {
	int s[20], sn = 0;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

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
		if (n == 0)putChar('0', x-8, y);
		while (n > 0) {
			x -= 8;
			putChar((char)(n % 10 + '0'), x, y);
			n /= 10;
		}
	}
}
int maskImage(int left, int top, bitMap *mask, bitMap *bitmap) {
	int x1, x2, y1, y2, i, j;
	bitMap *buf;

	if (_sglMode != BIT_MAP && !_innerFunc)return SG_INVALID_MODE;

	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;

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
			if (mask->data[(mask->sizeX*i + j) * 3])
				buf->data[(buf->sizeX * (i + top) + left + j) * 3] = bitmap->data[(bitmap->sizeX*i + j) * 3];
			if (mask->data[(mask->sizeX*i + j) * 3 + 1])
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 1] = bitmap->data[(bitmap->sizeX*i + j) * 3 + 1];
			if (mask->data[(mask->sizeX*i + j) * 3 + 2])
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 2] = bitmap->data[(bitmap->sizeX*i + j) * 3 + 2];
		}
	}

	return SG_NO_ERORR;
}
void funcMap(int x1, int x2, int y1, int y2, float(*vect)(float x)) {
	int tmp = 0, pre, i, j;
	int addition;
	float y;

	if (_sglMode != BIT_MAP && !_innerFunc)return;

#define XCHG(a, b) {tmp = a; a = b; b = tmp;}

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

#undef XCHG

}
bitMap *copyPic(bitMap *src) {
	bitMap *dst = (bitMap*)malloc(src->sizeX*src->sizeY * 3 * sizeof(char));
	dst->sizeX = src->sizeX;
	dst->sizeY = src->sizeY;
	memcpy(dst->data, src->data, src->sizeX*src->sizeY * 3 * sizeof(char));
	return dst;
}
bitMap *grayPic(bitMap *src) {
	int i, j, v;
	bitMap *dst = (bitMap*)malloc(src->sizeX*src->sizeY * 3 * sizeof(char));
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
	bitMap *dst = (bitMap*)malloc(src->sizeX*src->sizeY * 3 * sizeof(char));
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
	bitMap *dst = (bitMap*)malloc((int)(src->sizeX * rate) * (int)(src->sizeY * rate) * 3 * sizeof(char));
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
	int i, j, v, pos;
	float weight, sum;
	bitMap *dst = (bitMap*)malloc(src->sizeX*src->sizeY * 3 * sizeof(char));
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
					for (int z = 0; z < 3; z++) {
						v = 0;
						for (int y = 0; y < 3; y++) {
							for (int x = 0; x < 3; x++) {
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
					for (int z = 0; z < 3; z++) {
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
		for (int i = 0; i < src->sizeY; i++) {
			for (int j = 0; j < src->sizeX; j++) {
				int pos = (i*src->sizeX + j) * 3;
				if (i == 0 || j == 0 || i == src->sizeY - 1 || j == src->sizeX - 1)
					memcpy(dst->data + pos, src->data + pos, 3 * sizeof(char));
				else {
					for (int z = 0; z < 3; z++) {
						sum = 0;
						weight = 0;
						for (int y = 0; y < 3; y++) {
							for (int x = 0; x < 3; x++) {
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
	bitMap *dst = (bitMap*)malloc(src->sizeX*src->sizeY * 3 * sizeof(char));
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


//Textmap interfaces.
void setBfc(int bgc, int fgc) {
	Screen->bfc = (bgc << 4) | fgc;
}
void setCharColor(char color, int x, int y) {
	int pos;

	pos = y * Screen->changed->width + x;
	Screen->changed->content[pos] &= 0x00FF;
	Screen->changed->content[pos] |= (color << 8);
}
void setCharBgc(char color, int x, int y) {
	int pos;

	pos = y * Screen->changed->width + x;
	Screen->changed->content[pos] &= 0x0FFF;
	Screen->changed->content[pos] |= ((color & 0x0F) << 12);
}
void setCharFgc(char color, int x, int y) {
	int pos;

	pos = y * Screen->changed->width + x;
	Screen->changed->content[pos] &= 0xF0FF;
	Screen->changed->content[pos] |= ((color & 0x0F) << 8);
}
void clearText() {
	int i;

	for (i = 0; i < Screen->changed->width*Screen->changed->height; i++) {
		writeChar(0, i%Screen->changed->width, i / Screen->changed->width);
	}
}
void writeChar(char c, int x, int y) {
	int pos;

	pos = y * Screen->changed->width + x;
	Screen->changed->content[pos] = ((short)Screen->bfc << 8) | (short)c;
}
void writeString(char *s, int x, int y) {
	int i;

	for (i = 0; s[i] != '\0'; i++) {
		writeChar(s[i], x++, y);
	}
}
int getShort(int x, int y) {
	return Screen->changed->content[y * Screen->changed->width + x];
}
int getText(int left, int top, int right, int bottom, textMap *text) {
	int i;

	if (right <left)left ^= right ^= left ^= right;
	if (bottom < top)top ^= bottom ^= top ^= bottom;

	if (right > Screen->changed->width - 1)right = Screen->changed->width - 1;
	if (left < 0)right = 0;
	if (bottom > Screen->changed->height - 1)right = Screen->changed->height - 1;
	if (top < 0)top = 0;

	text->width = right - left + 1;
	text->height = bottom - top + 1;
	text->content = (short*)malloc(text->width * text->height * sizeof(short));
	if (text->content == NULL)return SG_NO_LOAD_MEM;

	for (i = 0; i < text->height; i++) {
		memcpy(text->content + (i*text->width), Screen->changed->content + (top + i) * Screen->changed->width + left, text->width * sizeof(short));
	}

	return SG_NO_ERORR;
}
void putText(int left, int top, textMap *text) {
	int x1, x2, y1, y2, i;

	if (left >= Screen->changed->width)return;
	if (top >= Screen->changed->height)return;
	if (left + text->width < 0)return;
	if (top + text->height < 0)return;

	if (left < 0)x1 = 0;
	else x1 = left;
	if (top < 0)y1 = 0;
	else y1 = top;

	if (left + text->width >= Screen->changed->width)x2 = Screen->changed->width - 1;
	else x2 = left + text->width - 1;
	if (top + text->height >= Screen->changed->height)y2 = Screen->changed->height - 1;
	else y2 = top + text->height - 1;

	for (i = 0; i < text->height; i++) {
		memcpy(Screen->changed->content + (top + i) * Screen->changed->width + left, text->content + (i*text->width), text->width * sizeof(short));
	}
}
