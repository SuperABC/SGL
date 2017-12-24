#ifndef LEXEME_H
#define LEXEME_H
#include <vector>
#include <string>

#define _CRT_SECURE_NO_WARNINGS
#define SYS_ID_NUM 37

using std::vector;
using std::string;

enum TOKENTYPE {
	TT_NUM,
	TT_OP,
};

enum OPERATOR {
	OP_NONE,
	OP_PLUS,
	OP_MINUS,
	OP_MULTY,
	OP_DIVIDE,
	OP_MOD,
	OP_LBRACE,
	OP_RBRACE,
	OP_COMMA,
	OP_DOT,
	OP_EXP,
	OP_LOG,
	OP_ROOT,
	OP_ANS
};

typedef struct {
	enum TOKENTYPE type;
	int id;
	float value;
}tokenPrim;

class Lexeme {
private:
	string content;

public:
	vector<tokenPrim> output;

	Lexeme(const char *input = NULL);
	~Lexeme();

	std::vector<tokenPrim>parse();
};

#endif