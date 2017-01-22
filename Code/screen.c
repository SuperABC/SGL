#include "screen.h"
struct _screen *Screen;
struct _win *Window;
struct _key *Key;
struct _mouse *Mouse;
struct _vect *Vector;
clock_t _delayBegin, _delayEnd, _vectDelta = 55;
int _delaySE = 0;
int _vectKey = 0;
int _enMouse = 0, _enKey = 0;
int _activePage = 0, _visualPage = 0;
int _inLoop = 0;

vecTwo Q[SG_QSIZE];

GLubyte letters[][13] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x00
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x01
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x02
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x03
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x04
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x05
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x06
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x07
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x08
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x09
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x0A
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x0B
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x0C
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x0D
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x0E
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x0F
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x10
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x11
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x12
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x13
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x14
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x15
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x16
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x17
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x18
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x19
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x1A
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x1B
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x1C
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x1D
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x1E
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x1F
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//Space
	{ 0x00, 0x18, 0x3C, 0x3C, 0x3C, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00 },//!
	{ 0x00, 0x63, 0x63, 0x63, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // "
	{ 0x00, 0x00, 0x00, 0x36, 0x36, 0x7F, 0x36, 0x36, 0x36, 0x7F, 0x36, 0x36, 0x00 }, // #
	{ 0x0C, 0x0C, 0x3E, 0x63, 0x61, 0x60, 0x3E, 0x03, 0x03, 0x43, 0x63, 0x3E, 0x0C }, // $
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x63, 0x06, 0x0C, 0x18, 0x33, 0x63, 0x00 }, // %
	{ 0x00, 0x00, 0x00, 0x1C, 0x36, 0x36, 0x1C, 0x3B, 0x6E, 0x66, 0x66, 0x3B, 0x00 }, // &
	{ 0x00, 0x30, 0x30, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '
	{ 0x00, 0x00, 0x0C, 0x18, 0x18, 0x30, 0x30, 0x30, 0x30, 0x18, 0x18, 0x0C, 0x00 }, // (
	{ 0x00, 0x00, 0x18, 0x0C, 0x0C, 0x06, 0x06, 0x06, 0x06, 0x0C, 0x0C, 0x18, 0x00 }, // )
	{ 0x00, 0x00, 0x00, 0x00, 0x42, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x42, 0x00, 0x00 }, // *
	{ 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18, 0x00, 0x00 }, // +
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30 }, // ,
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00 }, // -
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00 }, // .
	{ 0x00, 0x00, 0x01, 0x03, 0x07, 0x0E, 0x1C, 0x38, 0x70, 0xE0, 0xC0, 0x80, 0x00 }, // / 
	{ 0x00, 0x00, 0x3E, 0x63, 0x63, 0x63, 0x6B, 0x6B, 0x63, 0x63, 0x63, 0x3E, 0x00 },//0 
	{ 0x00, 0x00, 0x0C, 0x1C, 0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00 },//1
	{ 0x00, 0x00, 0x3E, 0x63, 0x03, 0x06, 0x0C, 0x18, 0x30, 0x61, 0x63, 0x7F, 0x00 },//2
	{ 0x00, 0x00, 0x3E, 0x63, 0x03, 0x03, 0x1E, 0x03, 0x03, 0x03, 0x63, 0x3E, 0x00 },//3
	{ 0x00, 0x00, 0x06, 0x0E, 0x1E, 0x36, 0x66, 0x66, 0x7F, 0x06, 0x06, 0x0F, 0x00 },//4
	{ 0x00, 0x00, 0x7F, 0x60, 0x60, 0x60, 0x7E, 0x03, 0x03, 0x63, 0x73, 0x3E, 0x00 },//5
	{ 0x00, 0x00, 0x1C, 0x30, 0x60, 0x60, 0x7E, 0x63, 0x63, 0x63, 0x63, 0x3E, 0x00 },//6
	{ 0x00, 0x00, 0x7F, 0x63, 0x03, 0x06, 0x06, 0x0C, 0x0C, 0x18, 0x18, 0x18, 0x00 },//7
	{ 0x00, 0x00, 0x3E, 0x63, 0x63, 0x63, 0x3E, 0x63, 0x63, 0x63, 0x63, 0x3E, 0x00 },//8
	{ 0x00, 0x00, 0x3E, 0x63, 0x63, 0x63, 0x63, 0x3F, 0x03, 0x03, 0x06, 0x3C, 0x00 },//9
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00 }, // :
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30 }, // ;
	{ 0x00, 0x00, 0x00, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x00 }, // <
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00 }, // =
	{ 0x00, 0x00, 0x00, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x00 }, // >
	{ 0x00, 0x00, 0x3E, 0x63, 0x63, 0x06, 0x0C, 0x0C, 0x0C, 0x00, 0x0C, 0x0C, 0x00 }, // ?
	{ 0x00, 0x00, 0x3E, 0x63, 0x63, 0x6F, 0x6B, 0x6B, 0x6E, 0x60, 0x60, 0x3E, 0x00 }, // @
	{ 0x18, 0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0x00, 0x00 },//A
	{ 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0x00, 0x00 },//B
	{ 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e, 0x00, 0x00 },//C
	{ 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc, 0x00, 0x00 },//D
	{ 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0x00, 0x00 },//E
	{ 0xff, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x00 },//F
	{ 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xcf, 0xc3, 0xc3, 0xe7, 0x7e, 0x00, 0x00 },//G
	{ 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x00, 0x00 },//H
	{ 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e, 0x00, 0x00 },//I
	{ 0x7c, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0xee, 0x7c, 0x00, 0x00 },//J
	{ 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3, 0x00, 0x00 },//K
	{ 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0x00, 0x00 },//L
	{ 0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x00, 0x00 },//M
	{ 0xe3, 0xe3, 0xf3, 0xf3, 0xfb, 0xdb, 0xdf, 0xcf, 0xcf, 0xc7, 0xc7, 0x00, 0x00 },//N
	{ 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e, 0x00, 0x00 },//O
	{ 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x00 },//P
	{ 0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xdf, 0x6e, 0x3f, 0x00, 0x00 },//Q
	{ 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3, 0x00, 0x00 },//R
	{ 0x7e, 0xe7, 0xc0, 0xc0, 0xe0, 0x7e, 0x07, 0x03, 0x03, 0xe7, 0x7e, 0x00, 0x00 },//S
	{ 0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00 },//T
	{ 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e, 0x00, 0x00 },//U
	{ 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x3c, 0x3c, 0x18, 0x00, 0x00 },//V
	{ 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xdb, 0xff, 0xff, 0xe7, 0xc3, 0x00, 0x00 },//W
	{ 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0x00, 0x00 },//X
	{ 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00 },//Y
	{ 0xff, 0x03, 0x03, 0x06, 0x0c, 0x7e, 0x30, 0x60, 0xc0, 0xc0, 0xff, 0x00, 0x00 },//Z
	{ 0x00, 0x00, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00 }, // [
	{ 0x00, 0x00, 0x80, 0xC0, 0xE0, 0x70, 0x38, 0x1C, 0x0E, 0x07, 0x03, 0x01, 0x00 }, // \'
	{ 0x00, 0x00, 0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, 0x00 }, // ]
	{ 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ^
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF }, // _
	{ 0x18, 0x18, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // `
	{ 0x00, 0x00, 0x00, 0x00, 0x3C, 0x46, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00, 0x00 }, // a
	{ 0x00, 0x70, 0x30, 0x30, 0x3C, 0x36, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00, 0x00 }, // b
	{ 0x00, 0x00, 0x00, 0x00, 0x3E, 0x63, 0x60, 0x60, 0x60, 0x63, 0x3E, 0x00, 0x00 }, // c
	{ 0x00, 0x0E, 0x06, 0x06, 0x1E, 0x36, 0x66, 0x66, 0x66, 0x66, 0x3B, 0x00, 0x00 }, // d
	{ 0x00, 0x00, 0x00, 0x00, 0x3E, 0x63, 0x63, 0x7E, 0x60, 0x63, 0x3E, 0x00, 0x00 }, // e
	{ 0x00, 0x1C, 0x36, 0x32, 0x30, 0x7C, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00, 0x00 }, // f
	{ 0x00, 0x00, 0x00, 0x00, 0x3B, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x66, 0x3C }, // g
	{ 0x00, 0x70, 0x30, 0x30, 0x36, 0x3B, 0x33, 0x33, 0x33, 0x33, 0x73, 0x00, 0x00 }, // h
	{ 0x00, 0x0C, 0x0C, 0x00, 0x1C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00, 0x00 }, // i
	{ 0x00, 0x06, 0x06, 0x00, 0x0E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3C }, // j
	{ 0x00, 0x70, 0x30, 0x30, 0x33, 0x33, 0x36, 0x3C, 0x36, 0x33, 0x73, 0x00, 0x00 }, // k
	{ 0x00, 0x1C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00, 0x00 }, // l
	{ 0x00, 0x00, 0x00, 0x00, 0x6E, 0x7F, 0x6B, 0x6B, 0x6B, 0x6B, 0x6B, 0x00, 0x00 }, // m
	{ 0x00, 0x00, 0x00, 0x00, 0x6E, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x00, 0x00 }, // n
	{ 0x00, 0x00, 0x00, 0x00, 0x3E, 0x63, 0x63, 0x63, 0x63, 0x63, 0x3E, 0x00, 0x00 }, // o
	{ 0x00, 0x00, 0x00, 0x00, 0x6E, 0x33, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x30, 0x78 }, // p
	{ 0x00, 0x00, 0x00, 0x00, 0x3B, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F }, // q
	{ 0x00, 0x00, 0x00, 0x00, 0x6E, 0x3B, 0x33, 0x30, 0x30, 0x30, 0x78, 0x00, 0x00 }, // r
	{ 0x00, 0x00, 0x00, 0x00, 0x3E, 0x63, 0x38, 0x0E, 0x03, 0x63, 0x3E, 0x00, 0x00 }, // s
	{ 0x00, 0x08, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x1B, 0x0E, 0x00, 0x00 }, // t
	{ 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3B, 0x00, 0x00 }, // u
	{ 0x00, 0x00, 0x00, 0x00, 0x63, 0x63, 0x36, 0x36, 0x1C, 0x1C, 0x08, 0x00, 0x00 }, // v
	{ 0x00, 0x00, 0x00, 0x00, 0x63, 0x63, 0x63, 0x6B, 0x6B, 0x7F, 0x36, 0x00, 0x00 }, // w
	{ 0x00, 0x00, 0x00, 0x00, 0x63, 0x36, 0x1C, 0x1C, 0x1C, 0x36, 0x63, 0x00, 0x00 }, // x
	{ 0x00, 0x00, 0x00, 0x00, 0x63, 0x63, 0x63, 0x63, 0x63, 0x3F, 0x03, 0x06, 0x3C }, // y
	{ 0x00, 0x00, 0x00, 0x00, 0x7F, 0x66, 0x0C, 0x18, 0x30, 0x63, 0x7F, 0x00, 0x00 }, // z
	{ 0x00, 0x00, 0x0E, 0x18, 0x18, 0x18, 0x70, 0x18, 0x18, 0x18, 0x18, 0x0E, 0x00 }, // {
	{ 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18 }, // |
	{ 0x00, 0x00, 0x70, 0x18, 0x18, 0x18, 0x0E, 0x18, 0x18, 0x18, 0x18, 0x70, 0x00 }, // }
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//~
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },//0x7F
};

static void sgInit();
static void sgReshape(int w, int h);
static void sgDisplay(void);
static void sgIdle(void);
static void sgMouse(int x, int y);
static void sgDrag(int x, int y);
static void sgClick(int button, int state, int x, int y);
static void sgWheel(int wheel, int dir, int x, int y);
static void sgKeyUp(unsigned char cAscii, int x, int y);
static void sgKey(unsigned char cAscii, int x, int y);
static void sgSpecialUp(GLint cAscii, int x, int y);
static void sgSpecial(GLint cAscii, int x, int y);
void makeFont(void);
void vectDefault(void);

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_MULTISAMPLE);
	sgInit();

	glutDisplayFunc(sgDisplay);
	glutReshapeFunc(sgReshape);
	glutPassiveMotionFunc(sgMouse);
	glutMotionFunc(sgDrag);
	glutMouseFunc(sgClick);
	glutMouseWheelFunc(sgWheel);
	glutKeyboardFunc(sgKey);
	glutKeyboardUpFunc(sgKeyUp);
	glutSpecialFunc(sgSpecial);
	glutSpecialUpFunc(sgSpecialUp);
	glutIdleFunc(sgIdle);
	_inLoop = 1;
	glutMainLoop();
}

static void sgInit() {
	//init the global structs.
	Screen = (struct _screen*)malloc(sizeof(struct _screen));
	Window = (struct _win*)malloc(sizeof(struct _win));
	Key = (struct _key*)malloc(sizeof(struct _key));
	Mouse = (struct _mouse*)malloc(sizeof(struct _mouse));
	Vector = (struct _vect*)malloc(sizeof(struct _vect));

	Key->front = -1;
	Key->rear = 0;

	Mouse->front = -1;
	Mouse->rear = 0;
	Mouse->left = 0;
	Mouse->right = 0;
	Mouse->middle = 0;
	Mouse->clicked = 0;
	Mouse->coord = 0;

	Vector->_v8 = (vect)vectDefault;
	Vector->_v9 = (vect)vectDefault;

	Window->winName = (char *)malloc(256 * sizeof(char));
	Window->winWidth = 640;
	Window->winHeight = 480;
	strcpy(Window->winName, "SGpro");

	//hello words.
	printf("Welcome to SGL which based on Black White's Dosbox.\n");
	delay(200);
	printf("Author: Super GP.\n");
	delay(200);
	printf("If you want to contact us, search QQ number 2168331489.\n");
	delay(200);
	printf("Having a wonderful programming day!");
	delay(500);

	//users' setup.
	sgSetup();

	//build the buffer.
	Screen->buffer1 = (bitMap *)malloc(sizeof(bitMap));
	Screen->buffer1->sizeX = Window->winWidth;
	Screen->buffer1->sizeY = Window->winHeight;
	Screen->buffer1->data = (unsigned char *)malloc(Window->winWidth * Window->winHeight * 3 * sizeof(char));
	memset(Screen->buffer1->data, -1, Window->winWidth * Window->winHeight * 3 * sizeof(char));
	Screen->buffer2 = (bitMap *)malloc(sizeof(bitMap));
	Screen->buffer2->sizeX = Window->winWidth;
	Screen->buffer2->sizeY = Window->winHeight;
	Screen->buffer2->data = (unsigned char *)malloc(Window->winWidth * Window->winHeight * 3 * sizeof(char));
	memset(Screen->buffer2->data, -1, Window->winWidth * Window->winHeight * 3 * sizeof(char));

	//create a window.
	int winPx, winPy;
	glutInitWindowSize(Window->winWidth, Window->winHeight);
	if (Window->winWidth >= 1280)winPx = 0;
	else winPx = 640 - Window->winWidth / 2;
	if (Window->winHeight >= 720)winPy = 0;
	else winPy = 360 - Window->winHeight / 2;
	glutInitWindowPosition(winPx, winPy);
	glutCreateWindow(Window->winName);

	//init texture.
	makeFont();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &Screen->texName);
	glBindTexture(GL_TEXTURE_2D, Screen->texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

}
static void sgDisplay(void) {
	if(_visualPage==0)glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Screen->buffer1->sizeX, Screen->buffer1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Screen->buffer1->data);
	else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Screen->buffer2->sizeX, Screen->buffer2->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Screen->buffer2->data);

	glLoadIdentity();
	glScalef((GLfloat)0.003123, (GLfloat)0.004163, (GLfloat)1.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex3f(-320, -240, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-320, 240, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(320, 240, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(320, -240, 0.0);
	glEnd();

	glFlush();
}
static void sgReshape(int w, int h) {
	Window->winWidth = w;
	Window->winHeight = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
	sgDisplay();
}
static void sgMouse(int x, int y) {
	bitMap *buf;
	if (!_enMouse)return;
	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;
	Mouse->Pos.x = x*buf->sizeX / Window->winWidth;
	Mouse->Pos.y = y*buf->sizeY / Window->winHeight;
}
static void sgDrag(int x, int y) {
	bitMap *buf;
	if (!_enMouse)return;
	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;
	Mouse->Pos.x = x*buf->sizeX / Window->winWidth;
	Mouse->Pos.y = y*buf->sizeY / Window->winHeight;
}
static void sgClick(int button, int state, int x, int y) {
	bitMap *buf;
	if (!_enMouse)return;
	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		Mouse->left = SG_BUTTON_DOWN;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		Mouse->left = SG_BUTTON_UP;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].x = x*buf->sizeX / Window->winWidth;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].y = y*buf->sizeY / Window->winHeight;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].m = SG_LEFT_BUTTON;
		Mouse->front = (Mouse->front + 1) % 1024;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		Mouse->right = SG_BUTTON_DOWN;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		Mouse->right = SG_BUTTON_UP;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].x = x*buf->sizeX / Window->winWidth;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].y = y*buf->sizeY / Window->winHeight;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].m = SG_RIGHT_BUTTON;
		Mouse->front = (Mouse->front + 1) % 1024;
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		Mouse->middle = SG_BUTTON_DOWN;
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) {
		Mouse->middle = SG_BUTTON_UP;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].x = x*buf->sizeX / Window->winWidth;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].y = y*buf->sizeY / Window->winHeight;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].m = SG_MIDDLE_BUTTON;
		Mouse->front = (Mouse->front + 1) % 1024;
	}
}
static void sgWheel(int wheel, int dir, int x, int y) {
	bitMap *buf;
	if (!_enMouse)return;
	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;
	if (dir > 0) {
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].x = x*buf->sizeX / Window->winWidth;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].y = y*buf->sizeY / Window->winHeight;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].m = SG_MIDDLE_BUTTON_UP;
		Mouse->front = (Mouse->front + 1) % 1024;
	}
	else if (dir < 0) {
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].x = x*buf->sizeX / Window->winWidth;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].y = y*buf->sizeY / Window->winHeight;
		Mouse->mouseBuf[(Mouse->front + 1) % 1024].m = SG_MIDDLE_BUTTON_DOWN;
		Mouse->front = (Mouse->front + 1) % 1024;
	}
}
static void sgKey(unsigned char cAscii, int x, int y) {
	if (!_enKey)return;
	Key->front = (Key->front + 1) % 1024;
	Key->keyBuf[Key->front] = (word)cAscii;
	Key->keyBuf[Key->front] |= (word)0x8000;
	_vectKey = cAscii;
	(*Vector->_v9)();
}
static void sgSpecial(GLint cAscii, int x, int y) {
	if (!_enKey)return;
	Key->front = (Key->front + 1) % 1024;
	switch (cAscii) {
	case GLUT_KEY_UP:
		cAscii = SG_UP;
		break;
	case GLUT_KEY_DOWN:
		cAscii = SG_DOWN;
		break;
	case GLUT_KEY_LEFT:
		cAscii = SG_LEFT;
		break;
	case GLUT_KEY_RIGHT:
		cAscii = SG_RIGHT;
		break;
	case GLUT_KEY_CTRL_L:
		cAscii = SG_LCTRL;
		break;
	case GLUT_KEY_CTRL_R:
		cAscii = SG_RCTRL;
		break;
	case GLUT_KEY_ALT_L:
		cAscii = SG_LALT;
		break;
	case GLUT_KEY_ALT_R:
		cAscii = SG_RALT;
		break;
	case GLUT_KEY_SHIFT_L:
		cAscii = SG_LSHIFT;
		break;
	case GLUT_KEY_SHIFT_R:
		cAscii = SG_RSHIFT;
		break;
	case GLUT_KEY_INSERT:
		cAscii = SG_INSERT;
		break;
	case GLUT_KEY_DELETE:
		cAscii = SG_DELETE;
		break;
	case GLUT_KEY_HOME:
		cAscii = SG_HOME;
		break;
	case GLUT_KEY_END:
		cAscii = SG_END;
		break;
	case GLUT_KEY_NUM_LOCK:
		cAscii = SG_NUMLOC;
		break;
	case GLUT_KEY_PAGE_UP:
		cAscii = SG_PAGEUP;
		break;
	case GLUT_KEY_PAGE_DOWN:
		cAscii = SG_PAGEDOWN;
		break;
	case GLUT_KEY_BEGIN:
		cAscii = SG_BEGIN;
		break;
	case GLUT_KEY_F1:
		cAscii = SG_F1;
		break;
	case GLUT_KEY_F2:
		cAscii = SG_F2;
		break;
	case GLUT_KEY_F3:
		cAscii = SG_F3;
		break;
	case GLUT_KEY_F4:
		cAscii = SG_F4;
		break;
	case GLUT_KEY_F5:
		cAscii = SG_F5;
		break;
	case GLUT_KEY_F6:
		cAscii = SG_F6;
		break;
	case GLUT_KEY_F7:
		cAscii = SG_F7;
		break;
	case GLUT_KEY_F8:
		cAscii = SG_F8;
		break;
	case GLUT_KEY_F9:
		cAscii = SG_F9;
		break;
	case GLUT_KEY_F10:
		cAscii = SG_F10;
		break;
	case GLUT_KEY_F11:
		cAscii = SG_F11;
		break;
	case GLUT_KEY_F12:
		cAscii = SG_F12;
		break;
	}
	Key->keyBuf[Key->front] = (word)cAscii;
	Key->keyBuf[Key->front] = Key->keyBuf[Key->front]|0x8000;
	_vectKey = cAscii;
	(*Vector->_v9)();
}
static void sgKeyUp(unsigned char cAscii, int x, int y) {
	if (!_enKey)return;
	Key->front = (Key->front + 1) % 1024;
	Key->keyBuf[Key->front] = (word)cAscii;
	Key->keyBuf[Key->front] &= (word)0x7fff;
}
static void sgSpecialUp(GLint cAscii, int x, int y) {
	if (!_enKey)return;
	Key->front = (Key->front+1)%1024;
	switch (cAscii) {
	case GLUT_KEY_UP:
		cAscii = SG_UP;
		break;
	case GLUT_KEY_DOWN:
		cAscii = SG_DOWN;
		break;
	case GLUT_KEY_LEFT:
		cAscii = SG_LEFT;
		break;
	case GLUT_KEY_RIGHT:
		cAscii = SG_RIGHT;
		break;
	case GLUT_KEY_CTRL_L:
		cAscii = SG_LCTRL;
		break;
	case GLUT_KEY_CTRL_R:
		cAscii = SG_RCTRL;
		break;
	case GLUT_KEY_ALT_L:
		cAscii = SG_LALT;
		break;
	case GLUT_KEY_ALT_R:
		cAscii = SG_RALT;
		break;
	case GLUT_KEY_SHIFT_L:
		cAscii = SG_LSHIFT;
		break;
	case GLUT_KEY_SHIFT_R:
		cAscii = SG_RSHIFT;
		break;
	case GLUT_KEY_INSERT:
		cAscii = SG_INSERT;
		break;
	case GLUT_KEY_DELETE:
		cAscii = SG_DELETE;
		break;
	case GLUT_KEY_HOME:
		cAscii = SG_HOME;
		break;
	case GLUT_KEY_END:
		cAscii = SG_END;
		break;
	case GLUT_KEY_NUM_LOCK:
		cAscii = SG_NUMLOC;
		break;
	case GLUT_KEY_PAGE_UP:
		cAscii = SG_PAGEUP;
		break;
	case GLUT_KEY_PAGE_DOWN:
		cAscii = SG_PAGEDOWN;
		break;
	case GLUT_KEY_BEGIN:
		cAscii = SG_BEGIN;
		break;
	case GLUT_KEY_F1:
		cAscii = SG_F1;
		break;
	case GLUT_KEY_F2:
		cAscii = SG_F2;
		break;
	case GLUT_KEY_F3:
		cAscii = SG_F3;
		break;
	case GLUT_KEY_F4:
		cAscii = SG_F4;
		break;
	case GLUT_KEY_F5:
		cAscii = SG_F5;
		break;
	case GLUT_KEY_F6:
		cAscii = SG_F6;
		break;
	case GLUT_KEY_F7:
		cAscii = SG_F7;
		break;
	case GLUT_KEY_F8:
		cAscii = SG_F8;
		break;
	case GLUT_KEY_F9:
		cAscii = SG_F9;
		break;
	case GLUT_KEY_F10:
		cAscii = SG_F10;
		break;
	case GLUT_KEY_F11:
		cAscii = SG_F11;
		break;
	case GLUT_KEY_F12:
		cAscii = SG_F12;
		break;
	}
	Key->keyBuf[Key->front] = (word)cAscii;
	Key->keyBuf[Key->front] &= (word)0x7fff;
}
static void sgIdle() {
	static clock_t proTime = 0, tmpTime;
	if ((tmpTime = clock()) - proTime > _vectDelta) {
		(*Vector->_v8)();
		proTime = tmpTime;
	}
	sgLoop();
	sgDisplay();
}
void makeFont() {
	int tmp;
#define swap(a, b) tmp = a; a = b; b = tmp;
	for (int i = 0; i < 127; i++) {
		swap(letters[i][0], letters[i][12]);
		swap(letters[i][1], letters[i][11]);
		swap(letters[i][2], letters[i][10]);
		swap(letters[i][3], letters[i][9]);
		swap(letters[i][4], letters[i][8]);
		swap(letters[i][5], letters[i][7]);
	}
#undef swap
}
void vectDefault(void) {
	return;
}

void setColor(int r, int g, int b) {
	Screen->rgb[0] = r % 256;
	Screen->rgb[1] = g % 256;
	Screen->rgb[2] = b % 256;
}
void clearScreen() {
	bitMap *buf;
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
	for (int i = 0; i < 3 * buf->sizeX*buf->sizeY; i += 3) {
		buf->data[i] = Screen->rgb[0];
		buf->data[i + 1] = Screen->rgb[1];
		buf->data[i + 2] = Screen->rgb[2];
	}
}
void putPixel(int x, int y) {
	bitMap *buf;
	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;
	if (x < 0 || x >= buf->sizeX || y < 0 || y >= buf->sizeY)return;
	int p = (y * buf->sizeX + x) * 3;
	buf->data[p] = Screen->rgb[0];
	buf->data[p + 1] = Screen->rgb[1];
	buf->data[p + 2] = Screen->rgb[2];
}
RGB getPixel(int x, int y) {
	bitMap *buf;
	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;
	int p = (y*buf->sizeX + x) * 3;
	RGB ret;
	ret.r = buf->data[p++];
	ret.g = buf->data[p++];
	ret.b = buf->data[p++];
	return ret;
}
void putQuad(int x1, int y1, int x2, int y2, int mode) {
	if(mode==SOLID_FILL)
		for (int i = x1; i <= x2; i++)
			for (int j = y1; j <= y2; j++)
				putPixel(i, j);
	if (mode == EMPTY_FILL) {
		for (int i = x1; i <= x2; i++) {
			putPixel(i, y1);
			putPixel(i, y2);
		}
		for (int j = y1; j <= y2; j++) {
			putPixel(x1, j);
			putPixel(x2, j);
		}
	}
}
int loadBmp(int x, int y, char *filename) {
	FILE *fp = NULL;
	SGstring p = NULL, tmp = NULL;
	GLint width, height;
	dword dataOffset, lineBytes;
	dword lines;
	bitMap *buf;
	byte *vp;
	char buffer = 0;

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
	for (int i = height - 1; i >= 0; i--) {
		fread(p, 1, lineBytes, fp);
		lines = i * buf->sizeX * 3;
		tmp = p;
		for (int j = 0; j < width; j++) {
			buffer = *tmp;
			*tmp = *(tmp + 2);
			*(tmp + 2) = buffer;
			tmp += 3;
		}
		if (x + width > buf->sizeX)
			memcpy(vp + lines, p, (buf->sizeX - x) * 3);
		else memcpy(vp + lines, p, width * 3);
	}
	free(p);
	fclose(fp);
	return 1;
displayError:
	if (p != NULL)free(p);
	if (fp != NULL)fclose(fp);
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
	if (cmd == 1)return (Key->front + 1) % 1024 != Key->rear;
	if (cmd != 0)return 0;
	while ((Key->front + 1) % 1024 == Key->rear);
	word ret = 0;
	ret = Key->keyBuf[Key->rear++];
	Key->rear %= 1024;
	return ret;
}
void clearKeyBuffer() {
	Key->rear = (Key->front + 1) % 1024;
}
int loadWave(char *filename, int mode) {
	FILE *fp = NULL;
	fp = fopen(filename, "rb");
	if (fp == NULL)return 0;
	fclose(fp);
	int rt;
	LPWSTR rs;
	rt = MultiByteToWideChar(CP_ACP, 0, filename, -1, NULL, 0);
	rs = (LPWSTR)malloc(rt * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, filename, -1, rs, rt * sizeof(wchar_t));
	PlaySound(rs, NULL, SND_FILENAME | SND_ASYNC | mode);
	return 1;
}
void delay(int t) {
	clock_t st, en;
	if (_inLoop)sgDisplay();
	st = clock();
	en = clock();
	while (en - st < t) {
		en = clock();
	}
}
void delayBegin() {
	_delayBegin = clock();
	_delaySE = 1;
}
int delayEnd(int t) {
	if (!_delaySE)return 0;
	if (_inLoop)sgDisplay();
	_delayEnd = clock();
	while (_delayEnd - _delayBegin < t) {
		_delayEnd = clock();
	}
	_delaySE = 0;
	return 1;
}
void putString(char *str, int x, int y) {
	int len = strlen(str);
	for (int i = 0; i < len; i++)
		for (int j = 0; j < 13; j++)
			for (int k = 7; k >= 0; k--)
				if ((letters[str[i]][j] >> k) & 1)putPixel(x + i * 10 + 7 - k, y + 16 - j);
}
void putChar(char ch, int x, int y) {
	for (int j = 0; j < 13; j++)
		for (int k = 7; k >= 0; k--)
			if ((letters[ch][j] >> k) & 1)putPixel(x + 7 - k, y + 16 - j);
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
void fullScreen() {
	glutFullScreen();
}
void showMouse() {
	ShowCursor(TRUE);
}
void hideMouse() {
	ShowCursor(FALSE);
}
void setActivePage(int page) {
	if (page != 0 && page != 1)return;
	_activePage = page;
}
void setVisualPage(int page) {
	if (page != 0 && page != 1)return;
	_visualPage = page;
}
void getImage(int left, int top, int right, int bottom, bitMap *bitmap) {
	int deltaX, deltaY;
	bitMap *buf;
	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;

	if (right > buf->sizeX)right = buf->sizeX;
	if (left < 0)left = 0;
	if (bottom > buf->sizeY)bottom = buf->sizeY;
	if (top < 0)top = 0;

	if (right >= left)deltaX = right - left + 1;
	else deltaX = left - right;
	if (bottom>top)deltaY = bottom - top + 1;
	else deltaY = top - bottom;

	bitmap->sizeX = deltaX;
	bitmap->sizeY = deltaY;
	bitmap->data = (byte*)malloc(deltaX * deltaY * 3 * sizeof(byte));

	for (int i = 0; i < deltaY; i++) {
		memcpy(bitmap->data + bitmap->sizeX * i * 3, buf->data + (buf->sizeX * (i + top) + left) * 3, deltaX * 3 * sizeof(char));
	}
}
void putImage(int left, int top, bitMap *bitmap, int op) {
	bitMap *buf;
	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;
	for (int i = 0; i < bitmap->sizeY; i++) {
		memcpy(buf->data + (buf->sizeX * (i + top) + left) * 3, bitmap->data + bitmap->sizeX * i * 3, bitmap->sizeX * 3 * sizeof(char));
	}
}
void floodFill(int x, int y, RGB c) {
	vecTwo tmp;
	int front = -1, rear = 0, p;
	bitMap *buf;
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
		if ((buf->data[p] == c.r&&buf->data[p + 1] == c.g&&buf->data[p + 2] == c.b) || (buf->data[p] == Screen->rgb[0] && buf->data[p + 1] == Screen->rgb[1] && buf->data[p + 2] == Screen->rgb[2]))continue;
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
#define ABS(x) ((x > 0) ? (x) : (-x))
	int dx = x2 - x1, dy = y2 - y1;
	int ux = ((dx > 0) << 1) - 1, uy = ((dy > 0) << 1) - 1;
	int x = x1, y = y1, eps;
	eps = 0; dx = ABS(dx); dy = ABS(dy);
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
	int x = 0, y = r, yi, d = 3 - 2 * r;
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
void funcMap(int x1, int x2, int y1, int y2, float(*vect)(int x)) {
#define XCHG(a, b) {tmp = a; a = b; b = tmp;}
	int tmp = 0;
	float y;
	if (x1 > x2)XCHG(x1, x2);
	if (y1 > y2)XCHG(y1, y2);
	for (int i = x1; i <= x2; i++) {
		y = vect(i);
		if (y<y1 || y>y2)continue;
		if (y - (int)y > 0.5)putPixel(i, (int)(y + 1));
		else putPixel(i, (int)y);
	}
#undef XCHG
}
void putNumber(int n, int x, int y, char lr) {
	int s[20], sn = 0;
	if (lr == 'l') {
		while (n > 0) {
			s[sn++] = n % 10;
			n /= 10;
		}
		while (sn > 0) {
			putChar(s[--sn] + '0', x, y);
			x += 8;
		}
	}
	else if (lr == 'r') {
		while (n > 0) {
			x -= 8;
			putChar(n % 10 + '0', x, y);
			n /= 10;
		}
	}
}
