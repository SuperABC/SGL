#include "game.h"
#include <ctime>

vector<Position> getSelection(State& s, ChessType type) {
	vector<Position> result;
	Position nextPosition;
	for (int i(0); i != 8; ++i) {
		nextPosition.x = i;
		for (int j(0); j != 8; ++j) {
			nextPosition.y = j;
			if (canExecute(s, nextPosition, type)) {
				result.push_back(nextPosition);
			}
		}
	}
	return result;
}

bool isFinal(State& s);

bool isEnd(false);

struct Vertex {
	Vertex(State ss, Position last, ChessType type, Vertex* father) : parent(father),
		q(0), visitedNum(0), lastPosition(last),
		child(nullptr), brother(nullptr), lastType(type),
		isKnown(64, false), s(ss), childNum(0) {
		execute(s, last, type);
		selectionSet = getSelection(s, static_cast<ChessType>(1 - lastType));
	}
	vector<Position> selectionSet;
	vector<bool> isKnown;
	State s;
	Position lastPosition;
	ChessType lastType;
	int q;
	int childNum;
	int visitedNum;
	Vertex* parent, *child, *brother;
};

void freeTree(Vertex* root) {
	if (root == nullptr) {
		return;
	}
	freeTree(root->child);
	freeTree(root->brother);
	delete root;
}

void addChild(Vertex* parent, Vertex* child) {
	if (parent->child == nullptr) {
		parent->child = child;
	}
	else {
		Vertex* lastChild = parent->child;
		while (lastChild->brother != nullptr) {
			lastChild = lastChild->brother;
		}
		lastChild->brother = child;
	}
	++(parent->childNum);
}

Vertex* treePolicy(Vertex * v);

int defaultPolicy(State s, ChessType type);

Vertex* bestChild(Vertex* v, int c);

Vertex* expand(Vertex* v);

void backUp(Vertex* v, int deta);

Position getNextPosition2(State& s, ChessType type) {
	Vertex* v0 = new Vertex(s, Position(-1, -1), static_cast<ChessType>(1 - type), nullptr);
	Position result;
	int i(0);
	isEnd = false;
	time_t start = clock(), end;
	while (true) {
		Vertex* vl = treePolicy(v0);
		end = clock();
		if (end - start > 3000 || isEnd) {
			break;
		}
		int deta = defaultPolicy(vl->s, vl->lastType);
		backUp(vl, deta);
		++i;
	}
	result = bestChild(v0, 0)->lastPosition;
	freeTree(v0);
	return result;
}

bool canExpand(Vertex* v) {
	if (v->child == nullptr) {
		return true;
	}
	if (v->selectionSet.size() == 0) {
		return v->childNum == 0 ? true : false;
	}
	if (v->childNum < v->selectionSet.size()) {
		return true;
	}
	return false;
}

bool isFinal(State& s) {
	bool isFull(true);
	for (int i(0); i != 8; ++i) {
		for (int j(0); j != 8; ++j) {
			if (s[i][j] == noChess) {
				isFull = false;
				break;
			}
		}
	}
	if (isFull) {
		return true;
	}
	if (!canInput(s, black) && !canInput(s, white)) {
		return true;
	}
	return false;
}

Vertex* treePolicy(Vertex * v) {
	while (!isFinal(v->s)) {
		if (canExpand(v)) {
			return expand(v);
		}
		v = bestChild(v, 1);
	}
	isEnd = true;
	return v;
}

Vertex* expand(Vertex* v) {
	int nodeNum = v->selectionSet.size();
	if (nodeNum == 0) {
		Vertex* newNode = new Vertex(v->s, Position{ -1, -1 }, static_cast<ChessType>(1 - v->lastType), v);
		addChild(v, newNode);
		v->isKnown[0] = true;
		return newNode;
	}
	for (int i(0); i != nodeNum; ++i) {
		if (v->isKnown[i])
			continue;
		Vertex* newNode = new Vertex(v->s, v->selectionSet[i], static_cast<ChessType>(1 - v->lastType), v);
		addChild(v, newNode);
		v->isKnown[i] = true;
		return newNode;
	}
}

Vertex* bestChild(Vertex* v, int c) {
	Vertex* child = v->child;
	Vertex* result = child;
	Vertex* boundResult = result;
	bool boundFlag(false);
	int x, y;
	double value = 0, temp = 0, boundValue = 0;
	while (child != nullptr) {
		temp = child->q*1.0 / child->visitedNum + c*1.0 * sqrt(2 * log(v->visitedNum) / child->visitedNum);
		x = child->lastPosition.x;
		y = child->lastPosition.y;
		if (x == 7 || y == 7 || x == 0 || y == 0) {
			if (temp > boundValue) {
				boundFlag = true;
				boundResult = child;
				boundValue = temp;
			}
		}
		else {
			if (temp > value) {
				result = child;
				value = temp;
			}
		}
		child = child->brother;
	}
	return boundFlag ? boundResult : result;
}

int defaultPolicy(State s, ChessType type) {
	int tempType = 1 - type;
	while (!isFinal(s)) {
		vector<Position> selectionSet = getSelection(s, static_cast<ChessType>(tempType));
		if (selectionSet.size() == 0) {
			tempType = 1 - tempType;
			continue;
		}
		int i = rand() % selectionSet.size();
		execute(s, selectionSet[i], static_cast<ChessType>(tempType));
		tempType = 1 - tempType;
	}
	return isWin(s, type);
}

void backUp(Vertex* v, int deta) {
	while (v != nullptr) {
		++(v->visitedNum);
		v->q += deta;
		deta = 1 - deta;
		v = v->parent;
	}
}
