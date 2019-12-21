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



#ifndef SGL_WINDOW_H
#define SGL_WINDOW_H


#include "winsgl.h"
#include "inner.h"
#include "util.h"
#include "users.h"
#include "widget.h"
#include <windows.h>

#include <vector>

using std::vector;

byte _letters[256][8] = {
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

struct Function {
	int id;
	char *name;
	vect function;
};
struct Panel {
	int en = 0;
	int maxId = 0;
	int x, y, outer = 40, inner = 20;
	bitMap *cover;
	vector<struct Function *>normalPanel = vector<struct Function *>(SG_MAX_PANEL_FUNCTION);
	vector<struct Function *>shiftPanel = vector<struct Function *>(SG_MAX_PANEL_FUNCTION);
	vector<struct Function *>ctrlPanel = vector<struct Function *>(SG_MAX_PANEL_FUNCTION);
	vector<struct Function *>shiftctrlPanel = vector<struct Function *>(SG_MAX_PANEL_FUNCTION);
};

void whiteBack(int left, int top, int right, int bottom) {
	setColor(255, 255, 255);
	putQuad(left, top, right, bottom, SOLID_FILL);
}
class Window {
private:
	HWND hwnd;
	HICON hIcon = NULL;
	char *winName;
	int winWidth, winHeight;
	int txtWidth, txtHeight;
	int posLeft, posUp;
	int tmpThread;
	int innerFunc = 0;

	bitMap *buffer;
	textMap *content, *changed;
	
	byte rgb[3] = { 0, 0, 0 };
	float alpha = 1.f;
	byte bfc;

	font tf;
	_text text;

	void makeBitmap(HDC hdc, LPBYTE lpBits, long width, long height, WORD wBitCount) {
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

		if (resize && sglMode == BIT_MAP) {
			StretchDIBits(hdc, 0, 0, bitinfo.bmiHeader.biWidth,
				bitinfo.bmiHeader.biHeight, 0, 0, bitinfo.bmiHeader.biWidth,
				bitinfo.bmiHeader.biHeight, buffer, (BITMAPINFO*)&bitinfo.bmiHeader,
				DIB_RGB_COLORS, SRCCOPY);
		}
		else {
			StretchDIBits(hdc, 0, 0, winWidth,
				winHeight, 0, 0, bitinfo.bmiHeader.biWidth,
				bitinfo.bmiHeader.biHeight, buffer, (BITMAPINFO*)&bitinfo.bmiHeader,
				DIB_RGB_COLORS, SRCCOPY);
		}
		free(buffer);
	}
	void initCanvas(int mode) {
		if (mode == BIT_MAP) {
			buffer = (bitMap *)malloc(sizeof(bitMap));
			buffer->sizeX = winWidth;
			buffer->sizeY = winHeight;
			buffer->data = (unsigned char *)malloc(buffer->sizeX * buffer->sizeY * 3 * sizeof(unsigned char));
			memset(buffer->data, -1, buffer->sizeX * buffer->sizeY * 3 * sizeof(unsigned char));
		}
		if (mode == TEXT_MAP) {
			buffer = (bitMap *)malloc(sizeof(bitMap));
			buffer->sizeX = txtWidth * SG_CHAR_WIDTH;
			buffer->sizeY = txtHeight * SG_CHAR_HEIGHT;
			buffer->data = (unsigned char *)malloc(buffer->sizeX * buffer->sizeY * 3 * sizeof(unsigned char));
			memset(buffer->data, -1, buffer->sizeX * buffer->sizeY * 3 * sizeof(unsigned char));

			content = (textMap *)malloc(sizeof(textMap));
			content->width = txtWidth;
			content->height = txtHeight;
			content->content = (unsigned short *)malloc(txtWidth * txtHeight * sizeof(unsigned short));
			for (int i = 0; i < txtWidth * txtHeight; i++)content->content[i] = 0xF000;

			changed = (textMap *)malloc(sizeof(textMap));
			changed->width = txtWidth;
			changed->height = txtHeight;
			changed->content = (unsigned short *)malloc(txtWidth * txtHeight * sizeof(unsigned short));
			for (int i = 0; i < txtWidth * txtHeight; i++)changed->content[i] = 0xF000;
		}
	}
	void prepareText(int width) {
		text.memDC = CreateCompatibleDC(NULL);
		text.hbm = CreateBitmap(width, SG_MAX_FONT_SIZE, 1, 32, NULL);

		GetObject(text.hbm, sizeof(text.bm), &text.bm);
		int bmRowCount = ((text.bm.bmWidth * 24 + 31) / 32) * 4;
		text.bufSize = bmRowCount * text.bm.bmHeight;
		text.bitBuf = (unsigned char *)malloc(text.bufSize);
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
		tf.name = (LPWSTR)malloc(16);
		_strcpy(tf.name,  _widen("微软雅黑"));
		tf.coeff = 0;

		SetTextColor(text.memDC, RGB(255, 255, 255));
		SetBkMode(text.memDC, TRANSPARENT);

		text.font = CreateFont(tf.size, 0, 0, 0, FW_THIN, tf.coeff & FONT_ITALIC,
			tf.coeff & FONT_UNDERLINE, tf.coeff & FONT_STRIKEOUT,
			DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
			DEFAULT_QUALITY, FF_MODERN, tf.name);
		SelectObject(text.memDC, text.font);
	}
	void putImageRev(int left, int top, bitMap *bitmap) {
		int x1, x2, y1, y2, i, j, rate;

		if (left >= buffer->sizeX || top >= buffer->sizeY)return;
		if (left + bitmap->sizeX <= 0 || top + bitmap->sizeY <= 0)return;

		if (left < 0)x1 = 0;
		else x1 = left;
		if (top < 0)y1 = 0;
		else y1 = top;

		if (left + text.strRect.cx >= buffer->sizeX)x2 = buffer->sizeX - 1;
		else x2 = left + text.strRect.cx;
		y2 = top + bitmap->sizeY - 1;

		int tmp;
		for (i = 0; i < y2 - y1; i++) {
			if (top + i >= buffer->sizeY)break;
			for (j = 0; j < x2 - x1; j++) {
				if (rate = bitmap->data[(bitmap->sizeX*(y2 - y1 - i) + j) * 3]) {
					tmp = buffer->data[(buffer->sizeX * (i + top) + left + j) * 3] * (255 - rate);
					tmp += tf.color.b*rate;
					buffer->data[(buffer->sizeX * (i + top) + left + j) * 3] = tmp >> 8;
				}
				if (rate = bitmap->data[(bitmap->sizeX*(y2 - y1 - i) + j) * 3 + 1]) {
					tmp = buffer->data[(buffer->sizeX * (i + top) + left + j) * 3 + 1] * (255 - rate);
					tmp += tf.color.g*rate;
					buffer->data[(buffer->sizeX * (i + top) + left + j) * 3 + 1] = tmp >> 8;
				}
				if (rate = bitmap->data[(bitmap->sizeX*(y2 - y1 - i) + j) * 3 + 2]) {
					tmp = buffer->data[(buffer->sizeX * (i + top) + left + j) * 3 + 2] * (255 - rate);
					tmp += tf.color.r*rate;
					buffer->data[(buffer->sizeX * (i + top) + left + j) * 3 + 2] = tmp >> 8;
				}
			}
		}
	}
	void hbmImage(HDC hdc, HBITMAP hbm, int x, int y, const char *str) {
		GetDIBits(hdc, hbm, 0, text.bm.bmHeight, text.bitBuf, (BITMAPINFO*)(&text.bi), DIB_RGB_COLORS);

		bitMap tmp;
		tmp.sizeX = text.bi.biWidth;
		tmp.sizeY = tf.size;
		tmp.data = text.bitBuf + text.bi.biWidth*(text.bi.biHeight - tmp.sizeY) * 3;
		putImageRev(x, y, &tmp);
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
	void textRenew(short c, int x, int y) {
		RGB b, f;

		innerFunc = 1;
		b = bgc(c), f = fgc(c);
		setColor(b.r, b.g, b.b);
		putQuad(x * 8, y * 16, x * 8 + 8, y * 16 + 16, SOLID_FILL);
		setColor(f.r, f.g, f.b);
		putChar((char)c, x * 8, y * 16);
		innerFunc = 0;
	}
	
public:
	int sglMode = BIT_MAP, fps = 0;
	void(*resize)(int w, int h) = NULL;
	int sglCircle = 0, sglCircleCx, sglCircleCy, sglCircleR;

	vect loop;
	int inLoop = 0;

	Key *key = new Key();
	Mouse *mouse = new Mouse();

	Panel *panel = new Panel();
	int _mainMenu = 0, _useTray = 0, _trayMenu = 0;
	struct _menu _mainMenuInfo;
	struct _menu _trayMenuInfo;

	void(*backgroundRefresh)(int left, int top, int right, int bottom);
	vector<Widget *>widgets;
	int drawingWidget = -1;

	Window(int width = 640, int height = 480, const char *title = "SGL program", int mode = BIT_MAP) {
		sglMode = mode;
		if (mode == BIT_MAP) {
			if (width == 0 || height == 0) {
				RECT rect;
				SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
				winWidth = rect.right - rect.left;
				winHeight = rect.bottom - rect.top;
				winWidth = winWidth - winWidth % 8;
				posLeft = posUp = 0;
			}
			else {
				winWidth = width;
				winHeight = height;

				int winPx = GetSystemMetrics(SM_CXFULLSCREEN);
				int winPy = GetSystemMetrics(SM_CYFULLSCREEN);
				if (winWidth >= winPx)posLeft = 0;
				else posLeft = (winPx - winWidth) / 2;
				if (winHeight >= winPy)posUp = 0;
				else posUp = (winPy - winHeight) / 2;
			}
			backgroundRefresh = whiteBack;
		}
		if (mode == TEXT_MAP) {
			txtWidth = width;
			txtHeight = height;
			winWidth = width * SG_CHAR_WIDTH;
			winHeight = height * SG_CHAR_HEIGHT;

			int winPx = GetSystemMetrics(SM_CXFULLSCREEN);
			int winPy = GetSystemMetrics(SM_CYFULLSCREEN);
			if (winWidth >= winPx)posLeft = 0;
			else posLeft = (winPx - winWidth) / 2;
			if (winHeight >= winPy)posUp = 0;
			else posUp = (winPy - winHeight) / 2;
		}

		winName = (char *)malloc(strlen(title) + 24);
		strcpy(winName, title);
		strcat(winName, "  --powered by Super GP");

		initCanvas(mode);
		prepareText(GetSystemMetrics(SM_CXFULLSCREEN) / 8 * 8 + 8);
	}
	~Window() {
		free(buffer->data);
		free(buffer);
		if (sglMode == TEXT_MAP) {
			free(content->content);
			free(content);
			free(changed->content);
			free(changed);
		}

		free(text.bitBuf);
		free(tf.name);
		free(winName);
	}
	HWND getHwnd() {
		return hwnd;
	}
	HICON getIcon() {
		return hIcon;
	}
	char *getTitle() {
		return winName;
	}
	vec2i getBufferSize() {
		vec2i ret;
		ret.x = buffer->sizeX;
		ret.y = buffer->sizeY;
		return ret;
	}
	vec2i getWindowSize() {
		vec2i ret;
		ret.x = winWidth;
		ret.y = winHeight;
		return ret;
	}
	bitMap *getCanvas() { return buffer; }

	void createWindow(HINSTANCE hInstance, WNDCLASSEX wc, HWND parent = NULL) {
		hwnd = CreateWindow(wc.lpszClassName, _widen(winName),
			WS_OVERLAPPEDWINDOW, posLeft, posUp, winWidth + 16,  winHeight + 16 + GetSystemMetrics(SM_CYCAPTION),
			parent, NULL, hInstance, NULL);
	}
	void setIcon(HANDLE icon) {
		hIcon = (HICON)icon;
	}
	void setResize() {
		if (sglMode == BIT_MAP) {
			free(buffer->data);
			free(buffer);

			RECT rect;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
			buffer = (bitMap *)malloc(sizeof(bitMap));
			buffer->sizeX = rect.right - rect.left;
			buffer->sizeY = rect.bottom - rect.top;
			buffer->data = (unsigned char *)malloc(buffer->sizeX * buffer->sizeY * 3 * sizeof(unsigned char));
			memset(buffer->data, -1, buffer->sizeX * buffer->sizeY * 3 * sizeof(unsigned char));
		}
	}
	void makeResize(int x, int y) {
		winWidth = x;
		winHeight = y;
		if (resize)resize(x, y);
	}
	void fillRect(HWND hwnd) {
		HDC hdc;
		PAINTSTRUCT ps;

		hdc = BeginPaint(hwnd, &ps);
		makeBitmap(hdc, buffer->data, buffer->sizeX, buffer->sizeY, 24);
		EndPaint(hwnd, &ps);
	}

	void setThread() {
		tmpThread = GetThreadId(GetCurrentThread());
	}
	int checkThread() {
		int tmp = GetThreadId(GetCurrentThread());
		if (tmp != tmpThread) {
			debugf("系统拒绝在非主线程中使用绘图函数。\n");
			return -1;
		}
		else return 0;
	}

	void setColor(int r, int g, int b) {
		if (sglMode != BIT_MAP && !innerFunc)return;
		if (checkThread())return;

		rgb[0] = r;
		rgb[1] = g;
		rgb[2] = b;

		tf.color.r = r;
		tf.color.g = g;
		tf.color.b = b;
	}
	void setFontSize(int height) {
		if (sglMode != BIT_MAP && !innerFunc)return;
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
	void setFontName(const char *name) {
		if (sglMode != BIT_MAP && !innerFunc)return;
		if (checkThread())return;

		_strcpy(tf.name, _widen(name));

		DeleteObject(text.font);
		text.font = CreateFont(tf.size, 0, 0, 0, FW_THIN, tf.coeff & FONT_ITALIC,
			tf.coeff & FONT_UNDERLINE, tf.coeff & FONT_STRIKEOUT,
			DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
			DEFAULT_QUALITY, FF_MODERN, tf.name);
		SelectObject(text.memDC, text.font);
	}
	void setFontStyle(int coeff) {
		if (sglMode != BIT_MAP && !innerFunc)return;
		if (checkThread())return;

		tf.coeff = coeff;

		DeleteObject(text.font);
		text.font = CreateFont(tf.size, 0, 0, 0, FW_THIN, tf.coeff & FONT_ITALIC,
			tf.coeff & FONT_UNDERLINE, tf.coeff & FONT_STRIKEOUT,
			DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
			DEFAULT_QUALITY, FF_MODERN, tf.name);
		SelectObject(text.memDC, text.font);
	}
	void setAlpha(float alpha) {
		if (sglMode != BIT_MAP && !innerFunc)return;
		if (checkThread())return;

		this->alpha = alpha;
	}
	float getAlpha() {
		if (sglMode != BIT_MAP && !innerFunc)return 0.f;
		return alpha;
	}
	int __forceinline putPixel(int x, int y) {
		int p;

		if (sglMode != BIT_MAP && !innerFunc)return SG_INVALID_MODE;
		if (checkThread())return SG_WRONG_THREAD;
		if (x < 0 || x >= buffer->sizeX || y < 0 || y >= buffer->sizeY)return SG_OUT_OF_RANGE;

		p = (y * buffer->sizeX + x) * 3;
		if (alpha == 1.f) {
			buffer->data[p] = rgb[2];
			buffer->data[p + 1] = rgb[1];
			buffer->data[p + 2] = rgb[0];
		}
		else {
			buffer->data[p] = (int)(buffer->data[p] * (1.f - alpha));
			buffer->data[p] += (int)(rgb[2] * alpha);
			buffer->data[p + 1] = (int)(buffer->data[p + 1] * (1.f - alpha));
			buffer->data[p + 1] += (int)(rgb[1] * alpha);
			buffer->data[p + 2] = (int)(buffer->data[p + 2] * (1.f - alpha));
			buffer->data[p + 2] += (int)(rgb[0] * alpha);
		}

		if (drawingWidget == -1)widgetCover(-1, x, y, x, y);
		return SG_NO_ERORR;
	}
	RGB getPixel(int x, int y) {
		RGB im, ret;
		int p;

		if (sglMode != BIT_MAP && !innerFunc) {
			im.r = im.g = im.b = SG_INVALID_MODE;
			return im;
		}

		ret.r = ret.g = ret.b = SG_OUT_OF_RANGE;

		if (x < 0 || x >= buffer->sizeX || y < 0 || y >= buffer->sizeY)return ret;
		p = (y*buffer->sizeX + x) * 3;
		ret.b = buffer->data[p++];
		ret.g = buffer->data[p++];
		ret.r = buffer->data[p++];
		return ret;
	}
	void clearScreen() {
		int i;

		if (sglMode != BIT_MAP && !innerFunc)return;
		if (checkThread())return;

		if ((rgb[0] & rgb[1] & rgb[2]) == 255) {
			memset(buffer->data, -1, 3 * buffer->sizeX*buffer->sizeY);
			if (drawingWidget == -1)widgetCover(-1, 0, 0, winWidth, winHeight);
			return;
		}
		if ((rgb[0] | rgb[1] | rgb[2]) == 0) {
			memset(buffer->data, 0, 3 * buffer->sizeX*buffer->sizeY);
			if (drawingWidget == -1)widgetCover(-1, 0, 0, winWidth, winHeight);
			return;
		}

		for (i = 0; i < 3 * buffer->sizeX*buffer->sizeY; i += 3) {
			buffer->data[i] = rgb[2];
			buffer->data[i + 1] = rgb[1];
			buffer->data[i + 2] = rgb[0];
		}
		if (drawingWidget == -1)widgetCover(-1, 0, 0, winWidth, winHeight);
	}
	void putLine(int x1, int y1, int x2, int y2, int mode) {
		int dx, dy, ux, uy, x, y, eps;

#define ABS(x) ((x > 0) ? (x) : (-x))

		if (sglMode != BIT_MAP && !innerFunc)return;
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
#undef ABS
	}
	int __forceinline putQuad(int x1, int y1, int x2, int y2, int mode) {
		int i, j, p;

		if (sglMode != BIT_MAP && !innerFunc)return SG_INVALID_MODE;
		if (checkThread())return SG_WRONG_THREAD;

		if (x1 > x2)x1 ^= x2 ^= x1 ^= x2;
		if (y1 > y2)y1 ^= y2 ^= y1 ^= y2;
		if (x1 < 0)x1 = 0;
		if (x2 >= buffer->sizeX)x2 = buffer->sizeX - 1;
		if (y1 < 0)y1 = 0;
		if (y2 >= buffer->sizeY)y2 = buffer->sizeY - 1;

		if (mode == SOLID_FILL) {
			for (j = y1; j < y2; j++) {
				p = (j * buffer->sizeX + x1) * 3;
				for (i = 0; i < 3 * (x2 - x1); i += 3) {
					buffer->data[p + i] = rgb[2];
					buffer->data[p + i + 1] = rgb[1];
					buffer->data[p + i + 2] = rgb[0];
				}
			}
			if(drawingWidget == -1)widgetCover(-1, x1, y1, x2, y2);
		}

		if (mode == EMPTY_FILL) {
			for (i = x1; i < x2; i++) {
				putPixel(i, y1);
				putPixel(i, y2 - 1);
			}
			for (j = y1; j < y2; j++) {
				putPixel(x1, j);
				putPixel(x2 - 1, j);
			}
		}

		return SG_NO_ERORR;
	}
	void putTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int mode) {
		RGB c;

		if (sglMode != BIT_MAP && !innerFunc)return;
		if (checkThread())return;

		c.r = rgb[0];
		c.g = rgb[1];
		c.b = rgb[2];
		putLine(x1, y1, x2, y2, SOLID_LINE);
		putLine(x1, y1, x3, y3, SOLID_LINE);
		putLine(x2, y2, x3, y3, SOLID_LINE);
		if (mode == SOLID_FILL)floodFill((x1 + x2 + x3) / 3, (y1 + y2 + y3) / 3, c);
	}
	void putCircle(int xc, int yc, int r, int mode) {
		int x, y, yi, d;

		if (sglMode != BIT_MAP && !innerFunc)return;
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
	void putEllipse(int xc, int yc, int a, int b, int mode) {
		int sqa, sqb, x, y, d, P_x, xi, jmp;

		if (sglMode != BIT_MAP && !innerFunc)return;
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
	void putBitmap(int x, int y, bitMap bmp) {
		int i, width, height, lines;
		byte *vp;

		if (sglMode != BIT_MAP && !innerFunc)return;
		if (checkThread())return;

		vp = buffer->data + (y*buffer->sizeX + x) * 3;

		width = bmp.sizeX;
		height = bmp.sizeY;
		for (i = height - 1; i >= 0; i--) {
			lines = i * buffer->sizeX * 3;
			if (x + width > buffer->sizeX)
				memcpy(vp + lines, bmp.data + i * width * 3, (buffer->sizeX - x) * 3);
			else memcpy(vp + lines, bmp.data + i * width * 3, width * 3);
		}

	}
	int drawBmp(int x, int y, const char *filename) {
		FILE *fp = NULL;
		SGstring p = NULL, tmp = NULL;
		int width, height, i;
		dword dataOffset, lineBytes;
		dword lines;
		byte *vp;

		if (sglMode != BIT_MAP && !innerFunc)return SG_INVALID_MODE;
		if (checkThread())return SG_WRONG_THREAD;

		vp = buffer->data + (y*buffer->sizeX + x) * 3;

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

		if (y + height > buffer->sizeY) {
			fseek(fp, dataOffset + (y + height - buffer->sizeY)*lineBytes, SEEK_SET);
			height = buffer->sizeY - y;
		}
		else fseek(fp, dataOffset, SEEK_SET);

		for (i = height - 1; i >= 0; i--) {
			fread(p, 1, lineBytes, fp);
			lines = i * buffer->sizeX * 3;
			tmp = p;
			if (x + width > buffer->sizeX)
				memcpy(vp + lines, p, (buffer->sizeX - x) * 3);
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

		if (sglMode != BIT_MAP && !innerFunc)return;
		if (checkThread())return;

		for (j = 0; j < 8; j++)
			for (k = 7; k >= 0; k--)
				if ((_letters[ch][j] >> k) & 1) {
					putPixel(x + 7 - k, y + 2 * j);
					putPixel(x + 7 - k, y + 2 * j + 1);
				}
	
	}
	void putNumber(int n, int x, int y, char lr) {
		int s[20], sn = 0;

		if (sglMode != BIT_MAP && !innerFunc)return;
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
	int stringWidth(const char *str, int x) {
		SIZE ret;
		int tab = 0, i, j;
		char *tmp;

		if (sglMode != BIT_MAP && !innerFunc)return SG_INVALID_MODE;
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

		GetTextExtentPoint32(text.memDC, _widen(tmp), x, &ret);
		free(tmp);
		return ret.cx;
	}
	int putString(const char *str, int x, int y) {

		if (sglMode != BIT_MAP && !innerFunc)return SG_INVALID_MODE;
		if (checkThread())return SG_WRONG_THREAD;
		if (str == NULL)return SG_NULL_POINTER;

		char *tmp = (char *)malloc(strlen(str) + 1);
		char *start = tmp;

		int line = 0;
		strcpy(tmp, str);
		str = tmp;
		while (*tmp) {
			while (*tmp && *tmp != '\n')tmp++;
			if (*tmp == '\n') {
				*tmp = '\0';

				GetTextExtentPoint32(text.memDC, _widen(str), _strlen(_widen(str)), &text.strRect);
				memset(text.bitBuf, 0, text.bufSize);
				RECT imgRect = { 0, 0, text.strRect.cx, text.strRect.cy };
				FillRect(text.memDC, &imgRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

				TextOut(text.memDC, 0, 0, _widen(str), _strlen(_widen(str)));
				hbmImage(text.memDC, text.hbm, x, y + line * tf.size, str);

				tmp = tmp + 1;
				str = tmp;
			}
			else {
				GetTextExtentPoint32(text.memDC, _widen(str), _strlen(_widen(str)), &text.strRect);
				memset(text.bitBuf, 0, text.bufSize);
				RECT imgRect = { 0, 0, text.strRect.cx, text.strRect.cy };
				FillRect(text.memDC, &imgRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

				TextOut(text.memDC, 0, 0, _widen(str), _strlen(_widen(str)));
				hbmImage(text.memDC, text.hbm, x, y + line * tf.size, str);
			}
			line++;
		}
		free(start);
		return SG_NO_ERORR;
	}
	int putStringConstraint(const char *str, int x, int y, int start, int constraint) {

		if (sglMode != BIT_MAP && !innerFunc)return SG_INVALID_MODE;
		if (checkThread())return SG_WRONG_THREAD;
		if (str == NULL)return SG_NULL_POINTER;

		int len, i;
		unsigned int tab = 0;
		char *tmp;

		len = _mixLen(str) - start > constraint / 5 ? constraint / 5 : _mixLen(str) - start;
		if (len <= 0)return 0;

		GetTextExtentPoint32(text.memDC, _widen(str) + start, len, &text.strRect);
		while (text.strRect.cx > constraint) {
			len--;
			GetTextExtentPoint32(text.memDC, _widen(str) + start, len, &text.strRect);
		}
		len = _wcharAt(str, len);
		if (len <= 1)len = 1;

		i = 0;
		while (str[i++] != '\n')
			if (i >= len)break;
		len = i;

		tmp = (char *)malloc(len + 1);
		memcpy(tmp, str, len);
		tmp[len] = 0;

		putString(_stringConvert(tmp), x, y);
		free(tmp);

		return len;
	}
	int getImage(int left, int top, int right, int bottom, bitMap *bitmap) {
		int deltaX, deltaY, i;

		if (sglMode != BIT_MAP && !innerFunc)return SG_INVALID_MODE;
		if (checkThread())return SG_WRONG_THREAD;

		if (right < left)left ^= right ^= left ^= right;
		if (bottom < top)top ^= bottom ^= top ^= bottom;

		if (right >= buffer->sizeX)right = buffer->sizeX - 1;
		if (left < 0)left = 0;
		if (bottom >= buffer->sizeY)bottom = buffer->sizeY - 1;
		if (top < 0)top = 0;

		deltaX = right - left + 1;
		deltaY = bottom - top + 1;

		bitmap->sizeX = deltaX;
		bitmap->sizeY = deltaY;
		bitmap->data = (byte*)malloc(deltaX * deltaY * 3 * sizeof(byte));
		if (bitmap->data == NULL)return SG_NO_LOAD_MEM;

		for (i = 0; i < deltaY; i++) {
			memcpy(bitmap->data + bitmap->sizeX * i * 3, buffer->data + (buffer->sizeX * (i + top) + left) * 3, deltaX * 3 * sizeof(char));
		}

		return SG_NO_ERORR;
	}
	void putImage(int left, int top, bitMap *bitmap, int op) {
		int x1, x2, y1, y2, i, j;

		if (sglMode != BIT_MAP && !innerFunc)return;
		if (checkThread())return;

		if (left >= buffer->sizeX || top >= buffer->sizeY)return;
		if (left + bitmap->sizeX <= 0 || top + bitmap->sizeY <= 0)return;

		if (left < 0)x1 = 0;
		else x1 = left;
		if (top < 0)y1 = 0;
		else y1 = top;

		if (left + bitmap->sizeX >= buffer->sizeX)x2 = buffer->sizeX - 1;
		else x2 = left + bitmap->sizeX - 1;
		if (top + bitmap->sizeY >= buffer->sizeY)y2 = buffer->sizeY - 1;
		else y2 = top + bitmap->sizeY - 1;

		switch (op) {
		case COPY_PUT:
			for (i = 0; i < y2 - y1 + 1; i++) {
				memcpy(buffer->data + (buffer->sizeX * (i + y1) + x1) * 3,
					bitmap->data + (bitmap->sizeX * (i + y1 - top) + x1 - left) * 3,
					(x2 - x1 + 1) * 3 * sizeof(char));
			}
			break;

		case AND_PUT:
			for (i = 0; i < y2 - y1 + 1; i++) {
				for (j = 0; j < x2 - x1 + 1; j++) {
					buffer->data[(buffer->sizeX * (i + y1) + x1 + j) * 3] &=
						bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3];
					buffer->data[(buffer->sizeX * (i + y1) + x1 + j) * 3 + 1] &=
						bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3 + 1];
					buffer->data[(buffer->sizeX * (i + y1) + x1 + j) * 3 + 2] &=
						bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3 + 2];
				}
			}
			break;

		case OR_PUT:
			for (i = 0; i < y2 - y1 + 1; i++) {
				for (j = 0; j < x2 - x1 + 1; j++) {
					buffer->data[(buffer->sizeX * (i + y1) + x1 + j) * 3] |=
						bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3];
					buffer->data[(buffer->sizeX * (i + y1) + x1 + j) * 3 + 1] |=
						bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3 + 1];
					buffer->data[(buffer->sizeX * (i + y1) + x1 + j) * 3 + 2] |=
						bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3 + 2];
				}
			}
			break;

		case XOR_PUT:
			for (i = 0; i < y2 - y1 + 1; i++) {
				for (j = 0; j < x2 - x1 + 1; j++) {
					buffer->data[(buffer->sizeX * (i + y1) + x1 + j) * 3] ^=
						bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3];
					buffer->data[(buffer->sizeX * (i + y1) + x1 + j) * 3 + 1] ^=
						bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3 + 1];
					buffer->data[(buffer->sizeX * (i + y1) + x1 + j) * 3 + 2] ^=
						bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3 + 2];
				}
			}
			break;

		case NOT_PUT:
			for (i = 0; i < y2 - y1 + 1; i++) {
				for (j = 0; j < x2 - x1 + 1; j++) {
					buffer->data[(buffer->sizeX * (i + y1) + x1 + j) * 3] =
						~bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3];
					buffer->data[(buffer->sizeX * (i + y1) + x1 + j) * 3 + 1] =
						~bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3 + 1];
					buffer->data[(buffer->sizeX * (i + y1) + x1 + j) * 3 + 2] =
						~bitmap->data[(bitmap->sizeX * (i + y1 - top) + x1 - left + j) * 3 + 2];
				}
			}
			break;
		}

	}
	int maskImage(int left, int top, bitMap *mask, bitMap *bitmap) {
		int x1, x2, y1, y2, i, j;
		int w1, w2, w3;

		if (sglMode != BIT_MAP && !innerFunc)return SG_INVALID_MODE;
		if (checkThread())return SG_WRONG_THREAD;

		if (mask->sizeX != bitmap->sizeX || mask->sizeY != bitmap->sizeY)return SG_SIZE_MISMATCH;
		if (left >= buffer->sizeX || top >= buffer->sizeY)return SG_OUT_OF_RANGE;
		if (left + bitmap->sizeX <= 0 || top + bitmap->sizeY <= 0)return SG_OUT_OF_RANGE;

		if (left < 0)x1 = 0;
		else x1 = left;
		if (top < 0)y1 = 0;
		else y1 = top;

		if (left + bitmap->sizeX > buffer->sizeX)x2 = buffer->sizeX - 1;
		else x2 = left + bitmap->sizeX - 1;
		if (top + bitmap->sizeY > buffer->sizeY)y2 = buffer->sizeY - 1;
		else y2 = top + bitmap->sizeY - 1;

		for (i = 0; i < y2 - y1 + 1; i++) {
			for (j = 0; j < x2 - x1 + 1; j++) {
				w1 = mask->data[(mask->sizeX*i + j) * 3];
				buffer->data[(buffer->sizeX * (i + top) + left + j) * 3] =
					buffer->data[(buffer->sizeX * (i + top) + left + j) * 3] * (255 - w1) / 255 +
					bitmap->data[(bitmap->sizeX*i + j) * 3] * w1 / 255;
				w2 = mask->data[(mask->sizeX*i + j) * 3 + 1];
				buffer->data[(buffer->sizeX * (i + top) + left + j) * 3 + 1] =
					buffer->data[(buffer->sizeX * (i + top) + left + j) * 3 + 1] * (255 - w2) / 255 +
					bitmap->data[(bitmap->sizeX*i + j) * 3 + 1] * w2 / 255;
				w3 = mask->data[(mask->sizeX*i + j) * 3 + 2];
				buffer->data[(buffer->sizeX * (i + top) + left + j) * 3 + 2] =
					buffer->data[(buffer->sizeX * (i + top) + left + j) * 3 + 2] * (255 - w3) / 255 +
					bitmap->data[(bitmap->sizeX*i + j) * 3 + 2] * w3 / 255;
			}
		}

		return SG_NO_ERORR;
	}
	void funcMap(int x1, int x2, int y1, int y2, float(*vect)(float x)) {
		int tmp = 0, pre, i, j;
		int addition;
		float y;

		if (sglMode != BIT_MAP && !innerFunc)return;
		if (checkThread())return;

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
	void floodFill(int x, int y, RGB c) {
		vec2i Q[SG_QSIZE];
		vec2i tmp;
		int front = -1, rear = 0, p;

		if (sglMode != BIT_MAP && !innerFunc)return;
		if (checkThread())return;

#define ENQUE(x) Q[(++front)%SG_QSIZE] = x
#define DEQUE() Q[(rear++)%SG_QSIZE]
#define ISEMPTY() ((front+1)%SG_QSIZE==(rear%SG_QSIZE))

		tmp.x = x;
		tmp.y = y;
		ENQUE(tmp);

		while (!ISEMPTY()) {
			tmp = DEQUE();
			p = (tmp.y*buffer->sizeX + tmp.x) * 3;
			if ((buffer->data[p] == c.b&&buffer->data[p + 1] == c.g&&buffer->data[p + 2] == c.r) || (buffer->data[p] == rgb[2] && buffer->data[p + 1] == rgb[1] && buffer->data[p + 2] == rgb[0]))continue;
			if (tmp.x < 0 || tmp.x >= buffer->sizeX)continue;
			if (tmp.y < 0 || tmp.y >= buffer->sizeY)continue;
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
	void textLoop() {
		int i;

		for (i = 0; i < changed->width * changed->height; i++) {
			if (changed->content[i] != content->content[i]) {
				content->content[i] = changed->content[i];
				textRenew(content->content[i], i % changed->width, i / changed->width);
			}
		}
	}
	void setBfc(int bgc, int fgc) {
		bfc = (bgc << 4) | fgc;
	}
	void setCharColor(char color, int x, int y) {
		int pos;

		pos = y * changed->width + x;
		changed->content[pos] &= 0x00FF;
		changed->content[pos] |= (color << 8);
	}
	void setCharBgc(char color, int x, int y) {
		int pos;

		pos = y * changed->width + x;
		changed->content[pos] &= 0x0FFF;
		changed->content[pos] |= ((color & 0x0F) << 12);
	}
	void setCharFgc(char color, int x, int y) {
		int pos;

		pos = y * changed->width + x;
		changed->content[pos] &= 0xF0FF;
		changed->content[pos] |= ((color & 0x0F) << 8);
	}
	void clearText() {
		int i;

		for (i = 0; i < changed->width*changed->height; i++) {
			writeChar(0, i%changed->width, i / changed->width);
		}
	}
	int getShort(int x, int y) {
		return changed->content[y * changed->width + x];
	}
	void writeChar(char c, int x, int y) {
		int pos;

		pos = y * changed->width + x;
		changed->content[pos] = ((short)bfc << 8) | (short)c;
	}
	void writeString(const char *s, int x, int y) {
		int i;

		for (i = 0; s[i] != '\0'; i++) {
			writeChar(s[i], x++, y);
		}
	}
	int getText(int left, int top, int right, int bottom, textMap *text) {
		int i;

		if (right <left)left ^= right ^= left ^= right;
		if (bottom < top)top ^= bottom ^= top ^= bottom;

		if (right > changed->width - 1)right = changed->width - 1;
		if (left < 0)right = 0;
		if (bottom > changed->height - 1)right = changed->height - 1;
		if (top < 0)top = 0;

		text->width = right - left + 1;
		text->height = bottom - top + 1;
		text->content = (unsigned short*)malloc(text->width * text->height * sizeof(short));
		if (text->content == NULL)return SG_NO_LOAD_MEM;

		for (i = 0; i < text->height; i++) {
			memcpy(text->content + (i*text->width), changed->content + (top + i) * changed->width + left, text->width * sizeof(short));
		}

		return SG_NO_ERORR;
	}
	void putText(int left, int top, textMap *text) {
		int x1, x2, y1, y2, i;

		if (left >= changed->width)return;
		if (top >= changed->height)return;
		if (left + text->width < 0)return;
		if (top + text->height < 0)return;

		if (left < 0)x1 = 0;
		else x1 = left;
		if (top < 0)y1 = 0;
		else y1 = top;

		if (left + text->width >= changed->width)x2 = changed->width - 1;
		else x2 = left + text->width - 1;
		if (top + text->height >= changed->height)y2 = changed->height - 1;
		else y2 = top + text->height - 1;

		for (i = 0; i < text->height; i++) {
			memcpy(changed->content + (top + i) * changed->width + left, text->content + (i*text->width), text->width * sizeof(short));
		}
	}
	int showFps() {
		if (sglMode != BIT_MAP)return SG_INVALID_MODE;
		fps = 1;
		return SG_NO_ERORR;
	}

	void drawPanel() {
		float alpha;
		int x, y, square, i, total;
		char *names[SG_MAX_PANEL_FUNCTION];

		if (panel->en == 0)return;

		if (mouseStatus(SG_RIGHT_BUTTON) == SG_BUTTON_DOWN) {
			getImage(panel->x - panel->outer - panel->inner, panel->y - panel->outer - panel->inner,
				panel->x + panel->outer + panel->inner, panel->y + panel->outer + panel->inner, panel->cover);

			alpha = getAlpha();
			setAlpha(.5f);
			setColor(255, 255, 255);
			putCircle(panel->x, panel->y, panel->outer, SOLID_FILL);

			x = mousePos().x;
			y = mousePos().y;
			x -= panel->x;
			y -= panel->y;
			square = x * x + y * y;
			if (sqrt(square) > panel->outer) {
				x = (int)(x / (sqrt(square) / panel->outer));
				y = (int)(y / (sqrt(square) / panel->outer));
			}
			putCircle(panel->x + x, panel->y + y, panel->inner, SOLID_FILL);

			if (square > 400) {
				setAlpha(1.f);
				setColor(0, 0, 0);

				total = 0;
				for (i = 0; i < SG_MAX_PANEL_FUNCTION; i++) {
					if (panel->normalPanel[i]) {
						names[total] = panel->normalPanel[i]->name;
						total++;
					}
				}
				switch (total) {
				case 0:
					break;
				case 1:
					putString(names[0],
						panel->x - stringWidth(names[0], (int)strlen(names[0])) / 2,
						panel->y - 10);
					break;
				case 2:
					if (x > 0)
						putString(names[0],
							panel->x - stringWidth(names[0], (int)strlen(names[0])) / 2,
							panel->y - 10);
					else
						putString(names[1],
							panel->x - stringWidth(names[1], (int)strlen(names[1])) / 2,
							panel->y - 10);
					break;
				case 3:
					if (y < 0 && (x == 0 ||
						(x > 0 && (-y) > x*tanf((float)PI / 6)) ||
						(x < 0 && (-y) > x*(-tanf((float)PI / 6)))))
						putString(names[0],
							panel->x - stringWidth(names[0], (int)strlen(names[0])) / 2,
							panel->y - 10);
					else if (x > 0)
						putString(names[1],
							panel->x - stringWidth(names[1], (int)strlen(names[1])) / 2,
							panel->y - 10);
					else
						putString(names[2],
							panel->x - stringWidth(names[2], (int)strlen(names[2])) / 2,
							panel->y - 10);
					break;
				case 4:
					if (x > y)
						if (x < -y)
							putString(names[0],
								panel->x - stringWidth(names[0], (int)strlen(names[0])) / 2,
								panel->y - 10);
						else
							putString(names[1],
								panel->x - stringWidth(names[1], (int)strlen(names[1])) / 2,
								panel->y - 10);
					else
						if (x > -y)
							putString(names[2],
								panel->x - stringWidth(names[2], (int)strlen(names[2])) / 2,
								panel->y - 10);
						else
							putString(names[3],
								panel->x - stringWidth(names[3], (int)strlen(names[3])) / 2,
								panel->y - 10);
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
	void clearPanel() {
		if (panel->en == 0)return;

		if (panel->cover->data) {
			putImage((panel->x - panel->outer - panel->inner) < 0 ? 0 : (panel->x - panel->outer - panel->inner),
				(panel->y - panel->outer - panel->inner) < 0 ? 0 : (panel->y - panel->outer - panel->inner),
				panel->cover, COPY_PUT);
			free(panel->cover->data);
			panel->cover->data = NULL;
		}

	}

	void drawWidgets(int fb) {
		for (unsigned int i = 0; i < widgets.size(); i++) {
			Widget *w = widgets[i];

			if (!w || !w->visible || w->priority != fb)continue;

			if (w->valid)continue;
			w->valid = 1;

			drawingWidget = i;
			w->draw(i);
			drawingWidget = -1;
		}
	}
	void widgetCover(int id, int left, int top, int right, int bottom) {
		for (int i = id + 1; i < widgets.size(); i++) {
			if (crossWidget(widgets[i]->getObj(), left, top, right, bottom)) {
				int prev = drawingWidget;
				drawingWidget = i;
				if(widgets[i]->visible)widgets[i]->draw(i);
				drawingWidget = prev;
			}
		}
	}
	void setBgRefresh(void(*refresh)(int left, int top, int right, int bottom)) {
		backgroundRefresh = refresh;
	}
	void useBgRefresh(int left, int top, int right, int bottom) {
		backgroundRefresh(left, top, right, bottom);
	}
};

#endif

