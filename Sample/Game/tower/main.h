#pragma once
#define __assembler
#include "sgl.h"
#define SIZE 40
#define inRect(x, y, x1, y1, x2, y2) ((x>=x1&&x<=x2)&&(y>=y1&&y<=y2))

typedef struct {
	int x, y;
	int face;
	int floor;
	int money;
	int level;
	int exp;
	int blood;
	int offence;
	int deffence;
	int time;
	int outlook;
	int box[100];
	char *name;
	bitMap *oldImage;
}knight;
typedef struct {
	int pos[11][11];
	int upx, upy;
	int downx, downy;
	int upstairx, upstairy;
	int downstairx, downstairy;
}tower;
typedef struct {
	int blood;
	int offence;
	int deffence;
	int money;
	int exp;
}opponent;
enum proProc {
	START,
	INSTRUCT,
	ARCHIVE,
	FIGURE,
	DIFFICULTY,
	NAME,
	PLAYING,
	FIGHT,
	PAUSE,
	BUY,
	DIE,
	SAVE,
	DICTION
};

void start();
void instruct();
void archive();
void figure();
void difficulty();
void name();
void playing();
void fight();
void pause();
void buy();
void die();
void save();
void diction();

void initPro();
void drawNumber(int num, int x, int y, int bs);
void drawNumbers(int rx, int ry, int temp, int digit, int bs);
void showArchive();
int loadFile(int card);
void saveFile(int card);
int up();
int down();
int left();
int right();
void putBlock(int x, int y, int style);
void showPlayer(int x, int y, int face);
void showMsg();
int response();
void putInfo(int count, int style);

