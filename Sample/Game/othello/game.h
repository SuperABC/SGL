#pragma once
#ifndef _MINIALPHAGO_H_
#include <iostream>
#include <vector>
#include "winsgl.h"

//#pragma comment(lib, SG_LIB("winsgl"))

using namespace std;
using ChessType = enum { white, black, noChess };
struct Row {
	Row() {
		for (int i(0); i != 8; ++i) {
			row[i] = noChess;
		}
	}
	Row(Row& r) {
		memcpy(row, r.row, 8 * sizeof(int));
	}
	int& operator [](int i) {
		return row[i];
	}
	Row& operator = (Row& r) {
		memcpy(row, r.row, 8 * sizeof(int));
	}
	int row[8];
};
struct State {
	Row& operator [](int i) {
		return board[i];
	}
	Row board[8];
};
struct Position {
	Position(int xx = 0, int yy = 0) : x(xx), y(yy) {}
	int x, y;
};

void initial();
Position getNextPosition(State& s, ChessType type);
void execute(State& s, Position nextPosition, ChessType type);
bool isWin(State& s, ChessType type);
bool canInput(State& s, ChessType type);
void printState(ChessType type);
bool canExecute(State& s, Position nextPosition, ChessType type);
void putCheck();
vector<Position> getSelection(State& s, ChessType type);
Position getNextPosition2(State& s, ChessType type);


#endif // !_MINIALPHAGO_H_
