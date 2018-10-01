#define _CRT_SECURE_NO_WARNINGS
#include "..\include\inner.h"
#include "..\include\winsgl.h"

int subObjectCont(struct JSON *obj, char *name, const char *json, int i);
int subArrayCont(struct JSON *obj, char *name, const char *json, int i);
int subObjectElement(struct JSON *obj, int idx, const char *json, int i);
int subArrayElement(struct JSON *obj, int idx, const char *json, int i);

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

int subObjectCont(struct JSON *obj, char *name, const char *json, int i) {
	int j;
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
		else if (json[i] >= '0' && json[i] <= '9') {

		}
		else if (json[i] == 't' || json[i] == 'f') {

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
	setObjectContent(obj, name, element);

	i++;
	return i;
}
int subArrayCont(struct JSON *obj, char *name, const char *json, int i) {
	int j;
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
		else if (json[i] >= '0' && json[i] <= '9') {

		}
		else if (json[i] == 't' || json[i] == 'f') {

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
	setArrayContent(obj, name, element);

	i++;
	return i;
}
int subObjectElement(struct JSON *obj, int idx, const char *json, int i) {
	int j;
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
		else if (json[i] >= '0' && json[i] <= '9') {

		}
		else if (json[i] == 't' || json[i] == 'f') {

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
int subArrayElement(struct JSON *obj, int idx, const char *json, int i) {
	int j;
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
		else if (json[i] >= '0' && json[i] <= '9') {

		}
		else if (json[i] == 't' || json[i] == 'f') {

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
struct JSON *readJson(const char *json) {
	int i = 0, j;
	char name[256];
	char cont[256];

	struct JSON *res;

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
			else if (json[i] >= '0' && json[i] <= '9') {

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
			else if (json[i] >= '0' && json[i] <= '9') {

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

	return res;
}
char *writeJson(struct JSON *json) {
	int i;
	struct JSON_Item *iter = NULL;
	char *res = (char *)malloc(256);
	int len = 0, bound = 256;

	if (json->arr == 0) {
		strcpy(res, "{\n");

		for (i = 0; i < SG_HASH_NUM; i++) {
			iter = json->hash[i];
			while (iter) {
				strcat(res, "\"");
				strcat(res, iter->name);
				strcat(res, "\":");

				switch (iter->t) {
				case JSON_INT:
					sprintf(res + strlen(res), "%d", iter->data.json_int);
					strcat(res, ",\n");
					break;
				case JSON_FLOAT:
					sprintf(res + strlen(res), "%f", iter->data.json_float);
					strcat(res, ",\n");
					break;
				case JSON_CHAR:
					sprintf(res + strlen(res), "\'%c\'", iter->data.json_char);
					strcat(res, ",\n");
					break;
				case JSON_BOOL:
					if (iter->data.json_bool)
						strcat(res, "true,\n");
					else
						strcat(res, "false,\n");
					break;
				case JSON_STRING:
					sprintf(res + strlen(res), "\"%s\"", iter->data.json_string);
					strcat(res, ",\n");
					break;
				case JSON_OBJECT:
					strcat(res, writeJson(iter->data.json_object));
					strcat(res, ",\n");
					break;
				case JSON_ARRAY:
					strcat(res, writeJson(iter->data.json_array));
					strcat(res, ",\n");
					break;
				default:
					break;
				}

				iter = iter->next;
			}
		}
		res[strlen(res) - 2] = '\0';
		strcat(res, "\n");
		strcat(res, "}");
	}
	else {
		strcpy(res, "[\n");

		iter = json->list;
		while (iter) {
			switch (iter->t) {
			case JSON_INT:
				sprintf(res + strlen(res), "%d", iter->data.json_int);
				strcat(res, ",\n");
				break;
			case JSON_FLOAT:
				sprintf(res + strlen(res), "%f", iter->data.json_float);
				strcat(res, ",\n");
				break;
			case JSON_CHAR:
				sprintf(res + strlen(res), "\'%c\'", iter->data.json_char);
				strcat(res, ",\n");
				break;
			case JSON_BOOL:
				if (iter->data.json_bool)
					strcat(res, "true,\n");
				else
					strcat(res, "false,\n");
				break;
			case JSON_STRING:
				sprintf(res + strlen(res), "\"%s\"", iter->data.json_string);
				strcat(res, ",\n");
				break;
			case JSON_OBJECT:
				strcat(res, writeJson(iter->data.json_object));
				strcat(res, ",\n");
				break;
			case JSON_ARRAY:
				strcat(res, writeJson(iter->data.json_array));
				strcat(res, ",\n");
				break;
			default:
				break;
			}

			iter = iter->next;
		}
		res[strlen(res) - 2] = '\0';
		strcat(res, "\n");
		strcat(res, "]");
	}

	return res;
}

struct JSON_Item *getContent(struct JSON *json, const char *name) {
	int i, sum = 0;
	struct JSON_Item *iter = NULL;

	if (json->arr == 0) {
		for (i = 0; i < (int)strlen(name); i++) {
			sum += name[i];
		}
		sum /= SG_HASH_NUM;

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
			if(idx==0)return iter;
			idx--;

			iter = iter->next;
		}
	}

	return NULL;
}
void deleteContent(struct JSON *json, const char *name) {
	int i, sum = 0;
	struct JSON_Item *iter = NULL, *tmp;

	if (json->arr == 0) {
		for (i = 0; i < (int)strlen(name); i++) {
			sum += name[i];
		}
		sum /= SG_HASH_NUM;

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

void setIntContent(struct JSON *json, const char *name, SGint i) {
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
void setFloatContent(struct JSON *json, const char *name, SGfloat f) {
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
void setCharContent(struct JSON *json, const char *name, SGchar c) {
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
void setBoolContent(struct JSON *json, const char *name, SGbool b) {
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
void setStringContent(struct JSON *json, const char *name, SGstring s) {
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
void setObjectContent(struct JSON *json, const char *name, struct JSON *j) {
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
void setArrayContent(struct JSON *json, const char *name, struct JSON *j) {
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

				if(iter->next)
					iter = iter->next;
				else {
					iter->next = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
					iter = iter->next;
					iter->t = JSON_INT;
					iter->name = "";
					iter->data.json_int = i;

					iter->next = NULL;
				}
			}
		}
		iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));

		iter->t = JSON_INT;
		iter->name = "";
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
					iter->name = "";
					iter->data.json_float = f;

					iter->next = NULL;
				}
			}
		}
		iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));

		iter->t = JSON_FLOAT;
		iter->name = "";
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
					iter->name = "";
					iter->data.json_char = c;

					iter->next = NULL;
				}
			}
		}
		iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));

		iter->t = JSON_CHAR;
		iter->name = "";
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
					iter->name = "";
					iter->data.json_bool = b;

					iter->next = NULL;
				}
			}
		}
		iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));

		iter->t = JSON_BOOL;
		iter->name = "";
		iter->data.json_bool = b;

		iter->next = json->list;
		json->list = iter;
	}
}
void setStringElement(struct JSON *json, int idx, SGstring s) {
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
					iter->name = "";
					iter->data.json_string = (char *)malloc(strlen(s) + 1);
					strcpy(iter->data.json_string, s);

					iter->next = NULL;
				}
			}
		}
		iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));

		iter->t = JSON_STRING;
		iter->name = "";
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
					iter->name = "";
					iter->data.json_object = j;

					iter->next = NULL;
					return;
				}
			}
		}
		iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));

		iter->t = JSON_OBJECT;
		iter->name = "";
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
					iter->name = "";
					iter->data.json_array = j;

					iter->next = NULL;
					return;
				}
			}
		}
		iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));

		iter->t = JSON_ARRAY;
		iter->name = "";
		iter->data.json_array = j;

		iter->next = json->list;
		json->list = iter;
	}
}


