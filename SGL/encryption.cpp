#define _CRT_SECURE_NO_WARNINGS

#include "winsgl.h"
#include "inner.h"
#include <vector>
#include <bitset>

using namespace std;

typedef bitset<8> _byte;
typedef bitset<32> _word;


/*
* SG encryption interfaces
* These functions are used to encrypt and decrypt.
*/

int DES_IP[64] = { //初始换位表
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};
int DES_E[48] = { //扩展换位表
	32, 1, 2, 3, 4, 5,
	4, 5, 6, 7, 8, 9,
	8, 9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32, 1 };

int DES_S1[4][16] = {
	14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
	0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
	4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
	15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
};
int DES_S2[4][16] = {
	15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
	3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
	0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
	13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
};
int DES_S3[4][16] = {
	10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
	13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
	13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
	1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
};
int DES_S4[4][16] = {
	7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
	13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
	10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
	3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
};
int DES_S5[4][16] = {
	2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
	14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
	4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
	11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
};
int DES_S6[4][16] = {
	12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
	10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
	9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
	4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
};
int DES_S7[4][16] = {
	4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
	13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
	1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
	6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
};
int DES_S8[4][16] = {
	13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
	1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
	7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
	2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
};
int DES_P[32] = {
	16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,
	2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25
};

void DESInitKey(const char *seed, int key[16][48]) {
	int PC1[56] = {
		57, 49, 41, 33, 25, 17, 9,
		1, 58, 50, 42, 34, 26, 18,
		10, 2, 59, 51, 43, 35, 27,
		19, 11, 3, 60, 52, 44, 36,
		63, 55, 47, 39, 31, 23, 15,
		7, 62, 54, 46, 38, 30, 22,
		14, 6, 61, 53, 45, 37, 29,
		21, 13, 5, 28, 20, 12, 4 };
	int PC2[48] = {
		14, 17, 11, 24, 1, 5,
		3, 28, 15, 6, 21, 10,
		23, 19, 12, 4, 26, 8,
		16, 7, 27, 20, 13, 2,
		41, 52, 31, 37, 47, 55,
		30, 40, 51, 45, 33, 48,
		44, 49, 39, 56, 34, 53,
		46, 42, 50, 36, 29, 32 };
	int move[16] = { 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 };

	int kbin[64];
	int keyPC1[56];
	int A[28], B[28];
	int keyAB[56];

	for (int i = 0; i < 8; i++) {
		int a[8] = { 0,0,0,0,0,0,0,0 };
		int m = (unsigned char)seed[i];
		for (int j = 0; m != 0; j++) {
			a[j] = m % 2;
			m = m / 2;
		}
		for (int j = 0; j < 8; j++)
			kbin[(i * 8) + j] = a[7 - j];
	}
	for (int i = 0; i < 56; i++) {
		keyPC1[i] = kbin[PC1[i] - 1];
	}
	for (int i = 0; i < 28; i++) {
		A[i] = keyPC1[i];
		B[i] = keyPC1[i + 28];
	}
	for (int t = 0; t < 16; t++) {
		if (move[t] == 1) {
			int n = A[0];
			for (int i = 0; i < 27; i++)
				A[i] = A[i + 1];
			A[27] = n;
			n = B[0];
			for (int i = 0; i < 28; i++)
				B[i] = B[i + 1];
			B[27] = n;
		}
		else
		{
			int n = A[0];
			int m = A[1];
			for (int i = 0; i < 26; i++)
				A[i] = A[i + 2];
			A[26] = n;
			A[27] = m;
			n = B[0];
			m = B[1];
			for (int i = 0; i < 26; i++)
				B[i] = B[i + 2];
			B[26] = n;
			B[27] = m;
		}

		for (int i = 0; i < 28; i++) {
			keyAB[i] = A[i];
			keyAB[i + 28] = B[i];
		}

		for (int i = 0; i < 48; i++) //通过PC2换位表变成48位密钥
			key[t][i] = keyAB[PC2[i] - 1];
	}
}
void DESInitVector(const char *init, int *bin) {
	for (int i = 0; i < 8; i++) {
		int a[8] = { 0,0,0,0,0,0,0,0 };
		int m = (unsigned char)init[i];
		for (int j = 0; m != 0; j++) {
			a[j] = m % 2;
			m = m / 2;
		}
		for (int j = 0; j < 8; j++)
			bin[(i * 8) + j] = a[7 - j];
	}
}
int DESEncrypt(const char *plain, int len, const char *key, char *cipher) {
	char init[9] = { " SG-Lib " };

	int blockNum = (len - 1) / 8 + 1;

	int K[16][48];
	vector<vector<int>> text_out(blockNum + 1, vector<int>(64));
	vector<vector<int>> result(blockNum, vector<int>(64));
	DESInitKey(key, K);
	DESInitVector(init, text_out[0].data());

	int k = len % 8;
	for (int l = 0; l < blockNum; l++) {
		unsigned char target[8];
		int text[64];
		int text_ip[64];
		if (l == (blockNum - 1) && k != 0) {
			for (int i = 0; i < k; i++)target[i] = plain[i + (8 * l)];
			for (int i = k; i < 8; i++)target[i] = '\0';
		}
		else for (int i = 0; i < 8; i++)target[i] = plain[i + (8 * l)];

		for (int i = 0; i < 8; i++) {
			int a[8] = { 0,0,0,0,0,0,0,0 };
			int m = target[i];
			for (int j = 0; m != 0; j++) {
				a[j] = m % 2;
				m = m / 2;
			}
			for (int j = 0; j < 8; j++)text[(i * 8) + j] = a[7 - j];
		}
		for (int i = 0; i < 64; i++)text[i] = text_out[l][i] ^ text[i];
		for (int i = 0; i < 64; i++)text_ip[i] = text[DES_IP[i] - 1];

		int L0[32], Li[32];
		int R0[32], Ri[32];
		int RE0[48];
		int RK[48];
		int RKS[8];
		int SP[32], RKSP[32];
		for (int i = 0; i < 32; i++) {
			L0[i] = text_ip[i];
			R0[i] = text_ip[i + 32];
		}
		for (int t = 0; t < 16; t++) {
			for (int i = 0; i < 48; i++)RE0[i] = R0[DES_E[i] - 1];
			for (int i = 0; i < 48; i++)RK[i] = RE0[i] ^ K[t][i];

			int r[8], c[8];
			for (int i = 0; i < 8; i++) {
				r[i] = RK[(i * 6) + 0] * 2 + RK[(i * 6) + 5];
				c[i] = RK[(i * 6) + 1] * 8 + RK[(i * 6) + 2] * 4 + RK[(i * 6) + 3] * 2 + RK[(i * 6) + 4];
			}
			RKS[0] = DES_S1[r[0]][c[0]];
			RKS[1] = DES_S2[r[1]][c[1]];
			RKS[2] = DES_S3[r[2]][c[2]];
			RKS[3] = DES_S4[r[3]][c[3]];
			RKS[4] = DES_S5[r[4]][c[4]];
			RKS[5] = DES_S6[r[5]][c[5]];
			RKS[6] = DES_S7[r[6]][c[6]];
			RKS[7] = DES_S8[r[7]][c[7]];

			for (int i = 0; i < 8; i++) {
				int b[4] = { 0,0,0,0 };
				int m = RKS[i];
				for (int j = 3; m != 0; j--) {
					b[j] = m % 2;
					m = m / 2;
				}
				for (int j = 0; j < 4; j++)SP[j + (i * 4)] = b[j];
			}
			for (int i = 0; i < 32; i++)RKSP[i] = SP[DES_P[i] - 1];
			for (int i = 0; i < 32; i++)Ri[i] = L0[i] ^ RKSP[i];
			for (int i = 0; i < 32; i++) {
				L0[i] = R0[i];
				R0[i] = Ri[i];
			}
		}
		for (int i = 0; i < 32; i++)Li[i] = R0[i];
		for (int i = 0; i < 32; i++)R0[i] = L0[i];
		for (int i = 0; i < 32; i++)L0[i] = Li[i];

		int text_end[64];
		for (int i = 0; i < 32; i++)text_end[i] = L0[i];
		for (int i = 32; i < 64; i++)text_end[i] = R0[i - 32];
		for (int i = 0; i < 64; i++)text_out[l + 1][DES_IP[i] - 1] = text_end[i];
		for (int i = 0; i < 64; i++)result[l][i] = text_out[l + 1][i];
	}

	for (int j = 0; j < blockNum; j++)
		for (int i = 0; i < 8; i++) {
			cipher[(j * 8) + i] = result[j][0 + (i * 8)] * 128 + result[j][1 + (i * 8)] * 64 + result[j][2 + (i * 8)] * 32 + result[j][3 + (i * 8)] * 16 +
				result[j][4 + (i * 8)] * 8 + result[j][5 + (i * 8)] * 4 + result[j][6 + (i * 8)] * 2 + result[j][7 + (i * 8)];
		}

	return blockNum * 8;
}
int DESDecrypt(const char *cipher, int len, const char *key, char *plain) {
	char init[9] = { " SG-Lib " };

	len = ((len - 1) / 8 + 1) * 8;
	int blockNum = len / 8;
	vector<vector<int>> text_out(blockNum + 1, vector<int>(64));
	vector<vector<int>> result(blockNum, vector<int>(64));

	int K[16][48];
	DESInitKey(key, K);
	DESInitVector(init, text_out[0].data());

	vector<int> bits(len * 8);
	for (int i = 0; i < len; i++) {
		unsigned char tmp = cipher[i];
		for (int j = 0; j < 8; j++) {
			bits[i * 8 + 7 - j] = (tmp & 0x1);
			tmp /= 2;
		}
	}

	int k = len / 8;
	for (int l = 0; l < k; l++) {
		for (int i = 0; i < 64; i++)
			text_out[l + 1][i] = bits[i + (l * 64)];

		int text[64];
		int text_ip[64];
		for (int i = 0; i < 64; i++)
			text_ip[i] = text_out[l + 1][DES_IP[i] - 1];

		int L0[32], Li[32];
		int R0[32], Ri[32];
		int RE0[48];
		int RK[48];
		int RKS[8];
		int SP[32], RKSP[32];
		for (int i = 0; i < 32; i++) {
			L0[i] = text_ip[i];
			R0[i] = text_ip[i + 32];
		}
		for (int t = 0; t < 16; t++) {
			for (int i = 0; i < 48; i++)RE0[i] = R0[DES_E[i] - 1];
			for (int i = 0; i < 48; i++)RK[i] = RE0[i] ^ K[15 - t][i];

			int r[8], c[8];
			for (int i = 0; i < 8; i++) {
				r[i] = RK[(i * 6) + 0] * 2 + RK[(i * 6) + 5];
				c[i] = RK[(i * 6) + 1] * 8 + RK[(i * 6) + 2] * 4 + RK[(i * 6) + 3] * 2 + RK[(i * 6) + 4];
			}

			RKS[0] = DES_S1[r[0]][c[0]];
			RKS[1] = DES_S2[r[1]][c[1]];
			RKS[2] = DES_S3[r[2]][c[2]];
			RKS[3] = DES_S4[r[3]][c[3]];
			RKS[4] = DES_S5[r[4]][c[4]];
			RKS[5] = DES_S6[r[5]][c[5]];
			RKS[6] = DES_S7[r[6]][c[6]];
			RKS[7] = DES_S8[r[7]][c[7]];

			for (int i = 0; i < 8; i++) {
				int b[4] = { 0,0,0,0 };
				int m = RKS[i];
				for (int j = 3; m != 0; j--) {
					b[j] = m % 2;
					m = m / 2;
				}
				for (int j = 0; j < 4; j++)SP[j + (i * 4)] = b[j];
			}
			for (int i = 0; i < 32; i++)RKSP[i] = SP[DES_P[i] - 1];
			for (int i = 0; i < 32; i++)Ri[i] = L0[i] ^ RKSP[i];
			for (int i = 0; i < 32; i++) {
				L0[i] = R0[i];
				R0[i] = Ri[i];
			}
		}

		for (int i = 0; i < 32; i++)Li[i] = R0[i];
		for (int i = 0; i < 32; i++)R0[i] = L0[i];
		for (int i = 0; i < 32; i++)L0[i] = Li[i];

		int text_end[64];
		for (int i = 0; i < 32; i++)text_end[i] = L0[i];
		for (int i = 32; i < 64; i++)text_end[i] = R0[i - 32];
		for (int i = 0; i < 64; i++)text[DES_IP[i] - 1] = text_end[i];
		for (int i = 0; i < 64; i++)result[l][i] = text_out[l][i] ^ text[i];
	}

	for (int i = 0; i < (len / 8); i++)
		for (int j = 0; j < 8; j++)
			plain[i * 8 + j] = result[i][(j * 8) + 0] * 128 + result[i][(j * 8) + 1] * 64 + result[i][(j * 8) + 2] * 32 + result[i][(j * 8) + 3] * 16 +
				result[i][(j * 8) + 4] * 8 + result[i][(j * 8) + 5] * 4 + result[i][(j * 8) + 6] * 2 + result[i][(j * 8) + 7];

	return len;
}

const int Nr = 10;
const int Nk = 4;

_byte AES_SBox[16][16] = {
	{ 0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76 },
	{ 0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0 },
	{ 0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15 },
	{ 0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75 },
	{ 0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84 },
	{ 0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF },
	{ 0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8 },
	{ 0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2 },
	{ 0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73 },
	{ 0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB },
	{ 0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79 },
	{ 0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08 },
	{ 0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A },
	{ 0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E },
	{ 0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF },
	{ 0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16 }
};
_byte AES_InvSBox[16][16] = {
	{ 0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB },
	{ 0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB },
	{ 0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E },
	{ 0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25 },
	{ 0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92 },
	{ 0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84 },
	{ 0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06 },
	{ 0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B },
	{ 0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73 },
	{ 0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E },
	{ 0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B },
	{ 0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4 },
	{ 0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F },
	{ 0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF },
	{ 0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61 },
	{ 0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D }
};
_word AES_Rcon[10] = {
	0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
	0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000
};
void AESSubBytes(_byte mtx[4 * 4]) {
	for (int i = 0; i<16; ++i) {
		int row = mtx[i][7] * 8 + mtx[i][6] * 4 + mtx[i][5] * 2 + mtx[i][4];
		int col = mtx[i][3] * 8 + mtx[i][2] * 4 + mtx[i][1] * 2 + mtx[i][0];
		mtx[i] = AES_SBox[row][col];
	}
}
void AESShiftRows(_byte mtx[4 * 4]) {
	_byte temp = mtx[4];
	for (int i = 0; i<3; ++i)
		mtx[i + 4] = mtx[i + 5];
	mtx[7] = temp;
	for (int i = 0; i<2; ++i){
		temp = mtx[i + 8];
		mtx[i + 8] = mtx[i + 10];
		mtx[i + 10] = temp;
	}
	temp = mtx[15];
	for (int i = 3; i>0; --i)
		mtx[i + 12] = mtx[i + 11];
	mtx[12] = temp;
}
_byte AESGFMul(_byte a, _byte b) {
	_byte p = 0;
	_byte hi_bit_set;
	for (int counter = 0; counter < 8; counter++) {
		if ((b & _byte(1)) != 0) {
			p ^= a;
		}
		hi_bit_set = (_byte)(a & _byte(0x80));
		a <<= 1;
		if (hi_bit_set != 0) {
			a ^= 0x1b;
		}
		b >>= 1;
	}
	return p;
}
void AESMixColumns(_byte mtx[4 * 4]) {
	_byte arr[4];
	for (int i = 0; i<4; ++i) {
		for (int j = 0; j<4; ++j)
			arr[j] = mtx[i + j * 4];

		mtx[i] = AESGFMul(0x02, arr[0]) ^ AESGFMul(0x03, arr[1]) ^ arr[2] ^ arr[3];
		mtx[i + 4] = arr[0] ^ AESGFMul(0x02, arr[1]) ^ AESGFMul(0x03, arr[2]) ^ arr[3];
		mtx[i + 8] = arr[0] ^ arr[1] ^ AESGFMul(0x02, arr[2]) ^ AESGFMul(0x03, arr[3]);
		mtx[i + 12] = AESGFMul(0x03, arr[0]) ^ arr[1] ^ arr[2] ^ AESGFMul(0x02, arr[3]);
	}
}
void AESAddRoundKey(_byte mtx[4 * 4], _word k[4]) {
	for (int i = 0; i<4; ++i) {
		_word k1 = k[i] >> 24;
		_word k2 = (k[i] << 8) >> 24;
		_word k3 = (k[i] << 16) >> 24;
		_word k4 = (k[i] << 24) >> 24;

		mtx[i] = mtx[i] ^ _byte(k1.to_ulong());
		mtx[i + 4] = mtx[i + 4] ^ _byte(k2.to_ulong());
		mtx[i + 8] = mtx[i + 8] ^ _byte(k3.to_ulong());
		mtx[i + 12] = mtx[i + 12] ^ _byte(k4.to_ulong());
	}
}
void AESInvSubBytes(_byte mtx[4 * 4]) {
	for (int i = 0; i<16; ++i) {
		int row = mtx[i][7] * 8 + mtx[i][6] * 4 + mtx[i][5] * 2 + mtx[i][4];
		int col = mtx[i][3] * 8 + mtx[i][2] * 4 + mtx[i][1] * 2 + mtx[i][0];
		mtx[i] = AES_InvSBox[row][col];
	}
}
void AESInvShiftRows(_byte mtx[4 * 4]) {
	_byte temp = mtx[7];
	for (int i = 3; i>0; --i)
		mtx[i + 4] = mtx[i + 3];
	mtx[4] = temp;
	for (int i = 0; i<2; ++i)
	{
		temp = mtx[i + 8];
		mtx[i + 8] = mtx[i + 10];
		mtx[i + 10] = temp;
	}
	temp = mtx[12];
	for (int i = 0; i<3; ++i)
		mtx[i + 12] = mtx[i + 13];
	mtx[15] = temp;
}
void AESInvMixColumns(_byte mtx[4 * 4]) {
	_byte arr[4];
	for (int i = 0; i<4; ++i) {
		for (int j = 0; j<4; ++j)
			arr[j] = mtx[i + j * 4];

		mtx[i] = AESGFMul(0x0e, arr[0]) ^ AESGFMul(0x0b, arr[1]) ^ AESGFMul(0x0d, arr[2]) ^ AESGFMul(0x09, arr[3]);
		mtx[i + 4] = AESGFMul(0x09, arr[0]) ^ AESGFMul(0x0e, arr[1]) ^ AESGFMul(0x0b, arr[2]) ^ AESGFMul(0x0d, arr[3]);
		mtx[i + 8] = AESGFMul(0x0d, arr[0]) ^ AESGFMul(0x09, arr[1]) ^ AESGFMul(0x0e, arr[2]) ^ AESGFMul(0x0b, arr[3]);
		mtx[i + 12] = AESGFMul(0x0b, arr[0]) ^ AESGFMul(0x0d, arr[1]) ^ AESGFMul(0x09, arr[2]) ^ AESGFMul(0x0e, arr[3]);
	}
}
_word AESWord(_byte& k1, _byte& k2, _byte& k3, _byte& k4) {
	_word result(0x00000000);
	_word temp;
	temp = k1.to_ulong();
	temp <<= 24;
	result |= temp;
	temp = k2.to_ulong();
	temp <<= 16;
	result |= temp;
	temp = k3.to_ulong();
	temp <<= 8;
	result |= temp;
	temp = k4.to_ulong();
	result |= temp;
	return result;
}
_word AESRotWord(_word rw) {
	_word high = rw << 8;
	_word low = rw >> 24;
	return high | low;
}
_word AESSubWord(_word sw) {
	_word temp;
	for (int i = 0; i<32; i += 8) {
		int row = sw[i + 7] * 8 + sw[i + 6] * 4 + sw[i + 5] * 2 + sw[i + 4];
		int col = sw[i + 3] * 8 + sw[i + 2] * 4 + sw[i + 1] * 2 + sw[i];
		_byte val = AES_SBox[row][col];
		for (int j = 0; j<8; ++j)
			temp[i + j] = val[j];
	}
	return temp;
}
void AESKeyExpansion(_byte key[4 * Nk], _word w[4 * (Nr + 1)]) {
	_word temp;
	int i = 0;
	while (i < Nk) {
		w[i] = AESWord(key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]);
		++i;
	}

	i = Nk;

	while (i < 4 * (Nr + 1)) {
		temp = w[i - 1];
		if (i % Nk == 0)
			w[i] = w[i - Nk] ^ AESSubWord(AESRotWord(temp)) ^ AES_Rcon[i / Nk - 1];
		else
			w[i] = w[i - Nk] ^ temp;
		++i;
	}
}
void AESEncryptFlow(_byte in[4 * 4], _word w[4 * (Nr + 1)]) {
	_word key[4];
	for (int i = 0; i<4; ++i)
		key[i] = w[i];
	AESAddRoundKey(in, key);

	for (int round = 1; round<Nr; ++round) {
		AESSubBytes(in);
		AESShiftRows(in);
		AESMixColumns(in);
		for (int i = 0; i<4; ++i)
			key[i] = w[4 * round + i];
		AESAddRoundKey(in, key);
	}

	AESSubBytes(in);
	AESShiftRows(in);
	for (int i = 0; i<4; ++i)
		key[i] = w[4 * Nr + i];
	AESAddRoundKey(in, key);
}
void AESDecryptFlow(_byte in[4 * 4], _word w[4 * (Nr + 1)]) {
	_word key[4];
	for (int i = 0; i<4; ++i)
		key[i] = w[4 * Nr + i];
	AESAddRoundKey(in, key);

	for (int round = Nr - 1; round>0; --round) {
		AESInvShiftRows(in);
		AESInvSubBytes(in);
		for (int i = 0; i<4; ++i)
			key[i] = w[4 * round + i];
		AESAddRoundKey(in, key);
		AESInvMixColumns(in);
	}

	AESInvShiftRows(in);
	AESInvSubBytes(in);
	for (int i = 0; i<4; ++i)
		key[i] = w[i];
	AESAddRoundKey(in, key);
}
void AESCharToByte(_byte out[16], const char s[16]) {
	for (int i = 0; i<16; ++i)
		for (int j = 0; j<8; ++j)
			out[i][j] = ((s[i] >> j) & 1);
}
void AESDivideToByte(_byte out[16], bitset<128>& data) {
	bitset<128> temp;
	for (int i = 0; i<16; ++i) {
		temp = (data << 8 * i) >> 120;
		out[i] = temp.to_ulong();
	}
}
bitset<128> AESMergeByte(_byte in[16]) {
	bitset<128> res;
	res.reset();
	bitset<128> temp;
	for (int i = 0; i<16; ++i) {
		temp = in[i].to_ulong();
		temp <<= 8 * (15 - i);
		res |= temp;
	}
	return res;
}
void AESEncrypt(const char *plain, int len, const char *k, char *cipher) {
	string keyStr = k;
	_byte key[16];
	AESCharToByte(key, keyStr.c_str());
	_word w[4 * (Nr + 1)];
	AESKeyExpansion(key, w);

	bitset<128> data = { 0 };
	_byte block[16];
	while (len > 0) {
		memcpy(&data, plain, len > sizeof(data) ? sizeof(data) : len);
		AESDivideToByte(block, data);
		AESEncryptFlow(block, w);
		data = AESMergeByte(block);
		memcpy(cipher, &data, sizeof(data));
		data.reset();
		plain += sizeof(data);
		cipher += sizeof(data);
		len -= sizeof(data);
	}
}
void AESDecrypt(const char *cipher, int len, const char *k, char *plain) {
	string keyStr = k;
	_byte key[16];
	AESCharToByte(key, keyStr.c_str());
	_word w[4 * (Nr + 1)];
	AESKeyExpansion(key, w);

	bitset<128> data = { 0 };
	_byte block[16];
	while (len > 0) {
		memcpy(&data, cipher, len > sizeof(data) ? sizeof(data) : len);
		AESDivideToByte(block, data);
		AESDecryptFlow(block, w);
		data = AESMergeByte(block);
		memcpy(plain, &data, sizeof(data));
		data.reset();
		plain += sizeof(data);
		cipher += sizeof(data);
		len -= sizeof(data);
	}

}

unsigned int RSARandomlyGenerateE(unsigned int t) {
	unsigned int e = 0;
	srand((unsigned int)time(0));
	e = 2 + rand() % (t - 3);
	return(e);//随机数
}
void RSAGcd(unsigned int BigNum, unsigned int SmallNum, unsigned int &MaxGcd) {
	int tmp = 0;
	while (BigNum%SmallNum) {
		tmp = SmallNum;
		SmallNum = BigNum % SmallNum;
		BigNum = tmp;
	}
	MaxGcd = SmallNum;
}
bool RSAJudgePrime(unsigned int BigNum, unsigned int SmallNum) {
	unsigned int M = 0;
	RSAGcd(BigNum, SmallNum, M);
	if (M == 1)
		return true;
	else
		return false;
}
int RSArevMod(unsigned int e, unsigned int model, unsigned int* d) {
	unsigned int i;
	unsigned int over = e;
	for (i = 1; i<model;) {
		over = over % model;
		if (over == 1) {
			*d = i;
			return 1;
		}
		else {
			if (over + e <= model) {
				do {
					i++;
					over += e;
				} while (over + e <= model);
			}
			else {
				i++;
				over += e;
			}
		}
	}
	return 0;
}
int RSABianaryTransform(int num, int bin_num[]) {
	int i = 0, mod = 0;
	while (num != 0) {
		mod = num % 2;
		bin_num[i] = mod;
		num = num / 2;
		i++;
	}
	return i;
}
unsigned int RSAModExo(unsigned int a, int b, int n) {
	int c = 0, bin_num[1000];
	int d = 1;
	int k = RSABianaryTransform(b, bin_num) - 1;

	for (int i = k; i >= 0; i--) {
		c = 2 * c;
		d = (d*d) % n;
		if (bin_num[i] == 1) {
			c = c + 1;
			d = (d*a) % n;
		}
	}
	return d;
}
void RSAProduceKey(unsigned int p, unsigned int q, unsigned int *e, unsigned int *d, unsigned int *n) {
	unsigned int t = 0;
	*n = p * q;
	t = (p - 1)*(q - 1);
	*e = RSARandomlyGenerateE(t);
	while (!RSAJudgePrime(t, *e)) {
		*e = RSARandomlyGenerateE(t);
	}
	RSArevMod(*e, t, d);
}
void RSAEncrypt(unsigned int plain, int key, int N, unsigned int *cipher) {
	*cipher = RSAModExo(plain, key, N);
}
void RSADecrypt(unsigned int cipher, int key, int N, unsigned int *plain) {
	*plain = RSAModExo(cipher, key, N);
}

