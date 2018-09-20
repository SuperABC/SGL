#define _CRT_SECURE_NO_WARNINGS
#include "lexeme.h"

Lexeme::Lexeme(const char *input) {
	if (input == NULL)return;
	content = std::string(input);
}
Lexeme::~Lexeme() {

}

vector<tokenPrim> Lexeme::parse() {
	int len = content.length();
	bool start = true;
	for (int i = 0; i < len; i++) {
		tokenPrim *node = new tokenPrim();
		if (content[i] >= '0' && content[i] <= '9' || content[i] == '-') {
			if (content[i] == '-' && (content[i + 1] < '0' || content[i + 1] > '9')) {
				node->type = TT_OP;
				node->value = 0;
				node->id = OP_MINUS;
				output.push_back(*node);
				continue;
			}
			else if (content[i] == '-' && content[i + 1] >= '0' && content[i + 1] <= '9') {
				if (!start) {
					node->type = TT_OP;
					node->value = 0;
					node->id = OP_MINUS;
					output.push_back(*node);
					continue;
				}
			}
			node->type = TT_NUM;
			node->id = 0;
			node->value = float(atof(content.c_str() + i));
			output.push_back(*node);
			start = false;
			i++;
			bool point = false;
			while (content[i] >= '0' && content[i] <= '9' || content[i] == '.') {
				if (content[i] == '.') {
					if (point)break;
					else point = true;
				}
				i++;
			}
			i--;
			if (content[i] == '.')i--;
			continue;
		}
		else {
			node->type = TT_OP;
			std::string str;
			char *a = new char[32];
			switch (content[i]) {
			case '+':
				node->id = OP_PLUS;
				output.push_back(*node);
				continue;
			case '-':
				node->id = OP_MINUS;
				output.push_back(*node);
				continue;
			case '*':
				node->id = OP_MULTY;
				output.push_back(*node);
				continue;
			case '/':
				node->id = OP_DIVIDE;
				output.push_back(*node);
				continue;
			case '(':
				node->id = OP_LBRACE;
				output.push_back(*node);
				start = true;
				continue;
			case ')':
				node->id = OP_RBRACE;
				output.push_back(*node);
				start = false;
				continue;
			case ',':
				node->id = OP_COMMA;
				output.push_back(*node);
				start = true;
				continue;
			case 'l':
				node->id = OP_LOG;
				output.push_back(*node);
				i += 2;
				continue;
			case 'e':
				node->id = OP_EXP;
				output.push_back(*node);
				i += 2;
				continue;
			case 'r':
				node->id = OP_ROOT;
				output.push_back(*node);
				i += 3;
				continue;
			case 'm':
				node->id = OP_MOD;
				output.push_back(*node);
				i += 2;
				continue;
			case 'a':
				node->id = OP_ANS;
				output.push_back(*node);
				i += 2;
				continue;
			}
		}
	}
	return output;
}

