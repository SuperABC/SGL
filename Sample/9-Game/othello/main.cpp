#include "game.h"

State  state;
int chessNum(0);

void sgSetup() {
	initial();
	initWindow(480, 540, "Othello", BIT_MAP);
	initMouse(SG_COORDINATE);
}
void sgLoop() {
	static bool inputFlag(true);
	static Position nextPosition;
	static int first = 1;
	putCheck();
	printState(inputFlag ? black : white);
	setColor(0, 0, 0);
	putString(!inputFlag ?
		"°×Æå£¨AI£©Âä×Ó£º" :
		"ºÚÆå£¨Íæ¼Ò£©Âä×Ó£º",
		16, 16);
	if (chessNum == 64 || (!canInput(state, black) && !canInput(state, white))) {
		setColor(0, 0, 0);
		if (isWin(state, black)) {
			putString("Black Win!", 0, 0);
		}
		else {
			putString("White Win!", 0, 0);
		}
		return;
	}
	if (first) {
		first = 0;
		return;
	}
	vecThree tmpMouse;
	if (inputFlag) {
		setColor(0, 0, 0);
		if (canInput(state, black)) {
			if (biosMouse(1).m) {
				tmpMouse = biosMouse(0);
				if (tmpMouse.y < 60)return;
			}
			else return;
			nextPosition.x = tmpMouse.x / 60;
			nextPosition.y = tmpMouse.y / 60 - 1;
			while (!canExecute(state, nextPosition, black)) {
				if (biosMouse(1).m) {
					tmpMouse = biosMouse(0);
					if (tmpMouse.y < 60)return;
				}
				else return;
				nextPosition.x = tmpMouse.x / 60;
				nextPosition.y = tmpMouse.y / 60 - 1;
			}
			execute(state, nextPosition, black);
		}
	}
	else {
		if (canInput(state, white)) {
			Position nextPosition = getNextPosition2(state,white);
			if (nextPosition.x == -1)return;
			execute(state, nextPosition, white);
		}
	}
	inputFlag = !inputFlag;
	putCheck();
	printState(inputFlag ? black : white);
	setColor(0, 0, 0);
	putString(!inputFlag ?
		"°×Æå£¨AI£©Âä×Ó£º" :
		"ºÚÆå£¨Íæ¼Ò£©Âä×Ó£º",
		16, 16);
}

void putCheck() {
	setColor(255, 255, 0);
	clearScreen();
	setColor(127, 127, 0);
	for (int i = 0; i < 8; i++) {
		putLine(0, 60 * i + 60, 479, 60 * i + 60, 0);
		putLine(60 * i, 60, 60 * i, 539, 0);
	}
}

void printState(ChessType type) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			switch (state[i][j]) {
			case white:
				setColor(255, 255, 255);
				putCircle(i * 60 + 30, j * 60 + 90, 20, SOLID_FILL);
				break;
			case black:
				setColor(0, 0, 0);
				putCircle(i * 60 + 30, j * 60 + 90, 20, SOLID_FILL);
				break;
			case noChess:
				break;
			}
		}
	}
	setColor(255, 127, 127);
	vector<Position> selections = getSelection(state,type);
	for (auto &it : selections) {
		putCircle(it.x * 60 + 30, it.y * 60 + 90, 10, EMPTY_FILL);
		putCircle(it.x * 60 + 30, it.y * 60 + 90, 9, EMPTY_FILL);
		putCircle(it.x * 60 + 30, it.y * 60 + 90, 8, EMPTY_FILL);
	}

}

void initial() {
	state[3][3] = state[4][4] = white;
	state[3][4] = state[4][3] = black;
}

bool isWin(State& s, ChessType type) {
	int result[2] = { 0, 0 };
	for (int i(0); i != 8; ++i) {
		for (int j(0); j != 8; ++j) {
			if (s[i][j] == noChess)
				continue;
			++result[s[i][j]];
			
		}
	}
	if (result[type] > result[!type]) {
		return true;
	}
	return false;
}

void execute(State& s, Position nextPosition, ChessType type) {
	int x(nextPosition.x), y(nextPosition.y);
	if (x < 0)
		return;
	s[x][y] = type;
	int differentNum(0);
	for (int i(x); i != 8; ++i) {
		if (i == x) {
			continue;
		}
		if (s[i][y] == noChess) {
			break;
		}
		else if (s[i][y] != type) {
			++differentNum;
		}
		else if (differentNum == 0) {
			break;
		}
		else {
			for (int k(x + 1); k != i; ++k) {
				s[k][y] = type;
			}
			break;
		}
	}
	differentNum = 0;
	for (int i(x); i >= 0; --i) {
		if (i == x) {
			continue;
		}
		if (s[i][y] == noChess) {
			break;
		}
		else if (s[i][y] != type) {
			++differentNum;
		}
		else if (differentNum == 0) {
			break;
		}
		else {
			for (int k(x - 1); k != i; --k) {
				s[k][y] = type;
			}

			break;
		}
	}
	differentNum = 0;
	for (int j(y); j != 8; ++j) {
		if (j == y) {
			continue;
		}
		if (s[x][j] == noChess) {
			break;
		}
		else if (s[x][j] != type) {
			++differentNum;
		}
		else if (differentNum == 0) {
			break;
		}
		else {
			for (int k(y + 1); k != j; ++k) {
				s[x][k] = type;
			}

			break;
		}
	}
	differentNum = 0;
	for (int j(y); j >= 0; --j) {
		if (j == y) {
			continue;
		}
		if (s[x][j] == noChess) {
			break;
		}
		else if (s[x][j] != type) {
			++differentNum;
		}
		else if (differentNum == 0) {
			break;
		}
		else {
			for (int k(y - 1); k != j; --k) {
				s[x][k] = type;
			}

			break;
		}
	}
	differentNum = 0;
	for (int i(x), j(y); i >= 0 && j >= 0; --i, --j) {
		if (j == y) {
			continue;
		}
		if (s[i][j] == noChess) {
			break;
		}
		else if (s[i][j] != type) {
			++differentNum;
		}
		else if (differentNum == 0) {
			break;
		}
		else {
			for (int ii(x - 1), jj(y - 1); ii != i && jj != j; --ii, --jj) {
				s[ii][jj] = type;
			}

			break;
		}
	}
	differentNum = 0;
	for (int i(x), j(y); i != 8 && j >= 0; ++i, --j) {
		if (j == y) {
			continue;
		}
		if (s[i][j] == noChess) {
			break;
		}
		else if (s[i][j] != type) {
			++differentNum;
		}
		else if (differentNum == 0) {
			break;
		}
		else {
			for (int ii(x + 1), jj(y - 1); ii != i && jj != j; ++ii, --jj) {
				s[ii][jj] = type;
			}

			break;
		}
	}
	differentNum = 0;
	for (int i(x), j(y); i >= 0 && j != 8; --i, ++j) {
		if (j == y) {
			continue;
		}
		if (s[i][j] == noChess) {
			break;
		}
		else if (s[i][j] != type) {
			++differentNum;
		}
		else if (differentNum == 0) {
			break;
		}
		else {
			for (int ii(x - 1), jj(y + 1); ii != i && jj != j; --ii, ++jj) {
				s[ii][jj] = type;
			}

			break;
		}
	}
	differentNum = 0;
	for (int i(x), j(y); i != 8 && j != 8; ++i, ++j) {
		if (j == y) {
			continue;
		}
		if (s[i][j] == noChess) {
			break;
		}
		else if (s[i][j] != type) {
			++differentNum;
		}
		else if (differentNum == 0) {
			break;
		}
		else {
			for (int ii(x + 1), jj(y + 1); ii != i && jj != j; ++ii, ++jj) {
				s[ii][jj] = type;
			}

			break;
		}
	}
}

bool canExecute(State& s, Position nextPosition, ChessType type) {
	int x(nextPosition.x), y(nextPosition.y);
	if (x < 0 || x > 7 || y < 0 || y > 7) {
		return false;
	}
	if (s[x][y] != noChess) {
		return false;
	}
	int differentNum(0);
	for (int i(x); i != 8; ++i) {
		if (i == x) {
			continue;
		}
		if (s[i][y] == noChess) {
			break;
		}
		else if (s[i][y] != type) {
			++differentNum;
		}
		else if (differentNum == 0) {
			break;
		}
		else {
			return true;
		}
	}
	differentNum = 0;
	for (int i(x); i >= 0; --i) {
		if (i == x) {
			continue;
		}
		if (s[i][y] == noChess) {
			break;
		}
		else if (s[i][y] != type) {
			++differentNum;
		}
		else if (differentNum == 0) {
			break;
		}
		else {
			return true;
		}
	}
	differentNum = 0;
	for (int j(y); j != 8; ++j) {
		if (j == y) {
			continue;
		}
		if (s[x][j] == noChess) {
			break;
		}
		else if (s[x][j] != type) {
			++differentNum;
		}
		else if (differentNum == 0) {
			break;
		}
		else {
			return true;
		}
	}
	differentNum = 0;
	for (int j(y); j >= 0; --j) {
		if (j == y) {
			continue;
		}
		if (s[x][j] == noChess) {
			break;
		}
		else if (s[x][j] != type) {
			++differentNum;
		}
		else if (differentNum == 0) {
			break;
		}
		else {
			return true;
		}
	}
	differentNum = 0;
	for (int i(x), j(y); i >= 0 && j >= 0; --i, --j) {
		if (j == y) {
			continue;
		}
		if (s[i][j] == noChess) {
			break;
		}
		else if (s[i][j] != type) {
			++differentNum;
		}
		else if (differentNum == 0) {
			break;
		}
		else {
			return true;
		}
	}
	differentNum = 0;
	for (int i(x), j(y); i != 8 && j >= 0; ++i, --j) {
		if (j == y) {
			continue;
		}
		if (s[i][j] == noChess) {
			break;
		}
		else if (s[i][j] != type) {
			++differentNum;
		}
		else if (differentNum == 0) {
			break;
		}
		else {
			return true;
		}
	}
	differentNum = 0;
	for (int i(x), j(y); i >= 0 && j != 8; --i, ++j) {
		if (j == y) {
			continue;
		}
		if (s[i][j] == noChess) {
			break;
		}
		else if (s[i][j] != type) {
			++differentNum;
		}
		else if (differentNum == 0) {
			break;
		}
		else {
			return true;
		}
	}
	differentNum = 0;
	for (int i(x), j(y); i != 8 && j != 8; ++i, ++j) {
		if (j == y) {
			continue;
		}
		if (s[i][j] == noChess) {
			break;
		}
		else if (s[i][j] != type) {
			++differentNum;
		}
		else if (differentNum == 0) {
			break;
		}
		else {
			return true;
		}
	}
	return false;
}

bool canInput(State& s, ChessType type) {
	Position nextPosition;
	for (int i(0); i != 8; ++i) {
		nextPosition.x = i;
		for (int j(0); j != 8; ++j) {
			nextPosition.y = j;
			if (canExecute(s, nextPosition, type)) {
				return true;
			}
		}
	}
	return false;
}

Position getNextPosition(State& s, ChessType type) {
	Position nextPosition;
	vecThree tmpMouse;
	setColor(0, 0, 0);
	putString("White's turn:", 0, 0);
	if (biosMouse(1).m) {
		tmpMouse = biosMouse(0);
		if (tmpMouse.y < 60)return{ -1 };
	}
	else return{ -1 };
	nextPosition.x = tmpMouse.x / 60;
	nextPosition.y = tmpMouse.y / 60 - 1;
	while (!canExecute(s, nextPosition, type)) {
		if (biosMouse(1).m) {
			tmpMouse = biosMouse(0);
			if (tmpMouse.y < 60)return{ -1 };
		}
		else return{ -1 };
		nextPosition.x = tmpMouse.x / 60;
		nextPosition.y = tmpMouse.y / 60 - 1;
	}
	return nextPosition;
}
