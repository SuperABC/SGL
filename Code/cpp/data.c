#define _CRT_SECURE_NO_WARNINGS
#include "..\include\inner.h"
#include "..\include\winsgl.h"

#define JSON_INIT_SIZE 16

enum Type {
	JSON_INT,
	JSON_FLOAT,
	JSON_CHAR,
	JSON_BOOL,
	JSON_STRING,
	JSON_OBJECT
};
union Data {
	SGint json_int;
	SGfloat json_float;
	SGchar json_char;
	SGbool json_bool;
	SGstring json_string;
	SGvoid *json_object;
};
struct Item {
	enum Type t;

	char *name;
	union Data data;
};

struct JSON {
	struct Item list[JSON_INIT_SIZE];
};

struct JSON *readJson(const char *json) {

}
char *writeJson(struct JSON *json) {

}
union Data getElement(struct JSON *json, const char *name) {

}
void setIntElement(struct JSON *json, const char *name, SGint i) {

}
void setFloatElement(struct JSON *json, const char *name, SGfloat f) {

}
void setCharElement(struct JSON *json, const char *name, SGchar c) {

}
void setBoolElement(struct JSON *json, const char *name, SGbool b) {

}
void setStringElement(struct JSON *json, const char *name, SGstring s) {

}
void setObjectElement(struct JSON *json, const char *name, SGvoid *o) {

}
