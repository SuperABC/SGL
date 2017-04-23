#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(){
	FILE *fsrc, *fdst;
	char tmp;
	int i = 0; 
	fsrc = fopen("chinese.c", "r");
	fdst = fopen("changed.c", "w");
	fseek(fsrc, 0, SEEK_SET);
	while(1) {
		if (feof(fsrc))break;
		if (i % 64 == 0) {
			fputc('{', fdst);
			fputc(' ', fdst);
			fputc('0', fdst);
			fputc('x', fdst);
		}
		tmp = fgetc(fsrc);
		if (tmp == ' ' || tmp== '\n') {
			fputc(',', fdst);
			fputc(' ', fdst);
			fputc('0', fdst);
			fputc('x', fdst);
			continue;
		}
		else fputc(tmp, fdst);
		if (i % 64 == 63) {
			fputc(' ', fdst);
			fputc('}', fdst);
			fputc(',', fdst);
			fputc('\n', fdst);
			fgetc(fsrc);
		}
		i = (i+1) % 64; 
	}
	fclose(fsrc);
	fclose(fdst);
}
