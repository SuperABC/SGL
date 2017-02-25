#include "sgl.h"
#include "inner.h"

struct _win *Window;
static struct _screen *Screen;
static struct _key *Key;
static struct _mouse *Mouse;
static struct _vect *Vector;

int _sglMode, _innerFunc;
int _delaySE = 0;
clock_t _delayBegin, _delayEnd, _vectDelta = 55;
int _vectKey = 0;
int _enMouse = 0, _enKey = 0;
int _activePage = 0, _visualPage = 0;
int _inLoop = 0, _full = 0, _hide = 0;

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
	if (_full)glutFullScreen();
	glutMainLoop();
}

//Register functions.
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
	Window->txtWidth = 80;
	Window->txtHeight = 25;
	_sglMode = BIT_MAP;
	_innerFunc = 0;
	strcpy(Window->winName, "SGpro");

	//hello words.
	printf("Welcome to SGL which based on Black White's Dosbox.\n");
	printf("Author: Super GP.\n");
	printf("If you want to contact us, search QQ number 2168331489.\n");
	printf("Having a wonderful programming day!");

	//users' setup.
	sgSetup();

	//build the buffer.
	Screen->content = (textMap *)malloc(sizeof(textMap));
	Screen->content->width = Window->txtWidth;
	Screen->content->height = Window->txtHeight;
	Screen->content->content = (unsigned short *)malloc(Window->txtWidth * Window->txtHeight * sizeof(unsigned short));
	memset(Screen->content->content, 0, Window->txtWidth*Window->txtHeight * sizeof(unsigned short));
	for (int i = 0; i < Window->txtWidth*Window->txtHeight; i++)Screen->content->content[i] |= 0xF000;
	Screen->changed = (textMap *)malloc(sizeof(textMap));
	Screen->changed->width = Window->txtWidth;
	Screen->changed->height = Window->txtHeight;
	Screen->changed->content = (unsigned short *)malloc(Window->txtWidth * Window->txtHeight * sizeof(unsigned short));
	memset(Screen->changed->content, 0, Window->txtWidth*Window->txtHeight * sizeof(unsigned short));
	for (int i = 0; i < Window->txtWidth*Window->txtHeight; i++)Screen->changed->content[i] |= 0xF000;

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
	Screen->bfc = WHITE << 4 | BLACK;
	
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
	if (_visualPage == 0)glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Screen->buffer1->sizeX, Screen->buffer1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Screen->buffer1->data);
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
	Key->keyBuf[Key->front] = Key->keyBuf[Key->front] | 0x8000;
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
	Key->keyBuf[Key->front] &= (word)0x7fff;
}
static void sgIdle() {
	static clock_t proTime = 0, tmpTime;
	if ((tmpTime = clock()) - proTime > _vectDelta) {
		(*Vector->_v8)();
		proTime = tmpTime;
	}
	sgLoop();
	if (_sglMode == TEXT_MAP)textLoop();
	sgDisplay();
}

//Inner functions.
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
void textLoop() {
	for (int i = 0; i < Screen->changed->width * Screen->changed->height; i++) {
		if (Screen->changed->content[i] != Screen->content->content[i]) {
			Screen->content->content[i] = Screen->changed->content[i];
			textRenew(Screen->content->content[i], i % Screen->changed->width, i / Screen->changed->width);
		}
	}
}
void textRenew(short c, int x, int y) {
	_innerFunc = 1;
	RGB b = bgc(c), f = fgc(c);
	setColor(b.r, b.g, b.b);
	putQuad(x * 8, y * 16, x * 8 + 7, y * 16 + 15, SOLID_FILL);
	setColor(f.r, f.g, f.b);
	putChar((char)c, x * 8, y * 16);
	_innerFunc = 0;
}
RGB bgc(short c) {
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
RGB fgc(short c) {
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

//Interfaces.
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
	}
	strcpy(Window->winName, title);
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
	if (fp == NULL)return SG_FILE_NOT_FOUND;
	fclose(fp);
	int rt;
	LPWSTR rs;
	rt = MultiByteToWideChar(CP_ACP, 0, filename, -1, NULL, 0);
	rs = (LPWSTR)malloc(rt * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, filename, -1, rs, rt * sizeof(wchar_t));
	PlaySound(rs, NULL, SND_FILENAME | SND_ASYNC | mode);
	return SG_NO_ERORR;
}
void delay(int t) {
	clock_t st, en;
	if (_inLoop) {
		if (_sglMode == TEXT_MAP)textLoop();
		sgDisplay();
	}
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
	if (_inLoop) {
		if (_sglMode == TEXT_MAP)textLoop();
		sgDisplay();
	}
	_delayEnd = clock();
	while (_delayEnd - _delayBegin < t) {
		_delayEnd = clock();
	}
	_delaySE = 0;
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
void fullScreen() {
	_full = 1;
}
void showMouse() {
	while (ShowCursor(TRUE) < 0)ShowCursor(TRUE);
}
void hideMouse() {
	while (ShowCursor(FALSE) >= 0)ShowCursor(FALSE);
}
void setMouse(int x, int y) {
	SetCursorPos(x, y);
}
void setActivePage(int page) {
	if (page != 0 && page != 1)return;
	_activePage = page;
}
void setVisualPage(int page) {
	if (page != 0 && page != 1)return;
	_visualPage = page;
}

//Bitmap functions.
void setColor(int r, int g, int b) {
	if (_sglMode != BIT_MAP && !_innerFunc)return;
	Screen->rgb[0] = r % 256;
	Screen->rgb[1] = g % 256;
	Screen->rgb[2] = b % 256;
}
void clearScreen() {
	if (_sglMode != BIT_MAP && !_innerFunc)return;
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
int putPixel(int x, int y) {
	if (_sglMode != BIT_MAP && !_innerFunc)return SG_INVALID_MODE;
	bitMap *buf;
	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;
	if (x < 0 || x >= buf->sizeX || y < 0 || y >= buf->sizeY)return SG_OUT_OF_RANGE;
	int p = (y * buf->sizeX + x) * 3;
	buf->data[p] = Screen->rgb[0];
	buf->data[p + 1] = Screen->rgb[1];
	buf->data[p + 2] = Screen->rgb[2];
	return SG_NO_ERORR;
}
RGB getPixel(int x, int y) {
	if (_sglMode != BIT_MAP && !_innerFunc) {
		RGB im = { SG_INVALID_MODE , SG_INVALID_MODE , SG_INVALID_MODE };
		return im;
	}
	bitMap *buf;
	if (_activePage == 0) buf = Screen->buffer1;
	else buf = Screen->buffer2;

	RGB ret = { SG_OUT_OF_RANGE, SG_OUT_OF_RANGE, SG_OUT_OF_RANGE };
	if (x < 0 || x >= buf->sizeX || y < 0 || y >= buf->sizeY)return ret;
	int p = (y*buf->sizeX + x) * 3;
	ret.r = buf->data[p++];
	ret.g = buf->data[p++];
	ret.b = buf->data[p++];
	return ret;
}
void putQuad(int x1, int y1, int x2, int y2, int mode) {
	if (_sglMode != BIT_MAP && !_innerFunc)return;
	if (mode == SOLID_FILL)
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
	if (_sglMode != BIT_MAP && !_innerFunc)return SG_INVALID_MODE;
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
void putString(char *str, int x, int y) {
	if (_sglMode != BIT_MAP && !_innerFunc)return;
	int len = strlen(str);
	for (int i = 0; i < len; i++)
		for (int j = 0; j < 13; j++)
			for (int k = 7; k >= 0; k--)
				if ((letters[str[i]][j] >> k) & 1)putPixel(x + i * 10 + 7 - k, y + 16 - j);
}
void putChar(char ch, int x, int y) {
	if (_sglMode != BIT_MAP && !_innerFunc)return;
	for (int j = 0; j < 13; j++)
		for (int k = 7; k >= 0; k--)
			if ((letters[ch][j] >> k) & 1)putPixel(x + 7 - k, y + 16 - j);
}
int getImage(int left, int top, int right, int bottom, bitMap *bitmap) {
	if (_sglMode != BIT_MAP && !_innerFunc)return SG_INVALID_MODE;
	int deltaX, deltaY;
	bitMap *buf;
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

	for (int i = 0; i < deltaY; i++) {
		memcpy(bitmap->data + bitmap->sizeX * i * 3, buf->data + (buf->sizeX * (i + top) + left) * 3, deltaX * 3 * sizeof(char));
	}
	return SG_NO_ERORR;
}
void putImage(int left, int top, bitMap *bitmap, int op) {
	if (_sglMode != BIT_MAP && !_innerFunc)return;
	int x1, x2, y1, y2;
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
	if (top + bitmap->sizeY >= buf->sizeY)y2 = buf->sizeY - 1;
	else y2 = top + bitmap->sizeY - 1;
	switch (op) {
	case COPY_PUT:
		for (int i = 0; i < y2 - y1 + 1; i++) {
			memcpy(buf->data + (buf->sizeX * (i + top) + left) * 3, bitmap->data + bitmap->sizeX * i * 3, (x2 - x1 + 1) * 3 * sizeof(char));
		}
		break;
	case AND_PUT:
		for (int i = 0; i < y2 - y1 + 1; i++) {
			for (int j = 0; j < x2 - x1 + 1; j++) {
				buf->data[(buf->sizeX * (i + top) + left + j) * 3] &= bitmap->data[(bitmap->sizeX*i + j) * 3];
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 1] &= bitmap->data[(bitmap->sizeX*i + j) * 3 + 1];
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 2] &= bitmap->data[(bitmap->sizeX*i + j) * 3 + 2];
			}
		}
		break;
	case OR_PUT:
		for (int i = 0; i < y2 - y1 + 1; i++) {
			for (int j = 0; j < x2 - x1 + 1; j++) {
				buf->data[(buf->sizeX * (i + top) + left + j) * 3] |= bitmap->data[(bitmap->sizeX*i + j) * 3];
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 1] |= bitmap->data[(bitmap->sizeX*i + j) * 3 + 1];
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 2] |= bitmap->data[(bitmap->sizeX*i + j) * 3 + 2];
			}
		}
		break;
	case XOR_PUT:
		for (int i = 0; i < y2 - y1 + 1; i++) {
			for (int j = 0; j < x2 - x1 + 1; j++) {
				buf->data[(buf->sizeX * (i + top) + left + j) * 3] ^= bitmap->data[(bitmap->sizeX*i + j) * 3];
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 1] ^= bitmap->data[(bitmap->sizeX*i + j) * 3 + 1];
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 2] ^= bitmap->data[(bitmap->sizeX*i + j) * 3 + 2];
			}
		}
		break;
	case NOT_PUT:
		for (int i = 0; i < y2 - y1 + 1; i++) {
			for (int j = 0; j < x2 - x1 + 1; j++) {
				buf->data[(buf->sizeX * (i + top) + left + j) * 3] = ~bitmap->data[(bitmap->sizeX*i + j) * 3];
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 1] = ~bitmap->data[(bitmap->sizeX*i + j) * 3 + 1];
				buf->data[(buf->sizeX * (i + top) + left + j) * 3 + 2] = ~bitmap->data[(bitmap->sizeX*i + j) * 3 + 2];
			}
		}
		break;
	}
}
void floodFill(int x, int y, RGB c) {
	if (_sglMode != BIT_MAP && !_innerFunc)return;
	vecTwo Q[SG_QSIZE];
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
	if (_sglMode != BIT_MAP && !_innerFunc)return;
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
	if (_sglMode != BIT_MAP && !_innerFunc)return;
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
void putEllipse(int xc, int yc, int a, int b, int mode) {
	if (_sglMode != BIT_MAP && !_innerFunc)return;
	int sqa = a * a, sqb = b * b;
	int x = 0, y = b;
	int d = 2 * sqb - 2 * b * sqa + sqa;
	int P_x = (int)((double)sqa / sqrt((double)(sqa + sqb)));
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
			for (int xi = 0; xi <= x; xi++) {
				putPixel(xc + xi, yc + y);
				putPixel(xc - xi, yc + y);
				putPixel(xc + xi, yc - y);
				putPixel(xc - xi, yc - y);
			}
		}
		d = sqb * (x * x + x) + sqa * (y * y - y) - sqa * sqb;
		while (y >= 0) {
			for (int xi = 0; xi <= x; xi++) {
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
	if (_sglMode != BIT_MAP && !_innerFunc)return;
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
int maskImage(int left, int top, bitMap *mask, bitMap *bitmap) {
	if (_sglMode != BIT_MAP && !_innerFunc)return SG_INVALID_MODE;
	int x1, x2, y1, y2;
	bitMap *buf;
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

	for (int i = 0; i < y2 - y1 + 1; i++) {
		for (int j = 0; j < x2 - x1 + 1; j++) {
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
void funcMap(int x1, int x2, int y1, int y2, float(*vect)(int x)) {
	if (_sglMode != BIT_MAP && !_innerFunc)return;
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

//Textmap functions.
void setBfc(int bgc, int fgc) {
	Screen->bfc = (bgc << 4) | fgc;
}
void setCharColor(char color, int x, int y) {
	int pos = y * Screen->changed->width + x;
	Screen->changed->content[pos] &= 0x00FF;
	Screen->changed->content[pos] |= (color << 8);
}
void setCharBgc(char color, int x, int y) {
	int pos = y * Screen->changed->width + x;
	Screen->changed->content[pos] &= 0x0FFF;
	Screen->changed->content[pos] |= ((color & 0x0F) << 12);
}
void setCharFgc(char color, int x, int y) {
	int pos = y * Screen->changed->width + x;
	Screen->changed->content[pos] &= 0xF0FF;
	Screen->changed->content[pos] |= ((color & 0x0F) << 8);
}
void clearText() {
	for (int i = 0; i < Screen->changed->width*Screen->changed->height; i++) {
		writeChar(0, i%Screen->changed->width, i / Screen->changed->width);
	}
}
void writeChar(char c, int x, int y) {
	int pos = y * Screen->changed->width + x;
	Screen->changed->content[pos] = ((short)Screen->bfc << 8) | (short)c;
}
void writeString(char *s, int x, int y) {
	for (int i = 0; s[i] != '\0'; i++) {
		writeChar(s[i], x++, y);
	}
}
int getShort(int x, int y) {
	return Screen->changed->content[y * Screen->changed->width + x];
}
int getText(int left, int top, int right, int bottom, textMap *text) {

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

	for (int i = 0; i < text->height; i++) {
		memcpy(text->content + (i*text->width), Screen->changed->content + (top + i) * Screen->changed->width + left, text->width * sizeof(short));
	}

	return SG_NO_ERORR;
}
void putText(int left, int top, textMap *text) {
	int x1, x2, y1, y2;

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

	for (int i = 0; i < text->height; i++) {
		memcpy(Screen->changed->content + (top + i) * Screen->changed->width + left, text->content + (i*text->width), text->width * sizeof(short));
	}
}
