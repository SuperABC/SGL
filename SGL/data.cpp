#define _CRT_SECURE_NO_WARNINGS

#include <tchar.h>
#include <direct.h>
#include <exception>
#include "winsgl.h"
#include "inner.h"
#include "util.h"

using std::exception;
using std::string;


/*
* SG json interfaces
* These functions are used to deal with json data.
*/

int subObjectCont(struct JSON *obj, char *name, SGtext json, int i);
int subArrayCont(struct JSON *obj, char *name, SGtext json, int i);
int subObjectElement(struct JSON *obj, int idx, SGtext json, int i);
int subArrayElement(struct JSON *obj, int idx, SGtext json, int i);

struct JSON *createJson() {
	int i;

	struct JSON *ret = (struct JSON *)malloc(sizeof(struct JSON));
	ret->arr = 0;
	for (i = 0; i < SG_HASH_NUM; i++) {
		ret->hash[i] = NULL;
	}

	return ret;
}
struct JSON *createJsonArray() {
	struct JSON *ret = (struct JSON *)malloc(sizeof(struct JSON));
	ret->arr = 1;

	ret->list = NULL;

	return ret;
}
void freeItem(struct JSON_Item *item) {
	switch (item->t) {
	case JSON_OBJECT:
		freeJson(item->data.json_object);
		break;
	case JSON_ARRAY:
		freeJson(item->data.json_array);
		break;
	default:
		break;
	}
	free(item);
}
void freeJson(struct JSON *json) {
	int i;
	struct JSON_Item *pre, *post;

	if (json->arr == 1) {
		if (json->list == NULL)return;

		pre = json->list;

		while (pre) {
			post = pre->next;
			freeItem(pre);
			pre = post;
		}
	}
	else {
		for (i = 0; i < SG_HASH_NUM; i++) {
			if (json->hash[i] == NULL)continue;

			pre = json->hash[i];

			while (pre) {
				post = pre->next;
				freeItem(pre);
				pre = post;
			}
		}
	}
}

int subObjectCont(struct JSON *obj, char *name, SGtext json, int i) {
	int j, point;
	char label[256];
	char cont[256];
	struct JSON *content = createJson();

	while (json[i++] != '{');
	while (json[i] == '\n' || json[i] == ' ' || json[i] == '\t')i++;

	while (json[i] != '}') {
		while (json[i++] != '\"');

		j = 0;
		while (json[i] != '\"') {
			label[j++] = json[i++];
		}
		label[j] = '\0';
		i++;

		while (json[i++] != ':');

		if (json[i] == '\"') {
			i++;

			j = 0;
			while (json[i] != '\"') {
				cont[j++] = json[i++];
			}
			cont[j] = '\0';

			setStringContent(content, label, cont);
			while (json[i] != '}' && json[i++] != ',');
		}
		else if (json[i] == '\'') {

		}
		else if (json[i] >= '0' && json[i] <= '9' || json[i] == '-') {
			point = 0;
			j = 0;
			cont[j++] = json[i++];
			while (json[i] >= '0' && json[i] <= '9' || json[i] == '.') {
				if (json[i] == '.') {
					if (point)break;
					else point = 1;
				}
				cont[j++] = json[i++];
			}
			cont[j] = '\0';
			if (point)setFloatContent(content, label, (float)atof(cont));
			else setIntContent(content, label, atoi(cont));
			while (json[i] != '}' && json[i++] != ',');
		}
		else if (json[i] == 't') {
			i += 4;
			setBoolContent(content, label, true);
			while (json[i] != '}' && json[i++] != ',');
		}
		else if (json[i] == 'f') {
			i += 5;
			setBoolContent(content, label, false);
			while (json[i] != '}' && json[i++] != ',');
		}
		else if (json[i] == '{') {
			i = subObjectCont(content, label, json, i);

			while (json[i] != '}' && json[i++] != ',');
		}
		else if (json[i] == '[') {
			i = subArrayCont(content, label, json, i);

			while (json[i] != '}' && json[i++] != ',');
		}

		while (json[i] == '\n' || json[i] == ' ' || json[i] == '\t')i++;
	}
	setObjectContent(obj, name, content);

	i++;
	return i;
}
int subArrayCont(struct JSON *obj, char *name, SGtext json, int i) {
	int j, point;
	char cont[256];
	struct JSON *content = createJsonArray();

	while (json[i++] != '[');
	while (json[i] == '\n' || json[i] == ' ' || json[i] == '\t')i++;

	while (json[i] != ']') {
		if (json[i] == '\"') {
			i++;

			j = 0;
			while (json[i] != '\"') {
				cont[j++] = json[i++];
			}
			cont[j] = '\0';

			setStringElement(content, INT_MAX, cont);
			while (json[i] != ']' && json[i++] != ',');
		}
		else if (json[i] == '\'') {

		}
		else if (json[i] >= '0' && json[i] <= '9' || json[i] == '-') {
			point = 0;
			j = 0;
			cont[j++] = json[i++];
			while (json[i] >= '0' && json[i] <= '9' || json[i] == '.') {
				if (json[i] == '.') {
					if (point)break;
					else point = 1;
				}
				cont[j++] = json[i++];
			}
			cont[j] = '\0';
			if (point)setFloatElement(content, INT_MAX, (float)atof(cont));
			else setIntElement(content, INT_MAX, atoi(cont));
			while (json[i] != ']' && json[i++] != ',');
		}
		else if (json[i] == 't') {
			i += 4;
			setBoolElement(content, INT_MAX, true);
			while (json[i] != ']' && json[i++] != ',');
		}
		else if (json[i] == 'f') {
			i += 5;
			setBoolElement(content, INT_MAX, false);
			while (json[i] != ']' && json[i++] != ',');
		}
		else if (json[i] == '{') {
			i = subObjectElement(content, INT_MAX, json, i);

			while (json[i] != ']' && json[i++] != ',');
		}
		else if (json[i] == '[') {
			i = subArrayElement(content, INT_MAX, json, i);

			while (json[i] != ']' && json[i++] != ',');
		}

		while (json[i] == '\n' || json[i] == ' ' || json[i] == '\t')i++;
	}
	setArrayContent(obj, name, content);

	i++;
	return i;
}
int subObjectElement(struct JSON *obj, int idx, SGtext json, int i) {
	int j, point;
	char label[256];
	char cont[256];
	struct JSON *element = createJson();

	while (json[i++] != '{');
	while (json[i] == '\n' || json[i] == ' ' || json[i] == '\t')i++;

	while (json[i] != '}') {
		while (json[i++] != '\"');

		j = 0;
		while (json[i] != '\"') {
			label[j++] = json[i++];
		}
		label[j] = '\0';
		i++;

		while (json[i++] != ':');

		if (json[i] == '\"') {
			i++;

			j = 0;
			while (json[i] != '\"') {
				cont[j++] = json[i++];
			}
			cont[j] = '\0';

			setStringContent(element, label, cont);
			while (json[i] != '}' && json[i++] != ',');
		}
		else if (json[i] == '\'') {

		}
		else if (json[i] >= '0' && json[i] <= '9' || json[i] == '-') {
			point = 0;
			j = 0;
			cont[j++] = json[i++];
			while (json[i] >= '0' && json[i] <= '9' || json[i] == '.') {
				if (json[i] == '.') {
					if (point)break;
					else point = 1;
				}
				cont[j++] = json[i++];
			}
			cont[j] = '\0';
			if (point)setFloatContent(element, label, (float)atof(cont));
			else setIntContent(element, label, atoi(cont));
			while (json[i] != '}' && json[i++] != ',');
		}
		else if (json[i] == 't') {
			i += 4;
			setBoolContent(element, label, true);
			while (json[i] != '}' && json[i++] != ',');
		}
		else if (json[i] == 'f') {
			i += 5;
			setBoolContent(element, label, false);
			while (json[i] != '}' && json[i++] != ',');
		}
		else if (json[i] == '{') {
			i = subObjectCont(element, label, json, i);

			while (json[i] != '}' && json[i++] != ',');
		}
		else if (json[i] == '[') {
			i = subArrayCont(element, label, json, i);

			while (json[i] != '}' && json[i++] != ',');
		}

		while (json[i] == '\n' || json[i] == ' ' || json[i] == '\t')i++;
	}
	setObjectElement(obj, INT_MAX, element);

	i++;
	return i;
}
int subArrayElement(struct JSON *obj, int idx, SGtext json, int i) {
	int j, point;
	char cont[256];
	struct JSON *element = createJsonArray();

	while (json[i++] != '[');
	while (json[i] == '\n' || json[i] == ' ' || json[i] == '\t')i++;

	while (json[i] != ']') {
		if (json[i] == '\"') {
			i++;

			j = 0;
			while (json[i] != '\"') {
				cont[j++] = json[i++];
			}
			cont[j] = '\0';

			setStringElement(element, INT_MAX, cont);
			while (json[i] != ']' && json[i++] != ',');
		}
		else if (json[i] == '\'') {

		}
		else if (json[i] >= '0' && json[i] <= '9' || json[i] == '-') {
			point = 0;
			j = 0;
			cont[j++] = json[i++];
			while (json[i] >= '0' && json[i] <= '9' || json[i] == '.') {
				if (json[i] == '.') {
					if (point)break;
					else point = 1;
				}
				cont[j++] = json[i++];
			}
			cont[j] = '\0';
			if (point)setFloatElement(element, INT_MAX, (float)atof(cont));
			else setIntElement(element, INT_MAX, atoi(cont));
			while (json[i] != ']' && json[i++] != ',');
		}
		else if (json[i] == 't') {
			i += 4;
			setBoolElement(element, INT_MAX, true);
			while (json[i] != ']' && json[i++] != ',');
		}
		else if (json[i] == 'f') {
			i += 5;
			setBoolElement(element, INT_MAX, false);
			while (json[i] != ']' && json[i++] != ',');
		}
		else if (json[i] == '{') {
			i = subObjectElement(element, INT_MAX, json, i);

			while (json[i] != ']' && json[i++] != ',');
		}
		else if (json[i] == '[') {
			i = subArrayElement(element, INT_MAX, json, i);

			while (json[i] != ']' && json[i++] != ',');
		}

		while (json[i] == '\n' || json[i] == ' ' || json[i] == '\t')i++;
	}
	setArrayElement(obj, INT_MAX, element);

	i++;
	return i;
}
struct JSON *readJson(SGtext json) {
	int i = 0, j;
	char point;
	char name[256];
	char cont[256];

	struct JSON *res;

	try {

		while (json[i] != '{' && json[i] != '[')i++;

		if (json[i] == '{') {
			res = createJson();

			i++;
			while (json[i] == '\n' || json[i] == ' ' || json[i] == '\t')i++;

			while (json[i] != '}') {
				while (json[i++] != '\"');

				j = 0;
				while (json[i] != '\"') {
					name[j++] = json[i++];
				}
				name[j] = '\0';
				i++;

				while (json[i++] != ':');

				if (json[i] == '\"') {
					i++;

					j = 0;
					while (json[i] != '\"') {
						cont[j++] = json[i++];
					}
					cont[j] = '\0';

					setStringContent(res, name, cont);
					while (json[i] != '}' && json[i++] != ',');
				}
				else if (json[i] == '\'') {

				}
				else if (json[i] >= '0' && json[i] <= '9' || json[i] == '-') {
					point = 0;
					j = 0;
					cont[j++] = json[i++];
					while (json[i] >= '0' && json[i] <= '9' || json[i] == '.') {
						if (json[i] == '.') {
							if (point)break;
							else point = 1;
						}
						cont[j++] = json[i++];
					}
					cont[j] = '\0';
					if (point)setFloatContent(res, name, (float)atof(cont));
					else setIntContent(res, name, atoi(cont));
					while (json[i] != '}' && json[i++] != ',');
				}
				else if (json[i] == 't' || json[i] == 'f') {

				}
				else if (json[i] == '{') {
					i = subObjectCont(res, name, json, i);

					while (json[i] != '}' && json[i++] != ',');
				}
				else if (json[i] == '[') {
					i = subArrayCont(res, name, json, i);

					while (json[i] != '}' && json[i++] != ',');
				}

				while (json[i] == '\n' || json[i] == ' ' || json[i] == '\t')i++;
			}
		}
		else if (json[i] == '[') {
			res = createJsonArray();

			i++;
			while (json[i] == '\n' || json[i] == ' ' || json[i] == '\t')i++;

			while (json[i] != ']') {

				if (json[i] == '\"') {
					i++;

					j = 0;
					while (json[i] != '\"') {
						cont[j++] = json[i++];
					}
					cont[j] = '\0';

					setStringContent(res, name, cont);
					while (json[i] != ']' && json[i++] != ',');
				}
				else if (json[i] == '\'') {

				}
				else if (json[i] >= '0' && json[i] <= '9' || json[i] == '-') {
					point = 0;
					j = 0;
					cont[j++] = json[i++];
					while (json[i] >= '0' && json[i] <= '9' || json[i] == '.') {
						if (json[i] == '.') {
							if (point)break;
							else point = 1;
						}
						cont[j++] = json[i++];
					}
					cont[j] = '\0';
					if (point)setFloatContent(res, name, (float)atof(cont));
					else setIntContent(res, name, atoi(cont));
					while (json[i] != ']' && json[i++] != ',');
				}
				else if (json[i] == 't' || json[i] == 'f') {

				}
				else if (json[i] == '{') {
					i = subObjectElement(res, INT_MAX, json, i);

					while (json[i] != ']' && json[i++] != ',');
				}
				else if (json[i] == '[') {
					i = subArrayElement(res, INT_MAX, json, i);

					while (json[i] != ']' && json[i++] != ',');
				}

				while (json[i] == '\n' || json[i] == ' ' || json[i] == '\t')i++;
			}
		}
	}
	catch (const char *str) {
		SGL_ASSERT(false, str);
		return NULL;
	}
	catch (exception e) {
		SGL_ASSERT(false, e.what());
		return NULL;
	}

	return res;
}
char *reallocBuffer(char *buffer, int length) {
	char *ret = (char *)malloc(length * 2);
	memcpy(ret, buffer, length);
	free(buffer);
	return ret;
}
char *strcatSafe(char *dst, const char *src, int *length) {
	while (strlen(dst) + strlen(src) + 2 >= (unsigned int)*length) {
		dst = reallocBuffer(dst, *length);
		*length *= 2;
	}
	strcat(dst, src);
	return dst;
}
char *writeInnerJson(struct JSON *json) {
	int i, bufferLength = 256;
	struct JSON_Item *iter = NULL;
	char *res = (char *)malloc(256);
	char *tmp = (char *)malloc(1024);
	int len = 0, bound = 256;

	if (json->arr == 0) {
		strcpy(res, "{\n");

		for (i = 0; i < SG_HASH_NUM; i++) {
			iter = json->hash[i];
			while (iter) {
				res = strcatSafe(res, "\"", &bufferLength);
				res = strcatSafe(res, iter->name, &bufferLength);
				res = strcatSafe(res, "\":", &bufferLength);

				switch (iter->t) {
				case JSON_INT:
					sprintf(tmp, "%d", iter->data.json_int);
					res = strcatSafe(res, tmp, &bufferLength);
					res = strcatSafe(res, ",\n", &bufferLength);
					break;
				case JSON_FLOAT:
					sprintf(tmp, "%f", iter->data.json_float);
					res = strcatSafe(res, tmp, &bufferLength);
					res = strcatSafe(res, ",\n", &bufferLength);
					break;
				case JSON_CHAR:
					sprintf(tmp, "\'%c\'", iter->data.json_char);
					res = strcatSafe(res, tmp, &bufferLength);
					res = strcatSafe(res, ",\n", &bufferLength);
					break;
				case JSON_BOOL:
					if (iter->data.json_bool)
						res = strcatSafe(res, "true,\n", &bufferLength);
					else
						res = strcatSafe(res, "false,\n", &bufferLength);
					break;
				case JSON_STRING:
					sprintf(tmp, "\"%s\"", iter->data.json_string);
					res = strcatSafe(res, tmp, &bufferLength);
					res = strcatSafe(res, ",\n", &bufferLength);
					break;
				case JSON_OBJECT:
					res = strcatSafe(res, writeInnerJson(iter->data.json_object), &bufferLength);
					res = strcatSafe(res, ",\n", &bufferLength);
					break;
				case JSON_ARRAY:
					res = strcatSafe(res, writeInnerJson(iter->data.json_array), &bufferLength);
					res = strcatSafe(res, ",\n", &bufferLength);
					break;
				default:
					break;
				}

				iter = iter->next;
			}
		}
		if (res[strlen(res) - 2] == ',')res[strlen(res) - 2] = '\0';
		res = strcatSafe(res, "\n", &bufferLength);
		res = strcatSafe(res, "}", &bufferLength);
	}
	else {
		strcpy(res, "[\n");

		iter = json->list;
		while (iter) {
			switch (iter->t) {
			case JSON_INT:
				sprintf(tmp, "%d", iter->data.json_int);
				res = strcatSafe(res, tmp, &bufferLength);
				res = strcatSafe(res, ",\n", &bufferLength);
				break;
			case JSON_FLOAT:
				sprintf(tmp, "%f", iter->data.json_float);
				res = strcatSafe(res, tmp, &bufferLength);
				res = strcatSafe(res, ",\n", &bufferLength);
				break;
			case JSON_CHAR:
				sprintf(tmp, "\'%c\'", iter->data.json_char);
				res = strcatSafe(res, tmp, &bufferLength);
				res = strcatSafe(res, ",\n", &bufferLength);
				break;
			case JSON_BOOL:
				if (iter->data.json_bool)
					res = strcatSafe(res, "true,\n", &bufferLength);
				else
					res = strcatSafe(res, "false,\n", &bufferLength);
				break;
			case JSON_STRING:
				sprintf(tmp, "\"%s\"", iter->data.json_string);
				res = strcatSafe(res, tmp, &bufferLength);
				res = strcatSafe(res, ",\n", &bufferLength);
				break;
			case JSON_OBJECT:
				res = strcatSafe(res, writeInnerJson(iter->data.json_object), &bufferLength);
				res = strcatSafe(res, ",\n", &bufferLength);
				break;
			case JSON_ARRAY:
				res = strcatSafe(res, writeInnerJson(iter->data.json_array), &bufferLength);
				res = strcatSafe(res, ",\n", &bufferLength);
				break;
			default:
				break;
			}

			iter = iter->next;
		}
		if (res[strlen(res) - 2] == ',')res[strlen(res) - 2] = '\0';
		res = strcatSafe(res, "\n", &bufferLength);
		res = strcatSafe(res, "]", &bufferLength);
	}

	free(tmp);
	return res;
}
char *writeJson(struct JSON *json) {
	char *buf = writeInnerJson(json);
	strcat(buf, "\n\n");
	return buf;
}

struct JSON_Item *getContent(struct JSON *json, SGtext name) {
	int i, sum = 0;
	struct JSON_Item *iter = NULL;

	if (json->arr == 0) {
		for (i = 0; i < (int)strlen(name); i++) {
			sum += name[i];
		}
		sum %= SG_HASH_NUM;

		iter = json->hash[sum];
		while (iter) {
			if (strcmp(iter->name, name) == 0)return iter;
			iter = iter->next;
		}
	}

	return NULL;
}
struct JSON_Item *getElement(struct JSON *json, int idx) {
	struct JSON_Item *iter = NULL;

	if (json->arr == 1) {
		iter = json->list;
		while (iter) {
			if (idx == 0)return iter;
			idx--;

			iter = iter->next;
		}
	}

	return NULL;
}
void deleteContent(struct JSON *json, SGtext name) {
	int i, sum = 0;
	struct JSON_Item *iter = NULL, *tmp;

	if (json->arr == 0) {
		for (i = 0; i < (int)strlen(name); i++) {
			sum += name[i];
		}
		sum %= SG_HASH_NUM;

		iter = json->hash[sum];
		if (!iter)return;
		if (strcmp(iter->name, name) == 0) {
			json->hash[sum] = iter->next;
			freeItem(iter);
			return;
		}
		while (iter->next) {
			if (strcmp(iter->next->name, name) == 0) {
				tmp = iter->next;
				iter->next = iter->next->next;
				freeItem(tmp);
				return;
			}
			iter = iter->next;
		}
	}
}
void deleteElement(struct JSON *json, int idx) {
	struct JSON_Item *iter = NULL, *tmp;

	if (json->arr == 1) {
		iter = json->list;
		if (!iter)return;
		if (idx == 0) {
			json->list = iter->next;
			freeItem(iter);
			return;
		}
		while (iter->next) {
			if (--idx == 0) {
				tmp = iter->next;
				iter->next = iter->next->next;
				freeItem(tmp);
				return;
			}

			iter = iter->next;
		}
	}
}

void setIntContent(struct JSON *json, SGtext name, SGint i) {
	int ch, sum = 0;
	struct JSON_Item *iter;

	for (ch = 0; ch < (int)strlen(name); ch++) {
		sum += name[ch];
	}
	sum %= SG_HASH_NUM;

	iter = json->hash[sum];
	while (iter) {
		if (strcmp(iter->name, name) == 0) {
			iter->t = JSON_INT;
			iter->data.json_int = i;
			return;
		}
		iter = iter->next;
	}

	iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
	iter->t = JSON_INT;
	iter->name = (char *)malloc(strlen(name) + 1);
	strcpy(iter->name, name);
	iter->data.json_int = i;
	iter->next = json->hash[sum];
	json->hash[sum] = iter;
}
void setFloatContent(struct JSON *json, SGtext name, SGfloat f) {
	int ch, sum = 0;
	struct JSON_Item *iter;

	for (ch = 0; ch < (int)strlen(name); ch++) {
		sum += name[ch];
	}
	sum %= SG_HASH_NUM;

	iter = json->hash[sum];
	while (iter) {
		if (strcmp(iter->name, name) == 0) {
			iter->t = JSON_FLOAT;
			iter->data.json_float = f;
			return;
		}
		iter = iter->next;
	}

	iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
	iter->t = JSON_FLOAT;
	iter->name = (char *)malloc(strlen(name) + 1);
	strcpy(iter->name, name);
	iter->data.json_float = f;
	iter->next = json->hash[sum];
	json->hash[sum] = iter;
}
void setCharContent(struct JSON *json, SGtext name, SGchar c) {
	int ch, sum = 0;
	struct JSON_Item *iter;

	for (ch = 0; ch < (int)strlen(name); ch++) {
		sum += name[ch];
	}
	sum %= SG_HASH_NUM;

	iter = json->hash[sum];
	while (iter) {
		if (strcmp(iter->name, name) == 0) {
			iter->t = JSON_CHAR;
			iter->data.json_char = c;
			return;
		}
		iter = iter->next;
	}

	iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
	iter->t = JSON_CHAR;
	iter->name = (char *)malloc(strlen(name) + 1);
	strcpy(iter->name, name);
	iter->data.json_char = c;
	iter->next = json->hash[sum];
	json->hash[sum] = iter;
}
void setBoolContent(struct JSON *json, SGtext name, SGbool b) {
	int ch, sum = 0;
	struct JSON_Item *iter;

	for (ch = 0; ch < (int)strlen(name); ch++) {
		sum += name[ch];
	}
	sum %= SG_HASH_NUM;

	iter = json->hash[sum];
	while (iter) {
		if (strcmp(iter->name, name) == 0) {
			iter->t = JSON_BOOL;
			iter->data.json_bool = b;
			return;
		}
		iter = iter->next;
	}

	iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
	iter->t = JSON_BOOL;
	iter->name = (char *)malloc(strlen(name) + 1);
	strcpy(iter->name, name);
	iter->data.json_bool = b;
	iter->next = json->hash[sum];
	json->hash[sum] = iter;
}
void setStringContent(struct JSON *json, SGtext name, const char *s) {
	int ch, sum = 0;
	struct JSON_Item *iter;

	for (ch = 0; ch < (int)strlen(name); ch++) {
		sum += name[ch];
	}
	sum %= SG_HASH_NUM;

	iter = json->hash[sum];
	while (iter) {
		if (strcmp(iter->name, name) == 0) {
			iter->t = JSON_STRING;
			iter->data.json_string = (char *)malloc(strlen(s) + 1);
			strcpy(iter->data.json_string, s);
			return;
		}
		iter = iter->next;
	}

	iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
	iter->t = JSON_STRING;
	iter->name = (char *)malloc(strlen(name) + 1);
	strcpy(iter->name, name);
	iter->data.json_string = (char *)malloc(strlen(s) + 1);
	strcpy(iter->data.json_string, s);
	iter->next = json->hash[sum];
	json->hash[sum] = iter;
}
void setObjectContent(struct JSON *json, SGtext name, struct JSON *j) {
	int ch, sum = 0;
	struct JSON_Item *iter;

	for (ch = 0; ch < (int)strlen(name); ch++) {
		sum += name[ch];
	}
	sum %= SG_HASH_NUM;

	iter = json->hash[sum];
	while (iter) {
		if (strcmp(iter->name, name) == 0) {
			iter->t = JSON_OBJECT;
			free(iter->data.json_object);
			iter->data.json_object = j;
			return;
		}
		iter = iter->next;
	}

	iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
	iter->t = JSON_OBJECT;
	iter->name = (char *)malloc(strlen(name) + 1);
	strcpy(iter->name, name);
	iter->data.json_object = j;
	iter->next = json->hash[sum];
	json->hash[sum] = iter;
}
void setArrayContent(struct JSON *json, SGtext name, struct JSON *j) {
	int ch, sum = 0;
	struct JSON_Item *iter;

	for (ch = 0; ch < (int)strlen(name); ch++) {
		sum += name[ch];
	}
	sum %= SG_HASH_NUM;

	iter = json->hash[sum];
	while (iter) {
		if (strcmp(iter->name, name) == 0) {
			iter->t = JSON_ARRAY;
			free(iter->data.json_array);
			iter->data.json_array = j;
			return;
		}
		iter = iter->next;
	}

	iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
	iter->t = JSON_ARRAY;
	iter->name = (char *)malloc(strlen(name) + 1);
	strcpy(iter->name, name);
	iter->data.json_array = j;
	iter->next = json->hash[sum];
	json->hash[sum] = iter;
}
void setIntElement(struct JSON *json, int idx, SGint i) {
	struct JSON_Item *iter;

	if (json->arr == 1) {
		if (idx >= 0) {
			iter = json->list;
			while (iter) {
				if (idx == 0) {
					iter->t = JSON_INT;
					iter->data.json_int = i;
					return;
				}
				idx--;

				if (iter->next)
					iter = iter->next;
				else {
					iter->next = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
					iter = iter->next;
					iter->t = JSON_INT;
					iter->name = (char *)malloc(1);
					strcpy(iter->name, "");
					iter->data.json_int = i;

					iter->next = NULL;
					return;
				}
			}
		}
		iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));

		iter->t = JSON_INT;
		iter->name = (char *)malloc(1);
		strcpy(iter->name, "");
		iter->data.json_int = i;

		iter->next = json->list;
		json->list = iter;
	}
}
void setFloatElement(struct JSON *json, int idx, SGfloat f) {
	struct JSON_Item *iter;

	if (json->arr == 1) {
		if (idx >= 0) {
			iter = json->list;
			while (iter) {
				if (idx == 0) {
					iter->t = JSON_FLOAT;
					iter->data.json_float = f;
					return;
				}
				idx--;

				if (iter->next)
					iter = iter->next;
				else {
					iter->next = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
					iter = iter->next;
					iter->t = JSON_FLOAT;
					iter->name = (char *)malloc(1);
					strcpy(iter->name, "");
					iter->data.json_float = f;

					iter->next = NULL;
					return;
				}
			}
		}
		iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));

		iter->t = JSON_FLOAT;
		iter->name = (char *)malloc(1);
		strcpy(iter->name, "");
		iter->data.json_float = f;

		iter->next = json->list;
		json->list = iter;
	}
}
void setCharElement(struct JSON *json, int idx, SGchar c) {
	struct JSON_Item *iter;

	if (json->arr == 1) {
		if (idx >= 0) {
			iter = json->list;
			while (iter) {
				if (idx == 0) {
					iter->t = JSON_CHAR;
					iter->data.json_char = c;
					return;
				}
				idx--;

				if (iter->next)
					iter = iter->next;
				else {
					iter->next = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
					iter = iter->next;
					iter->t = JSON_CHAR;
					iter->name = (char *)malloc(1);
					strcpy(iter->name, "");
					iter->data.json_char = c;

					iter->next = NULL;
					return;
				}
			}
		}
		iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));

		iter->t = JSON_CHAR;
		iter->name = (char *)malloc(1);
		strcpy(iter->name, "");
		iter->data.json_char = c;

		iter->next = json->list;
		json->list = iter;
	}
}
void setBoolElement(struct JSON *json, int idx, SGbool b) {
	struct JSON_Item *iter;

	if (json->arr == 1) {
		if (idx >= 0) {
			iter = json->list;
			while (iter) {
				if (idx == 0) {
					iter->t = JSON_BOOL;
					iter->data.json_bool = b;
					return;
				}
				idx--;

				if (iter->next)
					iter = iter->next;
				else {
					iter->next = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
					iter = iter->next;
					iter->t = JSON_BOOL;
					iter->name = (char *)malloc(1);
					strcpy(iter->name, "");
					iter->data.json_bool = b;

					iter->next = NULL;
					return;
				}
			}
		}
		iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));

		iter->t = JSON_BOOL;
		iter->name = (char *)malloc(1);
		strcpy(iter->name, "");
		iter->data.json_bool = b;

		iter->next = json->list;
		json->list = iter;
	}
}
void setStringElement(struct JSON *json, int idx, const char *s) {
	struct JSON_Item *iter;

	if (json->arr == 1) {
		if (idx >= 0) {
			iter = json->list;
			while (iter) {
				if (idx == 0) {
					iter->t = JSON_STRING;
					iter->data.json_string = (char *)malloc(strlen(s) + 1);
					strcpy(iter->data.json_string, s);
					return;
				}
				idx--;

				if (iter->next)
					iter = iter->next;
				else {
					iter->next = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
					iter = iter->next;
					iter->t = JSON_STRING;
					iter->name = (char *)malloc(1);
					strcpy(iter->name, "");
					iter->data.json_string = (char *)malloc(strlen(s) + 1);
					strcpy(iter->data.json_string, s);

					iter->next = NULL;
					return;
				}
			}
		}
		iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));

		iter->t = JSON_STRING;
		iter->name = (char *)malloc(1);
		strcpy(iter->name, "");
		iter->data.json_string = (char *)malloc(strlen(s) + 1);
		strcpy(iter->data.json_string, s);

		iter->next = json->list;
		json->list = iter;
	}
}
void setObjectElement(struct JSON *json, int idx, struct JSON *j) {
	struct JSON_Item *iter;

	if (json->arr == 1) {
		if (idx >= 0) {
			iter = json->list;
			while (iter) {
				if (idx == 0) {
					iter->t = JSON_OBJECT;
					free(iter->data.json_object);
					iter->data.json_object = j;
					return;
				}
				idx--;

				if (iter->next)
					iter = iter->next;
				else {
					iter->next = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
					iter = iter->next;
					iter->t = JSON_OBJECT;
					iter->name = (char *)malloc(1);
					strcpy(iter->name, "");
					iter->data.json_object = j;

					iter->next = NULL;
					return;
				}
			}
		}
		iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));

		iter->t = JSON_OBJECT;
		iter->name = (char *)malloc(1);
		strcpy(iter->name, "");
		iter->data.json_object = j;

		iter->next = json->list;
		json->list = iter;
	}
}
void setArrayElement(struct JSON *json, int idx, struct JSON *j) {
	struct JSON_Item *iter;

	if (json->arr == 1) {
		if (idx >= 0) {
			iter = json->list;
			while (iter) {
				if (idx == 0) {
					iter->t = JSON_ARRAY;
					free(iter->data.json_array);
					iter->data.json_array = j;
					return;
				}
				idx--;

				if (iter->next)
					iter = iter->next;
				else {
					iter->next = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
					iter = iter->next;
					iter->t = JSON_ARRAY;
					iter->name = (char *)malloc(1);
					strcpy(iter->name, "");
					iter->data.json_array = j;

					iter->next = NULL;
					return;
				}
			}
		}
		iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));

		iter->t = JSON_ARRAY;
		iter->name = (char *)malloc(1);
		strcpy(iter->name, "");
		iter->data.json_array = j;

		iter->next = json->list;
		json->list = iter;
	}
}


/*
* SG zip interfaces
* These functions are used to deal with zip data.
*/

DECLARE_HANDLE(HZIP);

#define ZIP_FILENAME 1
#define ZIP_MEMORY 2
#define ZIP_FOLDER 3

long long filetimeInt(const FILETIME ft) {
	long long i = *(long long*)&ft;
	return (long long)((i - 116444736000000000) / 10000000);
}
void filetimeDosdatetime(const FILETIME ft, WORD *dosdate, WORD *dostime) {
	SYSTEMTIME st; FileTimeToSystemTime(&ft, &st);
	*dosdate = (WORD)(((st.wYear - 1980) & 0x7f) << 9);
	*dosdate |= (WORD)((st.wMonth & 0xf) << 5);
	*dosdate |= (WORD)((st.wDay & 0x1f));
	*dostime = (WORD)((st.wHour & 0x1f) << 11);
	*dostime |= (WORD)((st.wMinute & 0x3f) << 5);
	*dostime |= (WORD)((st.wSecond * 2) & 0x1f);
}
FILETIME intFiletime(const long long t) {
	LONGLONG i = Int32x32To64(t, 10000000) + 116444736000000000;
	FILETIME ft;
	ft.dwLowDateTime = (DWORD)i;
	ft.dwHighDateTime = (DWORD)(i >> 32);
	return ft;
}
FILETIME dosdatetimeFiletime(WORD dosdate, WORD dostime) {
	SYSTEMTIME st;
	st.wYear = (WORD)(((dosdate >> 9) & 0x7f) + 1980);
	st.wMonth = (WORD)((dosdate >> 5) & 0xf);
	st.wDay = (WORD)(dosdate & 0x1f);
	st.wHour = (WORD)((dostime >> 11) & 0x1f);
	st.wMinute = (WORD)((dostime >> 5) & 0x3f);
	st.wSecond = (WORD)((dostime & 0x1f) * 2);
	st.wMilliseconds = 0;
	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);
	return ft;
}


#define EB_UT_LEN(n)      (1 + 4 * (n))
#define EB_L_UT_SIZE    (4 + EB_UT_LEN(3))
#define EB_C_UT_SIZE    (4 + EB_UT_LEN(1))

#define LOCSIG     0x04034b50L
#define CENSIG     0x02014b50L
#define ENDSIG     0x06054b50L
#define EXTLOCSIG  0x08074b50L

#define MIN_MATCH  3
#define MAX_MATCH  258

#define WSIZE  (0x8000)
#define MIN_LOOKAHEAD (MAX_MATCH+MIN_MATCH+1)
#define MAX_DIST  (WSIZE-MIN_LOOKAHEAD)

#define MAX_BITS 15
#define MAX_BL_BITS 7
#define LENGTH_CODES 29
#define LITERALS  256
#define END_BLOCK 256
#define L_CODES (LITERALS+1+LENGTH_CODES)
#define D_CODES   30
#define BL_CODES  19

#define LIT_BUFSIZE  0x8000
#define DIST_BUFSIZE  LIT_BUFSIZE
#define REP_3_6      16
#define REPZ_3_10    17
#define REPZ_11_138  18
#define HEAP_SIZE (2*L_CODES+1)
#define Buf_size (8 * 2*sizeof(char))

#define HASH_SIZE (unsigned)(1<<15)
#define HASH_MASK (HASH_SIZE-1)
#define WMASK (WSIZE-1)
#define FAST 4
#define SLOW 2
#define TOO_FAR 4096
#define H_SHIFT ((15+MIN_MATCH-1)/MIN_MATCH)

#define UPDATE_HASH(h,c) (h = (((h)<<H_SHIFT) ^ (c)) & HASH_MASK)
#define PUTSH(a,f) {char _putsh_c=(char)((a)&0xff); wfunc(param,&_putsh_c,1); _putsh_c=(char)((a)>>8); wfunc(param,&_putsh_c,1);}
#define PUTLG(a,f) {PUTSH((a) & 0xffff,(f)) PUTSH((a) >> 16,(f))}
#define PUTSHORT(state,w) {\
	if (state.bs.out_offset >= state.bs.out_size-1) \
		state.flush_outbuf(state.param,state.bs.out_buf, &state.bs.out_offset); \
		state.bs.out_buf[state.bs.out_offset++] = (char) ((w) & 0xff); \
		state.bs.out_buf[state.bs.out_offset++] = (char) ((unsigned short)(w) >> 8); \
	}
#define PUTBYTE(state,b) {\
	if (state.bs.out_offset >= state.bs.out_size) \
		state.flush_outbuf(state.param,state.bs.out_buf, &state.bs.out_offset); \
		state.bs.out_buf[state.bs.out_offset++] = (char) (b); \
	}

const int extra_lbits[LENGTH_CODES] = { 0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0 };
const int extra_dbits[D_CODES] = { 0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13 };
const int extra_blbits[BL_CODES] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,3,7 };
const unsigned char bl_order[BL_CODES] = { 16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15 };
struct config {
	unsigned short good_length;
	unsigned short max_lazy;
	unsigned short nice_length;
	unsigned short max_chain;
} configuration_table[10] = {
	{ 0, 0, 0, 0 },  // 0 store only
{ 4, 4, 8, 4 },  // 1 maximum speed, no lazy matches
{ 4, 5, 16, 8 },  // 2
{ 4, 6, 32, 32 },  // 3
{ 4, 4, 16, 16 },  // 4 lazy matches */
{ 8, 16, 32, 32 },  // 5
{ 8, 16, 128, 128 },  // 6
{ 8, 32, 128, 256 },  // 7
{ 32, 128, 258, 1024 }, // 8
{ 32, 258, 258, 4096 }
};
const unsigned long crc_table[256] = {
	0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
	0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
	0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
	0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
	0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
	0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
	0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
	0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
	0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
	0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
	0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
	0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
	0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
	0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
	0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
	0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
	0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
	0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
	0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
	0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
	0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
	0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
	0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
	0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
	0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
	0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
	0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
	0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
	0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
	0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
	0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
	0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
	0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
	0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
	0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
	0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
	0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
	0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
	0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
	0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
	0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
	0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
	0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
	0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
	0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
	0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
	0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
	0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
	0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
	0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
	0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
	0x2d02ef8dL
};

typedef struct ct_data {
	union {
		unsigned short  freq;       // frequency count
		unsigned short  code;       // bit string
	} fc;
	union {
		unsigned short  dad;        // father node in Huffman tree
		unsigned short  len;        // length of bit string
	} dl;
} ct_data;
typedef struct tree_desc {
	ct_data *dyn_tree;      // the dynamic tree
	ct_data *static_tree;   // corresponding static tree or NULL
	const int *extra_bits;  // extra bits for each code or NULL
	int     extra_base;     // base index for extra_bits
	int     elems;          // max number of elements in the tree
	int     max_length;     // max bit length for the codes
	int     max_code;       // largest code with non zero frequency
} tree_desc;
typedef struct iztimes {
	long long atime, mtime, ctime;
} iztimes;
typedef struct zlist {
	unsigned short vem, ver, flg, how;       // See central header in zipfile.c for what vem..off are
	unsigned long tim, crc, siz, len;
	size_t nam, ext, cext, com;   // offset of ext must be >= LOCHEAD
	unsigned short dsk, att, lflg;           // offset of lflg must be >= LOCHEAD
	unsigned long atx, off;
	char name[MAX_PATH];          // File name in zip file
	char *extra;                  // Extra field (set only if ext != 0)
	char *cextra;                 // Extra in central (set only if cext != 0)
	char *comment;                // Comment (set only if com != 0)
	char iname[MAX_PATH];         // Internal file name after cleanup
	char zname[MAX_PATH];         // External version of internal name
	int mark;                     // Marker for files to operate on
	int trash;                    // Marker for files to delete
	int dosflag;                  // Set to force MSDOS file attributes
	struct zlist far *nxt;        // Pointer to next header in list
} TZipFileInfo;

class TTreeState {
public:
	TTreeState() {
		tree_desc a = { dyn_ltree, static_ltree, extra_lbits, LITERALS + 1, L_CODES, MAX_BITS, 0 };  l_desc = a;
		tree_desc b = { dyn_dtree, static_dtree, extra_dbits, 0,          D_CODES, MAX_BITS, 0 };  d_desc = b;
		tree_desc c = { bl_tree, NULL,       extra_blbits, 0,         BL_CODES, MAX_BL_BITS, 0 };  bl_desc = c;
		last_lit = 0;
		last_dist = 0;
		last_flags = 0;
	}

	ct_data dyn_ltree[HEAP_SIZE];
	ct_data dyn_dtree[2 * D_CODES + 1];
	ct_data static_ltree[L_CODES + 2];
	ct_data static_dtree[D_CODES];
	ct_data bl_tree[2 * BL_CODES + 1];

	tree_desc l_desc;
	tree_desc d_desc;
	tree_desc bl_desc;

	unsigned short bl_count[MAX_BITS + 1];

	int heap[2 * L_CODES + 1];
	int heap_len; 
	int heap_max;
	unsigned char depth[2 * L_CODES + 1];
	unsigned char length_code[MAX_MATCH - MIN_MATCH + 1];
	unsigned char dist_code[512];
	int base_length[LENGTH_CODES];
	int base_dist[D_CODES];
	unsigned char far l_buf[LIT_BUFSIZE];
	unsigned short far d_buf[DIST_BUFSIZE];
	unsigned char flag_buf[(LIT_BUFSIZE / 8)];

	unsigned last_lit;
	unsigned last_dist;
	unsigned last_flags;
	unsigned char flags;
	unsigned char flag_bit;
	unsigned long opt_len;
	unsigned long static_len;
	unsigned long cmpr_bytelen;
	unsigned long cmpr_len_bits;
	unsigned long input_len;
	unsigned short *file_type;
};
class TBitState {
public:

	int flush_flg;
	unsigned bi_buf;
	int bi_valid;
	char *out_buf;
	unsigned out_offset;
	unsigned out_size;
	unsigned long bits_sent;
};
class TDeflateState {
public:
	TDeflateState() { window_size = 0; }

	unsigned char    window[2L * WSIZE];
	unsigned int    prev[WSIZE];
	unsigned int    head[HASH_SIZE];
	unsigned long window_size;
	long block_start;
	int sliding;
	unsigned ins_h;
	unsigned int prev_length;
	unsigned strstart;
	unsigned match_start;
	int eofile;
	unsigned lookahead;

	unsigned max_chain_length;
	unsigned int max_lazy_match;
	unsigned good_match;
	int nice_match;
};

struct TState;
typedef unsigned(*READFUNC)(TState &state, char *buf, unsigned size);
typedef unsigned(*FLUSHFUNC)(void *param, const char *buf, unsigned *size);
typedef unsigned(*WRITEFUNC)(void *param, const char *buf, unsigned size);
struct TState {
	void *param;
	int level;
	bool seekable;

	READFUNC readfunc;
	FLUSHFUNC flush_outbuf;

	TTreeState ts;
	TBitState bs;
	TDeflateState ds;
	const char *err;
};

void init_block(TState &);
void pqdownheap(TState &, ct_data *tree, int k);
void gen_codes(TState &state, ct_data *tree, int max_code);
void compress_block(TState &state, ct_data *ltree, ct_data *dtree);
void set_file_type(TState &);
void send_bits(TState &state, int value, int length);
unsigned bi_reverse(unsigned code, int len);
void bi_windup(TState &state);
void copy_block(TState &state, char *buf, unsigned len, int header);
void fill_window(TState &state);

void ct_init(TState &state, unsigned short *attr) {
	int n;        /* iterates over tree elements */
	int bits;     /* bit counter */
	int length;   /* length value */
	int code;     /* code value */
	int dist;     /* distance index */

	state.ts.file_type = attr;
	state.ts.cmpr_bytelen = state.ts.cmpr_len_bits = 0L;
	state.ts.input_len = 0L;

	if (state.ts.static_dtree[0].dl.len != 0) return;

	length = 0;
	for (code = 0; code < LENGTH_CODES - 1; code++) {
		state.ts.base_length[code] = length;
		for (n = 0; n < (1 << extra_lbits[code]); n++) {
			state.ts.length_code[length++] = (unsigned char)code;
		}
	}
	state.ts.length_code[length - 1] = (unsigned char)code;

	dist = 0;
	for (code = 0; code < 16; code++) {
		state.ts.base_dist[code] = dist;
		for (n = 0; n < (1 << extra_dbits[code]); n++) {
			state.ts.dist_code[dist++] = (unsigned char)code;
		}
	}
	dist >>= 7;
	for (; code < D_CODES; code++) {
		state.ts.base_dist[code] = dist << 7;
		for (n = 0; n < (1 << (extra_dbits[code] - 7)); n++) {
			state.ts.dist_code[256 + dist++] = (unsigned char)code;
		}
	}

	for (bits = 0; bits <= MAX_BITS; bits++) state.ts.bl_count[bits] = 0;
	n = 0;
	while (n <= 143) state.ts.static_ltree[n++].dl.len = 8, state.ts.bl_count[8]++;
	while (n <= 255) state.ts.static_ltree[n++].dl.len = 9, state.ts.bl_count[9]++;
	while (n <= 279) state.ts.static_ltree[n++].dl.len = 7, state.ts.bl_count[7]++;
	while (n <= 287) state.ts.static_ltree[n++].dl.len = 8, state.ts.bl_count[8]++;

	gen_codes(state, (ct_data *)state.ts.static_ltree, L_CODES + 1);

	for (n = 0; n < D_CODES; n++) {
		state.ts.static_dtree[n].dl.len = 5;
		state.ts.static_dtree[n].fc.code = (unsigned short)bi_reverse(n, 5);
	}
	init_block(state);
}
void init_block(TState &state) {
	int n;
	for (n = 0; n < L_CODES; n++) state.ts.dyn_ltree[n].fc.freq = 0;
	for (n = 0; n < D_CODES; n++) state.ts.dyn_dtree[n].fc.freq = 0;
	for (n = 0; n < BL_CODES; n++) state.ts.bl_tree[n].fc.freq = 0;

	state.ts.dyn_ltree[END_BLOCK].fc.freq = 1;
	state.ts.opt_len = state.ts.static_len = 0L;
	state.ts.last_lit = state.ts.last_dist = state.ts.last_flags = 0;
	state.ts.flags = 0; state.ts.flag_bit = 1;
}

void pqdownheap(TState &state, ct_data *tree, int k) {

#define smaller(tree, n, m) (tree[n].fc.freq < tree[m].fc.freq || (tree[n].fc.freq == tree[m].fc.freq && state.ts.depth[n] <= state.ts.depth[m]))

	int v = state.ts.heap[k];
	int j = k << 1;
	int htemp;

	while (j <= state.ts.heap_len) {
		if (j < state.ts.heap_len && smaller(tree, state.ts.heap[j + 1], state.ts.heap[j])) j++;
		htemp = state.ts.heap[j];
		if (smaller(tree, v, htemp)) break;

		state.ts.heap[k] = htemp;
		k = j;
		j <<= 1;
	}
	state.ts.heap[k] = v;
}
void gen_bitlen(TState &state, tree_desc *desc) {
	ct_data *tree = desc->dyn_tree;
	const int *extra = desc->extra_bits;
	int base = desc->extra_base;
	int max_code = desc->max_code;
	int max_length = desc->max_length;
	ct_data *stree = desc->static_tree;
	int h;
	int n, m;
	int bits;
	int xbits;
	unsigned short f;
	int overflow = 0;

	for (bits = 0; bits <= MAX_BITS; bits++) state.ts.bl_count[bits] = 0;

	tree[state.ts.heap[state.ts.heap_max]].dl.len = 0; /* root of the heap */

	for (h = state.ts.heap_max + 1; h < HEAP_SIZE; h++) {
		n = state.ts.heap[h];
		bits = tree[tree[n].dl.dad].dl.len + 1;
		if (bits > max_length) bits = max_length, overflow++;
		tree[n].dl.len = (unsigned short)bits;
		if (n > max_code) continue;

		state.ts.bl_count[bits]++;
		xbits = 0;
		if (n >= base) xbits = extra[n - base];
		f = tree[n].fc.freq;
		state.ts.opt_len += (unsigned long)f * (bits + xbits);
		if (stree) state.ts.static_len += (unsigned long)f * (stree[n].dl.len + xbits);
	}
	if (overflow == 0) return;
	do {
		bits = max_length - 1;
		while (state.ts.bl_count[bits] == 0) bits--;
		state.ts.bl_count[bits]--;
		state.ts.bl_count[bits + 1] += (unsigned short)2;
		state.ts.bl_count[max_length]--;
		overflow -= 2;
	} while (overflow > 0);

	for (bits = max_length; bits != 0; bits--) {
		n = state.ts.bl_count[bits];
		while (n != 0) {
			m = state.ts.heap[--h];
			if (m > max_code) continue;
			if (tree[m].dl.len != (unsigned short)bits) {
				state.ts.opt_len += ((long)bits - (long)tree[m].dl.len)*(long)tree[m].fc.freq;
				tree[m].dl.len = (unsigned short)bits;
			}
			n--;
		}
	}
}
void gen_codes(TState &state, ct_data *tree, int max_code) {
	unsigned short next_code[MAX_BITS + 1];
	unsigned short code = 0;
	int bits, n;

	for (bits = 1; bits <= MAX_BITS; bits++) {
		next_code[bits] = code = (unsigned short)((code + state.ts.bl_count[bits - 1]) << 1);
	}
	for (n = 0; n <= max_code; n++) {
		int len = tree[n].dl.len;
		if (len == 0) continue;
		tree[n].fc.code = (unsigned short)bi_reverse(next_code[len]++, len);
	}
}
void build_tree(TState &state, tree_desc *desc) {

#define pqremove(tree, top) {\
    top = state.ts.heap[1]; \
    state.ts.heap[1] = state.ts.heap[state.ts.heap_len--]; \
    pqdownheap(state,tree, 1); \
}

	ct_data *tree = desc->dyn_tree;
	ct_data *stree = desc->static_tree;
	int elems = desc->elems;
	int n, m;
	int max_code = -1;
	int node = elems;
	state.ts.heap_len = 0, state.ts.heap_max = HEAP_SIZE;

	for (n = 0; n < elems; n++) {
		if (tree[n].fc.freq != 0) {
			state.ts.heap[++state.ts.heap_len] = max_code = n;
			state.ts.depth[n] = 0;
		}
		else {
			tree[n].dl.len = 0;
		}
	}
	while (state.ts.heap_len < 2) {
		int newcp = state.ts.heap[++state.ts.heap_len] = (max_code < 2 ? ++max_code : 0);
		tree[newcp].fc.freq = 1;
		state.ts.depth[newcp] = 0;
		state.ts.opt_len--; if (stree) state.ts.static_len -= stree[newcp].dl.len;
	}
	desc->max_code = max_code;

	for (n = state.ts.heap_len / 2; n >= 1; n--) pqdownheap(state, tree, n);
	do {
		pqremove(tree, n);
		m = state.ts.heap[1];

		state.ts.heap[--state.ts.heap_max] = n;
		state.ts.heap[--state.ts.heap_max] = m;

		tree[node].fc.freq = (unsigned short)(tree[n].fc.freq + tree[m].fc.freq);
		state.ts.depth[node] = (unsigned char)(max(state.ts.depth[n], state.ts.depth[m]) + 1);
		tree[n].dl.dad = tree[m].dl.dad = (unsigned short)node;
		state.ts.heap[1] = node++;
		pqdownheap(state, tree, 1);

	} while (state.ts.heap_len >= 2);

	state.ts.heap[--state.ts.heap_max] = state.ts.heap[1];
	gen_bitlen(state, (tree_desc *)desc);
	gen_codes(state, (ct_data *)tree, max_code);
}
void scan_tree(TState &state, ct_data *tree, int max_code) {
	int n;
	int prevlen = -1;
	int curlen;
	int nextlen = tree[0].dl.len;
	int count = 0;
	int max_count = 7;
	int min_count = 4;

	if (nextlen == 0) max_count = 138, min_count = 3;
	tree[max_code + 1].dl.len = (unsigned short)-1; /* guard */

	for (n = 0; n <= max_code; n++) {
		curlen = nextlen; nextlen = tree[n + 1].dl.len;
		if (++count < max_count && curlen == nextlen) continue;
		else if (count < min_count) state.ts.bl_tree[curlen].fc.freq = (unsigned short)(state.ts.bl_tree[curlen].fc.freq + count);
		else if (curlen != 0) {
			if (curlen != prevlen) state.ts.bl_tree[curlen].fc.freq++;
			state.ts.bl_tree[REP_3_6].fc.freq++;
		}
		else if (count <= 10)state.ts.bl_tree[REPZ_3_10].fc.freq++;
		else state.ts.bl_tree[REPZ_11_138].fc.freq++;
		count = 0; prevlen = curlen;
		if (nextlen == 0) {
			max_count = 138;
			min_count = 3;
		}
		else if (curlen == nextlen) {
			max_count = 6;
			min_count = 3;
		}
		else {
			max_count = 7;
			min_count = 4;
		}
	}
}
void send_tree(TState &state, ct_data *tree, int max_code) {
	int n;
	int prevlen = -1;
	int curlen;
	int nextlen = tree[0].dl.len;
	int count = 0;
	int max_count = 7;
	int min_count = 4;
	if (nextlen == 0) max_count = 138, min_count = 3;

	for (n = 0; n <= max_code; n++) {
		curlen = nextlen; nextlen = tree[n + 1].dl.len;
		if (++count < max_count && curlen == nextlen)continue;
		else if (count < min_count)do { send_bits(state, state.ts.bl_tree[curlen].fc.code, state.ts.bl_tree[curlen].dl.len); } while (--count != 0);
		else if (curlen != 0) {
			if (curlen != prevlen) {
				send_bits(state, state.ts.bl_tree[curlen].fc.code, state.ts.bl_tree[curlen].dl.len);
				count--;
			}
			send_bits(state, state.ts.bl_tree[REP_3_6].fc.code, state.ts.bl_tree[REP_3_6].dl.len);
			send_bits(state, count - 3, 2);
		}
		else if (count <= 10) {
			send_bits(state, state.ts.bl_tree[REPZ_3_10].fc.code, state.ts.bl_tree[REPZ_3_10].dl.len);
			send_bits(state, count - 3, 3);
		}
		else {
			send_bits(state, state.ts.bl_tree[REPZ_11_138].fc.code, state.ts.bl_tree[REPZ_11_138].dl.len);
			send_bits(state, count - 11, 7);
		}
		count = 0; prevlen = curlen;
		if (nextlen == 0) {
			max_count = 138;
			min_count = 3;
		}
		else if (curlen == nextlen) {
			max_count = 6;
			min_count = 3;
		}
		else {
			max_count = 7;
			min_count = 4;
		}
	}
}
int build_bl_tree(TState &state) {
	int max_blindex;

	scan_tree(state, (ct_data *)state.ts.dyn_ltree, state.ts.l_desc.max_code);
	scan_tree(state, (ct_data *)state.ts.dyn_dtree, state.ts.d_desc.max_code);
	build_tree(state, (tree_desc *)(&state.ts.bl_desc));

	for (max_blindex = BL_CODES - 1; max_blindex >= 3; max_blindex--) {
		if (state.ts.bl_tree[bl_order[max_blindex]].dl.len != 0) break;
	}
	state.ts.opt_len += 3 * (max_blindex + 1) + 5 + 5 + 4;
	return max_blindex;
}
void send_all_trees(TState &state, int lcodes, int dcodes, int blcodes) {
	int rank;

	send_bits(state, lcodes - 257, 5);
	send_bits(state, dcodes - 1, 5);
	send_bits(state, blcodes - 4, 4);
	for (rank = 0; rank < blcodes; rank++) {
		send_bits(state, state.ts.bl_tree[bl_order[rank]].dl.len, 3);
	}

	send_tree(state, (ct_data *)state.ts.dyn_ltree, lcodes - 1);
	send_tree(state, (ct_data *)state.ts.dyn_dtree, dcodes - 1);
}
unsigned long flush_block(TState &state, char *buf, unsigned long stored_len, int eof) {
	unsigned long opt_lenb, static_lenb;
	int max_blindex;

	state.ts.flag_buf[state.ts.last_flags] = state.ts.flags;

	build_tree(state, (tree_desc *)(&state.ts.l_desc));
	build_tree(state, (tree_desc *)(&state.ts.d_desc));
	max_blindex = build_bl_tree(state);

	opt_lenb = (state.ts.opt_len + 3 + 7) >> 3;
	static_lenb = (state.ts.static_len + 3 + 7) >> 3;
	state.ts.input_len += stored_len;

	if (static_lenb <= opt_lenb) opt_lenb = static_lenb;

	if (stored_len + 4 <= opt_lenb && buf != (char*)NULL) {
		send_bits(state, 0 + eof, 3);  /* send block type */
		state.ts.cmpr_bytelen += ((state.ts.cmpr_len_bits + 3 + 7) >> 3) + stored_len + 4;
		state.ts.cmpr_len_bits = 0L;

		copy_block(state, buf, (unsigned)stored_len, 1); /* with header */
	}
	else if (static_lenb == opt_lenb) {
		send_bits(state, 2 + eof, 3);
		compress_block(state, (ct_data *)state.ts.static_ltree, (ct_data *)state.ts.static_dtree);
		state.ts.cmpr_len_bits += 3 + state.ts.static_len;
		state.ts.cmpr_bytelen += state.ts.cmpr_len_bits >> 3;
		state.ts.cmpr_len_bits &= 7L;
	}
	else {
		send_bits(state, 4 + eof, 3);
		send_all_trees(state, state.ts.l_desc.max_code + 1, state.ts.d_desc.max_code + 1, max_blindex + 1);
		compress_block(state, (ct_data *)state.ts.dyn_ltree, (ct_data *)state.ts.dyn_dtree);
		state.ts.cmpr_len_bits += 3 + state.ts.opt_len;
		state.ts.cmpr_bytelen += state.ts.cmpr_len_bits >> 3;
		state.ts.cmpr_len_bits &= 7L;
	}
	init_block(state);

	if (eof) {
		bi_windup(state);
		state.ts.cmpr_len_bits += 7;
	}
	return state.ts.cmpr_bytelen + (state.ts.cmpr_len_bits >> 3);
}
int ct_tally(TState &state, int dist, int lc) {
	state.ts.l_buf[state.ts.last_lit++] = (unsigned char)lc;
	if (dist == 0) {
		state.ts.dyn_ltree[lc].fc.freq++;
	}
	else {
		dist--;
		state.ts.dyn_ltree[state.ts.length_code[lc] + LITERALS + 1].fc.freq++;
		state.ts.dyn_dtree[(dist) < 256 ? state.ts.dist_code[dist] : state.ts.dist_code[256 + ((dist) >> 7)]].fc.freq++;

		state.ts.d_buf[state.ts.last_dist++] = (unsigned short)dist;
		state.ts.flags |= state.ts.flag_bit;
	}
	state.ts.flag_bit <<= 1;

	if ((state.ts.last_lit & 7) == 0) {
		state.ts.flag_buf[state.ts.last_flags++] = state.ts.flags;
		state.ts.flags = 0, state.ts.flag_bit = 1;
	}
	if (state.level > 2 && (state.ts.last_lit & 0xfff) == 0) {
		unsigned long out_length = (unsigned long)state.ts.last_lit * 8L;
		unsigned long in_length = (unsigned long)state.ds.strstart - state.ds.block_start;
		int dcode;
		for (dcode = 0; dcode < D_CODES; dcode++) {
			out_length += (unsigned long)state.ts.dyn_dtree[dcode].fc.freq*(5L + extra_dbits[dcode]);
		}
		out_length >>= 3;
		if (state.ts.last_dist < state.ts.last_lit / 2 && out_length < in_length / 2) return 1;
	}
	return (state.ts.last_lit == LIT_BUFSIZE - 1 || state.ts.last_dist == DIST_BUFSIZE);
}
void compress_block(TState &state, ct_data *ltree, ct_data *dtree) {
	unsigned dist;
	int lc;
	unsigned lx = 0;
	unsigned dx = 0;
	unsigned fx = 0;
	unsigned char flag = 0;
	unsigned code;
	int extra;

	if (state.ts.last_lit != 0) do {
		if ((lx & 7) == 0) flag = state.ts.flag_buf[fx++];
		lc = state.ts.l_buf[lx++];
		if ((flag & 1) == 0) {
			send_bits(state, ltree[lc].fc.code, ltree[lc].dl.len);
		}
		else {
			code = state.ts.length_code[lc];
			send_bits(state, ltree[code + LITERALS + 1].fc.code, ltree[code + LITERALS + 1].dl.len);
			extra = extra_lbits[code];
			if (extra != 0) {
				lc -= state.ts.base_length[code];
				send_bits(state, lc, extra);
			}
			dist = state.ts.d_buf[dx++];
			code = (dist) < 256 ? state.ts.dist_code[dist] : state.ts.dist_code[256 + ((dist) >> 7)];

			send_bits(state, dtree[code].fc.code, dtree[code].dl.len);
			extra = extra_dbits[code];
			if (extra != 0) {
				dist -= state.ts.base_dist[code];
				send_bits(state, dist, extra);
			}
		}
		flag >>= 1;
	} while (lx < state.ts.last_lit);

	send_bits(state, ltree[END_BLOCK].fc.code, ltree[END_BLOCK].dl.len);
}
void bi_init(TState &state, char *tgt_buf, unsigned tgt_size, int flsh_allowed) {
	state.bs.out_buf = tgt_buf;
	state.bs.out_size = tgt_size;
	state.bs.out_offset = 0;
	state.bs.flush_flg = flsh_allowed;

	state.bs.bi_buf = 0;
	state.bs.bi_valid = 0;
	state.bs.bits_sent = 0L;
}
void send_bits(TState &state, int value, int length) {
	state.bs.bits_sent += (unsigned long)length;

	state.bs.bi_buf |= (value << state.bs.bi_valid);
	state.bs.bi_valid += length;
	if (state.bs.bi_valid > (int)Buf_size) {
		PUTSHORT(state, state.bs.bi_buf);
		state.bs.bi_valid -= Buf_size;
		state.bs.bi_buf = (unsigned)value >> (length - state.bs.bi_valid);
	}
}
unsigned bi_reverse(unsigned code, int len) {
	register unsigned res = 0;
	do {
		res |= code & 1;
		code >>= 1, res <<= 1;
	} while (--len > 0);
	return res >> 1;
}
void bi_windup(TState &state) {
	if (state.bs.bi_valid > 8) {
		PUTSHORT(state, state.bs.bi_buf);
	}
	else if (state.bs.bi_valid > 0) {
		PUTBYTE(state, state.bs.bi_buf);
	}
	if (state.bs.flush_flg) {
		state.flush_outbuf(state.param, state.bs.out_buf, &state.bs.out_offset);
	}
	state.bs.bi_buf = 0;
	state.bs.bi_valid = 0;
	state.bs.bits_sent = (state.bs.bits_sent + 7) & ~7;
}
void copy_block(TState &state, char *block, unsigned len, int header) {
	bi_windup(state);

	if (header) {
		PUTSHORT(state, (unsigned short)len);
		PUTSHORT(state, (unsigned short)~len);
		state.bs.bits_sent += 2 * 16;
	}
	if (state.bs.flush_flg) {
		state.flush_outbuf(state.param, state.bs.out_buf, &state.bs.out_offset);
		state.bs.out_offset = len;
		state.flush_outbuf(state.param, block, &state.bs.out_offset);
	}
	else {
		memcpy(state.bs.out_buf + state.bs.out_offset, block, len);
		state.bs.out_offset += len;
	}
	state.bs.bits_sent += (unsigned long)len << 3;
}
void lm_init(TState &state, int pack_level, unsigned short *flags) {
	register unsigned j;

	state.ds.sliding = 0;
	if (state.ds.window_size == 0L) {
		state.ds.sliding = 1;
		state.ds.window_size = (unsigned long)2L * WSIZE;
	}

	state.ds.head[HASH_SIZE - 1] = 0;
	memset((char*)state.ds.head, 0, (unsigned)(HASH_SIZE - 1) * sizeof(*state.ds.head));

	state.ds.max_lazy_match = configuration_table[pack_level].max_lazy;
	state.ds.good_match = configuration_table[pack_level].good_length;
	state.ds.nice_match = configuration_table[pack_level].nice_length;
	state.ds.max_chain_length = configuration_table[pack_level].max_chain;
	if (pack_level <= 2) {
		*flags |= FAST;
	}
	else if (pack_level >= 8) {
		*flags |= SLOW;
	}
	state.ds.strstart = 0;
	state.ds.block_start = 0L;

	j = WSIZE;
	j <<= 1;
	state.ds.lookahead = state.readfunc(state, (char*)state.ds.window, j);

	if (state.ds.lookahead == 0 || state.ds.lookahead == (unsigned)EOF) {
		state.ds.eofile = 1, state.ds.lookahead = 0;
		return;
	}
	state.ds.eofile = 0;
	if (state.ds.lookahead < MIN_LOOKAHEAD) fill_window(state);

	state.ds.ins_h = 0;
	for (j = 0; j<MIN_MATCH - 1; j++) UPDATE_HASH(state.ds.ins_h, state.ds.window[j]);
}
int longest_match(TState &state, unsigned int cur_match) {
	unsigned chain_length = state.ds.max_chain_length;
	register unsigned char far *scan = state.ds.window + state.ds.strstart;
	register unsigned char far *match;
	register int len;
	int best_len = state.ds.prev_length;
	unsigned int limit = state.ds.strstart > (unsigned int)MAX_DIST ? state.ds.strstart - (unsigned int)MAX_DIST : 0;

	register unsigned char far *strend = state.ds.window + state.ds.strstart + MAX_MATCH;
	register unsigned char scan_end1 = scan[best_len - 1];
	register unsigned char scan_end = scan[best_len];

	if (state.ds.prev_length >= state.ds.good_match) {
		chain_length >>= 2;
	}
	do {
		match = state.ds.window + cur_match;

		if (match[best_len] != scan_end ||
			match[best_len - 1] != scan_end1 ||
			*match != *scan ||
			*++match != scan[1])      continue;

		scan += 2, match++;

		do {
		} while (*++scan == *++match && *++scan == *++match &&
			*++scan == *++match && *++scan == *++match &&
			*++scan == *++match && *++scan == *++match &&
			*++scan == *++match && *++scan == *++match &&
			scan < strend);

		len = MAX_MATCH - (int)(strend - scan);
		scan = strend - MAX_MATCH;


		if (len > best_len) {
			state.ds.match_start = cur_match;
			best_len = len;
			if (len >= state.ds.nice_match) break;
			scan_end1 = scan[best_len - 1];
			scan_end = scan[best_len];
		}
	} while ((cur_match = state.ds.prev[cur_match & WMASK]) > limit
		&& --chain_length != 0);

	return best_len;
}
void fill_window(TState &state) {
	register unsigned n, m;
	unsigned more;

	do {
		more = (unsigned)(state.ds.window_size - (unsigned long)state.ds.lookahead - (unsigned long)state.ds.strstart);
		if (more == (unsigned)EOF) {
			more--;
		}
		else if (state.ds.strstart >= WSIZE + MAX_DIST && state.ds.sliding) {
			memcpy((char*)state.ds.window, (char*)state.ds.window + WSIZE, (unsigned)WSIZE);
			state.ds.match_start -= WSIZE;
			state.ds.strstart -= WSIZE; /* we now have strstart >= MAX_DIST: */

			state.ds.block_start -= (long)WSIZE;

			for (n = 0; n < HASH_SIZE; n++) {
				m = state.ds.head[n];
				state.ds.head[n] = (unsigned int)(m >= WSIZE ? m - WSIZE : 0);
			}
			for (n = 0; n < WSIZE; n++) {
				m = state.ds.prev[n];
				state.ds.prev[n] = (unsigned int)(m >= WSIZE ? m - WSIZE : 0);
			}
			more += WSIZE;
		}
		if (state.ds.eofile) return;
		n = state.readfunc(state, (char*)state.ds.window + state.ds.strstart + state.ds.lookahead, more);

		if (n == 0 || n == (unsigned)EOF) state.ds.eofile = 1;
		else state.ds.lookahead += n;
	} while (state.ds.lookahead < MIN_LOOKAHEAD && !state.ds.eofile);
}

unsigned long deflate(TState &state) {

#define INSERT_STRING(s, match_head) \
   (UPDATE_HASH(state.ds.ins_h, state.ds.window[(s) + (MIN_MATCH-1)]), \
    state.ds.prev[(s) & WMASK] = match_head = state.ds.head[state.ds.ins_h], \
    state.ds.head[state.ds.ins_h] = (s))
#define FLUSH_BLOCK(state,eof) \
   flush_block(state,state.ds.block_start >= 0L ? (char*)&state.ds.window[(unsigned)state.ds.block_start] : \
                (char*)NULL, (long)state.ds.strstart - state.ds.block_start, (eof))
#define CHECK_MATCH(state,start, match, length)

	unsigned int hash_head = 0;
	unsigned int prev_match;
	int flush;
	int match_available = 0;
	register unsigned match_length = MIN_MATCH - 1;
	while (state.ds.lookahead != 0) {
		if (state.ds.lookahead >= MIN_MATCH)
			INSERT_STRING(state.ds.strstart, hash_head);
		state.ds.prev_length = match_length, prev_match = state.ds.match_start;
		match_length = MIN_MATCH - 1;

		if (hash_head != 0 && state.ds.prev_length < state.ds.max_lazy_match &&
			state.ds.strstart - hash_head <= MAX_DIST) {
			if ((unsigned)state.ds.nice_match > state.ds.lookahead) state.ds.nice_match = (int)state.ds.lookahead;
			match_length = longest_match(state, hash_head);
			if (match_length > state.ds.lookahead) match_length = state.ds.lookahead;
			if (match_length == MIN_MATCH && state.ds.strstart - state.ds.match_start > TOO_FAR) {
				match_length = MIN_MATCH - 1;
			}
		}
		if (state.ds.prev_length >= MIN_MATCH && match_length <= state.ds.prev_length) {
			unsigned max_insert = state.ds.strstart + state.ds.lookahead - MIN_MATCH;
			CHECK_MATCH(state, state.ds.strstart - 1, prev_match, state.ds.prev_length);
			flush = ct_tally(state, state.ds.strstart - 1 - prev_match, state.ds.prev_length - MIN_MATCH);

			state.ds.lookahead -= state.ds.prev_length - 1;
			state.ds.prev_length -= 2;
			do {
				if (++state.ds.strstart <= max_insert) {
					INSERT_STRING(state.ds.strstart, hash_head);
				}
			} while (--state.ds.prev_length != 0);
			state.ds.strstart++;
			match_available = 0;
			match_length = MIN_MATCH - 1;

			if (flush) FLUSH_BLOCK(state, 0), state.ds.block_start = state.ds.strstart;

		}
		else if (match_available) {
			if (ct_tally(state, 0, state.ds.window[state.ds.strstart - 1])) {
				FLUSH_BLOCK(state, 0), state.ds.block_start = state.ds.strstart;
			}
			state.ds.strstart++;
			state.ds.lookahead--;
		}
		else {
			match_available = 1;
			state.ds.strstart++;
			state.ds.lookahead--;
		}
		if (state.ds.lookahead < MIN_LOOKAHEAD) fill_window(state);
	}
	if (match_available) ct_tally(state, 0, state.ds.window[state.ds.strstart - 1]);

	return FLUSH_BLOCK(state, 1); /* eof */
}

int putlocal(struct zlist far *z, WRITEFUNC wfunc, void *param) {
	PUTLG(LOCSIG, f);
	PUTSH(z->ver, f);
	PUTSH(z->lflg, f);
	PUTSH(z->how, f);
	PUTLG(z->tim, f);
	PUTLG(z->crc, f);
	PUTLG(z->siz, f);
	PUTLG(z->len, f);
	PUTSH(z->nam, f);
	PUTSH(z->ext, f);
	size_t res = (size_t)wfunc(param, z->iname, (unsigned int)z->nam);
	if (res != z->nam) return SG_IO_ERROR;
	if (z->ext) {
		res = (size_t)wfunc(param, z->extra, (unsigned int)z->ext);
		if (res != z->ext) return SG_IO_ERROR;
	}
	return SG_NO_ERROR;
}
int putextended(struct zlist far *z, WRITEFUNC wfunc, void *param) {
	PUTLG(EXTLOCSIG, f);
	PUTLG(z->crc, f);
	PUTLG(z->siz, f);
	PUTLG(z->len, f);
	return SG_NO_ERROR;
}
int putcentral(struct zlist far *z, WRITEFUNC wfunc, void *param) {
	PUTLG(CENSIG, f);
	PUTSH(z->vem, f);
	PUTSH(z->ver, f);
	PUTSH(z->flg, f);
	PUTSH(z->how, f);
	PUTLG(z->tim, f);
	PUTLG(z->crc, f);
	PUTLG(z->siz, f);
	PUTLG(z->len, f);
	PUTSH(z->nam, f);
	PUTSH(z->cext, f);
	PUTSH(z->com, f);
	PUTSH(z->dsk, f);
	PUTSH(z->att, f);
	PUTLG(z->atx, f);
	PUTLG(z->off, f);
	if ((size_t)wfunc(param, z->iname, (unsigned int)z->nam) != z->nam ||
		(z->cext && (size_t)wfunc(param, z->cextra, (unsigned int)z->cext) != z->cext) ||
		(z->com && (size_t)wfunc(param, z->comment, (unsigned int)z->com) != z->com))
		return SG_IO_ERROR;
	return SG_NO_ERROR;
}
int putend(int n, unsigned long s, unsigned long c, size_t m, char *z, WRITEFUNC wfunc, void *param) {
	PUTLG(ENDSIG, f);
	PUTSH(0, f);
	PUTSH(0, f);
	PUTSH(n, f);
	PUTSH(n, f);
	PUTLG(s, f);
	PUTLG(c, f);
	PUTSH(m, f);
	if (m && wfunc(param, z, (unsigned int)m) != m) return SG_IO_ERROR;
	return SG_NO_ERROR;
}

unsigned long crc32(unsigned long crc, const unsigned char *buf, size_t len) {
	if (buf == NULL) return 0L;
	crc = crc ^ 0xffffffffL;
	if (len) do { crc = crc_table[((int)(crc) ^ (*buf++)) & 0xff] ^ ((crc) >> 8); } while (--len);
	return crc ^ 0xffffffffL;
}
bool HasZipSuffix(const TCHAR *fn) {
	const TCHAR *ext = fn + _tcslen(fn);
	while (ext>fn && *ext != '.') ext--;
	if (ext == fn && *ext != '.') return false;
	if (_tcsicmp(ext, _T(".Z")) == 0) return true;
	if (_tcsicmp(ext, _T(".zip")) == 0) return true;
	if (_tcsicmp(ext, _T(".zoo")) == 0) return true;
	if (_tcsicmp(ext, _T(".arc")) == 0) return true;
	if (_tcsicmp(ext, _T(".lzh")) == 0) return true;
	if (_tcsicmp(ext, _T(".arj")) == 0) return true;
	if (_tcsicmp(ext, _T(".gz")) == 0) return true;
	if (_tcsicmp(ext, _T(".tgz")) == 0) return true;
	return false;
}
int GetFileInfo(HANDLE hf, unsigned long *attr, long *size, iztimes *times, unsigned long *timestamp) {
	BY_HANDLE_FILE_INFORMATION bhi; BOOL res = GetFileInformationByHandle(hf, &bhi);
	if (!res) return SG_OBJECT_NOT_FOUND;
	DWORD fa = bhi.dwFileAttributes; unsigned long a = 0;
	if (fa&FILE_ATTRIBUTE_READONLY) a |= 0x01;
	if (fa&FILE_ATTRIBUTE_HIDDEN)   a |= 0x02;
	if (fa&FILE_ATTRIBUTE_SYSTEM)   a |= 0x04;
	if (fa&FILE_ATTRIBUTE_DIRECTORY)a |= 0x10;
	if (fa&FILE_ATTRIBUTE_ARCHIVE)  a |= 0x20;
	if (fa&FILE_ATTRIBUTE_DIRECTORY)a |= 0x40000000;
	else a |= 0x80000000;
	a |= 0x01000000;
	if (fa&FILE_ATTRIBUTE_READONLY) {}
	else a |= 0x00800000;
	DWORD red, hsize = GetFileSize(hf, NULL);
	if (hsize>40) {
		SetFilePointer(hf, 0, NULL, FILE_BEGIN); unsigned short magic; ReadFile(hf, &magic, sizeof(magic), &red, NULL);
		SetFilePointer(hf, 36, NULL, FILE_BEGIN); unsigned long hpos;  ReadFile(hf, &hpos, sizeof(hpos), &red, NULL);
		if (magic == 0x54AD && hsize>hpos + 4 + 20 + 28) {
			SetFilePointer(hf, hpos, NULL, FILE_BEGIN); unsigned long signature; ReadFile(hf, &signature, sizeof(signature), &red, NULL);
			if (signature == IMAGE_DOS_SIGNATURE || signature == IMAGE_OS2_SIGNATURE
				|| signature == IMAGE_OS2_SIGNATURE_LE || signature == IMAGE_NT_SIGNATURE) {
				a |= 0x00400000;
			}
		}
	}
	if (attr != NULL) *attr = a;
	if (size != NULL) *size = hsize;
	if (times != NULL) {
		times->atime = filetimeInt(bhi.ftLastAccessTime);
		times->mtime = filetimeInt(bhi.ftLastWriteTime);
		times->ctime = filetimeInt(bhi.ftCreationTime);
	}
	if (timestamp != NULL) {
		WORD dosdate, dostime;
		filetimeDosdatetime(bhi.ftLastWriteTime, &dosdate, &dostime);
		*timestamp = (WORD)dostime | (((DWORD)dosdate) << 16);
	}
	return SG_NO_ERROR;
}

class TZip {
private:
	unsigned writeLength;
	TZipFileInfo *fileList;
	TState *state;

	unsigned long attr;
	iztimes times;
	unsigned long timestamp;
	bool iseekable;
	long isize, ired;
	unsigned long crc;
	HANDLE hfin;
	bool selfclosehf;
	const char *bufin;
	unsigned int lenin, posin;
	unsigned long csize;
	char buf[16384];

	static unsigned sread(TState &s, char *buf, unsigned size) {
		TZip *zip = (TZip*)s.param;
		return zip->read(buf, size);
	}
	static unsigned swrite(void *param, const char *buf, unsigned size) {
		if (size == 0) return 0;
		TZip *zip = (TZip*)param;
		DWORD writ;
		WriteFile(zip->hfout, buf, size, &writ, NULL);
		return writ;
	}
	static unsigned sflush(void *param, const char *buf, unsigned *size) {
		if (*size == 0) return 0;
		TZip *zip = (TZip*)param;
		DWORD writ;
		WriteFile(zip->hfout, buf, *size, &writ, NULL);
		if (writ != 0) *size = 0;
		return writ;
	}

	int open_handle(HANDLE hf, unsigned int len) {
		hfin = 0; bufin = 0; selfclosehf = false; crc = 0; isize = 0; csize = 0; ired = 0;
		if (hf == 0 || hf == INVALID_HANDLE_VALUE) return SG_IO_ERROR;
		DWORD res = SetFilePointer(hfout, 0, 0, FILE_CURRENT);
		if (res != 0xFFFFFFFF) {
			int res = GetFileInfo(hf, &attr, &isize, &times, &timestamp);
			if (res != SG_NO_ERROR) return res;
			SetFilePointer(hf, 0, NULL, FILE_BEGIN); // because GetFileInfo will have screwed it up
			iseekable = true; hfin = hf;
			return SG_NO_ERROR;
		}
		else {
			attr = 0x80000000;      // just a normal file
			isize = -1;            // can't know size until at the end
			if (len != 0) isize = len; // unless we were told explicitly!
			iseekable = false;
			SYSTEMTIME st; GetLocalTime(&st);
			FILETIME ft;   SystemTimeToFileTime(&st, &ft);
			WORD dosdate, dostime; filetimeDosdatetime(ft, &dosdate, &dostime);
			times.atime = filetimeInt(ft);
			times.mtime = times.atime;
			times.ctime = times.atime;
			timestamp = (WORD)dostime | (((DWORD)dosdate) << 16);
			hfin = hf;
			return SG_NO_ERROR;
		}
	}
	int open_file(const TCHAR *fn) {
		hfin = 0; bufin = 0; selfclosehf = false; crc = 0; isize = 0; csize = 0; ired = 0;
		if (fn == 0) return SG_IO_ERROR;
		HANDLE hf = CreateFile(fn, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hf == INVALID_HANDLE_VALUE) return SG_OBJECT_NOT_FOUND;
		int res = open_handle(hf, 0);
		if (res != SG_NO_ERROR) { CloseHandle(hf); return res; }
		selfclosehf = true;
		return SG_NO_ERROR;
	}
	int open_mem(void *src, unsigned int len) {
		hfin = 0; bufin = (const char*)src; selfclosehf = false; crc = 0; ired = 0; csize = 0; ired = 0;
		lenin = len; posin = 0;
		if (src == 0 || len == 0) return SG_IO_ERROR;
		attr = 0x80000000; // just a normal file
		isize = len;
		iseekable = true;
		SYSTEMTIME st; GetLocalTime(&st);
		FILETIME ft; SystemTimeToFileTime(&st, &ft);
		WORD dosdate, dostime; filetimeDosdatetime(ft, &dosdate, &dostime);
		times.atime = filetimeInt(ft);
		times.mtime = times.atime;
		times.ctime = times.atime;
		timestamp = (WORD)dostime | (((DWORD)dosdate) << 16);
		return SG_NO_ERROR;
	}
	int open_dir() {
		hfin = 0; bufin = 0; selfclosehf = false; crc = 0; isize = 0; csize = 0; ired = 0;
		attr = 0x41C00010; // a readable writable directory, and again directory
		isize = 0;
		iseekable = false;
		SYSTEMTIME st; GetLocalTime(&st);
		FILETIME ft;   SystemTimeToFileTime(&st, &ft);
		WORD dosdate, dostime; filetimeDosdatetime(ft, &dosdate, &dostime);
		times.atime = filetimeInt(ft);
		times.mtime = times.atime;
		times.ctime = times.atime;
		timestamp = (WORD)dostime | (((DWORD)dosdate) << 16);
		return SG_NO_ERROR;
	}
	unsigned read(char *buf, unsigned size) {
		if (bufin != 0) {
			if (posin >= lenin) return 0; // end of input
			unsigned long red = lenin - posin;
			if (red>size) red = size;
			memcpy(buf, bufin + posin, red);
			posin += red;
			ired += red;
			crc = crc32(crc, (unsigned char*)buf, red);
			return red;
		}
		else if (hfin != 0) {
			DWORD red;
			BOOL ok = ReadFile(hfin, buf, size, &red, NULL);
			if (!ok) return 0;
			ired += red;
			crc = crc32(crc, (unsigned char*)buf, red);
			return red;
		}
		else {
			error = SG_IO_ERROR;
			return 0;
		}
	}
	int iclose() {
		if (selfclosehf && hfin != 0) CloseHandle(hfin); hfin = 0;
		bool mismatch = (isize != -1 && isize != ired);
		isize = ired; // and crc has been being updated anyway
		if (mismatch) return SG_IO_ERROR;
		else return SG_NO_ERROR;
	}
	int ideflate(TZipFileInfo *zfi) {
		if (state == 0) state = new TState();
		state->err = 0;
		state->readfunc = sread;
		state->flush_outbuf = sflush;
		state->param = this;
		state->level = 8;
		state->seekable = iseekable;
		state->err = NULL;
		state->ts.static_dtree[0].dl.len = 0;
		state->ds.window_size = 0;
		bi_init(*state, buf, sizeof(buf), TRUE);
		ct_init(*state, &zfi->att);
		lm_init(*state, state->level, &zfi->flg);
		unsigned long sz = deflate(*state);
		csize = sz;
		int r = SG_NO_ERROR;
		if (state->err != NULL) r = SG_IO_ERROR;
		return r;
	}
	int istore() {
		unsigned long size = 0;
		for (;;) {
			unsigned int cin = read(buf, 16384);
			if (cin <= 0 || cin == (unsigned int)EOF) break;
			DWORD cout;
			WriteFile(hfout, buf, cin, &cout, NULL);
			if (cout != cin) return SG_IO_ERROR;
			size += cin;
		}
		csize = size;
		return SG_NO_ERROR;
	}

public:
	TZip() : hfout(0), fileList(0), hfin(0), writeLength(0), error(0), state(0) {}
	~TZip() {
		if (state != 0) delete state;
		state = 0;
	}

	HANDLE hfout;
	int error;

	void Create(SGWINSTR filename) {
		hfout = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hfout == INVALID_HANDLE_VALUE) {
			hfout = 0;
			error = SG_IO_ERROR;
		}
	}
	int Add(const TCHAR *odstzn, void *src, unsigned int len, DWORD flags) {
		if (error) return SG_IO_ERROR;

		TCHAR dstzn[MAX_PATH];
		_tcscpy(dstzn, odstzn);
		if (*dstzn == 0) return SG_IO_ERROR;
		TCHAR *d = dstzn;
		while (*d != 0) {
			if (*d == '\\') *d = '/';
			d++;
		}
		bool isdir = (flags == ZIP_FOLDER);
		bool needDeflate = true;
		if (isdir || HasZipSuffix(dstzn)) needDeflate = false;

		// now open whatever was our input source:
		int openres;
		if (flags == ZIP_FILENAME) openres = open_file((const TCHAR*)src);
		else if (flags == ZIP_MEMORY) openres = open_mem(src, len);
		else if (flags == ZIP_FOLDER) openres = open_dir();
		else return SG_IO_ERROR;
		if (openres != SG_NO_ERROR) return openres;

		// Initialize the local header
		TZipFileInfo zfi; zfi.nxt = NULL;
		strcpy(zfi.name, "");
		_strcpy(zfi.iname, _shorten(dstzn));
		zfi.nam = strlen(zfi.iname);
		if (isdir) { strcat(zfi.iname, "/"); zfi.nam++; }
		strcpy(zfi.zname, "");
		zfi.extra = NULL; zfi.ext = 0;   // extra header to go after this compressed data, and its length
		zfi.cextra = NULL; zfi.cext = 0; // extra header to go in the central end-of-zip directory, and its length
		zfi.comment = NULL; zfi.com = 0; // comment, and its length
		zfi.mark = 1;
		zfi.dosflag = 0;
		zfi.att = (unsigned short)0;
		zfi.vem = (unsigned short)0xB17; // 0xB00 is win32 os-code. 0x17 is 23 in decimal: zip 2.3
		zfi.ver = (unsigned short)20;    // Needs PKUNZIP 2.0 to unzip it
		zfi.tim = timestamp;
		// Even though we write the header now, it will have to be rewritten, since we don't know compressed size or crc.
		zfi.crc = 0;            // to be updated later
		zfi.flg = 8;            // 8 means 'there is an extra header'. Assume for the moment that we need it.
		zfi.lflg = zfi.flg;     // to be updated later
		zfi.how = needDeflate ? 8 : 0;  // to be updated later
		zfi.siz = (unsigned long)(!needDeflate && isize >= 0 ? isize : 0); // to be updated later
		zfi.len = (unsigned long)(isize);  // to be updated later
		zfi.dsk = 0;
		zfi.atx = attr;
		zfi.off = writeLength;         // offset within file of the start of this local record
									   // stuff the 'times' structure into zfi.extra

									   // nb. apparently there's a problem with PocketPC CE(zip)->CE(unzip) fails. And removing the following block fixes it up.
		char xloc[EB_L_UT_SIZE]; zfi.extra = xloc;  zfi.ext = EB_L_UT_SIZE;
		char xcen[EB_C_UT_SIZE]; zfi.cextra = xcen; zfi.cext = EB_C_UT_SIZE;
		xloc[0] = 'U';
		xloc[1] = 'T';
		xloc[2] = EB_UT_LEN(3);       // length of data part of e.f.
		xloc[3] = 0;
		xloc[4] = (1 << 0) | (1 << 1) | (1 << 2);
		xloc[5] = (char)(times.mtime);
		xloc[6] = (char)(times.mtime >> 8);
		xloc[7] = (char)(times.mtime >> 16);
		xloc[8] = (char)(times.mtime >> 24);
		xloc[9] = (char)(times.atime);
		xloc[10] = (char)(times.atime >> 8);
		xloc[11] = (char)(times.atime >> 16);
		xloc[12] = (char)(times.atime >> 24);
		xloc[13] = (char)(times.ctime);
		xloc[14] = (char)(times.ctime >> 8);
		xloc[15] = (char)(times.ctime >> 16);
		xloc[16] = (char)(times.ctime >> 24);
		memcpy(zfi.cextra, zfi.extra, EB_C_UT_SIZE);
		zfi.cextra[2] = EB_UT_LEN(1);


		// (1) Start by writing the local header:
		int r = putlocal(&zfi, swrite, this);
		if (r != SG_NO_ERROR) { iclose(); return SG_IO_ERROR; }
		writeLength += 30 + (unsigned int)zfi.nam + (unsigned int)zfi.ext;
		if (error != SG_NO_ERROR) { iclose(); return error; }

		//(2) Write deflated/stored file to zip file
		int writeres = SG_NO_ERROR;
		if (!isdir && needDeflate) writeres = ideflate(&zfi);
		else if (!isdir && !needDeflate) writeres = istore();
		else if (isdir) csize = 0;
		iclose();
		writeLength += csize;
		if (error != SG_NO_ERROR) return error;
		if (writeres != SG_NO_ERROR) return SG_IO_ERROR;

		// (3) Either rewrite the local header with correct information...
		bool first_header_has_size_right = (zfi.siz == csize);
		zfi.crc = crc;
		zfi.siz = csize;
		zfi.len = isize;
		zfi.how = needDeflate ? 8 : 0;
		if ((zfi.flg & 1) == 0) zfi.flg &= ~8; // clear the extended local header flag
		zfi.lflg = zfi.flg;
		// rewrite the local header:
		SetFilePointer(hfout, zfi.off, NULL, FILE_BEGIN);
		if ((r = putlocal(&zfi, swrite, this)) != SG_NO_ERROR) return SG_IO_ERROR;
		SetFilePointer(hfout, writeLength, NULL, FILE_BEGIN);

		// Keep a copy of the zipfileinfo, for our end-of-zip directory
		char *cextra = new char[zfi.cext]; memcpy(cextra, zfi.cextra, zfi.cext); zfi.cextra = cextra;
		TZipFileInfo *pzfi = new TZipFileInfo; memcpy(pzfi, &zfi, sizeof(zfi));
		if (fileList == NULL) fileList = pzfi;
		else { TZipFileInfo *z = fileList; while (z->nxt != NULL) z = z->nxt; z->nxt = pzfi; }
		return SG_NO_ERROR;
	}
	int Close() {
		int numentries = 0;
		unsigned long pos_at_start_of_central = writeLength;
		bool okay = true;
		for (TZipFileInfo *zfi = fileList; zfi != NULL; ) {
			if (okay) {
				int res = putcentral(zfi, swrite, this);
				if (res != SG_NO_ERROR) okay = false;
			}
			writeLength += 46 + (unsigned int)zfi->nam + (unsigned int)zfi->cext + (unsigned int)zfi->com;
			numentries++;
			//
			TZipFileInfo *zfinext = zfi->nxt;
			if (zfi->cextra != 0) delete[] zfi->cextra;
			delete zfi;
			zfi = zfinext;
		}
		unsigned long center_size = writeLength - pos_at_start_of_central;
		if (okay) {
			int res = putend(numentries, center_size, pos_at_start_of_central, 0, NULL, swrite, this);
			if (res != SG_NO_ERROR) okay = false;
			writeLength += 22;
		}
		if (!okay) return SG_IO_ERROR;
		if (hfout != 0) CloseHandle(hfout);
		hfout = 0;
		return SG_NO_ERROR;
	}
};





#define Z_DEFLATED   8
#define CASE_SENSITIVE 1
#define CASE_INSENSITIVE 2
#define Z_STREAM_END    1
#define Z_NEED_DICT     2
#define PRESET_DICT 0x20
#define MANY 1440
#define BASE 65521L
#define NMAX 5552
#define UNZ_BUFSIZE 16384
#define UNZ_MAXFILENAMEINZIP 256
#define SIZECENTRALDIRITEM 0x2e
#define SIZEZIPLOCALHEADER 0x1e
#define BUFREADCOMMENT 0x400

#define ZALLOC(strm, items, size) \
           (*((strm)->zalloc))((items), (size))
#define ZFREE(strm, addr)  (*((strm)->zfree))((void *)(addr))

#define UPDBITS {s->bitb=b;s->bitk=k;}
#define UPDIN {z->avail_in=n;z->total_in+=(unsigned long)(p-z->next_in);z->next_in=p;}
#define UPDOUT {s->write=q;}
#define UPDATE {UPDBITS UPDIN UPDOUT}
#define LEAVE {UPDATE return inflate_flush(s,z,r);}
#define LOADIN {p=z->next_in;n=z->avail_in;b=s->bitb;k=s->bitk;}
#define NEEDBYTE {if(n)r=SG_NO_ERROR;else LEAVE}
#define NEXTBYTE (n--,*p++)
#define NEEDBITS(j) {while(k<(j)){NEEDBYTE;b|=((unsigned long)NEXTBYTE)<<k;k+=8;}}
#define DUMPBITS(j) {b>>=(j);k-=(j);}
#define WAVAIL (unsigned int)(q<s->read?s->read-q-1:s->end-q)
#define LOADOUT {q=s->write;m=(unsigned int)WAVAIL;m;}
#define WRAP {if(q==s->end&&s->read!=s->window){q=s->window;m=(unsigned int)WAVAIL;}}
#define FLUSH {UPDOUT r=inflate_flush(s,z,r); LOADOUT}
#define NEEDOUT {if(m==0){WRAP if(m==0){FLUSH WRAP if(m==0) LEAVE}}r=SG_NO_ERROR;}
#define LOAD {LOADIN LOADOUT}

typedef struct inflate_huft_s {
	union {
		struct {
			unsigned char Exop;        // number of extra bits or operation
			unsigned char Bits;        // number of bits in this code or subcode
		} what;
		unsigned int pad;           // pad structure to a power of 2 (4 unsigned chars for
	} word;               //  16-bit, 8 unsigned chars for 32-bit int's)
	unsigned int base;            // literal, length base, distance base, or table offset
}inflate_huft;
const unsigned int fixed_bl = 9;
const unsigned int fixed_bd = 5;
const inflate_huft fixed_tl[] = {
	{ { { 96,7 } },256 },{ { { 0,8 } },80 },{ { { 0,8 } },16 },{ { { 84,8 } },115 },
{ { { 82,7 } },31 },{ { { 0,8 } },112 },{ { { 0,8 } },48 },{ { { 0,9 } },192 },
{ { { 80,7 } },10 },{ { { 0,8 } },96 },{ { { 0,8 } },32 },{ { { 0,9 } },160 },
{ { { 0,8 } },0 },{ { { 0,8 } },128 },{ { { 0,8 } },64 },{ { { 0,9 } },224 },
{ { { 80,7 } },6 },{ { { 0,8 } },88 },{ { { 0,8 } },24 },{ { { 0,9 } },144 },
{ { { 83,7 } },59 },{ { { 0,8 } },120 },{ { { 0,8 } },56 },{ { { 0,9 } },208 },
{ { { 81,7 } },17 },{ { { 0,8 } },104 },{ { { 0,8 } },40 },{ { { 0,9 } },176 },
{ { { 0,8 } },8 },{ { { 0,8 } },136 },{ { { 0,8 } },72 },{ { { 0,9 } },240 },
{ { { 80,7 } },4 },{ { { 0,8 } },84 },{ { { 0,8 } },20 },{ { { 85,8 } },227 },
{ { { 83,7 } },43 },{ { { 0,8 } },116 },{ { { 0,8 } },52 },{ { { 0,9 } },200 },
{ { { 81,7 } },13 },{ { { 0,8 } },100 },{ { { 0,8 } },36 },{ { { 0,9 } },168 },
{ { { 0,8 } },4 },{ { { 0,8 } },132 },{ { { 0,8 } },68 },{ { { 0,9 } },232 },
{ { { 80,7 } },8 },{ { { 0,8 } },92 },{ { { 0,8 } },28 },{ { { 0,9 } },152 },
{ { { 84,7 } },83 },{ { { 0,8 } },124 },{ { { 0,8 } },60 },{ { { 0,9 } },216 },
{ { { 82,7 } },23 },{ { { 0,8 } },108 },{ { { 0,8 } },44 },{ { { 0,9 } },184 },
{ { { 0,8 } },12 },{ { { 0,8 } },140 },{ { { 0,8 } },76 },{ { { 0,9 } },248 },
{ { { 80,7 } },3 },{ { { 0,8 } },82 },{ { { 0,8 } },18 },{ { { 85,8 } },163 },
{ { { 83,7 } },35 },{ { { 0,8 } },114 },{ { { 0,8 } },50 },{ { { 0,9 } },196 },
{ { { 81,7 } },11 },{ { { 0,8 } },98 },{ { { 0,8 } },34 },{ { { 0,9 } },164 },
{ { { 0,8 } },2 },{ { { 0,8 } },130 },{ { { 0,8 } },66 },{ { { 0,9 } },228 },
{ { { 80,7 } },7 },{ { { 0,8 } },90 },{ { { 0,8 } },26 },{ { { 0,9 } },148 },
{ { { 84,7 } },67 },{ { { 0,8 } },122 },{ { { 0,8 } },58 },{ { { 0,9 } },212 },
{ { { 82,7 } },19 },{ { { 0,8 } },106 },{ { { 0,8 } },42 },{ { { 0,9 } },180 },
{ { { 0,8 } },10 },{ { { 0,8 } },138 },{ { { 0,8 } },74 },{ { { 0,9 } },244 },
{ { { 80,7 } },5 },{ { { 0,8 } },86 },{ { { 0,8 } },22 },{ { { 192,8 } },0 },
{ { { 83,7 } },51 },{ { { 0,8 } },118 },{ { { 0,8 } },54 },{ { { 0,9 } },204 },
{ { { 81,7 } },15 },{ { { 0,8 } },102 },{ { { 0,8 } },38 },{ { { 0,9 } },172 },
{ { { 0,8 } },6 },{ { { 0,8 } },134 },{ { { 0,8 } },70 },{ { { 0,9 } },236 },
{ { { 80,7 } },9 },{ { { 0,8 } },94 },{ { { 0,8 } },30 },{ { { 0,9 } },156 },
{ { { 84,7 } },99 },{ { { 0,8 } },126 },{ { { 0,8 } },62 },{ { { 0,9 } },220 },
{ { { 82,7 } },27 },{ { { 0,8 } },110 },{ { { 0,8 } },46 },{ { { 0,9 } },188 },
{ { { 0,8 } },14 },{ { { 0,8 } },142 },{ { { 0,8 } },78 },{ { { 0,9 } },252 },
{ { { 96,7 } },256 },{ { { 0,8 } },81 },{ { { 0,8 } },17 },{ { { 85,8 } },131 },
{ { { 82,7 } },31 },{ { { 0,8 } },113 },{ { { 0,8 } },49 },{ { { 0,9 } },194 },
{ { { 80,7 } },10 },{ { { 0,8 } },97 },{ { { 0,8 } },33 },{ { { 0,9 } },162 },
{ { { 0,8 } },1 },{ { { 0,8 } },129 },{ { { 0,8 } },65 },{ { { 0,9 } },226 },
{ { { 80,7 } },6 },{ { { 0,8 } },89 },{ { { 0,8 } },25 },{ { { 0,9 } },146 },
{ { { 83,7 } },59 },{ { { 0,8 } },121 },{ { { 0,8 } },57 },{ { { 0,9 } },210 },
{ { { 81,7 } },17 },{ { { 0,8 } },105 },{ { { 0,8 } },41 },{ { { 0,9 } },178 },
{ { { 0,8 } },9 },{ { { 0,8 } },137 },{ { { 0,8 } },73 },{ { { 0,9 } },242 },
{ { { 80,7 } },4 },{ { { 0,8 } },85 },{ { { 0,8 } },21 },{ { { 80,8 } },258 },
{ { { 83,7 } },43 },{ { { 0,8 } },117 },{ { { 0,8 } },53 },{ { { 0,9 } },202 },
{ { { 81,7 } },13 },{ { { 0,8 } },101 },{ { { 0,8 } },37 },{ { { 0,9 } },170 },
{ { { 0,8 } },5 },{ { { 0,8 } },133 },{ { { 0,8 } },69 },{ { { 0,9 } },234 },
{ { { 80,7 } },8 },{ { { 0,8 } },93 },{ { { 0,8 } },29 },{ { { 0,9 } },154 },
{ { { 84,7 } },83 },{ { { 0,8 } },125 },{ { { 0,8 } },61 },{ { { 0,9 } },218 },
{ { { 82,7 } },23 },{ { { 0,8 } },109 },{ { { 0,8 } },45 },{ { { 0,9 } },186 },
{ { { 0,8 } },13 },{ { { 0,8 } },141 },{ { { 0,8 } },77 },{ { { 0,9 } },250 },
{ { { 80,7 } },3 },{ { { 0,8 } },83 },{ { { 0,8 } },19 },{ { { 85,8 } },195 },
{ { { 83,7 } },35 },{ { { 0,8 } },115 },{ { { 0,8 } },51 },{ { { 0,9 } },198 },
{ { { 81,7 } },11 },{ { { 0,8 } },99 },{ { { 0,8 } },35 },{ { { 0,9 } },166 },
{ { { 0,8 } },3 },{ { { 0,8 } },131 },{ { { 0,8 } },67 },{ { { 0,9 } },230 },
{ { { 80,7 } },7 },{ { { 0,8 } },91 },{ { { 0,8 } },27 },{ { { 0,9 } },150 },
{ { { 84,7 } },67 },{ { { 0,8 } },123 },{ { { 0,8 } },59 },{ { { 0,9 } },214 },
{ { { 82,7 } },19 },{ { { 0,8 } },107 },{ { { 0,8 } },43 },{ { { 0,9 } },182 },
{ { { 0,8 } },11 },{ { { 0,8 } },139 },{ { { 0,8 } },75 },{ { { 0,9 } },246 },
{ { { 80,7 } },5 },{ { { 0,8 } },87 },{ { { 0,8 } },23 },{ { { 192,8 } },0 },
{ { { 83,7 } },51 },{ { { 0,8 } },119 },{ { { 0,8 } },55 },{ { { 0,9 } },206 },
{ { { 81,7 } },15 },{ { { 0,8 } },103 },{ { { 0,8 } },39 },{ { { 0,9 } },174 },
{ { { 0,8 } },7 },{ { { 0,8 } },135 },{ { { 0,8 } },71 },{ { { 0,9 } },238 },
{ { { 80,7 } },9 },{ { { 0,8 } },95 },{ { { 0,8 } },31 },{ { { 0,9 } },158 },
{ { { 84,7 } },99 },{ { { 0,8 } },127 },{ { { 0,8 } },63 },{ { { 0,9 } },222 },
{ { { 82,7 } },27 },{ { { 0,8 } },111 },{ { { 0,8 } },47 },{ { { 0,9 } },190 },
{ { { 0,8 } },15 },{ { { 0,8 } },143 },{ { { 0,8 } },79 },{ { { 0,9 } },254 },
{ { { 96,7 } },256 },{ { { 0,8 } },80 },{ { { 0,8 } },16 },{ { { 84,8 } },115 },
{ { { 82,7 } },31 },{ { { 0,8 } },112 },{ { { 0,8 } },48 },{ { { 0,9 } },193 },
{ { { 80,7 } },10 },{ { { 0,8 } },96 },{ { { 0,8 } },32 },{ { { 0,9 } },161 },
{ { { 0,8 } },0 },{ { { 0,8 } },128 },{ { { 0,8 } },64 },{ { { 0,9 } },225 },
{ { { 80,7 } },6 },{ { { 0,8 } },88 },{ { { 0,8 } },24 },{ { { 0,9 } },145 },
{ { { 83,7 } },59 },{ { { 0,8 } },120 },{ { { 0,8 } },56 },{ { { 0,9 } },209 },
{ { { 81,7 } },17 },{ { { 0,8 } },104 },{ { { 0,8 } },40 },{ { { 0,9 } },177 },
{ { { 0,8 } },8 },{ { { 0,8 } },136 },{ { { 0,8 } },72 },{ { { 0,9 } },241 },
{ { { 80,7 } },4 },{ { { 0,8 } },84 },{ { { 0,8 } },20 },{ { { 85,8 } },227 },
{ { { 83,7 } },43 },{ { { 0,8 } },116 },{ { { 0,8 } },52 },{ { { 0,9 } },201 },
{ { { 81,7 } },13 },{ { { 0,8 } },100 },{ { { 0,8 } },36 },{ { { 0,9 } },169 },
{ { { 0,8 } },4 },{ { { 0,8 } },132 },{ { { 0,8 } },68 },{ { { 0,9 } },233 },
{ { { 80,7 } },8 },{ { { 0,8 } },92 },{ { { 0,8 } },28 },{ { { 0,9 } },153 },
{ { { 84,7 } },83 },{ { { 0,8 } },124 },{ { { 0,8 } },60 },{ { { 0,9 } },217 },
{ { { 82,7 } },23 },{ { { 0,8 } },108 },{ { { 0,8 } },44 },{ { { 0,9 } },185 },
{ { { 0,8 } },12 },{ { { 0,8 } },140 },{ { { 0,8 } },76 },{ { { 0,9 } },249 },
{ { { 80,7 } },3 },{ { { 0,8 } },82 },{ { { 0,8 } },18 },{ { { 85,8 } },163 },
{ { { 83,7 } },35 },{ { { 0,8 } },114 },{ { { 0,8 } },50 },{ { { 0,9 } },197 },
{ { { 81,7 } },11 },{ { { 0,8 } },98 },{ { { 0,8 } },34 },{ { { 0,9 } },165 },
{ { { 0,8 } },2 },{ { { 0,8 } },130 },{ { { 0,8 } },66 },{ { { 0,9 } },229 },
{ { { 80,7 } },7 },{ { { 0,8 } },90 },{ { { 0,8 } },26 },{ { { 0,9 } },149 },
{ { { 84,7 } },67 },{ { { 0,8 } },122 },{ { { 0,8 } },58 },{ { { 0,9 } },213 },
{ { { 82,7 } },19 },{ { { 0,8 } },106 },{ { { 0,8 } },42 },{ { { 0,9 } },181 },
{ { { 0,8 } },10 },{ { { 0,8 } },138 },{ { { 0,8 } },74 },{ { { 0,9 } },245 },
{ { { 80,7 } },5 },{ { { 0,8 } },86 },{ { { 0,8 } },22 },{ { { 192,8 } },0 },
{ { { 83,7 } },51 },{ { { 0,8 } },118 },{ { { 0,8 } },54 },{ { { 0,9 } },205 },
{ { { 81,7 } },15 },{ { { 0,8 } },102 },{ { { 0,8 } },38 },{ { { 0,9 } },173 },
{ { { 0,8 } },6 },{ { { 0,8 } },134 },{ { { 0,8 } },70 },{ { { 0,9 } },237 },
{ { { 80,7 } },9 },{ { { 0,8 } },94 },{ { { 0,8 } },30 },{ { { 0,9 } },157 },
{ { { 84,7 } },99 },{ { { 0,8 } },126 },{ { { 0,8 } },62 },{ { { 0,9 } },221 },
{ { { 82,7 } },27 },{ { { 0,8 } },110 },{ { { 0,8 } },46 },{ { { 0,9 } },189 },
{ { { 0,8 } },14 },{ { { 0,8 } },142 },{ { { 0,8 } },78 },{ { { 0,9 } },253 },
{ { { 96,7 } },256 },{ { { 0,8 } },81 },{ { { 0,8 } },17 },{ { { 85,8 } },131 },
{ { { 82,7 } },31 },{ { { 0,8 } },113 },{ { { 0,8 } },49 },{ { { 0,9 } },195 },
{ { { 80,7 } },10 },{ { { 0,8 } },97 },{ { { 0,8 } },33 },{ { { 0,9 } },163 },
{ { { 0,8 } },1 },{ { { 0,8 } },129 },{ { { 0,8 } },65 },{ { { 0,9 } },227 },
{ { { 80,7 } },6 },{ { { 0,8 } },89 },{ { { 0,8 } },25 },{ { { 0,9 } },147 },
{ { { 83,7 } },59 },{ { { 0,8 } },121 },{ { { 0,8 } },57 },{ { { 0,9 } },211 },
{ { { 81,7 } },17 },{ { { 0,8 } },105 },{ { { 0,8 } },41 },{ { { 0,9 } },179 },
{ { { 0,8 } },9 },{ { { 0,8 } },137 },{ { { 0,8 } },73 },{ { { 0,9 } },243 },
{ { { 80,7 } },4 },{ { { 0,8 } },85 },{ { { 0,8 } },21 },{ { { 80,8 } },258 },
{ { { 83,7 } },43 },{ { { 0,8 } },117 },{ { { 0,8 } },53 },{ { { 0,9 } },203 },
{ { { 81,7 } },13 },{ { { 0,8 } },101 },{ { { 0,8 } },37 },{ { { 0,9 } },171 },
{ { { 0,8 } },5 },{ { { 0,8 } },133 },{ { { 0,8 } },69 },{ { { 0,9 } },235 },
{ { { 80,7 } },8 },{ { { 0,8 } },93 },{ { { 0,8 } },29 },{ { { 0,9 } },155 },
{ { { 84,7 } },83 },{ { { 0,8 } },125 },{ { { 0,8 } },61 },{ { { 0,9 } },219 },
{ { { 82,7 } },23 },{ { { 0,8 } },109 },{ { { 0,8 } },45 },{ { { 0,9 } },187 },
{ { { 0,8 } },13 },{ { { 0,8 } },141 },{ { { 0,8 } },77 },{ { { 0,9 } },251 },
{ { { 80,7 } },3 },{ { { 0,8 } },83 },{ { { 0,8 } },19 },{ { { 85,8 } },195 },
{ { { 83,7 } },35 },{ { { 0,8 } },115 },{ { { 0,8 } },51 },{ { { 0,9 } },199 },
{ { { 81,7 } },11 },{ { { 0,8 } },99 },{ { { 0,8 } },35 },{ { { 0,9 } },167 },
{ { { 0,8 } },3 },{ { { 0,8 } },131 },{ { { 0,8 } },67 },{ { { 0,9 } },231 },
{ { { 80,7 } },7 },{ { { 0,8 } },91 },{ { { 0,8 } },27 },{ { { 0,9 } },151 },
{ { { 84,7 } },67 },{ { { 0,8 } },123 },{ { { 0,8 } },59 },{ { { 0,9 } },215 },
{ { { 82,7 } },19 },{ { { 0,8 } },107 },{ { { 0,8 } },43 },{ { { 0,9 } },183 },
{ { { 0,8 } },11 },{ { { 0,8 } },139 },{ { { 0,8 } },75 },{ { { 0,9 } },247 },
{ { { 80,7 } },5 },{ { { 0,8 } },87 },{ { { 0,8 } },23 },{ { { 192,8 } },0 },
{ { { 83,7 } },51 },{ { { 0,8 } },119 },{ { { 0,8 } },55 },{ { { 0,9 } },207 },
{ { { 81,7 } },15 },{ { { 0,8 } },103 },{ { { 0,8 } },39 },{ { { 0,9 } },175 },
{ { { 0,8 } },7 },{ { { 0,8 } },135 },{ { { 0,8 } },71 },{ { { 0,9 } },239 },
{ { { 80,7 } },9 },{ { { 0,8 } },95 },{ { { 0,8 } },31 },{ { { 0,9 } },159 },
{ { { 84,7 } },99 },{ { { 0,8 } },127 },{ { { 0,8 } },63 },{ { { 0,9 } },223 },
{ { { 82,7 } },27 },{ { { 0,8 } },111 },{ { { 0,8 } },47 },{ { { 0,9 } },191 },
{ { { 0,8 } },15 },{ { { 0,8 } },143 },{ { { 0,8 } },79 },{ { { 0,9 } },255 }
};
const inflate_huft fixed_td[] = {
	{ { { 80,5 } },1 },{ { { 87,5 } },257 },{ { { 83,5 } },17 },{ { { 91,5 } },4097 },
{ { { 81,5 } },5 },{ { { 89,5 } },1025 },{ { { 85,5 } },65 },{ { { 93,5 } },16385 },
{ { { 80,5 } },3 },{ { { 88,5 } },513 },{ { { 84,5 } },33 },{ { { 92,5 } },8193 },
{ { { 82,5 } },9 },{ { { 90,5 } },2049 },{ { { 86,5 } },129 },{ { { 192,5 } },24577 },
{ { { 80,5 } },2 },{ { { 87,5 } },385 },{ { { 83,5 } },25 },{ { { 91,5 } },6145 },
{ { { 81,5 } },7 },{ { { 89,5 } },1537 },{ { { 85,5 } },97 },{ { { 93,5 } },24577 },
{ { { 80,5 } },4 },{ { { 88,5 } },769 },{ { { 84,5 } },49 },{ { { 92,5 } },12289 },
{ { { 82,5 } },13 },{ { { 90,5 } },3073 },{ { { 86,5 } },193 },{ { { 192,5 } },24577 }
};
const unsigned int inflate_mask[17] = {
	0x0000,
	0x0001, 0x0003, 0x0007, 0x000f, 0x001f, 0x003f, 0x007f, 0x00ff,
	0x01ff, 0x03ff, 0x07ff, 0x0fff, 0x1fff, 0x3fff, 0x7fff, 0xffff
};
const unsigned int border[] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
const unsigned int cplens[31] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0 };
const unsigned int cplext[31] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 112, 112 }; // 112==invalid
const unsigned int cpdist[30] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577 };
const unsigned int cpdext[30] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };

typedef void *(*alloc_func) (unsigned int items, unsigned int size);
typedef void(*free_func)  (void * address);
typedef unsigned long(*check_func) (unsigned long check, const unsigned char *buf, unsigned int len);

typedef struct {
	int index;
	TCHAR name[MAX_PATH];
	DWORD attr;
	FILETIME atime, ctime, mtime;
	long comp_size;
	long unc_size;
} ZIPENTRY;
typedef struct {
	unsigned int tm_sec;
	unsigned int tm_min;
	unsigned int tm_hour;
	unsigned int tm_mday;
	unsigned int tm_mon;
	unsigned int tm_year;
} tm_unz;
typedef struct {
	unsigned long number_entry;
	unsigned long size_comment;
} unz_global_info;
typedef struct {
	unsigned long version;
	unsigned long version_needed;
	unsigned long flag;
	unsigned long compression_method;
	unsigned long dosDate;
	unsigned long crc;
	unsigned long compressed_size;
	unsigned long uncompressed_size;
	unsigned long size_filename;
	unsigned long size_file_extra;
	unsigned long size_file_comment;
	unsigned long disk_num_start;
	unsigned long internal_fa;
	unsigned long external_fa;
	tm_unz tmu_date;
} unz_file_info;

struct internal_state;
typedef struct z_stream_s {
	unsigned char    *next_in;
	unsigned int     avail_in;
	unsigned long    total_in;

	unsigned char    *next_out;
	unsigned int     avail_out;
	unsigned long    total_out;

	char     *msg;
	struct internal_state *state;

	alloc_func zalloc;
	free_func  zfree;
	void *     opaque;

	int     data_type;
	unsigned long   adler;
	unsigned long   reserved;
} z_stream;

typedef enum {
	IBM_TYPE,     // get type bits (3, including end bit)
	IBM_LENS,     // get lengths for stored
	IBM_STORED,   // processing stored block
	IBM_TABLE,    // get table lengths
	IBM_BTREE,    // get bit lengths tree for a dynamic block
	IBM_DTREE,    // get length, distance trees for a dynamic block
	IBM_CODES,    // processing fixed or dynamic block
	IBM_DRY,      // output remaining window unsigned chars
	IBM_DONE,     // finished last block, done 
	IBM_BAD
} inflate_block_mode;
typedef enum {
	START,    // x: set up for LEN 
	LEN,      // i: get length/literal/eob next 
	LENEXT,   // i: getting length extra (have base) 
	DIST,     // i: get distance next 
	DISTEXT,  // i: getting distance extra 
	COPY,     // o: copying unsigned chars in window, waiting for space
	LIT,      // o: got literal, waiting for output space 
	WASH,     // o: got eob, possibly still output waiting 
	END,      // x: got eob and all data flushed 
	BADCODE
} inflate_codes_mode;
typedef struct inflate_codes_state {
	inflate_codes_mode mode;      // current inflate_codes mode 

	unsigned int len;
	union {
		struct {
			const inflate_huft *tree;       // pointer into tree 
			unsigned int need;                // bits needed 
		} code;             // if LEN or DIST, where in tree 
		unsigned int lit;           // if LIT, literal 
		struct {
			unsigned int get;                 // bits to get for extra 
			unsigned int dist;                // distance back to copy from 
		} copy;             // if EXT or COPY, where and how much 
	} sub;                // submode

	unsigned char lbits;           // ltree bits decoded per branch 
	unsigned char dbits;           // dtree bits decoder per branch 
	const inflate_huft *ltree;          // literal/length/eob tree
	const inflate_huft *dtree;          // distance tree
} inflate_codes_statef;
typedef struct inflate_blocks_state {
	inflate_block_mode  mode;     // current inflate_block mode 

	union {
		unsigned int left;          // if STORED, unsigned chars left to copy 
		struct {
			unsigned int table;               // table lengths (14 bits) 
			unsigned int index;               // index into blens (or border)
			unsigned int *blens;             // bit lengths of codes
			unsigned int bb;                  // bit length tree depth 
			inflate_huft *tb;         // bit length decoding tree 
		} trees;            // if DTREE, decoding info for trees 
		struct {
			inflate_codes_statef *codes;
		} decode;           // if CODES, current state 
	} sub;                // submode
	unsigned int last;            // true if this block is the last block 

	unsigned int bitk;            // bits in bit buffer 
	unsigned long bitb;           // bit buffer 
	inflate_huft *hufts;  // single malloc for tree space 
	unsigned char *window;        // sliding window 
	unsigned char *end;           // one unsigned char after sliding window 
	unsigned char *read;          // window read pointer 
	unsigned char *write;         // window write pointer 
	check_func checkfn;   // check function 
	unsigned long check;          // check on output 

} inflate_blocks_statef;
typedef enum {
	IM_METHOD,   // waiting for method byte
	IM_FLAG,     // waiting for flag byte
	IM_DICT4,    // four dictionary check bytes to go
	IM_DICT3,    // three dictionary check bytes to go
	IM_DICT2,    // two dictionary check bytes to go
	IM_DICT1,    // one dictionary check byte to go
	IM_DICT0,    // waiting for inflateSetDictionary
	IM_BLOCKS,   // decompressing blocks
	IM_CHECK4,   // four check bytes to go
	IM_CHECK3,   // three check bytes to go
	IM_CHECK2,   // two check bytes to go
	IM_CHECK1,   // one check byte to go
	IM_DONE,     // finished check, done
	IM_BAD
} inflate_mode;
struct internal_state {
	inflate_mode  mode;   // current inflate mode

	union {
		unsigned int method;        // if IM_FLAGS, method byte
		struct {
			unsigned long was;                // computed check value
			unsigned long need;               // stream check value
		} check;            // if CHECK, check values to compare
		unsigned int marker;        // if IM_BAD, inflateSync's marker bytes count
	} sub;        // submode

	int  nowrap;          // flag for no wrapper
	unsigned int wbits;           // log2(window size)  (8..15, defaults to 15)
	inflate_blocks_statef
		*blocks;            // current inflate_blocks state
};

typedef struct {
	HANDLE h;
	bool herr;
	unsigned long initial_offset;
} LUFILE;

typedef struct {
	char  *read_buffer;         // internal buffer for compressed data
	z_stream stream;            // zLib stream structure for inflate

	unsigned long pos_in_zipfile;       // position in byte on the zipfile, for fseek
	unsigned long stream_initialised;   // flag set if stream structure is initialised

	unsigned long offset_local_extrafield;// offset of the local extra field
	unsigned int  size_local_extrafield;// size of the local extra field
	unsigned long pos_local_extrafield;   // position in the local extra field in read

	unsigned long crc32;                // crc32 of all data uncompressed
	unsigned long crc32_wait;           // crc32 we must obtain after decompress all
	unsigned long rest_read_compressed; // number of byte to be decompressed
	unsigned long rest_read_uncompressed;//number of byte to be obtained after decomp
	LUFILE* file;                 // io structore of the zipfile
	unsigned long compression_method;   // compression method (0==store)
	unsigned long byte_before_the_zipfile;// byte before the zipfile, (>0 for sfx)
} file_in_zip_read_info_s;
typedef struct {
	LUFILE* file;               // io structore of the zipfile
	unz_global_info gi;         // public global information
	unsigned long byte_before_the_zipfile;// byte before the zipfile, (>0 for sfx)
	unsigned long num_file;             // number of the current file in the zipfile
	unsigned long pos_in_central_dir;   // pos of the current file in the central dir
	unsigned long current_file_ok;      // flag about the usability of the current file
	unsigned long central_pos;          // position of the beginning of the central dir

	unsigned long size_central_dir;     // size of the central directory
	unsigned long offset_central_dir;   // offset of start of central directory with respect to the starting disk number

	unz_file_info cur_file_info; // public info about the current file in zip
	unsigned long cur_file_info_internal; // private info about it
	file_in_zip_read_info_s* pfile_in_zip_read; // structure about the current file if we are decompressing it
} unz_s, *unzFile;

int huft_build(unsigned int *b, unsigned int n, unsigned int s, const unsigned int *d, const unsigned int *e, inflate_huft * *t, unsigned int *m, inflate_huft *hp, unsigned int *hn, unsigned int *v) {
	unsigned int a;                       // counter for codes of length k
	unsigned int c[16];               // bit length count table
	unsigned int f;                       // i repeats in table every f entries 
	int g;                        // maximum code length 
	int h;                        // table level 
	register unsigned int i;              // counter, current code 
	register unsigned int j;              // counter
	register int k;               // number of bits in current code 
	int l;                        // bits per table (returned in m) 
	unsigned int mask;                    // (1 << w) - 1, to avoid cc -O bug on HP 
	register unsigned int *p;            // pointer into c[], b[], or v[]
	inflate_huft *q;              // points to current table 
	struct inflate_huft_s r;      // table entry for structure assignment 
	inflate_huft *u[15];        // table stack 
	register int w;               // bits before this table == (l * h) 
	unsigned int x[16];               // bit offsets, then code stack 
	unsigned int *xp;                    // pointer into x 
	int y;                        // number of dummy codes added 
	unsigned int z;                       // number of entries in current table 

	p = c;
#define C0 *p++ = 0;
#define C2 C0 C0 C0 C0
#define C4 C2 C2 C2 C2
	C4; p;                          // clear c[]--assume BMAX+1 is 16
	p = b;  i = n;
	do {
		c[*p++]++;                  // assume all entries <= BMAX 
	} while (--i);
	if (c[0] == n) {
		*t = (inflate_huft *)NULL;
		*m = 0;
		return SG_NO_ERROR;
	}

	l = *m;
	for (j = 1; j <= 15; j++)if (c[j])break;
	k = j;
	if ((unsigned int)l < j)l = j;
	for (i = 15; i; i--)if (c[i])break;
	g = i;
	if ((unsigned int)l > i)l = i;
	*m = l;

	for (y = 1 << j; j < i; j++, y <<= 1)if ((y -= c[j]) < 0)return SG_IO_ERROR;
	if ((y -= c[i]) < 0)return SG_IO_ERROR;
	c[i] += y;

	x[1] = j = 0;
	p = c + 1;  xp = x + 2;
	while (--i) {
		*xp++ = (j += *p++);
	}

	p = b;  i = 0;
	do {
		if ((j = *p++) != 0)
			v[x[j]++] = i;
	} while (++i < n);
	n = x[g];

	x[0] = i = 0;
	p = v;
	h = -1;
	w = -l;
	u[0] = (inflate_huft *)NULL;
	q = (inflate_huft *)NULL;
	z = 0;
	for (; k <= g; k++) {
		a = c[k];
		while (a--) {
			while (k > w + l) {
				h++;
				w += l;
				z = g - w;
				z = z > (unsigned int)l ? l : z;
				if ((f = 1 << (j = k - w)) > a + 1) {
					f -= a + 1;
					xp = c + k;
					if (j < z)
						while (++j < z) {
							if ((f <<= 1) <= *++xp)break;
							f -= *xp;
						}
				}
				z = 1 << j;
				if (*hn + z > MANY)return SG_IO_ERROR;
				u[h] = q = hp + *hn;
				*hn += z;

				if (h) {
					x[h] = i;
					r.word.what.Bits = (unsigned char)l;
					r.word.what.Exop = (unsigned char)j;
					j = i >> (w - l);
					r.base = (unsigned int)(q - u[h - 1] - j);
					u[h - 1][j] = r;
				}
				else
					*t = q;
			}

			r.word.what.Bits = (unsigned char)(k - w);
			if (p >= v + n)r.word.what.Exop = 128 + 64;
			else if (*p < s) {
				r.word.what.Exop = (unsigned char)(*p < 256 ? 0 : 32 + 64);
				r.base = *p++;
			}
			else {
				r.word.what.Exop = (unsigned char)(e[*p - s] + 16 + 64);
				r.base = d[*p++ - s];
			}

			f = 1 << (k - w);
			for (j = i >> w; j < z; j += f)q[j] = r;
			for (j = 1 << (k - 1); i & j; j >>= 1)i ^= j;
			i ^= j;

			mask = (1 << w) - 1;
			while ((i & mask) != x[h]) {
				h--;
				w -= l;
				mask = (1 << w) - 1;
			}
		}
	}
	return y != 0 && g != 1 ? SG_IO_ERROR : SG_NO_ERROR;
}
int inflate_trees_bits(unsigned int *c, unsigned int *bb, inflate_huft * *tb, inflate_huft *hp, z_stream *z) {
	int r;
	unsigned int hn = 0;
	unsigned int *v;

	if ((v = (unsigned int*)ZALLOC(z, 19, sizeof(unsigned int))) == NULL)return SG_NO_LOAD_MEM;
	r = huft_build(c, 19, 19, (unsigned int*)NULL, (unsigned int*)NULL,
		tb, bb, hp, &hn, v);
	if (*bb == 0)r = SG_IO_ERROR;
	ZFREE(z, v);
	return r;
}
int inflate_trees_dynamic(unsigned int nl, unsigned int nd, unsigned int *c, unsigned int *bl, unsigned int *bd, inflate_huft * *tl, inflate_huft * *td, inflate_huft *hp, z_stream *z) {
	int r;
	unsigned int hn = 0;          // hufts used in space 
	unsigned int *v;             // work area for huft_build 

	if ((v = (unsigned int*)ZALLOC(z, 288, sizeof(unsigned int))) == NULL)
		return SG_NO_LOAD_MEM;

	r = huft_build(c, nl, 257, cplens, cplext, tl, bl, hp, &hn, v);
	if (*bl == 0) {
		r = SG_IO_ERROR;
		ZFREE(z, v);
		return r;
	}

	r = huft_build(c + nl, nd, 0, cpdist, cpdext, td, bd, hp, &hn, v);
	if (r != SG_NO_ERROR || (*bd == 0 && nl > 257)){
		ZFREE(z, v);
		return r;
	}
	ZFREE(z, v);
	return SG_NO_ERROR;
}
int inflate_trees_fixed(unsigned int *bl, unsigned int *bd, const inflate_huft * * tl, const inflate_huft * *td) {
	*bl = fixed_bl;
	*bd = fixed_bd;
	*tl = fixed_tl;
	*td = fixed_td;
	return SG_NO_ERROR;
}
int inflate_flush(inflate_blocks_statef *s, z_stream *z, int r) {
	unsigned int n;
	unsigned char *p;
	unsigned char *q;

	p = z->next_out;
	q = s->read;

	n = (unsigned int)((q <= s->write ? s->write : s->end) - q);
	if (n > z->avail_out) n = z->avail_out;
	if (n && r == SG_IO_ERROR) r = SG_NO_ERROR;

	z->avail_out -= n;
	z->total_out += n;

	if (s->checkfn != NULL)
		z->adler = s->check = (*s->checkfn)(s->check, q, n);

	if (n != 0) {
		memcpy(p, q, n);
		p += n;
		q += n;
	}

	if (q == s->end) {
		q = s->window;
		if (s->write == s->end)
			s->write = s->window;

		n = (unsigned int)(s->write - q);
		if (n > z->avail_out) n = z->avail_out;
		if (n && r == SG_IO_ERROR) r = SG_NO_ERROR;

		z->avail_out -= n;
		z->total_out += n;

		if (s->checkfn != NULL)
			z->adler = s->check = (*s->checkfn)(s->check, q, n);

		if (n != 0) { memcpy(p, q, n); p += n; q += n; }
	}
	z->next_out = p;
	s->read = q;
	return r;
}

inflate_codes_statef *inflate_codes_new(unsigned int bl, unsigned int bd, const inflate_huft *tl, const inflate_huft *td, z_stream *z) {
	inflate_codes_statef *c;

	if ((c = (inflate_codes_statef *)ZALLOC(z, 1, sizeof(struct inflate_codes_state))) != NULL) {
		c->mode = START;
		c->lbits = (unsigned char)bl;
		c->dbits = (unsigned char)bd;
		c->ltree = tl;
		c->dtree = td;
	}
	return c;
}
int inflate_codes(inflate_blocks_statef *s, z_stream *z, int r) {

#define OUTBYTE(a) {*q++=(unsigned char)(a);m--;}

	unsigned int j;               // temporary storage
	const inflate_huft *t;      // temporary pointer
	unsigned int e;               // extra bits or operation
	unsigned long b;              // bit buffer
	unsigned int k;               // bits in bit buffer
	unsigned char *p;             // input data pointer
	unsigned int n;               // unsigned chars available there
	unsigned char *q;             // output window write pointer
	unsigned int m;               // unsigned chars to end of window or read pointer
	unsigned char *f;             // pointer to copy strings from
	inflate_codes_statef *c = s->sub.decode.codes;  // codes state

	LOAD

	for (;;)
		switch (c->mode) {             // waiting for "i:"=input, "o:"=output, "x:"=nothing
		case START:
			c->sub.code.need = c->lbits;
			c->sub.code.tree = c->ltree;
			c->mode = LEN;
		case LEN:           // i: get length/literal/eob next
			j = c->sub.code.need;
			NEEDBITS(j)
				t = c->sub.code.tree + ((unsigned int)b & inflate_mask[j]);
			DUMPBITS(t->word.what.Bits)
				e = (unsigned int)(t->word.what.Exop);
			if (e == 0) {
				c->sub.lit = t->base;
				c->mode = LIT;
				break;
			}
			if (e & 16) {
				c->sub.copy.get = e & 15;
				c->len = t->base;
				c->mode = LENEXT;
				break;
			}
			if ((e & 64) == 0) {
				c->sub.code.need = e;
				c->sub.code.tree = t + t->base;
				break;
			}
			if (e & 32) {
				c->mode = WASH;
				break;
			}
			c->mode = BADCODE;        // invalid code 
			z->msg = (char*)"invalid literal/length code";
			r = SG_IO_ERROR;
			LEAVE
		case LENEXT:        // i: getting length extra (have base) 
			j = c->sub.copy.get;
			NEEDBITS(j)
				c->len += (unsigned int)b & inflate_mask[j];
			DUMPBITS(j)
				c->sub.code.need = c->dbits;
			c->sub.code.tree = c->dtree;
			c->mode = DIST;
		case DIST:          // i: get distance next 
			j = c->sub.code.need;
			NEEDBITS(j)
				t = c->sub.code.tree + ((unsigned int)b & inflate_mask[j]);
			DUMPBITS(t->word.what.Bits)
				e = (unsigned int)(t->word.what.Exop);
			if (e & 16) {
				c->sub.copy.get = e & 15;
				c->sub.copy.dist = t->base;
				c->mode = DISTEXT;
				break;
			}
			if ((e & 64) == 0) {
				c->sub.code.need = e;
				c->sub.code.tree = t + t->base;
				break;
			}
			c->mode = BADCODE;        // invalid code 
			z->msg = (char*)"invalid distance code";
			r = SG_IO_ERROR;
			LEAVE
		case DISTEXT:       // i: getting distance extra 
			j = c->sub.copy.get;
			NEEDBITS(j)
				c->sub.copy.dist += (unsigned int)b & inflate_mask[j];
			DUMPBITS(j)
				c->mode = COPY;
		case COPY:          // o: copying unsigned chars in window, waiting for space 
			f = q - c->sub.copy.dist;
			while (f < s->window)             // modulo window size-"while" instead
				f += s->end - s->window;        // of "if" handles invalid distances 
			while (c->len) {
				NEEDOUT
					OUTBYTE(*f++)
					if (f == s->end)
						f = s->window;
				c->len--;
			}
			c->mode = START;
			break;
		case LIT:           // o: got literal, waiting for output space 
			NEEDOUT
				OUTBYTE(c->sub.lit)
				c->mode = START;
			break;
		case WASH:          // o: got eob, possibly more output 
			if (k > 7) {
				//Assert(k < 16, "inflate_codes grabbed too many bytes")
				k -= 8;
				n++;
				p--;            // can always return one 
			}
			FLUSH
				if (s->read != s->write)
					LEAVE
					c->mode = END;
		case END:
			r = Z_STREAM_END;
			LEAVE
		case BADCODE:       // x: got error
			r = SG_IO_ERROR;
			LEAVE
		default:
			r = SG_IO_ERROR;
			LEAVE
		}
}
void inflate_codes_free(inflate_codes_statef *c, z_stream *z) {
	ZFREE(z, c);
}

void inflate_blocks_reset(inflate_blocks_statef *s, z_stream *z, unsigned long *c) {
	if (c != NULL)
		*c = s->check;
	if (s->mode == IBM_BTREE || s->mode == IBM_DTREE)
		ZFREE(z, s->sub.trees.blens);
	if (s->mode == IBM_CODES)
		inflate_codes_free(s->sub.decode.codes, z);
	s->mode = IBM_TYPE;
	s->bitk = 0;
	s->bitb = 0;
	s->read = s->write = s->window;
	if (s->checkfn != NULL)
		z->adler = s->check = (*s->checkfn)(0L, (const unsigned char *)NULL, 0);
}
inflate_blocks_statef *inflate_blocks_new(z_stream *z, check_func c, unsigned int w) {
	inflate_blocks_statef *s;

	if ((s = (inflate_blocks_statef *)ZALLOC(z, 1, sizeof(struct inflate_blocks_state))) == NULL)return s;
	if ((s->hufts = (inflate_huft *)ZALLOC(z, sizeof(inflate_huft), MANY)) == NULL) {
		ZFREE(z, s);
		return NULL;
	}
	if ((s->window = (unsigned char *)ZALLOC(z, 1, w)) == NULL) {
		ZFREE(z, s->hufts);
		ZFREE(z, s);
		return NULL;
	}
	s->end = s->window + w;
	s->checkfn = c;
	s->mode = IBM_TYPE;
	inflate_blocks_reset(s, z, NULL);
	return s;
}
int inflate_blocks(inflate_blocks_statef *s, z_stream *z, int r) {
	unsigned int t;               // temporary storage
	unsigned long b;              // bit buffer
	unsigned int k;               // bits in bit buffer
	unsigned char *p;             // input data pointer
	unsigned int n;               // unsigned chars available there
	unsigned char *q;             // output window write pointer
	unsigned int m;               // unsigned chars to end of window or read pointer 

	LOAD
	for (;;)
		switch (s->mode) {
		case IBM_TYPE:
			NEEDBITS(3)
				t = (unsigned int)b & 7;
			s->last = t & 1;
			switch (t >> 1) {
			case 0:                         // stored 
				DUMPBITS(3)
					t = k & 7;                    // go to unsigned char boundary 
				DUMPBITS(t)
					s->mode = IBM_LENS;               // get length of stored block
				break;
			case 1: {
				unsigned int bl, bd;
				const inflate_huft *tl, *td;

				inflate_trees_fixed(&bl, &bd, &tl, &td);
				s->sub.decode.codes = inflate_codes_new(bl, bd, tl, td, z);
				if (s->sub.decode.codes == NULL) {
					r = SG_NO_LOAD_MEM;
					LEAVE
				}
			}
			DUMPBITS(3)
				s->mode = IBM_CODES;
			break;
			case 2:                         // dynamic 
				DUMPBITS(3)
					s->mode = IBM_TABLE;
				break;
			case 3:                         // illegal
				DUMPBITS(3)
					s->mode = IBM_BAD;
				z->msg = (char*)"invalid block type";
				r = SG_IO_ERROR;
				LEAVE
			}
			break;
		case IBM_LENS:
			NEEDBITS(32)
				if ((((~b) >> 16) & 0xffff) != (b & 0xffff)) {
					s->mode = IBM_BAD;
					z->msg = (char*)"invalid stored block lengths";
					r = SG_IO_ERROR;
					LEAVE
				}
			s->sub.left = (unsigned int)b & 0xffff;
			b = k = 0;                      // dump bits 
			s->mode = s->sub.left ? IBM_STORED : (s->last ? IBM_DRY : IBM_TYPE);
			break;
		case IBM_STORED:
			if (n == 0)
				LEAVE
				NEEDOUT
				t = s->sub.left;
			if (t > n) t = n;
			if (t > m) t = m;
			memcpy(q, p, t);
			p += t;  n -= t;
			q += t;  m -= t;
			if ((s->sub.left -= t) != 0)
				break;
			s->mode = s->last ? IBM_DRY : IBM_TYPE;
			break;
		case IBM_TABLE:
			NEEDBITS(14)
				s->sub.trees.table = t = (unsigned int)b & 0x3fff;
			if ((t & 0x1f) > 29 || ((t >> 5) & 0x1f) > 29) {
				s->mode = IBM_BAD;
				z->msg = (char*)"too many length or distance symbols";
				r = SG_IO_ERROR;
				LEAVE
			}
			t = 258 + (t & 0x1f) + ((t >> 5) & 0x1f);
			if ((s->sub.trees.blens = (unsigned int*)ZALLOC(z, t, sizeof(unsigned int))) == NULL) {
				r = SG_NO_LOAD_MEM;
				LEAVE
			}
			DUMPBITS(14)
				s->sub.trees.index = 0;
			s->mode = IBM_BTREE;
		case IBM_BTREE:
			while (s->sub.trees.index < 4 + (s->sub.trees.table >> 10)) {
				NEEDBITS(3)
					s->sub.trees.blens[border[s->sub.trees.index++]] = (unsigned int)b & 7;
				DUMPBITS(3)
			}
			while (s->sub.trees.index < 19)
				s->sub.trees.blens[border[s->sub.trees.index++]] = 0;
			s->sub.trees.bb = 7;
			t = inflate_trees_bits(s->sub.trees.blens, &s->sub.trees.bb,
				&s->sub.trees.tb, s->hufts, z);
			if (t != SG_NO_ERROR) {
				r = t;
				if (r == SG_IO_ERROR) {
					ZFREE(z, s->sub.trees.blens);
					s->mode = IBM_BAD;
				}
				LEAVE
			}
			s->sub.trees.index = 0;
			s->mode = IBM_DTREE;
		case IBM_DTREE:
			while (t = s->sub.trees.table,
				s->sub.trees.index < 258 + (t & 0x1f) + ((t >> 5) & 0x1f)) {
				inflate_huft *h;
				unsigned int i, j, c;

				t = s->sub.trees.bb;
				NEEDBITS(t)
					h = s->sub.trees.tb + ((unsigned int)b & inflate_mask[t]);
				t = h->word.what.Bits;
				c = h->base;
				if (c < 16) {
					DUMPBITS(t)
						s->sub.trees.blens[s->sub.trees.index++] = c;
				}
				else {
					i = c == 18 ? 7 : c - 14;
					j = c == 18 ? 11 : 3;
					NEEDBITS(t + i)
						DUMPBITS(t)
						j += (unsigned int)b & inflate_mask[i];
					DUMPBITS(i)
						i = s->sub.trees.index;
					t = s->sub.trees.table;
					if (i + j > 258 + (t & 0x1f) + ((t >> 5) & 0x1f) ||
						(c == 16 && i < 1)) {
						ZFREE(z, s->sub.trees.blens);
						s->mode = IBM_BAD;
						z->msg = (char*)"invalid bit length repeat";
						r = SG_IO_ERROR;
						LEAVE
					}
					c = c == 16 ? s->sub.trees.blens[i - 1] : 0;
					do {
						s->sub.trees.blens[i++] = c;
					} while (--j);
					s->sub.trees.index = i;
				}
			}
			s->sub.trees.tb = NULL; 
			{
				unsigned int bl, bd;
				inflate_huft *tl, *td;
				inflate_codes_statef *c;

				bl = 9;         // must be <= 9 for lookahead assumptions 
				bd = 6;         // must be <= 9 for lookahead assumptions
				t = s->sub.trees.table;
				t = inflate_trees_dynamic(257 + (t & 0x1f), 1 + ((t >> 5) & 0x1f),
					s->sub.trees.blens, &bl, &bd, &tl, &td,
					s->hufts, z);
				if (t != SG_NO_ERROR) {
					if (t == (unsigned int)SG_IO_ERROR) {
						ZFREE(z, s->sub.trees.blens);
						s->mode = IBM_BAD;
					}
					r = t;
					LEAVE
				}
				if ((c = inflate_codes_new(bl, bd, tl, td, z)) == NULL) {
					r = SG_NO_LOAD_MEM;
					LEAVE
				}
				s->sub.decode.codes = c;
			}
			ZFREE(z, s->sub.trees.blens);
			s->mode = IBM_CODES;
		case IBM_CODES:
			UPDATE
				if ((r = inflate_codes(s, z, r)) != Z_STREAM_END)
					return inflate_flush(s, z, r);
			r = SG_NO_ERROR;
			inflate_codes_free(s->sub.decode.codes, z);
			LOAD
				if (!s->last) {
					s->mode = IBM_TYPE;
					break;
				}
			s->mode = IBM_DRY;
		case IBM_DRY:
			FLUSH
				if (s->read != s->write)
					LEAVE
					s->mode = IBM_DONE;
		case IBM_DONE:
			r = Z_STREAM_END;
			LEAVE
		case IBM_BAD:
			r = SG_IO_ERROR;
			LEAVE
		default:
			r = SG_IO_ERROR;
			LEAVE
		}
}
int inflate_blocks_free(inflate_blocks_statef *s, z_stream *z) {
	inflate_blocks_reset(s, z, NULL);
	ZFREE(z, s->window);
	ZFREE(z, s->hufts);
	ZFREE(z, s);
	return SG_NO_ERROR;
}

unsigned long ucrc32(unsigned long crc, const unsigned char *buf, unsigned int len) {
	if (buf == NULL) return 0L;
	crc = crc ^ 0xffffffffL;
	if (len) do { crc = crc_table[((int)crc ^ (*buf++)) & 0xff] ^ (crc >> 8); } while (--len);
	return crc ^ 0xffffffffL;
}
unsigned long adler32(unsigned long adler, const unsigned char *buf, unsigned int len) {
	unsigned long s1 = adler & 0xffff;
	unsigned long s2 = (adler >> 16) & 0xffff;
	int k;

	if (buf == NULL) return 1L;

	while (len > 0) {
		k = len < NMAX ? len : NMAX;
		len -= k;
		if (k != 0) do {
			s1 += *buf++;
			s2 += s1;
		} while (--k);
		s1 %= BASE;
		s2 %= BASE;
	}
	return (s2 << 16) | s1;
}

int inflateReset(z_stream *z) {
	if (z == NULL || z->state == NULL)
		return SG_IO_ERROR;
	z->total_in = z->total_out = 0;
	z->msg = NULL;
	z->state->mode = z->state->nowrap ? IM_BLOCKS : IM_METHOD;
	inflate_blocks_reset(z->state->blocks, z, NULL);
	return SG_NO_ERROR;
}
int inflateEnd(z_stream *z) {
	if (z == NULL || z->state == NULL || z->zfree == NULL)
		return SG_IO_ERROR;
	if (z->state->blocks != NULL)
		inflate_blocks_free(z->state->blocks, z);
	ZFREE(z, z->state);
	z->state = NULL;
	return SG_NO_ERROR;
}
int inflateInit2(z_stream *z) {
	int stream_size = sizeof(z_stream);

	int w = -15;
	if (z == NULL) return SG_IO_ERROR;
	z->msg = NULL;
	if (z->zalloc == NULL) {
		z->zalloc = (alloc_func)calloc;
		z->opaque = (void *)0;
	}
	if (z->zfree == NULL) z->zfree = free;
	if ((z->state = (struct internal_state *)
		ZALLOC(z, 1, sizeof(struct internal_state))) == NULL)
		return SG_NO_LOAD_MEM;
	z->state->blocks = NULL;

	z->state->nowrap = 0;
	if (w < 0) {
		w = -w;
		z->state->nowrap = 1;
	}
	if (w < 8 || w > 15) {
		inflateEnd(z);
		return SG_IO_ERROR;
	}
	z->state->wbits = (unsigned int)w;

	if ((z->state->blocks = inflate_blocks_new(z, z->state->nowrap ? NULL : adler32, (unsigned int)1 << w)) == NULL) {
		inflateEnd(z);
		return SG_NO_LOAD_MEM;
	}

	inflateReset(z);
	return SG_NO_ERROR;
}
int inflate(z_stream *z) {

#define IM_NEEDBYTE {if(z->avail_in==0)return r;r=f;}
#define IM_NEXTBYTE (z->avail_in--,z->total_in++,*z->next_in++)

	int r, f = SG_NO_ERROR;
	unsigned int b;

	if (z == NULL || z->state == NULL || z->next_in == NULL)
		return SG_IO_ERROR;
	r = SG_IO_ERROR;
	for (;;)
		switch (z->state->mode) {
		case IM_METHOD:
			IM_NEEDBYTE
				if (((z->state->sub.method = IM_NEXTBYTE) & 0xf) != Z_DEFLATED) {
					z->state->mode = IM_BAD;
					z->msg = (char*)"unknown compression method";
					z->state->sub.marker = 5;       // can't try inflateSync
					break;
				}
			if ((z->state->sub.method >> 4) + 8 > z->state->wbits) {
				z->state->mode = IM_BAD;
				z->msg = (char*)"invalid window size";
				z->state->sub.marker = 5;       // can't try inflateSync
				break;
			}
			z->state->mode = IM_FLAG;
		case IM_FLAG:
			IM_NEEDBYTE
				b = IM_NEXTBYTE;
			if (((z->state->sub.method << 8) + b) % 31) {
				z->state->mode = IM_BAD;
				z->msg = (char*)"incorrect header check";
				z->state->sub.marker = 5;       // can't try inflateSync 
				break;
			}
			if (!(b & PRESET_DICT)) {
				z->state->mode = IM_BLOCKS;
				break;
			}
			z->state->mode = IM_DICT4;
		case IM_DICT4:
			IM_NEEDBYTE
				z->state->sub.check.need = (unsigned long)IM_NEXTBYTE << 24;
			z->state->mode = IM_DICT3;
		case IM_DICT3:
			IM_NEEDBYTE
				z->state->sub.check.need += (unsigned long)IM_NEXTBYTE << 16;
			z->state->mode = IM_DICT2;
		case IM_DICT2:
			IM_NEEDBYTE
				z->state->sub.check.need += (unsigned long)IM_NEXTBYTE << 8;
			z->state->mode = IM_DICT1;
		case IM_DICT1:
			IM_NEEDBYTE; r;
			z->state->sub.check.need += (unsigned long)IM_NEXTBYTE;
			z->adler = z->state->sub.check.need;
			z->state->mode = IM_DICT0;
			return Z_NEED_DICT;
		case IM_DICT0:
			z->state->mode = IM_BAD;
			z->msg = (char*)"need dictionary";
			z->state->sub.marker = 0;       // can try inflateSync 
			return SG_IO_ERROR;
		case IM_BLOCKS:
			r = inflate_blocks(z->state->blocks, z, r);
			if (r == SG_IO_ERROR) {
				z->state->mode = IM_BAD;
				z->state->sub.marker = 0;       // can try inflateSync 
				break;
			}
			if (r == SG_NO_ERROR)
				r = f;
			if (r != Z_STREAM_END)
				return r;
			r = f;
			inflate_blocks_reset(z->state->blocks, z, &z->state->sub.check.was);
			if (z->state->nowrap) {
				z->state->mode = IM_DONE;
				break;
			}
			z->state->mode = IM_CHECK4;
		case IM_CHECK4:
			IM_NEEDBYTE
				z->state->sub.check.need = (unsigned long)IM_NEXTBYTE << 24;
			z->state->mode = IM_CHECK3;
		case IM_CHECK3:
			IM_NEEDBYTE
				z->state->sub.check.need += (unsigned long)IM_NEXTBYTE << 16;
			z->state->mode = IM_CHECK2;
		case IM_CHECK2:
			IM_NEEDBYTE
				z->state->sub.check.need += (unsigned long)IM_NEXTBYTE << 8;
			z->state->mode = IM_CHECK1;
		case IM_CHECK1:
			IM_NEEDBYTE
				z->state->sub.check.need += (unsigned long)IM_NEXTBYTE;

			if (z->state->sub.check.was != z->state->sub.check.need) {
				z->state->mode = IM_BAD;
				z->msg = (char*)"incorrect data check";
				z->state->sub.marker = 5;       // can't try inflateSync 
				break;
			}
			z->state->mode = IM_DONE;
		case IM_DONE:
			return Z_STREAM_END;
		case IM_BAD:
			return SG_IO_ERROR;
		default:
			return SG_IO_ERROR;
		}
}

int lufclose(LUFILE *stream) {
	if (stream == NULL) return EOF;
	CloseHandle(stream->h);
	delete stream;
	return 0;
}
long int luftell(LUFILE *stream) {
	return SetFilePointer(stream->h, 0, NULL, FILE_CURRENT) - stream->initial_offset;
}
int lufseek(LUFILE *stream, long offset, int whence) {
	if (whence == SEEK_SET) SetFilePointer(stream->h, stream->initial_offset + offset, 0, FILE_BEGIN);
	else if (whence == SEEK_CUR) SetFilePointer(stream->h, offset, NULL, FILE_CURRENT);
	else if (whence == SEEK_END) SetFilePointer(stream->h, offset, NULL, FILE_END);
	return 0;
}
size_t lufread(void *ptr, size_t size, size_t n, LUFILE *stream) {
	unsigned int toread = size*n;
	DWORD red;
	if (!ReadFile(stream->h, ptr, toread, &red, NULL)) stream->herr = true;
	return red / size;
}

int unzlocal_getByte(LUFILE *fin, int *pi) {
	unsigned char c;
	int err = (int)lufread(&c, 1, 1, fin);
	if (err == 1) {
		*pi = (int)c;
		return SG_NO_ERROR;
	}
	else {
		if (fin->herr) return SG_IO_ERROR;
		else return SG_NO_ERROR;
	}
}
int unzlocal_getShort(LUFILE *fin, unsigned long *pX) {
	unsigned long x;
	int i;
	int err;

	err = unzlocal_getByte(fin, &i);
	x = (unsigned long)i;

	if (err == SG_NO_ERROR)err = unzlocal_getByte(fin, &i);
	x += ((unsigned long)i) << 8;

	if (err == SG_NO_ERROR)*pX = x;
	else*pX = 0;
	return err;
}
int unzlocal_getLong(LUFILE *fin, unsigned long *pX) {
	unsigned long x;
	int i;
	int err;

	err = unzlocal_getByte(fin, &i);
	x = (unsigned long)i;

	if (err == SG_NO_ERROR)err = unzlocal_getByte(fin, &i);
	x += ((unsigned long)i) << 8;

	if (err == SG_NO_ERROR)err = unzlocal_getByte(fin, &i);
	x += ((unsigned long)i) << 16;

	if (err == SG_NO_ERROR)err = unzlocal_getByte(fin, &i);
	x += ((unsigned long)i) << 24;

	if (err == SG_NO_ERROR)*pX = x;
	else*pX = 0;
	return err;
}

unsigned long unzlocal_SearchCentralDir(LUFILE *fin) {
	if (lufseek(fin, 0, SEEK_END) != 0) return 0xFFFFFFFF;
	unsigned long uSizeFile = luftell(fin);

	unsigned long uMaxBack = 0xffff; // maximum size of global comment
	if (uMaxBack>uSizeFile) uMaxBack = uSizeFile;

	unsigned char *buf = (unsigned char*)malloc(BUFREADCOMMENT + 4);
	if (buf == NULL) return 0xFFFFFFFF;
	unsigned long uPosFound = 0xFFFFFFFF;

	unsigned long uBackRead = 4;
	while (uBackRead<uMaxBack) {
		unsigned long uReadSize, uReadPos;
		int i;
		if (uBackRead + BUFREADCOMMENT>uMaxBack) uBackRead = uMaxBack;
		else uBackRead += BUFREADCOMMENT;
		uReadPos = uSizeFile - uBackRead;
		uReadSize = ((BUFREADCOMMENT + 4) < (uSizeFile - uReadPos)) ? (BUFREADCOMMENT + 4) : (uSizeFile - uReadPos);
		if (lufseek(fin, uReadPos, SEEK_SET) != 0) break;
		if (lufread(buf, (unsigned int)uReadSize, 1, fin) != 1) break;
		for (i = (int)uReadSize - 3; (i--) >= 0;) {
			if (((*(buf + i)) == 0x50) && ((*(buf + i + 1)) == 0x4b) && ((*(buf + i + 2)) == 0x05) && ((*(buf + i + 3)) == 0x06)) {
				uPosFound = uReadPos + i;	break;
			}
		}
		if (uPosFound != 0) break;
	}
	if (buf) free(buf);
	return uPosFound;
}
void unzlocal_DosDateToTmuDate(unsigned long ulDosDate, tm_unz* ptm) {
	unsigned long uDate;
	uDate = (unsigned long)(ulDosDate >> 16);
	ptm->tm_mday = (unsigned int)(uDate & 0x1f);
	ptm->tm_mon = (unsigned int)((((uDate) & 0x1E0) / 0x20) - 1);
	ptm->tm_year = (unsigned int)(((uDate & 0x0FE00) / 0x0200) + 1980);

	ptm->tm_hour = (unsigned int)((ulDosDate & 0xF800) / 0x800);
	ptm->tm_min = (unsigned int)((ulDosDate & 0x7E0) / 0x20);
	ptm->tm_sec = (unsigned int)(2 * (ulDosDate & 0x1f));
}
int unzlocal_GetCurrentFileInfoInternal(unzFile file, unz_file_info *pfile_info, unsigned long *pfile_info_internal, char *szFileName, unsigned long fileNameBufferSize, void *extraField, unsigned long extraFieldBufferSize, char *szComment, unsigned long commentBufferSize) {
	unz_s* s;
	unz_file_info file_info;
	unsigned long file_info_internal;
	int err = SG_NO_ERROR;
	unsigned long uMagic;
	long lSeek = 0;

	if (file == NULL)return SG_IO_ERROR;
	s = (unz_s*)file;
	if (lufseek(s->file, s->pos_in_central_dir + s->byte_before_the_zipfile, SEEK_SET) != 0)err = SG_IO_ERROR;

	if (err == SG_NO_ERROR)
		if (unzlocal_getLong(s->file, &uMagic) != SG_NO_ERROR)err = SG_IO_ERROR;
		else if (uMagic != 0x02014b50)err = SG_IO_ERROR;
	if (unzlocal_getShort(s->file, &file_info.version) != SG_NO_ERROR)err = SG_IO_ERROR;
	if (unzlocal_getShort(s->file, &file_info.version_needed) != SG_NO_ERROR)err = SG_IO_ERROR;
	if (unzlocal_getShort(s->file, &file_info.flag) != SG_NO_ERROR)err = SG_IO_ERROR;
	if (unzlocal_getShort(s->file, &file_info.compression_method) != SG_NO_ERROR)err = SG_IO_ERROR;
	if (unzlocal_getLong(s->file, &file_info.dosDate) != SG_NO_ERROR)err = SG_IO_ERROR;
	unzlocal_DosDateToTmuDate(file_info.dosDate, &file_info.tmu_date);
	if (unzlocal_getLong(s->file, &file_info.crc) != SG_NO_ERROR)err = SG_IO_ERROR;
	if (unzlocal_getLong(s->file, &file_info.compressed_size) != SG_NO_ERROR)err = SG_IO_ERROR;
	if (unzlocal_getLong(s->file, &file_info.uncompressed_size) != SG_NO_ERROR)err = SG_IO_ERROR;
	if (unzlocal_getShort(s->file, &file_info.size_filename) != SG_NO_ERROR)err = SG_IO_ERROR;
	if (unzlocal_getShort(s->file, &file_info.size_file_extra) != SG_NO_ERROR)err = SG_IO_ERROR;
	if (unzlocal_getShort(s->file, &file_info.size_file_comment) != SG_NO_ERROR)err = SG_IO_ERROR;
	if (unzlocal_getShort(s->file, &file_info.disk_num_start) != SG_NO_ERROR)err = SG_IO_ERROR;
	if (unzlocal_getShort(s->file, &file_info.internal_fa) != SG_NO_ERROR)err = SG_IO_ERROR;
	if (unzlocal_getLong(s->file, &file_info.external_fa) != SG_NO_ERROR)err = SG_IO_ERROR;
	if (unzlocal_getLong(s->file, &file_info_internal) != SG_NO_ERROR)err = SG_IO_ERROR;
	lSeek += file_info.size_filename;
	if ((err == SG_NO_ERROR) && (szFileName != NULL)) {
		unsigned long uSizeRead;
		if (file_info.size_filename<fileNameBufferSize) {
			*(szFileName + file_info.size_filename) = '\0';
			uSizeRead = file_info.size_filename;
		}
		else uSizeRead = fileNameBufferSize;

		if ((file_info.size_filename>0) && (fileNameBufferSize>0))
			if (lufread(szFileName, (unsigned int)uSizeRead, 1, s->file) != 1)err = SG_IO_ERROR;
		lSeek -= uSizeRead;
	}

	if ((err == SG_NO_ERROR) && (extraField != NULL)) {
		unsigned long uSizeRead;
		if (file_info.size_file_extra<extraFieldBufferSize)uSizeRead = file_info.size_file_extra;
		else uSizeRead = extraFieldBufferSize;

		if (lSeek != 0)
			if (lufseek(s->file, lSeek, SEEK_CUR) == 0)lSeek = 0;
			else err = SG_IO_ERROR;
		if ((file_info.size_file_extra>0) && (extraFieldBufferSize>0))
			if (lufread(extraField, (unsigned int)uSizeRead, 1, s->file) != 1)err = SG_IO_ERROR;
		lSeek += file_info.size_file_extra - uSizeRead;
	}
	else lSeek += file_info.size_file_extra;


	if ((err == SG_NO_ERROR) && (szComment != NULL)) {
		unsigned long uSizeRead;
		if (file_info.size_file_comment<commentBufferSize) {
			*(szComment + file_info.size_file_comment) = '\0';
			uSizeRead = file_info.size_file_comment;
		}
		else uSizeRead = commentBufferSize;

		if (lSeek != 0)
			if (lufseek(s->file, lSeek, SEEK_CUR) != 0)err = SG_IO_ERROR;
		if ((file_info.size_file_comment>0) && (commentBufferSize>0))
			if (lufread(szComment, (unsigned int)uSizeRead, 1, s->file) != 1)err = SG_IO_ERROR;
	}
	if ((err == SG_NO_ERROR) && (pfile_info != NULL))*pfile_info = file_info;
	if ((err == SG_NO_ERROR) && (pfile_info_internal != NULL))*pfile_info_internal = file_info_internal;
	return err;
}
int unzGetCurrentFileInfo(unzFile file, unz_file_info *pfile_info, char *szFileName, unsigned long fileNameBufferSize, void *extraField, unsigned long extraFieldBufferSize, char *szComment, unsigned long commentBufferSize) {
	return unzlocal_GetCurrentFileInfoInternal(file, pfile_info, NULL, szFileName, fileNameBufferSize,
		extraField, extraFieldBufferSize, szComment, commentBufferSize);
}
int unzGoToFirstFile(unzFile file) {
	int err;
	unz_s* s;
	if (file == NULL) return SG_IO_ERROR;
	s = (unz_s*)file;
	s->pos_in_central_dir = s->offset_central_dir;
	s->num_file = 0;
	err = unzlocal_GetCurrentFileInfoInternal(file, &s->cur_file_info,
		&s->cur_file_info_internal,
		NULL, 0, NULL, 0, NULL, 0);
	s->current_file_ok = (err == SG_NO_ERROR);
	return err;
}
int unzGoToNextFile(unzFile file) {
	unz_s* s;
	int err;

	if (file == NULL)return SG_IO_ERROR;
	s = (unz_s*)file;
	if (!s->current_file_ok)return SG_IO_ERROR;
	if (s->num_file + 1 == s->gi.number_entry)return SG_IO_ERROR;

	s->pos_in_central_dir += SIZECENTRALDIRITEM + s->cur_file_info.size_filename +
		s->cur_file_info.size_file_extra + s->cur_file_info.size_file_comment;
	s->num_file++;
	err = unzlocal_GetCurrentFileInfoInternal(file, &s->cur_file_info,
		&s->cur_file_info_internal,
		NULL, 0, NULL, 0, NULL, 0);
	s->current_file_ok = (err == SG_NO_ERROR);
	return err;
}
int unzLocateFile(unzFile file, const char *szFileName) {
	unz_s* s;
	int err;

	unsigned long num_fileSaved;
	unsigned long pos_in_central_dirSaved;

	if (file == NULL)return SG_IO_ERROR;
	if (strlen(szFileName) >= UNZ_MAXFILENAMEINZIP)return SG_IO_ERROR;

	s = (unz_s*)file;
	if (!s->current_file_ok)
		return SG_IO_ERROR;

	num_fileSaved = s->num_file;
	pos_in_central_dirSaved = s->pos_in_central_dir;
	err = unzGoToFirstFile(file);

	while (err == SG_NO_ERROR) {
		char szCurrentFileName[UNZ_MAXFILENAMEINZIP + 1];
		unzGetCurrentFileInfo(file, NULL,
			szCurrentFileName, sizeof(szCurrentFileName) - 1,
			NULL, 0, NULL, 0);
		if (strcmp(szCurrentFileName, szFileName) == 0)
			return SG_NO_ERROR;
		err = unzGoToNextFile(file);
	}

	s->num_file = num_fileSaved;
	s->pos_in_central_dir = pos_in_central_dirSaved;
	return err;
}
int unzlocal_CheckCurrentFileCoherencyHeader(unz_s *s, unsigned int *piSizeVar, unsigned long *poffset_local_extrafield, unsigned int  *psize_local_extrafield) {
	unsigned long uMagic, uData, uFlags;
	unsigned long size_filename;
	unsigned long size_extra_field;
	int err = SG_NO_ERROR;

	*piSizeVar = 0;
	*poffset_local_extrafield = 0;
	*psize_local_extrafield = 0;

	if (lufseek(s->file, s->cur_file_info_internal + s->byte_before_the_zipfile, SEEK_SET) != 0)
		return SG_IO_ERROR;

	if (err == SG_NO_ERROR)
		if (unzlocal_getLong(s->file, &uMagic) != SG_NO_ERROR)err = SG_IO_ERROR;
		else if (uMagic != 0x04034b50)err = SG_IO_ERROR;

	if (unzlocal_getShort(s->file, &uData) != SG_NO_ERROR)err = SG_IO_ERROR;
	if (unzlocal_getShort(s->file, &uFlags) != SG_NO_ERROR)err = SG_IO_ERROR;
	if (unzlocal_getShort(s->file, &uData) != SG_NO_ERROR)err = SG_IO_ERROR;
	else if ((err == SG_NO_ERROR) && (uData != s->cur_file_info.compression_method))err = SG_IO_ERROR;
	if ((err == SG_NO_ERROR) && (s->cur_file_info.compression_method != 0) && (s->cur_file_info.compression_method != Z_DEFLATED))err = SG_IO_ERROR;
	if (unzlocal_getLong(s->file, &uData) != SG_NO_ERROR)err = SG_IO_ERROR;
	if (unzlocal_getLong(s->file, &uData) != SG_NO_ERROR)err = SG_IO_ERROR;
	else if ((err == SG_NO_ERROR) && (uData != s->cur_file_info.crc) && ((uFlags & 8) == 0))err = SG_IO_ERROR;
	if (unzlocal_getLong(s->file, &uData) != SG_NO_ERROR)err = SG_IO_ERROR;
	else if ((err == SG_NO_ERROR) && (uData != s->cur_file_info.compressed_size) && ((uFlags & 8) == 0))err = SG_IO_ERROR;
	if (unzlocal_getLong(s->file, &uData) != SG_NO_ERROR)err = SG_IO_ERROR;
	else if ((err == SG_NO_ERROR) && (uData != s->cur_file_info.uncompressed_size) && ((uFlags & 8) == 0))err = SG_IO_ERROR;
	if (unzlocal_getShort(s->file, &size_filename) != SG_NO_ERROR)err = SG_IO_ERROR;
	else if ((err == SG_NO_ERROR) && (size_filename != s->cur_file_info.size_filename))err = SG_IO_ERROR;

	*piSizeVar += (unsigned int)size_filename;

	if (unzlocal_getShort(s->file, &size_extra_field) != SG_NO_ERROR)
		err = SG_IO_ERROR;
	*poffset_local_extrafield = s->cur_file_info_internal +
		SIZEZIPLOCALHEADER + size_filename;
	*psize_local_extrafield = (unsigned int)size_extra_field;
	*piSizeVar += (unsigned int)size_extra_field;

	return err;
}
int unzCloseCurrentFile(unzFile file) {
	int err = SG_NO_ERROR;

	unz_s* s;
	file_in_zip_read_info_s* pfile_in_zip_read_info;
	if (file == NULL)
		return SG_IO_ERROR;
	s = (unz_s*)file;
	pfile_in_zip_read_info = s->pfile_in_zip_read;

	if (pfile_in_zip_read_info == NULL)return SG_IO_ERROR;
	if (pfile_in_zip_read_info->rest_read_uncompressed == 0) {
		if (pfile_in_zip_read_info->crc32 != pfile_in_zip_read_info->crc32_wait)
			err = SG_IO_ERROR;
	}
	if (pfile_in_zip_read_info->read_buffer != 0) {
		void *buf = pfile_in_zip_read_info->read_buffer;
		free(buf);
		pfile_in_zip_read_info->read_buffer = 0;
	}
	pfile_in_zip_read_info->read_buffer = NULL;
	if (pfile_in_zip_read_info->stream_initialised)
		inflateEnd(&pfile_in_zip_read_info->stream);

	pfile_in_zip_read_info->stream_initialised = 0;
	if (pfile_in_zip_read_info != 0) free(pfile_in_zip_read_info); // unused pfile_in_zip_read_info=0;

	s->pfile_in_zip_read = NULL;

	return err;
}
int unzOpenCurrentFile(unzFile file) {
	int err;
	int Store;
	unsigned int iSizeVar;
	unz_s* s;
	file_in_zip_read_info_s* pfile_in_zip_read_info;
	unsigned long offset_local_extrafield;  // offset of the local extra field
	unsigned int  size_local_extrafield;    // size of the local extra field

	if (file == NULL)return SG_IO_ERROR;
	s = (unz_s*)file;
	if (!s->current_file_ok)return SG_IO_ERROR;

	if (s->pfile_in_zip_read != NULL)unzCloseCurrentFile(file);
	if (unzlocal_CheckCurrentFileCoherencyHeader(s, &iSizeVar, &offset_local_extrafield, &size_local_extrafield) != SG_NO_ERROR)return SG_IO_ERROR;

	pfile_in_zip_read_info = (file_in_zip_read_info_s*)malloc(sizeof(file_in_zip_read_info_s));
	if (pfile_in_zip_read_info == NULL)return SG_IO_ERROR;

	pfile_in_zip_read_info->read_buffer = (char*)malloc(UNZ_BUFSIZE);
	pfile_in_zip_read_info->offset_local_extrafield = offset_local_extrafield;
	pfile_in_zip_read_info->size_local_extrafield = size_local_extrafield;
	pfile_in_zip_read_info->pos_local_extrafield = 0;

	if (pfile_in_zip_read_info->read_buffer == NULL) {
		if (pfile_in_zip_read_info != 0) free(pfile_in_zip_read_info); //unused pfile_in_zip_read_info=0;
		return SG_IO_ERROR;
	}
	pfile_in_zip_read_info->stream_initialised = 0;
	Store = s->cur_file_info.compression_method == 0;

	pfile_in_zip_read_info->crc32_wait = s->cur_file_info.crc;
	pfile_in_zip_read_info->crc32 = 0;
	pfile_in_zip_read_info->compression_method = s->cur_file_info.compression_method;
	pfile_in_zip_read_info->file = s->file;
	pfile_in_zip_read_info->byte_before_the_zipfile = s->byte_before_the_zipfile;

	pfile_in_zip_read_info->stream.total_out = 0;

	if (!Store) {
		pfile_in_zip_read_info->stream.zalloc = (alloc_func)0;
		pfile_in_zip_read_info->stream.zfree = (free_func)0;
		pfile_in_zip_read_info->stream.opaque = (void *)0;

		err = inflateInit2(&pfile_in_zip_read_info->stream);
		if (err == SG_NO_ERROR)
			pfile_in_zip_read_info->stream_initialised = 1;
	}
	pfile_in_zip_read_info->rest_read_compressed = s->cur_file_info.compressed_size;
	pfile_in_zip_read_info->rest_read_uncompressed = s->cur_file_info.uncompressed_size;
	bool extlochead = (s->cur_file_info.flag & 8) != 0;

	pfile_in_zip_read_info->pos_in_zipfile = s->cur_file_info_internal + SIZEZIPLOCALHEADER + iSizeVar;
	pfile_in_zip_read_info->stream.avail_in = (unsigned int)0;
	s->pfile_in_zip_read = pfile_in_zip_read_info;

	return SG_NO_ERROR;
}
int unzReadCurrentFile(unzFile file, void * buf, unsigned len, bool *reached_eof) {
	int err = SG_NO_ERROR;
	unsigned int iRead = 0;
	if (reached_eof != 0) *reached_eof = false;

	unz_s *s = (unz_s*)file;
	if (s == NULL) return SG_IO_ERROR;

	file_in_zip_read_info_s* pfile_in_zip_read_info = s->pfile_in_zip_read;
	if (pfile_in_zip_read_info == NULL) return SG_IO_ERROR;
	if ((pfile_in_zip_read_info->read_buffer == NULL)) return SG_IO_ERROR;
	if (len == 0) return 0;

	pfile_in_zip_read_info->stream.next_out = (unsigned char*)buf;
	pfile_in_zip_read_info->stream.avail_out = (unsigned int)len;

	if (len>pfile_in_zip_read_info->rest_read_uncompressed) {
		pfile_in_zip_read_info->stream.avail_out = (unsigned int)pfile_in_zip_read_info->rest_read_uncompressed;
	}
	while (pfile_in_zip_read_info->stream.avail_out>0) {
		if ((pfile_in_zip_read_info->stream.avail_in == 0) && (pfile_in_zip_read_info->rest_read_compressed>0)) {
			unsigned int uReadThis = UNZ_BUFSIZE;
			if (pfile_in_zip_read_info->rest_read_compressed<uReadThis) uReadThis = (unsigned int)pfile_in_zip_read_info->rest_read_compressed;
			if (uReadThis == 0) { if (reached_eof != 0) *reached_eof = true; return SG_NO_ERROR; }
			if (lufseek(pfile_in_zip_read_info->file, pfile_in_zip_read_info->pos_in_zipfile + pfile_in_zip_read_info->byte_before_the_zipfile, SEEK_SET) != 0) return SG_IO_ERROR;
			if (lufread(pfile_in_zip_read_info->read_buffer, uReadThis, 1, pfile_in_zip_read_info->file) != 1) return SG_IO_ERROR;
			pfile_in_zip_read_info->pos_in_zipfile += uReadThis;
			pfile_in_zip_read_info->rest_read_compressed -= uReadThis;
			pfile_in_zip_read_info->stream.next_in = (unsigned char*)pfile_in_zip_read_info->read_buffer;
			pfile_in_zip_read_info->stream.avail_in = (unsigned int)uReadThis;
		}

		unsigned int uDoEncHead = 0;
		if (uDoEncHead>pfile_in_zip_read_info->stream.avail_in) uDoEncHead = pfile_in_zip_read_info->stream.avail_in;
		if (uDoEncHead>0) {
			char bufcrc = pfile_in_zip_read_info->stream.next_in[uDoEncHead - 1];
			pfile_in_zip_read_info->rest_read_uncompressed -= uDoEncHead;
			pfile_in_zip_read_info->stream.avail_in -= uDoEncHead;
			pfile_in_zip_read_info->stream.next_in += uDoEncHead;
		}
		if (pfile_in_zip_read_info->compression_method == 0) {
			unsigned int uDoCopy, i;
			if (pfile_in_zip_read_info->stream.avail_out < pfile_in_zip_read_info->stream.avail_in) {
				uDoCopy = pfile_in_zip_read_info->stream.avail_out;
			}
			else {
				uDoCopy = pfile_in_zip_read_info->stream.avail_in;
			}
			for (i = 0; i<uDoCopy; i++) *(pfile_in_zip_read_info->stream.next_out + i) = *(pfile_in_zip_read_info->stream.next_in + i);
			pfile_in_zip_read_info->crc32 = ucrc32(pfile_in_zip_read_info->crc32, pfile_in_zip_read_info->stream.next_out, uDoCopy);
			pfile_in_zip_read_info->rest_read_uncompressed -= uDoCopy;
			pfile_in_zip_read_info->stream.avail_in -= uDoCopy;
			pfile_in_zip_read_info->stream.avail_out -= uDoCopy;
			pfile_in_zip_read_info->stream.next_out += uDoCopy;
			pfile_in_zip_read_info->stream.next_in += uDoCopy;
			pfile_in_zip_read_info->stream.total_out += uDoCopy;
			iRead += uDoCopy;
			if (pfile_in_zip_read_info->rest_read_uncompressed == 0) { if (reached_eof != 0) *reached_eof = true; }
		}
		else {
			unsigned long uTotalOutBefore, uTotalOutAfter;
			const unsigned char *bufBefore;
			unsigned long uOutThis;
			uTotalOutBefore = pfile_in_zip_read_info->stream.total_out;
			bufBefore = pfile_in_zip_read_info->stream.next_out;
			err = inflate(&pfile_in_zip_read_info->stream);
			uTotalOutAfter = pfile_in_zip_read_info->stream.total_out;
			uOutThis = uTotalOutAfter - uTotalOutBefore;
			pfile_in_zip_read_info->crc32 = ucrc32(pfile_in_zip_read_info->crc32, bufBefore, (unsigned int)(uOutThis));
			pfile_in_zip_read_info->rest_read_uncompressed -= uOutThis;
			iRead += (unsigned int)(uTotalOutAfter - uTotalOutBefore);
			if (err == Z_STREAM_END || pfile_in_zip_read_info->rest_read_uncompressed == 0) {
				if (reached_eof != 0) *reached_eof = true;
				return iRead;
			}
			if (err != SG_NO_ERROR) break;
		}
	}

	if (err == SG_NO_ERROR) return iRead;
	return err;
}
int unzClose(unzFile file) {
	unz_s* s;
	if (file == NULL)
		return SG_IO_ERROR;
	s = (unz_s*)file;

	if (s->pfile_in_zip_read != NULL)unzCloseCurrentFile(file);

	lufclose(s->file);
	if (s) free(s);
	return SG_NO_ERROR;
}

class TUnzip {
public:
	TUnzip() : uf(0), unzbuf(0), currentfile(-1) {}
	~TUnzip() {
		if (unzbuf != 0) delete[] unzbuf;
		unzbuf = 0;
	}

	unzFile uf;
	char *unzbuf;
	int currentfile;

	int Open(void *z) {
		HANDLE h = CreateFile((const TCHAR*)z, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (h == INVALID_HANDLE_VALUE)return NULL;

		DWORD res = SetFilePointer(h, 0, 0, FILE_CURRENT);

		LUFILE *lf = new LUFILE();
		lf->h = h;
		lf->herr = false;
		lf->initial_offset = SetFilePointer(h, 0, NULL, FILE_CURRENT);

		int err = SG_NO_ERROR;
		unz_s us;
		unsigned long central_pos, uL;
		central_pos = unzlocal_SearchCentralDir(lf);
		if (central_pos == 0xFFFFFFFF) err = SG_IO_ERROR;
		if (lufseek(lf, central_pos, SEEK_SET) != 0) err = SG_IO_ERROR;
		if (unzlocal_getLong(lf, &uL) != SG_NO_ERROR) err = SG_IO_ERROR;
		unsigned long number_disk;
		if (unzlocal_getShort(lf, &number_disk) != SG_NO_ERROR) err = SG_IO_ERROR;
		unsigned long number_disk_with_CD;
		if (unzlocal_getShort(lf, &number_disk_with_CD) != SG_NO_ERROR) err = SG_IO_ERROR;
		if (unzlocal_getShort(lf, &us.gi.number_entry) != SG_NO_ERROR) err = SG_IO_ERROR;
		unsigned long number_entry_CD;
		if (unzlocal_getShort(lf, &number_entry_CD) != SG_NO_ERROR) err = SG_IO_ERROR;
		if ((number_entry_CD != us.gi.number_entry) || (number_disk_with_CD != 0) || (number_disk != 0)) err = SG_IO_ERROR;
		if (unzlocal_getLong(lf, &us.size_central_dir) != SG_NO_ERROR) err = SG_IO_ERROR;
		if (unzlocal_getLong(lf, &us.offset_central_dir) != SG_NO_ERROR) err = SG_IO_ERROR;
		if (unzlocal_getShort(lf, &us.gi.size_comment) != SG_NO_ERROR) err = SG_IO_ERROR;
		if ((central_pos + lf->initial_offset<us.offset_central_dir + us.size_central_dir) && (err == SG_NO_ERROR)) err = SG_IO_ERROR;
		if (err != SG_NO_ERROR) { lufclose(lf); return NULL; }

		us.file = lf;
		us.byte_before_the_zipfile = central_pos + lf->initial_offset - (us.offset_central_dir + us.size_central_dir);
		us.central_pos = central_pos;
		us.pfile_in_zip_read = NULL;
		lf->initial_offset = 0;

		unz_s *s = (unz_s*)malloc(sizeof(unz_s));
		*s = us;
		unzGoToFirstFile((unzFile)s);
		uf = (unzFile)s;
		if (uf == 0) return SG_OBJECT_NOT_FOUND;
		return SG_NO_ERROR;
	}
	int Get(int index, ZIPENTRY *ze) {
		if (index<-1 || index >= (int)uf->gi.number_entry) return SG_IO_ERROR;
		if (index == -1) {
			ze->index = uf->gi.number_entry;
			ze->name[0] = 0;
			ze->attr = 0;
			ze->atime.dwLowDateTime = 0; ze->atime.dwHighDateTime = 0;
			ze->ctime.dwLowDateTime = 0; ze->ctime.dwHighDateTime = 0;
			ze->mtime.dwLowDateTime = 0; ze->mtime.dwHighDateTime = 0;
			ze->comp_size = 0;
			ze->unc_size = 0;
			return SG_NO_ERROR;
		}
		if (index<(int)uf->num_file) unzGoToFirstFile(uf);
		while ((int)uf->num_file<index) unzGoToNextFile(uf);
		unz_file_info ufi; char fn[MAX_PATH];
		unzGetCurrentFileInfo(uf, &ufi, fn, MAX_PATH, NULL, 0, NULL, 0);
		unsigned int extralen, iSizeVar; unsigned long offset;
		int res = unzlocal_CheckCurrentFileCoherencyHeader(uf, &iSizeVar, &offset, &extralen);
		if (res != SG_NO_ERROR) return SG_IO_ERROR;
		if (lufseek(uf->file, offset, SEEK_SET) != 0) return SG_IO_ERROR;
		unsigned char *extra = new unsigned char[extralen];
		if (lufread(extra, 1, (unsigned int)extralen, uf->file) != extralen) {
			delete[] extra;
			return SG_IO_ERROR;
		}
		ze->index = uf->num_file;
		TCHAR tfn[MAX_PATH];
		_strcpy(tfn, _widen(fn));
		const TCHAR *sfn = tfn;
		for (;;) {
			if (sfn[0] != 0 && sfn[1] == ':') { sfn += 2; continue; }
			if (sfn[0] == '\\') { sfn++; continue; }
			if (sfn[0] == '/') { sfn++; continue; }
			const TCHAR *c;
			c = _tcsstr(sfn, _T("\\..\\")); if (c != 0) { sfn = c + 4; continue; }
			c = _tcsstr(sfn, _T("\\../")); if (c != 0) { sfn = c + 4; continue; }
			c = _tcsstr(sfn, _T("/../")); if (c != 0) { sfn = c + 4; continue; }
			c = _tcsstr(sfn, _T("/..\\")); if (c != 0) { sfn = c + 4; continue; }
			break;
		}
		_tcscpy(ze->name, sfn);

		unsigned long a = ufi.external_fa;
		bool isdir = (a & 0x40000000) != 0;
		bool readonly = (a & 0x00800000) == 0;
		bool hidden = false, system = false, archive = true;
		int host = ufi.version >> 8;
		if (host == 0 || host == 7 || host == 11 || host == 14) {
			readonly = (a & 0x00000001) != 0;
			hidden = (a & 0x00000002) != 0;
			system = (a & 0x00000004) != 0;
			isdir = (a & 0x00000010) != 0;
			archive = (a & 0x00000020) != 0;
		}
		ze->attr = 0;
		if (isdir) ze->attr |= FILE_ATTRIBUTE_DIRECTORY;
		if (archive) ze->attr |= FILE_ATTRIBUTE_ARCHIVE;
		if (hidden) ze->attr |= FILE_ATTRIBUTE_HIDDEN;
		if (readonly) ze->attr |= FILE_ATTRIBUTE_READONLY;
		if (system) ze->attr |= FILE_ATTRIBUTE_SYSTEM;
		ze->comp_size = ufi.compressed_size;
		ze->unc_size = ufi.uncompressed_size;

		WORD dostime = (WORD)(ufi.dosDate & 0xFFFF);
		WORD dosdate = (WORD)((ufi.dosDate >> 16) & 0xFFFF);
		FILETIME ftd = dosdatetimeFiletime(dosdate, dostime);
		FILETIME ft; LocalFileTimeToFileTime(&ftd, &ft);
		ze->atime = ft; ze->ctime = ft; ze->mtime = ft;

		unsigned int epos = 0;
		while (epos + 4<extralen) {
			char etype[3]; etype[0] = extra[epos + 0]; etype[1] = extra[epos + 1]; etype[2] = 0;
			int size = extra[epos + 2];
			if (strcmp(etype, "UT") != 0) { epos += 4 + size; continue; }
			int flags = extra[epos + 4];
			bool hasmtime = (flags & 1) != 0;
			bool hasatime = (flags & 2) != 0;
			bool hasctime = (flags & 4) != 0;
			epos += 5;
			if (hasmtime) {
				long long mtime = ((extra[epos + 0]) << 0) | ((extra[epos + 1]) << 8) | ((extra[epos + 2]) << 16) | ((extra[epos + 3]) << 24);
				epos += 4;
				ze->mtime = intFiletime(mtime);
			}
			if (hasatime) {
				long long atime = ((extra[epos + 0]) << 0) | ((extra[epos + 1]) << 8) | ((extra[epos + 2]) << 16) | ((extra[epos + 3]) << 24);
				epos += 4;
				ze->atime = intFiletime(atime);
			}
			if (hasctime) {
				long long ctime = ((extra[epos + 0]) << 0) | ((extra[epos + 1]) << 8) | ((extra[epos + 2]) << 16) | ((extra[epos + 3]) << 24);
				epos += 4;
				ze->ctime = intFiletime(ctime);
			}
			break;
		}

		if (extra != 0) delete[] extra;
		return SG_NO_ERROR;
	}
	int Find(SGWINSTR tname, int *index, ZIPENTRY *ze) {
		char name[MAX_PATH];
		_strcpy(name, _shorten(tname));
		int res = unzLocateFile(uf, name);
		if (res != SG_NO_ERROR) {
			if (index != 0) *index = -1;
			if (ze != NULL) { ZeroMemory(ze, sizeof(ZIPENTRY)); ze->index = -1; }
			return SG_OBJECT_NOT_FOUND;
		}
		if (currentfile != -1) unzCloseCurrentFile(uf); currentfile = -1;
		int i = (int)uf->num_file;
		if (index != NULL) *index = i;
		if (ze != NULL) {
			int zres = Get(i, ze);
			if (zres != SG_NO_ERROR) return zres;
		}
		return SG_NO_ERROR;
	}
	int Unzip(int index, void *dst, unsigned int len, DWORD flags) {
		if (flags == ZIP_MEMORY) {
			if (index != currentfile) {
				if (currentfile != -1) unzCloseCurrentFile(uf); currentfile = -1;
				if (index >= (int)uf->gi.number_entry) return SG_IO_ERROR;
				if (index<(int)uf->num_file) unzGoToFirstFile(uf);
				while ((int)uf->num_file<index) unzGoToNextFile(uf);
				unzOpenCurrentFile(uf);
				currentfile = index;
			}
			bool reached_eof;
			int res = unzReadCurrentFile(uf, dst, len, &reached_eof);
			if (res <= 0) { unzCloseCurrentFile(uf); currentfile = -1; }
			if (reached_eof) return SG_NO_ERROR;
			if (res>0) return SG_IO_ERROR;
			return SG_IO_ERROR;
		}
		if (flags == ZIP_FILENAME) {
			if (currentfile != -1) unzCloseCurrentFile(uf); currentfile = -1;
			if (index >= (int)uf->gi.number_entry) return SG_IO_ERROR;
			if (index < (int)uf->num_file) unzGoToFirstFile(uf);
			while ((int)uf->num_file < index) unzGoToNextFile(uf);
			ZIPENTRY ze;
			Get(index, &ze);
			if ((ze.attr&FILE_ATTRIBUTE_DIRECTORY) != 0) {
				makePath(_shorten((LPWSTR)dst));
			}
			HANDLE h;
			const TCHAR *ufn = (const TCHAR*)dst;
			const TCHAR *name = ufn; const TCHAR *c = name; while (*c != 0) { if (*c == '/' || *c == '\\') name = c + 1; c++; }
			TCHAR dir[MAX_PATH]; _tcscpy(dir, ufn); if (name == ufn) *dir = 0; else dir[name - ufn] = 0;
			TCHAR fn[MAX_PATH];
			bool isabsolute = (dir[0] == '/' || dir[0] == '\\' || (dir[0] != 0 && dir[1] == ':'));
			wsprintf(fn, _T("%s%s"), dir, name);
			makePath(_shorten((LPWSTR)dir));

			h = CreateFile(fn, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, ze.attr, NULL);
			if (h == INVALID_HANDLE_VALUE) return SG_OBJECT_NOT_FOUND;
			unzOpenCurrentFile(uf);
			if (unzbuf == 0) unzbuf = new char[16384]; DWORD haderr = 0;

			for (; haderr == 0;) {
				bool reached_eof;
				int res = unzReadCurrentFile(uf, unzbuf, 16384, &reached_eof);
				if (res < 0) { haderr = SG_IO_ERROR; break; }
				if (res > 0) { DWORD writ; BOOL bres = WriteFile(h, unzbuf, res, &writ, NULL); if (!bres) { haderr = SG_IO_ERROR; break; } }
				if (reached_eof) break;
				if (res == 0) { haderr = SG_IO_ERROR; break; }
			}
			if (!haderr) SetFileTime(h, &ze.ctime, &ze.atime, &ze.mtime);
			CloseHandle(h);
			unzCloseCurrentFile(uf);
			if (haderr != 0) return haderr;
			return SG_NO_ERROR;
		}
	}
	int Close() {
		if (currentfile != -1) unzCloseCurrentFile(uf);
		currentfile = -1;
		if (uf != 0) unzClose(uf);
		uf = 0;
		return SG_NO_ERROR;
	}
};





vector<HANDLE> zipList, unzipList;
HANDLE createZip(SGstring file) {
	TZip *zip = new TZip();
	zip->Create(_widen(file));
	zipList.push_back(zip);
	return zip;
}
void zipFile(HANDLE h, SGstring src, SGstring dst) {
	TZip *zip = (TZip*)h;
	zip->Add(_widen(dst), _widen(src), 0, ZIP_FILENAME);
}
void zipMemory(HANDLE h, void *src, int len, SGstring dst) {
	TZip *zip = (TZip*)h;
	zip->Add(_widen(dst), src, len, ZIP_MEMORY);
}
void zipFolder(HANDLE h, SGstring dst) {
	TZip *zip = (TZip*)h;
	zip->Add(_widen(dst), 0, 0, ZIP_FOLDER);
}
HANDLE createUnzip(SGstring file) {
	if (fileExist(file)) {
		TUnzip *unzip = new TUnzip();
		unzip->Open(_widen(file));
		unzipList.push_back(unzip);
		return unzip;
	}
	else return NULL;
}
void readUnzip(HANDLE h, int idx, SGstring path) {
	ZIPENTRY ze;
	ze.index = 0;
	*ze.name = 0;
	ze.unc_size = 0;
	TUnzip *unzip = (TUnzip*)h;
	unzip->Get(idx, &ze);
	strcpy(path, _shorten(ze.name));
}
void unzipFile(HANDLE h, SGstring src, SGstring dst) {
	int index;
	ZIPENTRY ze;
	TUnzip *unzip = (TUnzip*)h;
	unzip->Find(_widen(src), &index, &ze);
	unzip->Unzip(index, _widen(dst), 0, ZIP_FILENAME);
}
void unzipMemory(HANDLE h, SGstring src, void *dst, int len) {
	int index;
	ZIPENTRY ze;
	TUnzip *unzip = (TUnzip*)h;
	unzip->Find(_widen(src), &index, &ze);
	unzip->Unzip(index, dst, len, ZIP_MEMORY);
}
void zipFinish(HANDLE h) {
	for (auto &handle : zipList) {
		if (handle == h) {
			handle = zipList.back();
			zipList.pop_back();
			TZip *zip = (TZip*)h;
			zip->Close();
			delete zip;
			break;
		}
	}
	for (auto &handle : unzipList) {
		if (handle == h) {
			handle = unzipList.back();
			unzipList.pop_back();
			TUnzip *unzip = (TUnzip*)h;
			unzip->Close();
			delete unzip;
			break;
		}
	}
}


