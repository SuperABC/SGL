#include "winsgl.h"


using namespace std;


//BMP

bitMap loadBmp(SGtext filename) {
	FILE *fp = NULL;
	SGstring p = NULL;
	int width, height, i;
	dword dataOffset, lineBytes;
	dword lines;
	bitMap ret;

	p = (SGstring)malloc(2048 * 3 * sizeof(char));
	if (p == NULL)goto displayError;
	fp = fopen(filename, "rb");
	if (fp == NULL)goto displayError;

	fread(p, 1, 0x36, fp);
	if (*(word *)p != 0x4D42)goto displayError;
	if (*(word *)(p + 0x1C) != 24)goto displayError;

	width = *(dword *)(p + 0x12);
	height = *(dword *)(p + 0x16);
	dataOffset = *(dword *)(p + 0x0A);
	lineBytes = (width * 3 + 3) / 4 * 4;

	ret.data = (unsigned char *)malloc(width * height * 3 * sizeof(char));
	if (ret.data == NULL)goto displayError;
	fseek(fp, dataOffset, SEEK_SET);

	ret.sizeX = width;
	ret.sizeY = height;
	for (i = height - 1; i >= 0; i--) {
		fread(p, 1, lineBytes, fp);
		lines = i * width * 3;
		memcpy(ret.data + lines, p, width * 3);
	}

	free(p);
	fclose(fp);
	return ret;

displayError:
	if (p != NULL)free(p);
	if (fp != NULL)fclose(fp);
	ret.data = NULL;
	ret.sizeX = ret.sizeY = SG_IO_ERROR;
	return ret;
}
void saveBmp(SGtext filename, bitMap bmp) {
	int lineByte = (bmp.sizeX * 24 / 8 + 3) / 4 * 4;
	FILE *fp = fopen(filename, "wb");
	if (fp == 0)return;

	BITMAPFILEHEADER fileHead;
	fileHead.bfType = 0x4D42;
	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + lineByte * bmp.sizeY;
	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;
	fileHead.bfOffBits = 54;
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);
	BITMAPINFOHEADER head;
	head.biBitCount = 24;
	head.biClrImportant = 0;
	head.biClrUsed = 0;
	head.biCompression = 0;
	head.biHeight = bmp.sizeY;
	head.biPlanes = 1;
	head.biSize = 40;
	head.biSizeImage = lineByte * bmp.sizeY;
	head.biWidth = bmp.sizeX;
	head.biXPelsPerMeter = 0;
	head.biYPelsPerMeter = 0;
	fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(bmp.data, bmp.sizeY*lineByte, 1, fp);
	fclose(fp);
}

//PNG

bitMap loadPng(SGtext filename) {
	return bitMap();
}
void savePng(SGtext filename, bitMap png) {

}

bitMap copyPic(bitMap src) {
	bitMap dst;
	dst.data = (unsigned char *)malloc(src.sizeX*src.sizeY * 3 * sizeof(char));

	dst.sizeX = src.sizeX;
	dst.sizeY = src.sizeY;
	memcpy(dst.data, src.data, src.sizeX*src.sizeY * 3 * sizeof(char));
	return dst;
}
bitMap grayPic(bitMap src) {
	int i, j, v;
	bitMap dst;
	dst.data = (unsigned char *)malloc(src.sizeX*src.sizeY * 3 * sizeof(char));

	dst.sizeX = src.sizeX;
	dst.sizeY = src.sizeY;
	for (i = 0; i < src.sizeY; i++) {
		for (j = 0; j < src.sizeX; j++) {
			v = (int)(src.data[(i*src.sizeX + j) * 3] * 0.299f +
				src.data[(i*src.sizeX + j) * 3 + 1] * 0.587f +
				src.data[(i*src.sizeX + j) * 3 + 2] * 0.114f);
			dst.data[(i*src.sizeX + j) * 3] =
				dst.data[(i*src.sizeX + j) * 3 + 1] =
				dst.data[(i*src.sizeX + j) * 3 + 2] = v;
		}
	}
	return dst;
}
bitMap binaryPic(bitMap src, int threshold) {
	int i, j, v;
	bitMap dst;
	dst.data = (unsigned char *)malloc(src.sizeX*src.sizeY * 3 * sizeof(char));

	dst.sizeX = src.sizeX;
	dst.sizeY = src.sizeY;
	for (i = 0; i < src.sizeY; i++) {
		for (j = 0; j < src.sizeX; j++) {
			v = (int)(src.data[(i*src.sizeX + j) * 3] * 0.299f +
				src.data[(i*src.sizeX + j) * 3 + 1] * 0.587f +
				src.data[(i*src.sizeX + j) * 3 + 2] * 0.114f);
			if (v < threshold)
				dst.data[(i*src.sizeX + j) * 3] =
				dst.data[(i*src.sizeX + j) * 3 + 1] =
				dst.data[(i*src.sizeX + j) * 3 + 2] = 0;
			else
				dst.data[(i*src.sizeX + j) * 3] =
				dst.data[(i*src.sizeX + j) * 3 + 1] =
				dst.data[(i*src.sizeX + j) * 3 + 2] = 255;
		}
	}
	return dst;
}
bitMap zoomPic(bitMap src, float rate) {
	int i, j;
	float tmpx, tmpy, deltax, deltay;
	bitMap dst;
	dst.data = (unsigned char *)malloc((int)(src.sizeX * rate) * (int)(src.sizeY * rate) * 3 * sizeof(char));
	dst.sizeX = (int)(src.sizeX * rate);
	dst.sizeY = (int)(src.sizeY * rate);
	for (i = 0; i < dst.sizeY; i++) {
		for (j = 0; j < dst.sizeX; j++) {
			tmpx = j / rate;
			tmpy = i / rate;
			deltax = tmpx - (int)tmpx;
			deltay = tmpy - (int)tmpy;
			dst.data[(i*dst.sizeX + j) * 3] = (unsigned char)
				((src.data[((int)(i / rate + 1)*src.sizeX + (int)(j / rate)) * 3] * deltay +
					src.data[((int)(i / rate)*src.sizeX + (int)(j / rate)) * 3] * (1 - deltay))*(1 - deltax) +
					(src.data[((int)(i / rate + 1)*src.sizeX + (int)(j / rate + 1)) * 3] * deltay +
						src.data[((int)(i / rate)*src.sizeX + (int)(j / rate + 1)) * 3] * (1 - deltay))*deltax);
			dst.data[(i*dst.sizeX + j) * 3 + 1] = (unsigned char)
				((src.data[((int)(i / rate + 1)*src.sizeX + (int)(j / rate)) * 3 + 1] * deltay +
					src.data[((int)(i / rate)*src.sizeX + (int)(j / rate)) * 3 + 1] * (1 - deltay))*(1 - deltax) +
					(src.data[((int)(i / rate + 1)*src.sizeX + (int)(j / rate + 1)) * 3 + 1] * deltay +
						src.data[((int)(i / rate)*src.sizeX + (int)(j / rate + 1)) * 3 + 1] * (1 - deltay))*deltax);
			dst.data[(i*dst.sizeX + j) * 3 + 2] = (unsigned char)
				((src.data[((int)(i / rate + 1)*src.sizeX + (int)(j / rate)) * 3 + 2] * deltay +
					src.data[((int)(i / rate)*src.sizeX + (int)(j / rate)) * 3 + 2] * (1 - deltay))*(1 - deltax) +
					(src.data[((int)(i / rate + 1)*src.sizeX + (int)(j / rate + 1)) * 3 + 2] * deltay +
						src.data[((int)(i / rate)*src.sizeX + (int)(j / rate + 1)) * 3 + 2] * (1 - deltay))*deltax);
		}
	}
	return dst;
}
bitMap rotatePic(bitMap src, float angle) {
	return bitMap();
}
bitMap filterPic(bitMap src, int mode) {
	int i, j, v, x, y, z, deal, pos;
	float weight, sum;
	bitMap dst;
	dst.data = (unsigned char *)malloc(src.sizeX*src.sizeY * 3 * sizeof(char));

	dst.sizeX = src.sizeX;
	dst.sizeY = src.sizeY;
	switch (mode) {
	case SGL_FILTER:
		for (i = 0; i < src.sizeY; i++) {
			for (j = 0; j < src.sizeX; j++) {
				pos = (i*src.sizeX + j) * 3;
				if (i == 0 || j == 0 || i == src.sizeY - 1 || j == src.sizeX - 1)
					memcpy(dst.data + pos, src.data + pos, 3 * sizeof(char));
				else {
					for (z = 0; z < 3; z++) {
						deal = 0;
						v = src.data[pos + z];
						for (y = 0; y < 3; y++) {
							for (x = 0; x < 3; x++) {
								if (abs(v - src.data[pos + ((y - 1)*src.sizeX + x - 1) * 3 + z]) > 16)
									deal++;
							}
						}
						if (deal >= 8) {
							v = 0;
							for (y = 0; y < 3; y++) {
								for (x = 0; x < 3; x++) {
									if (x == 1 && y == 1)continue;
									v += src.data[pos + ((y - 1)*src.sizeX + x - 1) * 3 + z];
								}
							}
							dst.data[pos + z] = v / 8;
						}
						else dst.data[pos + z] = src.data[pos + z];
					}
				}
			}
		}
		break;
	case MEAN_FILTER:
		for (i = 0; i < src.sizeY; i++) {
			for (j = 0; j < src.sizeX; j++) {
				pos = (i*src.sizeX + j) * 3;
				if (i == 0 || j == 0 || i == src.sizeY - 1 || j == src.sizeX - 1)
					memcpy(dst.data + pos, src.data + pos, 3 * sizeof(char));
				else {
					for (z = 0; z < 3; z++) {
						v = 0;
						for (y = 0; y < 3; y++) {
							for (x = 0; x < 3; x++) {
								v += src.data[pos + ((y - 1)*src.sizeX + x - 1) * 3 + z];
							}
						}
						dst.data[pos + z] = v / 9;
					}
				}
			}
		}
		break;
	case LAPLACIAN_FILTER:
		for (i = 0; i < src.sizeY; i++) {
			for (j = 0; j < src.sizeX; j++) {
				pos = (i*src.sizeX + j) * 3;
				if (i == 0 || j == 0 || i == src.sizeY - 1 || j == src.sizeX - 1)
					memcpy(dst.data + pos, src.data + pos, 3 * sizeof(char));
				else {
					for (z = 0; z < 3; z++) {
						v = 0;
						v += src.data[pos - 3 + z];
						v += src.data[pos + 3 + z];
						v += src.data[pos - src.sizeX * 3 + z];
						v += src.data[pos + src.sizeX * 3 + z];
						v -= 4 * src.data[pos + z];
						dst.data[pos + z] = src.data[pos + z] + v / 9;
					}
				}
			}
		}
		break;
	case BILATERAL_FILTER:
		for (i = 0; i < src.sizeY; i++) {
			for (j = 0; j < src.sizeX; j++) {
				int pos = (i*src.sizeX + j) * 3;
				if (i == 0 || j == 0 || i == src.sizeY - 1 || j == src.sizeX - 1)
					memcpy(dst.data + pos, src.data + pos, 3 * sizeof(char));
				else {
					for (z = 0; z < 3; z++) {
						sum = 0;
						weight = 0;
						for (y = 0; y < 3; y++) {
							for (x = 0; x < 3; x++) {
								weight += (float)exp(-((x - 1)*(x - 1) + (y - 1)*(y - 1)) -
									((src.data[pos + z] - src.data[pos + ((y - 1)*src.sizeX + x - 1) * 3 + z])*
									(src.data[pos + z] - src.data[pos + ((y - 1)*src.sizeX + x - 1) * 3 + z])) / 1024);
								sum += src.data[pos + ((y - 1)*src.sizeX + x - 1) * 3 + z] *
									(float)exp(-((x - 1)*(x - 1) + (y - 1)*(y - 1)) -
									((src.data[pos + z] - src.data[pos + ((y - 1)*src.sizeX + x - 1) * 3 + z])*
										(src.data[pos + z] - src.data[pos + ((y - 1)*src.sizeX + x - 1) * 3 + z])) / 1024);
							}
						}
						dst.data[pos + z] = (int)(sum / weight);
					}
				}
			}
		}
		break;
	default:
		return bitMap();
	}
	return dst;
}
bitMap luminantPic(bitMap src, int delta) {
	int i, j, y, u, v;
	bitMap dst;
	dst.data = (unsigned char *)malloc(src.sizeX*src.sizeY * 3 * sizeof(char));

	dst.sizeX = src.sizeX;
	dst.sizeY = src.sizeY;
	for (i = 0; i < src.sizeY; i++) {
		for (j = 0; j < src.sizeX; j++) {
			y = (int)(src.data[(i*src.sizeX + j) * 3] * 0.299f +
				src.data[(i*src.sizeX + j) * 3 + 1] * 0.587f +
				src.data[(i*src.sizeX + j) * 3 + 2] * 0.114f);
			u = -(int)(src.data[(i*src.sizeX + j) * 3] * 0.417f -
				src.data[(i*src.sizeX + j) * 3 + 1] * 0.289f +
				src.data[(i*src.sizeX + j) * 3 + 2] * 0.436f);
			v = (int)(src.data[(i*src.sizeX + j) * 3] * 0.615f -
				src.data[(i*src.sizeX + j) * 3 + 1] * 0.515f -
				src.data[(i*src.sizeX + j) * 3 + 2] * 0.100f);

			y += delta;
			if (y > 255)y = 255;
			if (y < 0)y = 0;

			dst.data[(i*src.sizeX + j) * 3] = (char)(y + 1.14f * v);
			dst.data[(i*src.sizeX + j) * 3 + 1] = (char)(y - 0.39f * u - 0.58f * v);
			dst.data[(i*src.sizeX + j) * 3 + 2] = (char)(y + 2.03f * u);
		}
	}
	return dst;
}
bitMap contrastPic(bitMap src) {
	int i, j, y, u, v;
	char *gray = (char *)malloc(src.sizeX*src.sizeY * sizeof(char));
	float *prob = (float *)malloc(256 * sizeof(float));
	bitMap dst;
	dst.data = (unsigned char *)malloc(src.sizeX*src.sizeY * 3 * sizeof(char));

	for (i = 0; i < 256; i++) {
		prob[i] = 0.f;
	}
	for (i = 0; i < src.sizeY; i++) {
		for (j = 0; j < src.sizeX; j++) {
			gray[(i*src.sizeX + j)] = (int)(src.data[(i*src.sizeX + j) * 3] * 0.299f +
				src.data[(i*src.sizeX + j) * 3 + 1] * 0.587f +
				src.data[(i*src.sizeX + j) * 3 + 2] * 0.114f);
			prob[gray[(i*src.sizeX + j)]] += 1.f / (src.sizeX*src.sizeY);
		}
	}
	for (i = 1; i < 256; i++) {
		prob[i] += prob[i - 1];
	}

	dst.sizeX = src.sizeX;
	dst.sizeY = src.sizeY;
	for (i = 0; i < src.sizeY; i++) {
		for (j = 0; j < src.sizeX; j++) {
			y = (int)(src.data[(i*src.sizeX + j) * 3] * 0.299f +
				src.data[(i*src.sizeX + j) * 3 + 1] * 0.587f +
				src.data[(i*src.sizeX + j) * 3 + 2] * 0.114f);
			u = -(int)(src.data[(i*src.sizeX + j) * 3] * 0.417f -
				src.data[(i*src.sizeX + j) * 3 + 1] * 0.289f +
				src.data[(i*src.sizeX + j) * 3 + 2] * 0.436f);
			v = (int)(src.data[(i*src.sizeX + j) * 3] * 0.615f -
				src.data[(i*src.sizeX + j) * 3 + 1] * 0.515f -
				src.data[(i*src.sizeX + j) * 3 + 2] * 0.100f);

			y = (int)prob[y] * 255;

			dst.data[(i*src.sizeX + j) * 3] = (char)(y + 1.14f * v);
			dst.data[(i*src.sizeX + j) * 3 + 1] = (char)(y - 0.39f * u - 0.58f * v);
			dst.data[(i*src.sizeX + j) * 3 + 2] = (char)(y + 2.03f * u);
		}
	}

	free(gray);
	free(prob);
	return dst;
}
