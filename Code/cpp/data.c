#define _CRT_SECURE_NO_WARNINGS
#include "..\include\inner.h"
#include "..\include\winsgl.h"

struct JSON *createJson() {
	int i;

	struct JSON *ret = (struct JSON *)malloc(sizeof(struct JSON));
	for (i = 0; i < SG_HASH_NUM; i++) {
		ret->hash[i] = NULL;
	}

	return ret;
}
struct JSON *readJson(const char *json) {

}
char *writeJson(struct JSON *json) {
	int i;
	struct JSON_Item *iter;
	char *res = (char *)malloc(256);
	int len = 0, bound = 256;

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
				sprintf(res + strlen(res), "%c", iter->data.json_char);
				strcat(res, ",\n");
				break;
			case JSON_BOOL:
				if (iter->data.json_bool)
					strcat(res, "true,\n");
				else
					strcat(res, "false,\n");
				break;
			case JSON_STRING:
				sprintf(res + strlen(res), "%s", iter->data.json_string);
				strcat(res, ",\n");
				break;
			default:
				break;
			}

			iter = iter->next;
		}
	}
	strcat(res, "}");
}
union JSON_Data getElement(struct JSON *json, const char *name) {
	int i, sum = 0;
	struct JSON_Item *iter;
	union JSON_Data ret;

	for (i = 0; i < strlen(name); i++) {
		sum += name[i];
	}
	sum /= SG_HASH_NUM;

	iter = json->hash[sum];
	while(iter) {
		if (strcmp(iter->name, name) == 0)return iter->data;
		iter = iter->next;
	}

	return ret;
}
void deleteElement(struct JSON *json, const char *name) {

}
void setIntElement(struct JSON *json, const char *name, SGint i) {
	int ch, sum = 0;
	struct JSON_Item *iter;

	for (ch = 0; ch < strlen(name); ch++) {
		sum += name[ch];
	}
	sum %= SG_HASH_NUM;

	iter = json->hash[sum];
	while (iter) {
		if (strcmp(iter->name, name) == 0) {
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
void setFloatElement(struct JSON *json, const char *name, SGfloat f) {
	int ch, sum = 0;
	struct JSON_Item *iter;

	for (ch = 0; ch < strlen(name); ch++) {
		sum += name[ch];
	}
	sum /= SG_HASH_NUM;

	iter = json->hash[sum];
	while (iter) {
		if (strcmp(iter->name, name) == 0) {
			iter->data.json_float = f;
			return;
		}
		iter = iter->next;
	}

	iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
	iter->t = JSON_INT;
	iter->name = (char *)malloc(strlen(name) + 1);
	strcpy(iter->name, name);
	iter->data.json_float = f;
	iter->next = json->hash[sum];
	json->hash[sum] = iter;
}
void setCharElement(struct JSON *json, const char *name, SGchar c) {
	int ch, sum = 0;
	struct JSON_Item *iter;

	for (ch = 0; ch < strlen(name); ch++) {
		sum += name[ch];
	}
	sum /= SG_HASH_NUM;

	iter = json->hash[sum];
	while (iter) {
		if (strcmp(iter->name, name) == 0) {
			iter->data.json_char = c;
			return;
		}
		iter = iter->next;
	}

	iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
	iter->t = JSON_INT;
	iter->name = (char *)malloc(strlen(name) + 1);
	strcpy(iter->name, name);
	iter->data.json_char = c;
	iter->next = json->hash[sum];
	json->hash[sum] = iter;
}
void setBoolElement(struct JSON *json, const char *name, SGbool b) {
	int ch, sum = 0;
	struct JSON_Item *iter;

	for (ch = 0; ch < strlen(name); ch++) {
		sum += name[ch];
	}
	sum /= SG_HASH_NUM;

	iter = json->hash[sum];
	while (iter) {
		if (strcmp(iter->name, name) == 0) {
			iter->data.json_bool = b;
			return;
		}
		iter = iter->next;
	}

	iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
	iter->t = JSON_INT;
	iter->name = (char *)malloc(strlen(name) + 1);
	strcpy(iter->name, name);
	iter->data.json_bool = b;
	iter->next = json->hash[sum];
	json->hash[sum] = iter;
}
void setStringElement(struct JSON *json, const char *name, SGstring s) {
	int ch, sum = 0;
	struct JSON_Item *iter;

	for (ch = 0; ch < strlen(name); ch++) {
		sum += name[ch];
	}
	sum /= SG_HASH_NUM;

	iter = json->hash[sum];
	while (iter) {
		if (strcmp(iter->name, name) == 0) {
			iter->data.json_string = s;
			return;
		}
		iter = iter->next;
	}

	iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
	iter->t = JSON_INT;
	iter->name = (char *)malloc(strlen(name) + 1);
	strcpy(iter->name, name);
	iter->data.json_string = s;
	iter->next = json->hash[sum];
	json->hash[sum] = iter;
}
