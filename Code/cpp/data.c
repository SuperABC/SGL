#define _CRT_SECURE_NO_WARNINGS
#include "..\include\inner.h"
#include "..\include\winsgl.h"

struct JSON *readJson(const char *json) {

}
char *writeJson(struct JSON *json) {

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

	ret.json_valid = 0;
	return ret;
}
void setIntElement(struct JSON *json, const char *name, SGint i) {
	int ch, sum = 0;
	struct JSON_Item *iter;

	for (ch = 0; ch < strlen(name); ch++) {
		sum += name[ch];
	}
	sum /= SG_HASH_NUM;

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
void setObjectElement(struct JSON *json, const char *name, SGvoid *o) {
	int ch, sum = 0;
	struct JSON_Item *iter;

	for (ch = 0; ch < strlen(name); ch++) {
		sum += name[ch];
	}
	sum /= SG_HASH_NUM;

	iter = json->hash[sum];
	while (iter) {
		if (strcmp(iter->name, name) == 0) {
			iter->data.json_object = o;
			return;
		}
		iter = iter->next;
	}

	iter = (struct JSON_Item *)malloc(sizeof(struct JSON_Item));
	iter->t = JSON_INT;
	iter->name = (char *)malloc(strlen(name) + 1);
	strcpy(iter->name, name);
	iter->data.json_object = o;
	iter->next = json->hash[sum];
	json->hash[sum] = iter;
}
