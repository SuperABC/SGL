#pragma once
#include "screen.h"
#pragma comment(lib, SG_Lib("sgl"))
#define __assembler
#define inRect(x, y, x1, y1, x2, y2) ((x>=x1&&x<=x2)&&(y>=y1&&y<=y2))

enum proProc {
	START,
	INSTRUCT,
	SETTING,
	CHOOSE,
	PLAYING,
	SCORE
};

struct set {
	int speed;
	int effect;
	int skin;
};
struct archive {
	int interval;
	int begin;
	int total;
	long *data;
};
struct status {
	int pressed;
};

void start();
void instruct();
void setting();
void choose();
void playing();
void score();

void initPro();
void readSong();
void operate();
