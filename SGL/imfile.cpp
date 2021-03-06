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
	for (int i = 0; i < bmp.sizeY; i++) {
		fwrite(bmp.data + (bmp.sizeY - 1 - i) * lineByte, lineByte, 1, fp);
	}
	fclose(fp);
}





typedef enum {
	LCT_GREY = 0, /*grayscale: 1,2,4,8,16 bit*/
	LCT_RGB = 2, /*RGB: 8,16 bit*/
	LCT_PALETTE = 3, /*palette: 1,2,4,8 bit*/
	LCT_GREY_ALPHA = 4, /*grayscale with alpha: 8,16 bit*/
	LCT_RGBA = 6, /*RGB with alpha: 8,16 bit*/
	LCT_MAX_OCTET_VALUE = 255
} LoadPNGColorType;
typedef struct {
	LoadPNGColorType colortype; /*color type, see PNG standard or documentation further in this header file*/
	unsigned bitdepth;  /*bits per sample, see PNG standard or documentation further in this header file*/

	unsigned char* palette; /*palette in RGBARGBA... order. Must be either 0, or when allocated must have 1024 bytes*/
	size_t palettesize; /*palette size in number of colors (amount of used bytes is 4 * palettesize)*/

	unsigned key_defined; /*is a transparent color key given? 0 = false, 1 = true*/
	unsigned key_r;       /*red/grayscale component of color key*/
	unsigned key_g;       /*green component of color key*/
	unsigned key_b;       /*blue component of color key*/
} LoadPNGColorMode;
typedef struct {
	unsigned compression_method;/*compression method of the original file. Always 0.*/
	unsigned filter_method;     /*filter method of the original file*/
	unsigned interlace_method;  /*interlace method of the original file: 0=none, 1=Adam7*/
	LoadPNGColorMode color;     /*color type and bits, palette and transparency of the PNG file*/
} LoadPNGInfo;
typedef struct {
	unsigned colored; /*not grayscale*/
	unsigned key; /*image is not opaque and color key is possible instead of full alpha*/
	unsigned short key_r; /*key values, always as 16-bit, in 8-bit case the byte is duplicated, e.g. 65535 means 255*/
	unsigned short key_g;
	unsigned short key_b;
	unsigned alpha; /*image is not opaque and alpha channel or alpha palette required*/
	unsigned numcolors; /*amount of colors, up to 257. Not valid if bits == 16 or allow_palette is disabled.*/
	unsigned char palette[1024]; /*Remembers up to the first 256 RGBA colors, in no particular order, only valid when numcolors is valid*/
	unsigned bits; /*bits per channel (not for palette). 1,2 or 4 for grayscale only. 16 if 16-bit per channel required.*/
	size_t numpixels;

	unsigned allow_palette; /*default 1. if 0, disallow choosing palette colortype in auto_choose_color, and don't count numcolors*/
	unsigned allow_greyscale; /*default 1. if 0, choose RGB or RGBA even if the image only has gray colors*/
} LoadPNGColorStats;
typedef enum {
	LFS_ZERO = 0,
	LFS_ONE = 1,
	LFS_TWO = 2,
	LFS_THREE = 3,
	LFS_FOUR = 4,
	LFS_MINSUM,
	LFS_ENTROPY,
	LFS_BRUTE_FORCE,
	LFS_PREDEFINED
} LoadPNGFilterStrategy;

typedef struct {
	unsigned ignore_adler32; /*if 1, continue and don't give an error message if the Adler32 checksum is corrupted*/
	unsigned ignore_nlen; /*ignore complement of len checksum in uncompressed blocks*/
} LoadPNGDecompressSettings;
typedef struct {
	LoadPNGDecompressSettings zlibsettings; /*in here is the setting to ignore Adler32 checksums*/
	unsigned ignore_crc; /*ignore CRC checksums*/
	unsigned ignore_critical; /*ignore unknown critical chunks*/
	unsigned ignore_end;
} LoadPNGDecoderSettings;
typedef struct {
	unsigned btype; /*the block type for LZ (0, 1, 2 or 3, see zlib standard). Should be 2 for proper compression.*/
	unsigned use_lz77; /*whether or not to use LZ77. Should be 1 for proper compression.*/
	unsigned windowsize; /*must be a power of two <= 32768. higher compresses more but is slower. Default value: 2048.*/
	unsigned minmatch; /*minimum lz77 length. 3 is normally best, 6 can be better for some PNGs. Default: 0*/
	unsigned nicematch; /*stop searching if >= this length found. Set to 258 for best compression. Default: 128*/
	unsigned lazymatching; /*use lazy matching: better compression but a bit slower. Default: true*/
} LoadPNGCompressSettings;
typedef struct {
	LoadPNGCompressSettings zlibsettings; /*settings for the zlib encoder, such as window size, ...*/

	unsigned filter_palette_zero;
	LoadPNGFilterStrategy filter_strategy;
	const unsigned char* predefined_filters;
	unsigned force_palette;
} LoadPNGEncoderSettings;
typedef struct {
	LoadPNGDecoderSettings decoder; /*the decoding settings*/
	LoadPNGEncoderSettings encoder; /*the encoding settings*/
	LoadPNGColorMode info_raw; /*specifies the format in which you would like to get the raw pixel buffer*/
	LoadPNGInfo info_png; /*info of the PNG image obtained after decoding*/
	unsigned error;
} LoadPNGState;

class pngBits {
public:
	vector<unsigned char> *data;
	unsigned int bp;
	unsigned int buffer;

	void setData(vector<unsigned char> *data) {
		this->data = data;
		bp = buffer = 0;
	}
	void writeOrderBits(unsigned value, size_t nbits) {
		for (int i = 0; i != nbits; i++) {
			if ((bp & 7u) == 0)data->push_back(0);
			(*data)[data->size() - 1] |= ((value >> i) & 1) << (bp & 7u);
			++bp;
		}
	}
	void writeReverseBits(unsigned value, size_t nbits) {
		for (int i = 0; i != nbits; i++) {
			if ((bp & 7u) == 0)data->push_back(0);
			(*data)[data->size() - 1] |= ((value >> (nbits - 1u - i)) & 1u) << (bp & 7u);
			++bp;
		}
	}

	unsigned ensureBits9(size_t nbits) {
		size_t start = bp >> 3u;
		size_t size = data->size();
		if (start + 1u < size) {
			buffer = (*data)[start + 0] | ((*data)[start + 1] << 8u);
			buffer >>= (bp & 7u);
			return 1;
		}
		else {
			buffer = 0;
			if (start + 0u < size) buffer |= (*data)[start + 0];
			buffer >>= (bp & 7u);
			return bp + nbits <= data->size() * 8;
		}
	}
	unsigned ensureBits17(size_t nbits) {
		size_t start = bp >> 3u;
		size_t size = data->size();
		if (start + 2u < size) {
			buffer = (*data)[start + 0] | ((*data)[start + 1] << 8u) | ((*data)[start + 2] << 16u);
			buffer >>= (bp & 7u);
			return 1;
		}
		else {
			buffer = 0;
			if (start + 0u < size) buffer |= (*data)[start + 0];
			if (start + 1u < size) buffer |= ((*data)[start + 1] << 8u);
			buffer >>= (bp & 7u);
			return bp + nbits <= data->size() * 8;
		}
	}
	unsigned ensureBits25(size_t nbits) {
		size_t start = bp >> 3u;
		size_t size = data->size();
		if (start + 3u < size) {
			buffer = (*data)[start + 0] | ((*data)[start + 1] << 8u) | ((*data)[start + 2] << 16u) | ((*data)[start + 3] << 24u);
			buffer >>= (bp & 7u);
			return 1;
		}
		else {
			buffer = 0;
			if (start + 0u < size) buffer |= (*data)[start + 0];
			if (start + 1u < size) buffer |= ((*data)[start + 1] << 8u);
			if (start + 2u < size) buffer |= ((*data)[start + 2] << 16u);
			buffer >>= (bp & 7u);
			return bp + nbits <= data->size() * 8;
		}
	}
	unsigned ensureBits32(size_t nbits) {
		size_t start = bp >> 3u;
		size_t size = data->size();
		if (start + 4u < size) {
			buffer = (*data)[start + 0] | ((*data)[start + 1] << 8u) | ((*data)[start + 2] << 16u) | ((*data)[start + 3] << 24u);
			buffer >>= (bp & 7u);
			buffer |= (((*data)[start + 4] << 24u) << (8u - (bp & 7u)));
			return 1;
		}
		else {
			buffer = 0;
			if (start + 0u < size) buffer |= (*data)[start + 0];
			if (start + 1u < size) buffer |= ((*data)[start + 1] << 8u);
			if (start + 2u < size) buffer |= ((*data)[start + 2] << 16u);
			if (start + 3u < size) buffer |= ((*data)[start + 3] << 24u);
			buffer >>= (bp & 7u);
			return bp + nbits <= data->size() * 8;
		}
	}
	int peekBits(size_t nbits) {
		return buffer & ((1u << nbits) - 1u);
	}
	void advanceBits(size_t nbits) {
		buffer >>= nbits;
		bp += nbits;
	}
	int readBits(size_t nbits) {
		int result = peekBits(nbits);
		advanceBits(nbits);
		return result;
	}
};
static unsigned reverseBits(unsigned bits, unsigned num) {
	unsigned i, result = 0;
	for (i = 0; i < num; i++) result |= ((bits >> (num - i - 1u)) & 1u) << i;
	return result;
}
static unsigned readInt32(const unsigned char* buffer) {
	return (((unsigned)buffer[0] << 24u) | ((unsigned)buffer[1] << 16u) | ((unsigned)buffer[2] << 8u) | (unsigned)buffer[3]);
}
static void writeInt32(unsigned char* buffer, unsigned value) {
	buffer[0] = (unsigned char)((value >> 24) & 0xff);
	buffer[1] = (unsigned char)((value >> 16) & 0xff);
	buffer[2] = (unsigned char)((value >> 8) & 0xff);
	buffer[3] = (unsigned char)((value) & 0xff);
}


#define FIRST_LENGTH_CODE_INDEX 257
#define LAST_LENGTH_CODE_INDEX 285
#define NUM_DEFLATE_CODE_SYMBOLS 288
#define NUM_DISTANCE_SYMBOLS 32
#define NUM_CODE_LENGTH_CODES 19

static const unsigned LENGTHBASE[29] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258 };
static const unsigned LENGTHEXTRA[29] = { 0, 0, 0, 0, 0, 0, 0,  0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0 };
static const unsigned DISTANCEBASE[30] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577 };
static const unsigned DISTANCEEXTRA[30] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };
static const unsigned CLCL_ORDER[NUM_CODE_LENGTH_CODES] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };

#define FIRSTBITS 9u
#define INVALIDSYMBOL 65535u
typedef struct HuffmanTree {
	unsigned* codes; /*the huffman codes (bit patterns representing the symbols)*/
	unsigned* lengths; /*the lengths of the huffman codes*/
	unsigned maxbitlen; /*maximum number of bits a single code can get*/
	unsigned numcodes; /*number of symbols in the alphabet = number of codes*/

	unsigned char* table_len; /*length of symbol from lookup table, or max length if secondary lookup needed*/
	unsigned short* table_value; /*value of symbol from lookup table, or pointer to secondary table if needed*/
} HuffmanTree;
static void HuffmanTree_init(HuffmanTree* tree) {
	tree->codes = 0;
	tree->lengths = 0;
	tree->table_len = 0;
	tree->table_value = 0;
}
static void HuffmanTree_cleanup(HuffmanTree* tree) {
	free(tree->codes);
	free(tree->lengths);
	free(tree->table_len);
	free(tree->table_value);
}
static unsigned HuffmanTree_makeTable(HuffmanTree* tree) {
	static const unsigned headsize = 1u << FIRSTBITS; /*size of the first table*/
	static const unsigned mask = (1u << FIRSTBITS) /*headsize*/ - 1u;
	size_t i, numpresent, pointer, size; /*total table size*/
	unsigned* maxlens = (unsigned*)malloc(headsize * sizeof(unsigned));
	if (!maxlens) return 83; /*alloc fail*/

	memset(maxlens, 0, headsize * sizeof(*maxlens));
	for (i = 0; i < tree->numcodes; i++) {
		unsigned symbol = tree->codes[i];
		unsigned l = tree->lengths[i];
		unsigned index;
		if (l <= FIRSTBITS) continue;
		index = reverseBits(symbol >> (l - FIRSTBITS), FIRSTBITS);
		maxlens[index] = max(maxlens[index], l);
	}
	size = headsize;
	for (i = 0; i < headsize; ++i) {
		unsigned l = maxlens[i];
		if (l > FIRSTBITS) size += (1u << (l - FIRSTBITS));
	}
	tree->table_len = (unsigned char*)malloc(size * sizeof(*tree->table_len));
	tree->table_value = (unsigned short*)malloc(size * sizeof(*tree->table_value));
	if (!tree->table_len || !tree->table_value) {
		free(maxlens);
		return 83;
	}
	for (i = 0; i < size; ++i) tree->table_len[i] = 16;

	pointer = headsize;
	for (i = 0; i < headsize; ++i) {
		unsigned l = maxlens[i];
		if (l <= FIRSTBITS) continue;
		tree->table_len[i] = l;
		tree->table_value[i] = pointer;
		pointer += (1u << (l - FIRSTBITS));
	}
	free(maxlens);

	numpresent = 0;
	for (i = 0; i < tree->numcodes; ++i) {
		unsigned l = tree->lengths[i];
		unsigned symbol = tree->codes[i];
		unsigned reverse = reverseBits(symbol, l);
		if (l == 0) continue;
		numpresent++;

		if (l <= FIRSTBITS) {
			unsigned num = 1u << (FIRSTBITS - l);
			unsigned j;
			for (j = 0; j < num; ++j) {
				unsigned index = reverse | (j << l);
				if (tree->table_len[index] != 16) return 55;
				tree->table_len[index] = l;
				tree->table_value[index] = i;
			}
		}
		else {
			unsigned index = reverse & mask;
			unsigned maxlen = tree->table_len[index];
			unsigned tablelen = maxlen - FIRSTBITS;
			unsigned start = tree->table_value[index];
			unsigned num = 1u << (tablelen - (l - FIRSTBITS));
			unsigned j;
			if (maxlen < l) return 55;
			for (j = 0; j < num; ++j) {
				unsigned reverse2 = reverse >> FIRSTBITS;
				unsigned index2 = start + (reverse2 | (j << (l - FIRSTBITS)));
				tree->table_len[index2] = l;
				tree->table_value[index2] = i;
			}
		}
	}

	if (numpresent < 2) {
		for (i = 0; i < size; ++i) {
			if (tree->table_len[i] == 16) {
				tree->table_len[i] = (i < headsize) ? 1 : (FIRSTBITS + 1);
				tree->table_value[i] = INVALIDSYMBOL;
			}
		}
	}
	else {
		for (i = 0; i < size; ++i) {
			if (tree->table_len[i] == 16) return 55;
		}
	}

	return 0;
}
static unsigned HuffmanTree_makeFromLengths2(HuffmanTree* tree) {
	unsigned* blcount;
	unsigned* nextcode;
	unsigned error = 0;
	unsigned bits, n;

	tree->codes = (unsigned*)malloc(tree->numcodes * sizeof(unsigned));
	blcount = (unsigned*)malloc((tree->maxbitlen + 1) * sizeof(unsigned));
	nextcode = (unsigned*)malloc((tree->maxbitlen + 1) * sizeof(unsigned));
	if (!tree->codes || !blcount || !nextcode) error = 83; /*alloc fail*/

	if (!error) {
		for (n = 0; n != tree->maxbitlen + 1; n++) blcount[n] = nextcode[n] = 0;
		for (bits = 0; bits != tree->numcodes; ++bits) ++blcount[tree->lengths[bits]];
		for (bits = 1; bits <= tree->maxbitlen; ++bits) {
			nextcode[bits] = (nextcode[bits - 1] + blcount[bits - 1]) << 1u;
		}
		for (n = 0; n != tree->numcodes; ++n) {
			if (tree->lengths[n] != 0) {
				tree->codes[n] = nextcode[tree->lengths[n]]++;
				tree->codes[n] &= ((1u << tree->lengths[n]) - 1u);
			}
		}
	}

	free(blcount);
	free(nextcode);

	if (!error) error = HuffmanTree_makeTable(tree);
	return error;
}
static unsigned HuffmanTree_makeFromLengths(HuffmanTree* tree, const unsigned* bitlen, size_t numcodes, unsigned maxbitlen) {
	unsigned i;
	tree->lengths = (unsigned*)malloc(numcodes * sizeof(unsigned));
	if (!tree->lengths) return 83; /*alloc fail*/
	for (i = 0; i != numcodes; ++i) tree->lengths[i] = bitlen[i];
	tree->numcodes = (unsigned)numcodes; /*number of symbols*/
	tree->maxbitlen = maxbitlen;
	return HuffmanTree_makeFromLengths2(tree);
}

typedef struct BPMNode {
	int weight; /*the sum of all weights in this chain*/
	unsigned index; /*index of this leaf node (called "count" in the paper)*/
	struct BPMNode* tail; /*the next nodes in this chain (null if last)*/
	int in_use;
} BPMNode;
typedef struct BPMLists {
	unsigned memsize;
	BPMNode* memory;
	unsigned numfree;
	unsigned nextfree;
	BPMNode** freelist;
	unsigned listsize;
	BPMNode** chains0;
	BPMNode** chains1;
} BPMLists;
static BPMNode* bpmnode_create(BPMLists* lists, int weight, unsigned index, BPMNode* tail) {
	unsigned i;
	BPMNode* result;

	if (lists->nextfree >= lists->numfree) {
		for (i = 0; i != lists->memsize; ++i) lists->memory[i].in_use = 0;
		for (i = 0; i != lists->listsize; ++i) {
			BPMNode* node;
			for (node = lists->chains0[i]; node != 0; node = node->tail) node->in_use = 1;
			for (node = lists->chains1[i]; node != 0; node = node->tail) node->in_use = 1;
		}
		lists->numfree = 0;
		for (i = 0; i != lists->memsize; ++i) {
			if (!lists->memory[i].in_use) lists->freelist[lists->numfree++] = &lists->memory[i];
		}
		lists->nextfree = 0;
	}

	result = lists->freelist[lists->nextfree++];
	result->weight = weight;
	result->index = index;
	result->tail = tail;
	return result;
}
static void bpmnode_sort(BPMNode* leaves, size_t num) {
	BPMNode* mem = (BPMNode*)malloc(sizeof(*leaves) * num);
	size_t width, counter = 0;
	for (width = 1; width < num; width *= 2) {
		BPMNode* a = (counter & 1) ? mem : leaves;
		BPMNode* b = (counter & 1) ? leaves : mem;
		size_t p;
		for (p = 0; p < num; p += 2 * width) {
			size_t q = (p + width > num) ? num : (p + width);
			size_t r = (p + 2 * width > num) ? num : (p + 2 * width);
			size_t i = p, j = q, k;
			for (k = p; k < r; k++) {
				if (i < q && (j >= r || a[i].weight <= a[j].weight)) b[k] = a[i++];
				else b[k] = a[j++];
			}
		}
		counter++;
	}
	if (counter & 1) memcpy(leaves, mem, sizeof(*leaves) * num);
	free(mem);
}
static void boundaryPM(BPMLists* lists, BPMNode* leaves, size_t numpresent, int c, int num) {
	unsigned lastindex = lists->chains1[c]->index;

	if (c == 0) {
		if (lastindex >= numpresent) return;
		lists->chains0[c] = lists->chains1[c];
		lists->chains1[c] = bpmnode_create(lists, leaves[lastindex].weight, lastindex + 1, 0);
	}
	else {
		int sum = lists->chains0[c - 1]->weight + lists->chains1[c - 1]->weight;
		lists->chains0[c] = lists->chains1[c];
		if (lastindex < numpresent && sum > leaves[lastindex].weight) {
			lists->chains1[c] = bpmnode_create(lists, leaves[lastindex].weight, lastindex + 1, lists->chains1[c]->tail);
			return;
		}
		lists->chains1[c] = bpmnode_create(lists, sum, lastindex, lists->chains1[c - 1]);
		if (num + 1 < (int)(2 * numpresent - 2)) {
			boundaryPM(lists, leaves, numpresent, c - 1, num);
			boundaryPM(lists, leaves, numpresent, c - 1, num);
		}
	}
}
unsigned loadpng_huffman_code_lengths(unsigned* lengths, const unsigned* frequencies, size_t numcodes, unsigned maxbitlen) {
	unsigned error = 0;
	unsigned i;
	size_t numpresent = 0; /*number of symbols with non-zero frequency*/
	BPMNode* leaves; /*the symbols, only those with > 0 frequency*/

	if (numcodes == 0) return 80; /*error: a tree of 0 symbols is not supposed to be made*/
	if ((1u << maxbitlen) < (unsigned)numcodes) return 80; /*error: represent all symbols*/

	leaves = (BPMNode*)malloc(numcodes * sizeof(*leaves));
	if (!leaves) return 83;

	for (i = 0; i != numcodes; ++i) {
		if (frequencies[i] > 0) {
			leaves[numpresent].weight = (int)frequencies[i];
			leaves[numpresent].index = i;
			++numpresent;
		}
	}

	memset(lengths, 0, numcodes * sizeof(*lengths));

	/*ensure at least two present symbols. There should be at least one symbol
	according to RFC 1951 section 3.2.7. Some decoders incorrectly require two. To
	make these work as well ensure there are at least two symbols. The
	Package-Merge code below also doesn't work correctly if there's only one
	symbol, it'd give it the theoretical 0 bits but in practice zlib wants 1 bit*/
	if (numpresent == 0) {
		lengths[0] = lengths[1] = 1; /*note that for RFC 1951 section 3.2.7, only lengths[0] = 1 is needed*/
	}
	else if (numpresent == 1) {
		lengths[leaves[0].index] = 1;
		lengths[leaves[0].index == 0 ? 1 : 0] = 1;
	}
	else {
		BPMLists lists;
		BPMNode* node;

		bpmnode_sort(leaves, numpresent);

		lists.listsize = maxbitlen;
		lists.memsize = 2 * maxbitlen * (maxbitlen + 1);
		lists.nextfree = 0;
		lists.numfree = lists.memsize;
		lists.memory = (BPMNode*)malloc(lists.memsize * sizeof(*lists.memory));
		lists.freelist = (BPMNode**)malloc(lists.memsize * sizeof(BPMNode*));
		lists.chains0 = (BPMNode**)malloc(lists.listsize * sizeof(BPMNode*));
		lists.chains1 = (BPMNode**)malloc(lists.listsize * sizeof(BPMNode*));
		if (!lists.memory || !lists.freelist || !lists.chains0 || !lists.chains1) error = 83; /*alloc fail*/

		if (!error) {
			for (i = 0; i != lists.memsize; ++i) lists.freelist[i] = &lists.memory[i];

			bpmnode_create(&lists, leaves[0].weight, 1, 0);
			bpmnode_create(&lists, leaves[1].weight, 2, 0);

			for (i = 0; i != lists.listsize; ++i) {
				lists.chains0[i] = &lists.memory[0];
				lists.chains1[i] = &lists.memory[1];
			}

			/*each boundaryPM call adds one chain to the last list, and we need 2 * numpresent - 2 chains.*/
			for (i = 2; i != 2 * numpresent - 2; ++i) boundaryPM(&lists, leaves, numpresent, (int)maxbitlen - 1, (int)i);

			for (node = lists.chains1[maxbitlen - 1]; node; node = node->tail) {
				for (i = 0; i != node->index; ++i) ++lengths[leaves[i].index];
			}
		}

		free(lists.memory);
		free(lists.freelist);
		free(lists.chains0);
		free(lists.chains1);
	}

	free(leaves);
	return error;
}
static unsigned HuffmanTree_makeFromFrequencies(HuffmanTree* tree, const unsigned* frequencies, size_t mincodes, size_t numcodes, unsigned maxbitlen) {
	unsigned error = 0;
	while (!frequencies[numcodes - 1] && numcodes > mincodes) --numcodes; /*trim zeroes*/
	tree->lengths = (unsigned*)malloc(numcodes * sizeof(unsigned));
	if (!tree->lengths) return 83; /*alloc fail*/
	tree->maxbitlen = maxbitlen;
	tree->numcodes = (unsigned)numcodes; /*number of symbols*/

	error = loadpng_huffman_code_lengths(tree->lengths, frequencies, numcodes, maxbitlen);
	if (!error) error = HuffmanTree_makeFromLengths2(tree);
	return error;
}
static unsigned huffmanDecodeSymbol(pngBits *bits, const HuffmanTree* codetree) {
	unsigned short code = bits->peekBits(FIRSTBITS);
	unsigned short l = codetree->table_len[code];
	unsigned short value = codetree->table_value[code];
	if (l <= FIRSTBITS) {
		bits->advanceBits(l);
		return value;
	}
	else {
		unsigned index2;
		bits->advanceBits(FIRSTBITS);
		index2 = value + bits->peekBits(l - FIRSTBITS);
		bits->advanceBits(codetree->table_len[index2] - FIRSTBITS);
		return codetree->table_value[index2];
	}
}
static unsigned generateFixedLitLenTree(HuffmanTree* tree) {
	unsigned i, error = 0;
	unsigned* bitlen = (unsigned*)malloc(NUM_DEFLATE_CODE_SYMBOLS * sizeof(unsigned));
	if (!bitlen) return 83; /*alloc fail*/
	for (i = 0; i <= 143; ++i) bitlen[i] = 8;
	for (i = 144; i <= 255; ++i) bitlen[i] = 9;
	for (i = 256; i <= 279; ++i) bitlen[i] = 7;
	for (i = 280; i <= 287; ++i) bitlen[i] = 8;

	error = HuffmanTree_makeFromLengths(tree, bitlen, NUM_DEFLATE_CODE_SYMBOLS, 15);
	free(bitlen);
	return error;
}
static unsigned generateFixedDistanceTree(HuffmanTree* tree) {
	unsigned i, error = 0;
	unsigned* bitlen = (unsigned*)malloc(NUM_DISTANCE_SYMBOLS * sizeof(unsigned));
	if (!bitlen) return 83; /*alloc fail*/
	for (i = 0; i != NUM_DISTANCE_SYMBOLS; ++i) bitlen[i] = 5;
	error = HuffmanTree_makeFromLengths(tree, bitlen, NUM_DISTANCE_SYMBOLS, 15);

	free(bitlen);
	return error;
}

static const size_t MAX_SUPPORTED_DEFLATE_LENGTH = 258;
static size_t searchCodeIndex(const unsigned* array, size_t array_size, size_t value) {
	size_t left = 1;
	size_t right = array_size - 1;

	while (left <= right) {
		size_t mid = (left + right) >> 1;
		if (array[mid] >= value) right = mid - 1;
		else left = mid + 1;
	}
	if (left >= array_size || array[left] > value) left--;
	return left;
}
static void addLengthDistance(vector<unsigned int> &values, size_t length, size_t distance) {
	unsigned length_code = (unsigned)searchCodeIndex(LENGTHBASE, 29, length);
	unsigned extra_length = (unsigned)(length - LENGTHBASE[length_code]);
	unsigned dist_code = (unsigned)searchCodeIndex(DISTANCEBASE, 30, distance);
	unsigned extra_distance = (unsigned)(distance - DISTANCEBASE[dist_code]);

	values.push_back(length_code + FIRST_LENGTH_CODE_INDEX);
	values.push_back(extra_length);
	values.push_back(dist_code);
	values.push_back(extra_distance);
}
static const unsigned HASH_NUM_VALUES = 65536;
static const unsigned HASH_BIT_MASK = 65535; /*HASH_NUM_VALUES - 1, but C90 does not like that as initializer*/
typedef struct Hash {
	int* head; /*hash value to head circular pos - can be outdated if went around window*/
	unsigned short* chain;
	int* val; /*circular pos to hash value*/

	int* headz; /*similar to head, but for chainz*/
	unsigned short* chainz; /*those with same amount of zeros*/
	unsigned short* zeros; /*length of zeros streak, used as a second hash chain*/
} Hash;
static unsigned hash_init(Hash* hash, unsigned windowsize) {
	unsigned i;
	hash->head = (int*)malloc(sizeof(int) * HASH_NUM_VALUES);
	hash->val = (int*)malloc(sizeof(int) * windowsize);
	hash->chain = (unsigned short*)malloc(sizeof(unsigned short) * windowsize);

	hash->zeros = (unsigned short*)malloc(sizeof(unsigned short) * windowsize);
	hash->headz = (int*)malloc(sizeof(int) * (MAX_SUPPORTED_DEFLATE_LENGTH + 1));
	hash->chainz = (unsigned short*)malloc(sizeof(unsigned short) * windowsize);

	if (!hash->head || !hash->chain || !hash->val || !hash->headz || !hash->chainz || !hash->zeros) {
		return 83; /*alloc fail*/
	}

	for (i = 0; i != HASH_NUM_VALUES; ++i) hash->head[i] = -1;
	for (i = 0; i != windowsize; ++i) hash->val[i] = -1;
	for (i = 0; i != windowsize; ++i) hash->chain[i] = i; /*same value as index indicates uninitialized*/

	for (i = 0; i <= MAX_SUPPORTED_DEFLATE_LENGTH; ++i) hash->headz[i] = -1;
	for (i = 0; i != windowsize; ++i) hash->chainz[i] = i; /*same value as index indicates uninitialized*/

	return 0;
}
static void hash_cleanup(Hash* hash) {
	free(hash->head);
	free(hash->val);
	free(hash->chain);

	free(hash->zeros);
	free(hash->headz);
	free(hash->chainz);
}
static unsigned getHash(const unsigned char* data, size_t size, size_t pos) {
	unsigned result = 0;
	if (pos + 2 < size) {
		result ^= ((unsigned)data[pos + 0] << 0u);
		result ^= ((unsigned)data[pos + 1] << 4u);
		result ^= ((unsigned)data[pos + 2] << 8u);
	}
	else {
		size_t amount, i;
		if (pos >= size) return 0;
		amount = size - pos;
		for (i = 0; i != amount; ++i) result ^= ((unsigned)data[pos + i] << (i * 8u));
	}
	return result & HASH_BIT_MASK;
}
static unsigned countZeros(const unsigned char* data, size_t size, size_t pos) {
	const unsigned char* start = data + pos;
	const unsigned char* end = start + MAX_SUPPORTED_DEFLATE_LENGTH;
	if (end > data + size) end = data + size;
	data = start;
	while (data != end && *data == 0) ++data;
	return (unsigned)(data - start);
}
static void updateHashChain(Hash* hash, size_t wpos, unsigned hashval, unsigned short numzeros) {
	hash->val[wpos] = (int)hashval;
	if (hash->head[hashval] != -1) hash->chain[wpos] = hash->head[hashval];
	hash->head[hashval] = (int)wpos;

	hash->zeros[wpos] = numzeros;
	if (hash->headz[numzeros] != -1) hash->chainz[wpos] = hash->headz[numzeros];
	hash->headz[numzeros] = (int)wpos;
}

static unsigned encodeLZ77(vector<unsigned int> &out, Hash* hash, const unsigned char* in, size_t inpos, size_t insize, unsigned windowsize, unsigned minmatch, unsigned nicematch, unsigned lazymatching) {
	size_t pos;
	unsigned i, error = 0;
	unsigned maxchainlength = windowsize >= 8192 ? windowsize : windowsize / 8u;
	unsigned maxlazymatch = windowsize >= 8192 ? MAX_SUPPORTED_DEFLATE_LENGTH : 64;

	unsigned usezeros = 1; /*not sure if setting it to false for windowsize < 8192 is better or worse*/
	unsigned numzeros = 0;

	unsigned offset; /*the offset represents the distance in LZ77 terminology*/
	unsigned length;
	unsigned lazy = 0;
	unsigned lazylength = 0, lazyoffset = 0;
	unsigned hashval;
	unsigned current_offset, current_length;
	unsigned prev_offset;
	const unsigned char *lastptr, *foreptr, *backptr;
	unsigned hashpos;

	if (windowsize == 0 || windowsize > 32768) return 60; /*error: windowsize smaller/larger than allowed*/
	if ((windowsize & (windowsize - 1)) != 0) return 90; /*error: must be power of two*/

	if (nicematch > MAX_SUPPORTED_DEFLATE_LENGTH) nicematch = MAX_SUPPORTED_DEFLATE_LENGTH;

	for (pos = inpos; pos < insize; ++pos) {
		size_t wpos = pos & (windowsize - 1); /*position for in 'circular' hash buffers*/
		unsigned chainlength = 0;

		hashval = getHash(in, insize, pos);
		if (usezeros && hashval == 0) {
			if (numzeros == 0) numzeros = countZeros(in, insize, pos);
			else if (pos + numzeros > insize || in[pos + numzeros - 1] != 0) --numzeros;
		}
		else {
			numzeros = 0;
		}
		updateHashChain(hash, wpos, hashval, numzeros);
		length = 0;
		offset = 0;
		hashpos = hash->chain[wpos];
		lastptr = &in[insize < pos + MAX_SUPPORTED_DEFLATE_LENGTH ? insize : pos + MAX_SUPPORTED_DEFLATE_LENGTH];
		prev_offset = 0;
		for (;;) {
			if (chainlength++ >= maxchainlength) break;
			current_offset = (unsigned)(hashpos <= wpos ? wpos - hashpos : wpos - hashpos + windowsize);

			if (current_offset < prev_offset) break; /*stop when went completely around the circular buffer*/
			prev_offset = current_offset;
			if (current_offset > 0) {
				foreptr = &in[pos];
				backptr = &in[pos - current_offset];
				if (numzeros >= 3) {
					unsigned skip = hash->zeros[hashpos];
					if (skip > numzeros) skip = numzeros;
					backptr += skip;
					foreptr += skip;
				}

				while (foreptr != lastptr && *backptr == *foreptr) /*maximum supported length by deflate is max length*/ {
					++backptr;
					++foreptr;
				}
				current_length = (unsigned)(foreptr - &in[pos]);

				if (current_length > length) {
					length = current_length; /*the longest length*/
					offset = current_offset;
					if (current_length >= nicematch) break;
				}
			}
			if (hashpos == hash->chain[hashpos]) break;
			if (numzeros >= 3 && length > numzeros) {
				hashpos = hash->chainz[hashpos];
				if (hash->zeros[hashpos] != numzeros) break;
			}
			else {
				hashpos = hash->chain[hashpos];
				if (hash->val[hashpos] != (int)hashval) break;
			}
		}

		if (lazymatching) {
			if (!lazy && length >= 3 && length <= maxlazymatch && length < MAX_SUPPORTED_DEFLATE_LENGTH) {
				lazy = 1;
				lazylength = length;
				lazyoffset = offset;
				continue; /*try the next byte*/
			}
			if (lazy) {
				lazy = 0;
				if (pos == 0) {
					error = 81;
					break;
				}
				if (length > lazylength + 1) {
					out.push_back(in[pos - 1]);
				}
				else {
					length = lazylength;
					offset = lazyoffset;
					hash->head[hashval] = -1; /*the same hashchain update will be done, this ensures no wrong alteration*/
					hash->headz[numzeros] = -1; /*idem*/
					--pos;
				}
			}
		}
		if (length >= 3 && offset > windowsize) {
			error = 86;
			break;
		}
		if (length < 3) {
			out.push_back(in[pos]);
		}
		else if (length < minmatch || (length == 3 && offset > 4096)) {
			out.push_back(in[pos]);
		}
		else {
			addLengthDistance(out, length, offset);
			for (i = 1; i < length; ++i) {
				++pos;
				wpos = pos & (windowsize - 1);
				hashval = getHash(in, insize, pos);
				if (usezeros && hashval == 0) {
					if (numzeros == 0) numzeros = countZeros(in, insize, pos);
					else if (pos + numzeros > insize || in[pos + numzeros - 1] != 0) --numzeros;
				}
				else {
					numzeros = 0;
				}
				updateHashChain(hash, wpos, hashval, numzeros);
			}
		}
	}

	return error;
}
static unsigned deflateNoCompression(vector<unsigned char> &out, const unsigned char* data, size_t datasize) {
	size_t i, numdeflateblocks = (datasize + 65534u) / 65535u;
	unsigned datapos = 0;
	for (i = 0; i != numdeflateblocks; ++i) {
		unsigned BFINAL, BTYPE, LEN, NLEN;
		unsigned char firstbyte;
		size_t pos = out.size();

		BFINAL = (i == numdeflateblocks - 1);
		BTYPE = 0;

		LEN = 65535;
		if (datasize - datapos < 65535u) LEN = (unsigned)datasize - datapos;
		NLEN = 65535 - LEN;

		out.resize(out.size() + LEN + 5); /*alloc fail*/

		firstbyte = (unsigned char)(BFINAL + ((BTYPE & 1u) << 1u) + ((BTYPE & 2u) << 1u));
		out[pos + 0] = firstbyte;
		out[pos + 1] = (unsigned char)(LEN & 255);
		out[pos + 2] = (unsigned char)(LEN >> 8u);
		out[pos + 3] = (unsigned char)(NLEN & 255);
		out[pos + 4] = (unsigned char)(NLEN >> 8u);
		memcpy(out.data() + pos + 5, data + datapos, LEN);
		datapos += LEN;
	}

	return 0;
}
static void writeLZ77data(pngBits* bits, const vector<unsigned int> &lz77_encoded, const HuffmanTree* tree_ll, const HuffmanTree* tree_d) {
	size_t i = 0;
	for (i = 0; i != lz77_encoded.size(); ++i) {
		unsigned val = lz77_encoded[i];
		bits->writeReverseBits(tree_ll->codes[val], tree_ll->lengths[val]);
		if (val > 256) {
			unsigned length_index = val - FIRST_LENGTH_CODE_INDEX;
			unsigned n_length_extra_bits = LENGTHEXTRA[length_index];
			unsigned length_extra_bits = lz77_encoded[++i];

			unsigned distance_code = lz77_encoded[++i];

			unsigned distance_index = distance_code;
			unsigned n_distance_extra_bits = DISTANCEEXTRA[distance_index];
			unsigned distance_extra_bits = lz77_encoded[++i];

			bits->writeOrderBits(length_extra_bits, n_length_extra_bits);
			bits->writeReverseBits(tree_d->codes[distance_code], tree_d->lengths[distance_code]);
			bits->writeOrderBits(distance_extra_bits, n_distance_extra_bits);
		}
	}
}
static unsigned deflateDynamic(pngBits* bits, Hash* hash, const unsigned char* data, size_t datapos, size_t dataend, const LoadPNGCompressSettings* settings, unsigned final) {
	unsigned error = 0;
	vector<unsigned int> lz77_encoded;
	HuffmanTree tree_ll; /*tree for lit,len values*/
	HuffmanTree tree_d; /*tree for distance codes*/
	HuffmanTree tree_cl; /*tree for encoding the code lengths representing tree_ll and tree_d*/
	unsigned* frequencies_ll = 0; /*frequency of lit,len codes*/
	unsigned* frequencies_d = 0; /*frequency of dist codes*/
	unsigned* frequencies_cl = 0; /*frequency of code length codes*/
	unsigned* bitlen_lld = 0; /*lit,len,dist code lengths (int bits), literally (without repeat codes).*/
	unsigned* bitlen_lld_e = 0; /*bitlen_lld encoded with repeat codes (this is a rudimentary run length compression)*/
	size_t datasize = dataend - datapos;

	unsigned BFINAL = final;
	size_t i;
	size_t numcodes_ll, numcodes_d, numcodes_lld, numcodes_lld_e, numcodes_cl;
	unsigned HLIT, HDIST, HCLEN;

	HuffmanTree_init(&tree_ll);
	HuffmanTree_init(&tree_d);
	HuffmanTree_init(&tree_cl);
	frequencies_ll = (unsigned*)malloc(286 * sizeof(*frequencies_ll));
	frequencies_d = (unsigned*)malloc(30 * sizeof(*frequencies_d));
	frequencies_cl = (unsigned*)malloc(NUM_CODE_LENGTH_CODES * sizeof(*frequencies_cl));

	if (!frequencies_ll || !frequencies_d || !frequencies_cl) error = 83; /*alloc fail*/
	while (!error) {
		memset(frequencies_ll, 0, 286 * sizeof(*frequencies_ll));
		memset(frequencies_d, 0, 30 * sizeof(*frequencies_d));
		memset(frequencies_cl, 0, NUM_CODE_LENGTH_CODES * sizeof(*frequencies_cl));

		if (settings->use_lz77) {
			error = encodeLZ77(lz77_encoded, hash, data, datapos, dataend, settings->windowsize,
				settings->minmatch, settings->nicematch, settings->lazymatching);
			if (error) break;
		}
		else {
			lz77_encoded.resize(datasize);
			for (i = datapos; i < dataend; ++i) lz77_encoded[i - datapos] = data[i]; /*no LZ77, but still will be Huffman compressed*/
		}
		for (i = 0; i != lz77_encoded.size(); ++i) {
			unsigned symbol = lz77_encoded[i];
			++frequencies_ll[symbol];
			if (symbol > 256) {
				unsigned dist = lz77_encoded[i + 2];
				++frequencies_d[dist];
				i += 3;
			}
		}
		frequencies_ll[256] = 1; /*there will be exactly 1 end code, at the end of the block*/
		error = HuffmanTree_makeFromFrequencies(&tree_ll, frequencies_ll, 257, 286, 15);
		if (error) break;
		error = HuffmanTree_makeFromFrequencies(&tree_d, frequencies_d, 2, 30, 15);
		if (error) break;

		numcodes_ll = min(tree_ll.numcodes, 286);
		numcodes_d = min(tree_d.numcodes, 30);
		numcodes_lld = numcodes_ll + numcodes_d;
		bitlen_lld = (unsigned*)malloc(numcodes_lld * sizeof(*bitlen_lld));
		bitlen_lld_e = (unsigned*)malloc(numcodes_lld * sizeof(*bitlen_lld_e));
		if (!bitlen_lld || !bitlen_lld_e) {
			error = 83;
			break;
		}
		numcodes_lld_e = 0;

		for (i = 0; i != numcodes_ll; ++i) bitlen_lld[i] = tree_ll.lengths[i];
		for (i = 0; i != numcodes_d; ++i) bitlen_lld[numcodes_ll + i] = tree_d.lengths[i];
		for (i = 0; i != numcodes_lld; ++i) {
			unsigned j = 0;
			while (i + j + 1 < numcodes_lld && bitlen_lld[i + j + 1] == bitlen_lld[i]) ++j;

			if (bitlen_lld[i] == 0 && j >= 2) {
				++j;
				if (j <= 10) {
					bitlen_lld_e[numcodes_lld_e++] = 17;
					bitlen_lld_e[numcodes_lld_e++] = j - 3;
				}
				else {
					if (j > 138) j = 138;
					bitlen_lld_e[numcodes_lld_e++] = 18;
					bitlen_lld_e[numcodes_lld_e++] = j - 11;
				}
				i += (j - 1);
			}
			else if (j >= 3) {
				size_t k;
				unsigned num = j / 6u, rest = j % 6u;
				bitlen_lld_e[numcodes_lld_e++] = bitlen_lld[i];
				for (k = 0; k < num; ++k) {
					bitlen_lld_e[numcodes_lld_e++] = 16;
					bitlen_lld_e[numcodes_lld_e++] = 6 - 3;
				}
				if (rest >= 3) {
					bitlen_lld_e[numcodes_lld_e++] = 16;
					bitlen_lld_e[numcodes_lld_e++] = rest - 3;
				}
				else j -= rest;
				i += j;
			}
			else  {
				bitlen_lld_e[numcodes_lld_e++] = bitlen_lld[i];
			}
		}
		for (i = 0; i != numcodes_lld_e; ++i) {
			++frequencies_cl[bitlen_lld_e[i]];
			if (bitlen_lld_e[i] >= 16) ++i;
		}

		error = HuffmanTree_makeFromFrequencies(&tree_cl, frequencies_cl,
			NUM_CODE_LENGTH_CODES, NUM_CODE_LENGTH_CODES, 7);
		if (error) break;

		numcodes_cl = NUM_CODE_LENGTH_CODES;
		while (numcodes_cl > 4u && tree_cl.lengths[CLCL_ORDER[numcodes_cl - 1u]] == 0) {
			numcodes_cl--;
		}

		bits->writeOrderBits(BFINAL, 1);
		bits->writeOrderBits(0, 1);
		bits->writeOrderBits(1, 1);

		HLIT = (unsigned)(numcodes_ll - 257);
		HDIST = (unsigned)(numcodes_d - 1);
		HCLEN = (unsigned)(numcodes_cl - 4);
		bits->writeOrderBits(HLIT, 5);
		bits->writeOrderBits(HDIST, 5);
		bits->writeOrderBits(HCLEN, 4);
		for (i = 0; i != numcodes_cl; ++i) bits->writeOrderBits(tree_cl.lengths[CLCL_ORDER[i]], 3);
		for (i = 0; i != numcodes_lld_e; ++i) {
			bits->writeReverseBits(tree_cl.codes[bitlen_lld_e[i]], tree_cl.lengths[bitlen_lld_e[i]]);
			if (bitlen_lld_e[i] == 16) bits->writeOrderBits(bitlen_lld_e[++i], 2);
			else if (bitlen_lld_e[i] == 17) bits->writeOrderBits(bitlen_lld_e[++i], 3);
			else if (bitlen_lld_e[i] == 18) bits->writeOrderBits(bitlen_lld_e[++i], 7);
		}
		writeLZ77data(bits, lz77_encoded, &tree_ll, &tree_d);
		if (tree_ll.lengths[256] == 0) {
			error = 64;
			break;
		}
		bits->writeReverseBits(tree_ll.codes[256], tree_ll.lengths[256]);

		break;
	}
	HuffmanTree_cleanup(&tree_ll);
	HuffmanTree_cleanup(&tree_d);
	HuffmanTree_cleanup(&tree_cl);
	free(frequencies_ll);
	free(frequencies_d);
	free(frequencies_cl);
	free(bitlen_lld);
	free(bitlen_lld_e);

	return error;
}
static unsigned deflateFixed(pngBits* bits, Hash* hash, const unsigned char* data, size_t datapos, size_t dataend, const LoadPNGCompressSettings* settings, unsigned final) {
	HuffmanTree tree_ll; /*tree for literal values and length codes*/
	HuffmanTree tree_d; /*tree for distance codes*/

	unsigned BFINAL = final;
	unsigned error = 0;
	size_t i;

	HuffmanTree_init(&tree_ll);
	HuffmanTree_init(&tree_d);

	error = generateFixedLitLenTree(&tree_ll);
	if (!error) error = generateFixedDistanceTree(&tree_d);

	if (!error) {
		bits->writeOrderBits(BFINAL, 1);
		bits->writeOrderBits(1, 1);
		bits->writeOrderBits(0, 1);

		if (settings->use_lz77) {
			vector<unsigned int> lz77_encoded;
			error = encodeLZ77(lz77_encoded, hash, data, datapos, dataend, settings->windowsize,
				settings->minmatch, settings->nicematch, settings->lazymatching);
			if (!error) writeLZ77data(bits, lz77_encoded, &tree_ll, &tree_d);
		}
		else {
			for (i = datapos; i < dataend; ++i) {
				bits->writeReverseBits(tree_ll.codes[data[i]], tree_ll.lengths[data[i]]);
			}
		}
		if (!error) bits->writeReverseBits(tree_ll.codes[256], tree_ll.lengths[256]);
	}
	HuffmanTree_cleanup(&tree_ll);
	HuffmanTree_cleanup(&tree_d);

	return error;
}
static unsigned loadpng_deflatev(vector<unsigned char> &out, const unsigned char* in, size_t insize, const LoadPNGCompressSettings* settings) {
	unsigned error = 0;
	size_t i, blocksize, numdeflateblocks;
	Hash hash;
	pngBits bits;

	bits.setData(&out);

	if (settings->btype > 2) return 61;
	else if (settings->btype == 0) return deflateNoCompression(out, in, insize);
	else if (settings->btype == 1) blocksize = insize;
	else {
		blocksize = insize / 8u + 8;
		if (blocksize < 65536) blocksize = 65536;
		if (blocksize > 262144) blocksize = 262144;
	}

	numdeflateblocks = (insize + blocksize - 1) / blocksize;
	if (numdeflateblocks == 0) numdeflateblocks = 1;

	error = hash_init(&hash, settings->windowsize);

	if (!error) {
		for (i = 0; i != numdeflateblocks && !error; ++i) {
			unsigned final = (i == numdeflateblocks - 1);
			size_t start = i * blocksize;
			size_t end = start + blocksize;
			if (end > insize) end = insize;

			if (settings->btype == 1) error = deflateFixed(&bits, &hash, in, start, end, settings, final);
			else if (settings->btype == 2) error = deflateDynamic(&bits, &hash, in, start, end, settings, final);
		}
	}

	hash_cleanup(&hash);

	return error;
}
unsigned loadpng_deflate(unsigned char** out, size_t* outsize, const unsigned char* in, size_t insize, const LoadPNGCompressSettings* settings) {
	vector<unsigned char> v(*out, *out + *outsize);
	unsigned error = loadpng_deflatev(v, in, insize, settings);
	*out = (unsigned char *)malloc(v.size());
	memcpy(*out, v.data(), v.size());
	*outsize = v.size();
	return error;
}
static unsigned deflate(unsigned char** out, size_t* outsize, const unsigned char* in, size_t insize, const LoadPNGCompressSettings* settings) {
	return loadpng_deflate(out, outsize, in, insize, settings);
}

static unsigned getTreeInflateFixed(HuffmanTree* tree_ll, HuffmanTree* tree_d) {
	unsigned error = generateFixedLitLenTree(tree_ll);
	if (error) return error;
	return generateFixedDistanceTree(tree_d);
}
static unsigned getTreeInflateDynamic(HuffmanTree* tree_ll, HuffmanTree* tree_d, pngBits *bits) {
	unsigned error = 0;
	unsigned n, HLIT, HDIST, HCLEN, i;

	unsigned* bitlen_ll = 0; /*lit,len code lengths*/
	unsigned* bitlen_d = 0; /*dist code lengths*/
	unsigned* bitlen_cl = 0;
	HuffmanTree tree_cl; /*the code tree for code length codes (the huffman tree for compressed huffman trees)*/
	if (!bits->ensureBits17(14)) return 49; /*error: the bit pointer is or will go past the memory*/
	HLIT = bits->readBits(5) + 257;
	HDIST = bits->readBits(5) + 1;
	HCLEN = bits->readBits(4) + 4;

	bitlen_cl = (unsigned*)malloc(NUM_CODE_LENGTH_CODES * sizeof(unsigned));
	if (!bitlen_cl) return 83 /*alloc fail*/;

	HuffmanTree_init(&tree_cl);

	while (!error) {
		for (i = 0; i != HCLEN; ++i) {
			bits->ensureBits9(3); /*out of bounds already checked above */
			bitlen_cl[CLCL_ORDER[i]] = bits->readBits(3);
		}
		for (i = HCLEN; i != NUM_CODE_LENGTH_CODES; ++i) {
			bitlen_cl[CLCL_ORDER[i]] = 0;
		}

		error = HuffmanTree_makeFromLengths(&tree_cl, bitlen_cl, NUM_CODE_LENGTH_CODES, 7);
		if (error) break;
		bitlen_ll = (unsigned*)malloc(NUM_DEFLATE_CODE_SYMBOLS * sizeof(unsigned));
		bitlen_d = (unsigned*)malloc(NUM_DISTANCE_SYMBOLS * sizeof(unsigned));
		if (!bitlen_ll || !bitlen_d) {
			error = 83;
			break;
		}
		memset(bitlen_ll, 0, NUM_DEFLATE_CODE_SYMBOLS * sizeof(*bitlen_ll));
		memset(bitlen_d, 0, NUM_DISTANCE_SYMBOLS * sizeof(*bitlen_d));
		i = 0;
		while (i < HLIT + HDIST) {
			unsigned code;
			bits->ensureBits25(22); /* up to 15 bits for huffman code, up to 7 extra bits below*/
			code = huffmanDecodeSymbol(bits, &tree_cl);
			if (code <= 15) {
				if (i < HLIT)
					bitlen_ll[i] = code;
				else bitlen_d[i - HLIT] = code;
				++i;
			}
			else if (code == 16) {
				unsigned replength = 3; /*read in the 2 bits that indicate repeat length (3-6)*/
				unsigned value; /*set value to the previous code*/

				if (i == 0) {
					error = 54;
					break;
				}

				replength += bits->readBits(2);

				if (i < HLIT + 1) value = bitlen_ll[i - 1];
				else value = bitlen_d[i - HLIT - 1];
				for (n = 0; n < replength; ++n) {
					if (i >= HLIT + HDIST) {
						error = 13;
						break;
					}
					if (i < HLIT)
						bitlen_ll[i] = value;
					else bitlen_d[i - HLIT] = value;
					++i;
				}
			}
			else if (code == 17) {
				unsigned replength = 3; /*read in the bits that indicate repeat length*/
				replength += bits->readBits(3);
				for (n = 0; n < replength; ++n) {
					if (i >= HLIT + HDIST) {
						error = 14;
						break;
					}

					if (i < HLIT) bitlen_ll[i] = 0;
					else bitlen_d[i - HLIT] = 0;
					++i;
				}
			}
			else if (code == 18) {
				unsigned replength = 11; /*read in the bits that indicate repeat length*/
				replength += bits->readBits(7);
				for (n = 0; n < replength; ++n) {
					if (i >= HLIT + HDIST) {
						error = 15;
						break;
					}

					if (i < HLIT) bitlen_ll[i] = 0;
					else bitlen_d[i - HLIT] = 0;
					++i;
				}
			}
			else {
				error = 16;
				break;
			}
		}
		if (error) break;

		if (bitlen_ll[256] == 0) {
			error = 64;
			break;
		}
		error = HuffmanTree_makeFromLengths(tree_ll, bitlen_ll, NUM_DEFLATE_CODE_SYMBOLS, 15);
		if (error) break;
		error = HuffmanTree_makeFromLengths(tree_d, bitlen_d, NUM_DISTANCE_SYMBOLS, 15);

		break;
	}

	free(bitlen_cl);
	free(bitlen_ll);
	free(bitlen_d);
	HuffmanTree_cleanup(&tree_cl);

	return error;
}
static unsigned inflateHuffmanBlock(vector<unsigned char> &out, pngBits *bits, unsigned btype) {
	unsigned error = 0;
	HuffmanTree tree_ll; /*the huffman tree for literal and length codes*/
	HuffmanTree tree_d; /*the huffman tree for distance codes*/

	HuffmanTree_init(&tree_ll);
	HuffmanTree_init(&tree_d);

	if (btype == 1) error = getTreeInflateFixed(&tree_ll, &tree_d);
	else error = getTreeInflateDynamic(&tree_ll, &tree_d, bits);

	while (!error) {
		unsigned code_ll;
		bits->ensureBits25(20); /* up to 15 for the huffman symbol, up to 5 for the length extra bits */
		code_ll = huffmanDecodeSymbol(bits, &tree_ll);
		if (code_ll <= 255) /*literal symbol*/ {
			out.push_back((unsigned char)code_ll);
		}
		else if (code_ll >= FIRST_LENGTH_CODE_INDEX && code_ll <= LAST_LENGTH_CODE_INDEX) /*length code*/ {
			unsigned code_d, distance;
			unsigned numextrabits_l, numextrabits_d; /*extra bits for length and distance*/
			size_t start, backward, length;
			length = LENGTHBASE[code_ll - FIRST_LENGTH_CODE_INDEX];
			numextrabits_l = LENGTHEXTRA[code_ll - FIRST_LENGTH_CODE_INDEX];
			if (numextrabits_l != 0) {
				length += bits->readBits(numextrabits_l);
			}

			bits->ensureBits32(28); /* up to 15 for the huffman symbol, up to 13 for the extra bits */
			code_d = huffmanDecodeSymbol(bits, &tree_d);
			if (code_d > 29) {
				if (code_d <= 31) {
					error = 18;
					break;
				}
				else {
					error = 16;
					break;
				}
			}
			distance = DISTANCEBASE[code_d];
			numextrabits_d = DISTANCEEXTRA[code_d];
			if (numextrabits_d != 0) {
				distance += bits->readBits(numextrabits_d);
			}
			start = out.size();
			if (distance > start) {
				error = 52;
				break;
			}
			backward = start - distance;

			out.resize(out.size() + length);
			if (distance < length) {
				size_t forward;
				memcpy(out.data() + start, out.data() + backward, distance);
				start += distance;
				for (forward = distance; forward < length; ++forward) {
					out[start++] = out[backward++];
				}
			}
			else {
				memcpy(out.data() + start, out.data() + backward, length);
			}
		}
		else if (code_ll == 256) {
			break;
		}
		else {
			error = 16;
			break;
		}
	}

	HuffmanTree_cleanup(&tree_ll);
	HuffmanTree_cleanup(&tree_d);

	return error;
}
static unsigned inflateNoCompression(vector<unsigned char> &out, pngBits *bits, const LoadPNGDecompressSettings* settings) {
	size_t bytepos;
	size_t size = bits->data->size();
	unsigned LEN, NLEN, error = 0;

	bytepos = (bits->bp + 7u) >> 3u;
	if (bytepos + 4 >= size) return 52; /*error, bit pointer will jump past memory*/
	LEN = (*bits->data)[bytepos] + ((*bits->data)[bytepos + 1] << 8u); bytepos += 2;
	NLEN = (*bits->data)[bytepos] + ((*bits->data)[bytepos + 1] << 8u); bytepos += 2;
	if (!settings->ignore_nlen && LEN + NLEN != 65535) {
		return 21; /*error: NLEN is not one's complement of LEN*/
	}

	out.resize(out.size() + LEN);
	if (bytepos + LEN > size) return 23; /*error: reading outside of in buffer*/

	memcpy(out.data() + out.size() - LEN, bits->data->data() + bytepos, LEN);
	bytepos += LEN;

	bits->bp = bytepos << 3u;

	return error;
}
static unsigned loadpng_inflatev(vector<unsigned char> &out, vector<unsigned char> &in, const LoadPNGDecompressSettings* settings) {
	unsigned BFINAL = 0;
	pngBits bits;
	bits.setData(&in);

	int error;
	while (!BFINAL) {
		unsigned BTYPE;
		if (!bits.ensureBits9(3)) return 52; /*error, bit pointer will jump past memory*/
		BFINAL = bits.readBits(1);
		BTYPE = bits.readBits(2);

		if (BTYPE == 3) return 20; /*error: invalid BTYPE*/
		else if (BTYPE == 0) error = inflateNoCompression(out, &bits, settings); /*no compression*/
		else error = inflateHuffmanBlock(out, &bits, BTYPE); /*compression, BTYPE 01 or 10*/

		if (error) return error;
	}

	return error;
}
static unsigned inflatev(vector<unsigned char> &out, const unsigned char* indata, size_t insize, const LoadPNGDecompressSettings* settings) {
	vector<unsigned char> in(indata, indata + insize);
	return loadpng_inflatev(out, in, settings);
}

static unsigned update_adler32(unsigned adler, const unsigned char* data, unsigned len) {
	unsigned s1 = adler & 0xffffu;
	unsigned s2 = (adler >> 16u) & 0xffffu;

	while (len != 0u) {
		unsigned i;
		unsigned amount = len > 5552u ? 5552u : len;
		len -= amount;
		for (i = 0; i != amount; ++i) {
			s1 += (*data++);
			s2 += s1;
		}
		s1 %= 65521u;
		s2 %= 65521u;
	}

	return (s2 << 16u) | s1;
}
unsigned loadpng_zlib_compress(unsigned char** out, size_t* outsize, const unsigned char* in, size_t insize, const LoadPNGCompressSettings* settings) {
	size_t i;
	unsigned error;
	unsigned char* deflatedata = 0;
	size_t deflatesize = 0;

	error = deflate(&deflatedata, &deflatesize, in, insize, settings);

	*out = NULL;
	*outsize = 0;
	if (!error) {
		*outsize = deflatesize + 6;
		*out = (unsigned char*)malloc(*outsize);
		if (!*out) error = 83; /*alloc fail*/
	}

	if (!error) {
		unsigned ADLER32 = update_adler32(1, in, (unsigned)insize);
		unsigned CMF = 120; /*0b01111000: CM 8, CINFO 7. With CINFO 7, any window size up to 32768 can be used.*/
		unsigned FLEVEL = 0;
		unsigned FDICT = 0;
		unsigned CMFFLG = 256 * CMF + FDICT * 32 + FLEVEL * 64;
		unsigned FCHECK = 31 - CMFFLG % 31;
		CMFFLG += FCHECK;

		(*out)[0] = (unsigned char)(CMFFLG >> 8);
		(*out)[1] = (unsigned char)(CMFFLG & 255);
		for (i = 0; i != deflatesize; ++i) (*out)[i + 2] = deflatedata[i];
		writeInt32(&(*out)[*outsize - 4], ADLER32);
	}

	free(deflatedata);
	return error;
}
unsigned loadpng_zlib_decompress(vector<unsigned char> &out, const unsigned char* in, size_t insize, const LoadPNGDecompressSettings* settings) {
	unsigned error = 0;
	unsigned CM, CINFO, FDICT;

	if (insize < 2) return 53; /*error, size of zlib data too small*/
	if ((in[0] * 256 + in[1]) % 31 != 0) {
		return 24;
	}

	CM = in[0] & 15;
	CINFO = (in[0] >> 4) & 15;
	FDICT = (in[1] >> 5) & 1;

	if (CM != 8 || CINFO > 7) {
		return 25;
	}
	if (FDICT != 0) {
		return 26;
	}

	error = inflatev(out, in + 2, insize - 2, settings);
	if (error) return error;

	if (!settings->ignore_adler32) {
		unsigned ADLER32 = readInt32(&in[insize - 4]);
		unsigned checksum = update_adler32(1, out.data(), (unsigned)(out.size()));
		if (checksum != ADLER32) return 58; /*error, adler checksum not correct, data must be corrupted*/
	}

	return 0; /*no error*/
}
void loadpng_compress_settings_init(LoadPNGCompressSettings* settings) {
	settings->btype = 2;
	settings->use_lz77 = 1;
	settings->windowsize = 2048;
	settings->minmatch = 3;
	settings->nicematch = 128;
	settings->lazymatching = 1;
}
void loadpng_decompress_settings_init(LoadPNGDecompressSettings* settings) {
	settings->ignore_adler32 = 0;
	settings->ignore_nlen = 0;
}

static unsigned loadpng_crc32_table[256] = {
	0u, 1996959894u, 3993919788u, 2567524794u,  124634137u, 1886057615u, 3915621685u, 2657392035u,
	249268274u, 2044508324u, 3772115230u, 2547177864u,  162941995u, 2125561021u, 3887607047u, 2428444049u,
	498536548u, 1789927666u, 4089016648u, 2227061214u,  450548861u, 1843258603u, 4107580753u, 2211677639u,
	325883990u, 1684777152u, 4251122042u, 2321926636u,  335633487u, 1661365465u, 4195302755u, 2366115317u,
	997073096u, 1281953886u, 3579855332u, 2724688242u, 1006888145u, 1258607687u, 3524101629u, 2768942443u,
	901097722u, 1119000684u, 3686517206u, 2898065728u,  853044451u, 1172266101u, 3705015759u, 2882616665u,
	651767980u, 1373503546u, 3369554304u, 3218104598u,  565507253u, 1454621731u, 3485111705u, 3099436303u,
	671266974u, 1594198024u, 3322730930u, 2970347812u,  795835527u, 1483230225u, 3244367275u, 3060149565u,
	1994146192u,   31158534u, 2563907772u, 4023717930u, 1907459465u,  112637215u, 2680153253u, 3904427059u,
	2013776290u,  251722036u, 2517215374u, 3775830040u, 2137656763u,  141376813u, 2439277719u, 3865271297u,
	1802195444u,  476864866u, 2238001368u, 4066508878u, 1812370925u,  453092731u, 2181625025u, 4111451223u,
	1706088902u,  314042704u, 2344532202u, 4240017532u, 1658658271u,  366619977u, 2362670323u, 4224994405u,
	1303535960u,  984961486u, 2747007092u, 3569037538u, 1256170817u, 1037604311u, 2765210733u, 3554079995u,
	1131014506u,  879679996u, 2909243462u, 3663771856u, 1141124467u,  855842277u, 2852801631u, 3708648649u,
	1342533948u,  654459306u, 3188396048u, 3373015174u, 1466479909u,  544179635u, 3110523913u, 3462522015u,
	1591671054u,  702138776u, 2966460450u, 3352799412u, 1504918807u,  783551873u, 3082640443u, 3233442989u,
	3988292384u, 2596254646u,   62317068u, 1957810842u, 3939845945u, 2647816111u,   81470997u, 1943803523u,
	3814918930u, 2489596804u,  225274430u, 2053790376u, 3826175755u, 2466906013u,  167816743u, 2097651377u,
	4027552580u, 2265490386u,  503444072u, 1762050814u, 4150417245u, 2154129355u,  426522225u, 1852507879u,
	4275313526u, 2312317920u,  282753626u, 1742555852u, 4189708143u, 2394877945u,  397917763u, 1622183637u,
	3604390888u, 2714866558u,  953729732u, 1340076626u, 3518719985u, 2797360999u, 1068828381u, 1219638859u,
	3624741850u, 2936675148u,  906185462u, 1090812512u, 3747672003u, 2825379669u,  829329135u, 1181335161u,
	3412177804u, 3160834842u,  628085408u, 1382605366u, 3423369109u, 3138078467u,  570562233u, 1426400815u,
	3317316542u, 2998733608u,  733239954u, 1555261956u, 3268935591u, 3050360625u,  752459403u, 1541320221u,
	2607071920u, 3965973030u, 1969922972u,   40735498u, 2617837225u, 3943577151u, 1913087877u,   83908371u,
	2512341634u, 3803740692u, 2075208622u,  213261112u, 2463272603u, 3855990285u, 2094854071u,  198958881u,
	2262029012u, 4057260610u, 1759359992u,  534414190u, 2176718541u, 4139329115u, 1873836001u,  414664567u,
	2282248934u, 4279200368u, 1711684554u,  285281116u, 2405801727u, 4167216745u, 1634467795u,  376229701u,
	2685067896u, 3608007406u, 1308918612u,  956543938u, 2808555105u, 3495958263u, 1231636301u, 1047427035u,
	2932959818u, 3654703836u, 1088359270u,  936918000u, 2847714899u, 3736837829u, 1202900863u,  817233897u,
	3183342108u, 3401237130u, 1404277552u,  615818150u, 3134207493u, 3453421203u, 1423857449u,  601450431u,
	3009837614u, 3294710456u, 1567103746u,  711928724u, 3020668471u, 3272380065u, 1510334235u,  755167117u
};
unsigned loadpng_crc32(const unsigned char* data, size_t length) {
	unsigned r = 0xffffffffu;
	size_t i;
	for (i = 0; i < length; ++i) {
		r = loadpng_crc32_table[(r ^ data[i]) & 0xffu] ^ (r >> 8u);
	}
	return r ^ 0xffffffffu;
}
static unsigned char readBitFromReversedStream(size_t* bitpointer, const unsigned char* bitstream) {
	unsigned char result = (unsigned char)((bitstream[(*bitpointer) >> 3] >> (7 - ((*bitpointer) & 0x7))) & 1);
	++(*bitpointer);
	return result;
}
static unsigned readBitsFromReversedStream(size_t* bitpointer, const unsigned char* bitstream, size_t nbits) {
	unsigned result = 0;
	size_t i;
	for (i = 0; i < nbits; ++i) {
		result <<= 1u;
		result |= (unsigned)readBitFromReversedStream(bitpointer, bitstream);
	}
	return result;
}
static void setBitOfReversedStream(size_t* bitpointer, unsigned char* bitstream, unsigned char bit) {
	if (bit == 0) bitstream[(*bitpointer) >> 3u] &= (unsigned char)(~(1u << (7u - ((*bitpointer) & 7u))));
	else bitstream[(*bitpointer) >> 3u] |= (1u << (7u - ((*bitpointer) & 7u)));
	++(*bitpointer);
}
unsigned char loadpng_chunk_type_equals(const unsigned char* chunk, const char* type) {
	if (strlen(type) != 4) return 0;
	return (chunk[4] == type[0] && chunk[5] == type[1] && chunk[6] == type[2] && chunk[7] == type[3]);
}
unsigned loadpng_chunk_check_crc(const unsigned char* chunk) {
	unsigned length = readInt32(chunk);
	unsigned CRC = readInt32(&chunk[length + 8]);
	unsigned checksum = loadpng_crc32(&chunk[4], length + 4);
	if (CRC != checksum) return 1;
	else return 0;
}
void loadpng_chunk_generate_crc(unsigned char* chunk) {
	unsigned length = readInt32(chunk);
	unsigned CRC = loadpng_crc32(&chunk[4], length + 4);
	writeInt32(chunk + 8 + length, CRC);
}
unsigned char* loadpng_chunk_next(unsigned char* chunk, unsigned char* end) {
	if (chunk >= end || end - chunk < 12) return end; /*too small to contain a chunk*/
	if (chunk[0] == 0x89 && chunk[1] == 0x50 && chunk[2] == 0x4e && chunk[3] == 0x47
		&& chunk[4] == 0x0d && chunk[5] == 0x0a && chunk[6] == 0x1a && chunk[7] == 0x0a) {
		return chunk + 8;
	}
	else {
		SGuint total_chunk_length;
		unsigned char* result;
		if (safeAdd(readInt32(chunk), 12, &total_chunk_length)) return end;
		result = chunk + total_chunk_length;
		if (result < chunk) return end; /*pointer overflow*/
		return result;
	}
}
const unsigned char* loadpng_chunk_next_const(const unsigned char* chunk, const unsigned char* end) {
	if (chunk >= end || end - chunk < 12) return end; /*too small to contain a chunk*/
	if (chunk[0] == 0x89 && chunk[1] == 0x50 && chunk[2] == 0x4e && chunk[3] == 0x47
		&& chunk[4] == 0x0d && chunk[5] == 0x0a && chunk[6] == 0x1a && chunk[7] == 0x0a) {
		return chunk + 8;
	}
	else {
		SGuint total_chunk_length;
		const unsigned char* result;
		if (safeAdd(readInt32(chunk), 12, &total_chunk_length)) return end;
		result = chunk + total_chunk_length;
		if (result < chunk) return end; /*pointer overflow*/
		return result;
	}
}
static unsigned loadpng_chunk_init(unsigned char** chunk, vector<unsigned char> &out, unsigned length, const char* type) {
	SGuint new_length = out.size();
	if (safeAdd(new_length, length, &new_length)) return 77;
	if (safeAdd(new_length, 12, &new_length)) return 77;
	out.resize(new_length);
	*chunk = out.data() + new_length - length - 12u;

	writeInt32(*chunk, length);
	memcpy(*chunk + 4, type, 4);

	return 0;
}
static unsigned loadpng_chunk_createv(vector<unsigned char> &out, unsigned length, const char* type, const unsigned char* data) {
	unsigned char* chunk;
	if (unsigned error = loadpng_chunk_init(&chunk, out, length, type))return error;

	memcpy(chunk + 8, data, length);
	loadpng_chunk_generate_crc(chunk);

	return 0;
}
static unsigned checkColorValidity(LoadPNGColorType colortype, unsigned bd) {
	switch (colortype) {
	case LCT_GREY:       if (!(bd == 1 || bd == 2 || bd == 4 || bd == 8 || bd == 16)) return 37; break;
	case LCT_RGB:        if (!(bd == 8 || bd == 16)) return 37; break;
	case LCT_PALETTE:    if (!(bd == 1 || bd == 2 || bd == 4 || bd == 8)) return 37; break;
	case LCT_GREY_ALPHA: if (!(bd == 8 || bd == 16)) return 37; break;
	case LCT_RGBA:       if (!(bd == 8 || bd == 16)) return 37; break;
	case LCT_MAX_OCTET_VALUE: return 31; /* invalid color type */
	default: return 31; /* invalid color type */
	}
	return 0; /*allowed color type / bits combination*/
}
static unsigned getNumColorChannels(LoadPNGColorType colortype) {
	switch (colortype) {
	case LCT_GREY: return 1;
	case LCT_RGB: return 3;
	case LCT_PALETTE: return 1;
	case LCT_GREY_ALPHA: return 2;
	case LCT_RGBA: return 4;
	case LCT_MAX_OCTET_VALUE: return 0; /* invalid color type */
	default: return 0; /*invalid color type*/
	}
}
void loadpng_color_mode_init(LoadPNGColorMode* info) {
	info->key_defined = 0;
	info->key_r = info->key_g = info->key_b = 0;
	info->colortype = LCT_RGBA;
	info->bitdepth = 8;
	info->palette = 0;
	info->palettesize = 0;
}
static void loadpng_color_mode_alloc_palette(LoadPNGColorMode* info) {
	size_t i;
	if (!info->palette) info->palette = (unsigned char*)malloc(1024);
	if (!info->palette) return; /*alloc fail*/
	for (i = 0; i != 256; ++i) {
		info->palette[i * 4 + 0] = 0;
		info->palette[i * 4 + 1] = 0;
		info->palette[i * 4 + 2] = 0;
		info->palette[i * 4 + 3] = 255;
	}
}
void loadpng_palette_clear(LoadPNGColorMode* info) {
	if (info->palette) free(info->palette);
	info->palette = 0;
	info->palettesize = 0;
}
unsigned loadpng_color_mode_copy(LoadPNGColorMode* dest, const LoadPNGColorMode* source) {
	loadpng_palette_clear(dest);
	memcpy(dest, source, sizeof(LoadPNGColorMode));
	if (source->palette) {
		dest->palette = (unsigned char*)malloc(1024);
		if (!dest->palette && source->palettesize) return 83; /*alloc fail*/
		memcpy(dest->palette, source->palette, source->palettesize * 4);
	}
	return 0;
}
static int loadpng_color_mode_equal(const LoadPNGColorMode* a, const LoadPNGColorMode* b) {
	size_t i;
	if (a->colortype != b->colortype) return 0;
	if (a->bitdepth != b->bitdepth) return 0;
	if (a->key_defined != b->key_defined) return 0;
	if (a->key_defined) {
		if (a->key_r != b->key_r) return 0;
		if (a->key_g != b->key_g) return 0;
		if (a->key_b != b->key_b) return 0;
	}
	if (a->palettesize != b->palettesize) return 0;
	for (i = 0; i != a->palettesize * 4; ++i) {
		if (a->palette[i] != b->palette[i]) return 0;
	}
	return 1;
}
unsigned loadpng_palette_add(LoadPNGColorMode* info, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	if (!info->palette) /*allocate palette if empty*/ {
		loadpng_color_mode_alloc_palette(info);
		if (!info->palette) return 83; /*alloc fail*/
	}
	if (info->palettesize >= 256) {
		return 108; /*too many palette values*/
	}
	info->palette[4 * info->palettesize + 0] = r;
	info->palette[4 * info->palettesize + 1] = g;
	info->palette[4 * info->palettesize + 2] = b;
	info->palette[4 * info->palettesize + 3] = a;
	++info->palettesize;
	return 0;
}
unsigned loadpng_has_palette_alpha(const LoadPNGColorMode* info) {
	size_t i;
	for (i = 0; i != info->palettesize; ++i) {
		if (info->palette[i * 4 + 3] < 255) return 1;
	}
	return 0;
}
static size_t loadpng_get_raw_size_lct(unsigned w, unsigned h, LoadPNGColorType colortype, unsigned bitdepth) {
	size_t bpp = getNumColorChannels(colortype) * bitdepth;
	size_t n = (size_t)w * (size_t)h;
	return ((n / 8u) * bpp) + ((n & 7u) * bpp + 7u) / 8u;
}
static size_t loadpng_get_raw_size_idat(unsigned w, unsigned h, unsigned bpp) {
	size_t line = ((size_t)(w / 8u) * bpp) + 1u + ((w & 7u) * bpp + 7u) / 8u;
	return (size_t)h * line;
}
static int loadpng_pixel_overflow(unsigned w, unsigned h, const LoadPNGColorMode* pngcolor, const LoadPNGColorMode* rawcolor) {
	SGuint bpp = max(getNumColorChannels(pngcolor->colortype) * pngcolor->bitdepth, getNumColorChannels(rawcolor->colortype) * rawcolor->bitdepth);
	SGuint numpixels, total;
	SGuint line; /* bytes per line in worst case */

	if (safeMul((size_t)w, (size_t)h, &numpixels)) return 1;
	if (safeMul(numpixels, 8, &total)) return 1; /* bit pointer with 8-bit color, or 8 bytes per channel color */

												 /* Bytes per scanline with the expression "(w / 8u) * bpp) + ((w & 7u) * bpp + 7u) / 8u" */
	if (safeMul((size_t)(w / 8u), bpp, &line)) return 1;
	if (safeAdd(line, ((w & 7u) * bpp + 7u) / 8u, &line)) return 1;

	if (safeAdd(line, 5, &line)) return 1; /* 5 bytes overhead per line: 1 filterbyte, 4 for Adam7 worst case */
	if (safeMul(line, h, &total)) return 1; /* Total bytes in worst case */

	return 0; /* no overflow */
}
void loadpng_info_init(LoadPNGInfo* info) {
	loadpng_color_mode_init(&info->color);
	info->interlace_method = 0;
	info->compression_method = 0;
	info->filter_method = 0;
}
unsigned loadpng_info_copy(LoadPNGInfo* dest, const LoadPNGInfo* source) {
	loadpng_palette_clear(&dest->color);
	memcpy(dest, source, sizeof(LoadPNGInfo));
	loadpng_color_mode_init(&dest->color);
	if (unsigned error = loadpng_color_mode_copy(&dest->color, &source->color))return error;
	return 0;
}
static void addColorBits(unsigned char* out, size_t index, unsigned bits, unsigned in) {
	unsigned m = bits == 1 ? 7 : bits == 2 ? 3 : 1; /*8 / bits - 1*/
	unsigned p = index & m;
	in &= (1u << bits) - 1u; /*filter out any other bits of the input value*/
	in = in << (bits * (m - p));
	if (p == 0) out[index * bits / 8u] = in;
	else out[index * bits / 8u] |= in;
}

typedef struct ColorTree {
	ColorTree* children[16]; /*up to 16 pointers to ColorTree of next level*/
	int index; /*the payload. Only has a meaningful value if this is in the last level*/
} ColorTree;
static void color_tree_init(ColorTree* tree) {
	memset(tree->children, 0, 16 * sizeof(*tree->children));
	tree->index = -1;
}
static void color_tree_cleanup(ColorTree* tree) {
	int i;
	for (i = 0; i != 16; ++i) {
		if (tree->children[i]) {
			color_tree_cleanup(tree->children[i]);
			free(tree->children[i]);
		}
	}
}
static int color_tree_get(ColorTree* tree, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	int bit = 0;
	for (bit = 0; bit < 8; ++bit) {
		int i = 8 * ((r >> bit) & 1) + 4 * ((g >> bit) & 1) + 2 * ((b >> bit) & 1) + 1 * ((a >> bit) & 1);
		if (!tree->children[i]) return -1;
		else tree = tree->children[i];
	}
	return tree ? tree->index : -1;
}
static int color_tree_has(ColorTree* tree, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	return color_tree_get(tree, r, g, b, a) >= 0;
}
static unsigned color_tree_add(ColorTree* tree, unsigned char r, unsigned char g, unsigned char b, unsigned char a, unsigned index) {
	int bit;
	for (bit = 0; bit < 8; ++bit) {
		int i = 8 * ((r >> bit) & 1) + 4 * ((g >> bit) & 1) + 2 * ((b >> bit) & 1) + 1 * ((a >> bit) & 1);
		if (!tree->children[i]) {
			tree->children[i] = (ColorTree*)malloc(sizeof(ColorTree));
			if (!tree->children[i]) return 83; /*alloc fail*/
			color_tree_init(tree->children[i]);
		}
		tree = tree->children[i];
	}
	tree->index = (int)index;
	return 0;
}
static unsigned rgba8ToPixel(unsigned char* out, size_t i, const LoadPNGColorMode* mode, ColorTree* tree, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	if (mode->colortype == LCT_GREY) {
		unsigned char gray = r; /*((unsigned short)r + g + b) / 3u;*/
		if (mode->bitdepth == 8) out[i] = gray;
		else if (mode->bitdepth == 16) out[i * 2 + 0] = out[i * 2 + 1] = gray;
		else {
			gray = ((unsigned)gray >> (8u - mode->bitdepth)) & ((1u << mode->bitdepth) - 1u);
			addColorBits(out, i, mode->bitdepth, gray);
		}
	}
	else if (mode->colortype == LCT_RGB) {
		if (mode->bitdepth == 8) {
			out[i * 3 + 0] = r;
			out[i * 3 + 1] = g;
			out[i * 3 + 2] = b;
		}
		else {
			out[i * 6 + 0] = out[i * 6 + 1] = r;
			out[i * 6 + 2] = out[i * 6 + 3] = g;
			out[i * 6 + 4] = out[i * 6 + 5] = b;
		}
	}
	else if (mode->colortype == LCT_PALETTE) {
		int index = color_tree_get(tree, r, g, b, a);
		if (index < 0) return 82; /*color not in palette*/
		if (mode->bitdepth == 8) out[i] = index;
		else addColorBits(out, i, mode->bitdepth, (unsigned)index);
	}
	else if (mode->colortype == LCT_GREY_ALPHA) {
		unsigned char gray = r; /*((unsigned short)r + g + b) / 3u;*/
		if (mode->bitdepth == 8) {
			out[i * 2 + 0] = gray;
			out[i * 2 + 1] = a;
		}
		else if (mode->bitdepth == 16) {
			out[i * 4 + 0] = out[i * 4 + 1] = gray;
			out[i * 4 + 2] = out[i * 4 + 3] = a;
		}
	}
	else if (mode->colortype == LCT_RGBA) {
		if (mode->bitdepth == 8) {
			out[i * 4 + 0] = r;
			out[i * 4 + 1] = g;
			out[i * 4 + 2] = b;
			out[i * 4 + 3] = a;
		}
		else {
			out[i * 8 + 0] = out[i * 8 + 1] = r;
			out[i * 8 + 2] = out[i * 8 + 3] = g;
			out[i * 8 + 4] = out[i * 8 + 5] = b;
			out[i * 8 + 6] = out[i * 8 + 7] = a;
		}
	}

	return 0; /*no error*/
}
static void rgba16ToPixel(unsigned char* out, size_t i, const LoadPNGColorMode* mode, unsigned short r, unsigned short g, unsigned short b, unsigned short a) {
	if (mode->colortype == LCT_GREY) {
		unsigned short gray = r; /*((unsigned)r + g + b) / 3u;*/
		out[i * 2 + 0] = (gray >> 8) & 255;
		out[i * 2 + 1] = gray & 255;
	}
	else if (mode->colortype == LCT_RGB) {
		out[i * 6 + 0] = (r >> 8) & 255;
		out[i * 6 + 1] = r & 255;
		out[i * 6 + 2] = (g >> 8) & 255;
		out[i * 6 + 3] = g & 255;
		out[i * 6 + 4] = (b >> 8) & 255;
		out[i * 6 + 5] = b & 255;
	}
	else if (mode->colortype == LCT_GREY_ALPHA) {
		unsigned short gray = r; /*((unsigned)r + g + b) / 3u;*/
		out[i * 4 + 0] = (gray >> 8) & 255;
		out[i * 4 + 1] = gray & 255;
		out[i * 4 + 2] = (a >> 8) & 255;
		out[i * 4 + 3] = a & 255;
	}
	else if (mode->colortype == LCT_RGBA) {
		out[i * 8 + 0] = (r >> 8) & 255;
		out[i * 8 + 1] = r & 255;
		out[i * 8 + 2] = (g >> 8) & 255;
		out[i * 8 + 3] = g & 255;
		out[i * 8 + 4] = (b >> 8) & 255;
		out[i * 8 + 5] = b & 255;
		out[i * 8 + 6] = (a >> 8) & 255;
		out[i * 8 + 7] = a & 255;
	}
}
static void getPixelColorRGBA8(unsigned char* r, unsigned char* g, unsigned char* b, unsigned char* a, const unsigned char* in, size_t i, const LoadPNGColorMode* mode) {
	if (mode->colortype == LCT_GREY) {
		if (mode->bitdepth == 8) {
			*r = *g = *b = in[i];
			if (mode->key_defined && *r == mode->key_r) *a = 0;
			else *a = 255;
		}
		else if (mode->bitdepth == 16) {
			*r = *g = *b = in[i * 2 + 0];
			if (mode->key_defined && 256U * in[i * 2 + 0] + in[i * 2 + 1] == mode->key_r) *a = 0;
			else *a = 255;
		}
		else {
			unsigned highest = ((1U << mode->bitdepth) - 1U); /*highest possible value for this bit depth*/
			size_t j = i * mode->bitdepth;
			unsigned value = readBitsFromReversedStream(&j, in, mode->bitdepth);
			*r = *g = *b = (value * 255) / highest;
			if (mode->key_defined && value == mode->key_r) *a = 0;
			else *a = 255;
		}
	}
	else if (mode->colortype == LCT_RGB) {
		if (mode->bitdepth == 8) {
			*r = in[i * 3 + 0]; *g = in[i * 3 + 1]; *b = in[i * 3 + 2];
			if (mode->key_defined && *r == mode->key_r && *g == mode->key_g && *b == mode->key_b) *a = 0;
			else *a = 255;
		}
		else {
			*r = in[i * 6 + 0];
			*g = in[i * 6 + 2];
			*b = in[i * 6 + 4];
			if (mode->key_defined && 256U * in[i * 6 + 0] + in[i * 6 + 1] == mode->key_r
				&& 256U * in[i * 6 + 2] + in[i * 6 + 3] == mode->key_g
				&& 256U * in[i * 6 + 4] + in[i * 6 + 5] == mode->key_b) *a = 0;
			else *a = 255;
		}
	}
	else if (mode->colortype == LCT_PALETTE) {
		unsigned index;
		if (mode->bitdepth == 8) index = in[i];
		else {
			size_t j = i * mode->bitdepth;
			index = readBitsFromReversedStream(&j, in, mode->bitdepth);
		}
		*r = mode->palette[index * 4 + 0];
		*g = mode->palette[index * 4 + 1];
		*b = mode->palette[index * 4 + 2];
		*a = mode->palette[index * 4 + 3];
	}
	else if (mode->colortype == LCT_GREY_ALPHA) {
		if (mode->bitdepth == 8) {
			*r = *g = *b = in[i * 2 + 0];
			*a = in[i * 2 + 1];
		}
		else {
			*r = *g = *b = in[i * 4 + 0];
			*a = in[i * 4 + 2];
		}
	}
	else if (mode->colortype == LCT_RGBA) {
		if (mode->bitdepth == 8) {
			*r = in[i * 4 + 0];
			*g = in[i * 4 + 1];
			*b = in[i * 4 + 2];
			*a = in[i * 4 + 3];
		}
		else {
			*r = in[i * 8 + 0];
			*g = in[i * 8 + 2];
			*b = in[i * 8 + 4];
			*a = in[i * 8 + 6];
		}
	}
}
static void getPixelColorsRGBA8(unsigned char* __restrict buffer, size_t numpixels, const unsigned char* __restrict in, const LoadPNGColorMode* mode) {
	unsigned num_channels = 4;
	size_t i;
	if (mode->colortype == LCT_GREY) {
		if (mode->bitdepth == 8) {
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				buffer[0] = buffer[1] = buffer[2] = in[i];
				buffer[3] = 255;
			}
			if (mode->key_defined) {
				buffer -= numpixels * num_channels;
				for (i = 0; i != numpixels; ++i, buffer += num_channels) {
					if (buffer[0] == mode->key_r) buffer[3] = 0;
				}
			}
		}
		else if (mode->bitdepth == 16) {
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				buffer[0] = buffer[1] = buffer[2] = in[i * 2];
				buffer[3] = mode->key_defined && 256U * in[i * 2 + 0] + in[i * 2 + 1] == mode->key_r ? 0 : 255;
			}
		}
		else {
			unsigned highest = ((1U << mode->bitdepth) - 1U); /*highest possible value for this bit depth*/
			size_t j = 0;
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				unsigned value = readBitsFromReversedStream(&j, in, mode->bitdepth);
				buffer[0] = buffer[1] = buffer[2] = (value * 255) / highest;
				buffer[3] = mode->key_defined && value == mode->key_r ? 0 : 255;
			}
		}
	}
	else if (mode->colortype == LCT_RGB) {
		if (mode->bitdepth == 8) {
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				memcpy(buffer, &in[i * 3], 3);
				buffer[3] = 255;
			}
			if (mode->key_defined) {
				buffer -= numpixels * num_channels;
				for (i = 0; i != numpixels; ++i, buffer += num_channels) {
					if (buffer[0] == mode->key_r && buffer[1] == mode->key_g && buffer[2] == mode->key_b) buffer[3] = 0;
				}
			}
		}
		else {
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				buffer[0] = in[i * 6 + 0];
				buffer[1] = in[i * 6 + 2];
				buffer[2] = in[i * 6 + 4];
				buffer[3] = mode->key_defined
					&& 256U * in[i * 6 + 0] + in[i * 6 + 1] == mode->key_r
					&& 256U * in[i * 6 + 2] + in[i * 6 + 3] == mode->key_g
					&& 256U * in[i * 6 + 4] + in[i * 6 + 5] == mode->key_b ? 0 : 255;
			}
		}
	}
	else if (mode->colortype == LCT_PALETTE) {
		if (mode->bitdepth == 8) {
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				unsigned index = in[i];
				memcpy(buffer, &mode->palette[index * 4], 4);
			}
		}
		else {
			size_t j = 0;
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				unsigned index = readBitsFromReversedStream(&j, in, mode->bitdepth);
				memcpy(buffer, &mode->palette[index * 4], 4);
			}
		}
	}
	else if (mode->colortype == LCT_GREY_ALPHA) {
		if (mode->bitdepth == 8) {
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				buffer[0] = buffer[1] = buffer[2] = in[i * 2 + 0];
				buffer[3] = in[i * 2 + 1];
			}
		}
		else {
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				buffer[0] = buffer[1] = buffer[2] = in[i * 4 + 0];
				buffer[3] = in[i * 4 + 2];
			}
		}
	}
	else if (mode->colortype == LCT_RGBA) {
		if (mode->bitdepth == 8) {
			memcpy(buffer, in, numpixels * 4);
		}
		else {
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				buffer[0] = in[i * 8 + 0];
				buffer[1] = in[i * 8 + 2];
				buffer[2] = in[i * 8 + 4];
				buffer[3] = in[i * 8 + 6];
			}
		}
	}
}
static void getPixelColorsRGB8(unsigned char* __restrict buffer, size_t numpixels, const unsigned char* __restrict in, const LoadPNGColorMode* mode) {
	const unsigned num_channels = 3;
	size_t i;
	if (mode->colortype == LCT_GREY) {
		if (mode->bitdepth == 8) {
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				buffer[0] = buffer[1] = buffer[2] = in[i];
			}
		}
		else if (mode->bitdepth == 16) {
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				buffer[0] = buffer[1] = buffer[2] = in[i * 2];
			}
		}
		else {
			unsigned highest = ((1U << mode->bitdepth) - 1U); /*highest possible value for this bit depth*/
			size_t j = 0;
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				unsigned value = readBitsFromReversedStream(&j, in, mode->bitdepth);
				buffer[0] = buffer[1] = buffer[2] = (value * 255) / highest;
			}
		}
	}
	else if (mode->colortype == LCT_RGB) {
		if (mode->bitdepth == 8) {
			memcpy(buffer, in, numpixels * 3);
		}
		else {
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				buffer[0] = in[i * 6 + 0];
				buffer[1] = in[i * 6 + 2];
				buffer[2] = in[i * 6 + 4];
			}
		}
	}
	else if (mode->colortype == LCT_PALETTE) {
		if (mode->bitdepth == 8) {
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				unsigned index = in[i];
				memcpy(buffer, &mode->palette[index * 4], 3);
			}
		}
		else {
			size_t j = 0;
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				unsigned index = readBitsFromReversedStream(&j, in, mode->bitdepth);
				memcpy(buffer, &mode->palette[index * 4], 3);
			}
		}
	}
	else if (mode->colortype == LCT_GREY_ALPHA) {
		if (mode->bitdepth == 8) {
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				buffer[0] = buffer[1] = buffer[2] = in[i * 2 + 0];
			}
		}
		else {
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				buffer[0] = buffer[1] = buffer[2] = in[i * 4 + 0];
			}
		}
	}
	else if (mode->colortype == LCT_RGBA) {
		if (mode->bitdepth == 8) {
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				memcpy(buffer, &in[i * 4], 3);
			}
		}
		else {
			for (i = 0; i != numpixels; ++i, buffer += num_channels) {
				buffer[0] = in[i * 8 + 0];
				buffer[1] = in[i * 8 + 2];
				buffer[2] = in[i * 8 + 4];
			}
		}
	}
}
static void getPixelColorRGBA16(unsigned short* r, unsigned short* g, unsigned short* b, unsigned short* a, const unsigned char* in, size_t i, const LoadPNGColorMode* mode) {
	if (mode->colortype == LCT_GREY) {
		*r = *g = *b = 256 * in[i * 2 + 0] + in[i * 2 + 1];
		if (mode->key_defined && 256U * in[i * 2 + 0] + in[i * 2 + 1] == mode->key_r) *a = 0;
		else *a = 65535;
	}
	else if (mode->colortype == LCT_RGB) {
		*r = 256u * in[i * 6 + 0] + in[i * 6 + 1];
		*g = 256u * in[i * 6 + 2] + in[i * 6 + 3];
		*b = 256u * in[i * 6 + 4] + in[i * 6 + 5];
		if (mode->key_defined
			&& 256u * in[i * 6 + 0] + in[i * 6 + 1] == mode->key_r
			&& 256u * in[i * 6 + 2] + in[i * 6 + 3] == mode->key_g
			&& 256u * in[i * 6 + 4] + in[i * 6 + 5] == mode->key_b) *a = 0;
		else *a = 65535;
	}
	else if (mode->colortype == LCT_GREY_ALPHA) {
		*r = *g = *b = 256u * in[i * 4 + 0] + in[i * 4 + 1];
		*a = 256u * in[i * 4 + 2] + in[i * 4 + 3];
	}
	else if (mode->colortype == LCT_RGBA) {
		*r = 256u * in[i * 8 + 0] + in[i * 8 + 1];
		*g = 256u * in[i * 8 + 2] + in[i * 8 + 3];
		*b = 256u * in[i * 8 + 4] + in[i * 8 + 5];
		*a = 256u * in[i * 8 + 6] + in[i * 8 + 7];
	}
}
unsigned loadpng_convert(unsigned char* out, const unsigned char* in, const LoadPNGColorMode* mode_out, const LoadPNGColorMode* mode_in, unsigned w, unsigned h) {
	size_t i;
	ColorTree tree;
	size_t numpixels = (size_t)w * (size_t)h;
	unsigned error = 0;

	if (mode_in->colortype == LCT_PALETTE && !mode_in->palette) {
		return 107; /* error: must provide palette if input mode is palette */
	}

	if (loadpng_color_mode_equal(mode_out, mode_in)) {
		size_t numbytes = loadpng_get_raw_size_lct(w, h, mode_in->colortype, mode_in->bitdepth);
		memcpy(out, in, numbytes);
		return 0;
	}

	if (mode_out->colortype == LCT_PALETTE) {
		size_t palettesize = mode_out->palettesize;
		const unsigned char* palette = mode_out->palette;
		size_t palsize = (size_t)1u << mode_out->bitdepth;
		if (palettesize == 0) {
			palettesize = mode_in->palettesize;
			palette = mode_in->palette;
			if (mode_in->colortype == LCT_PALETTE && mode_in->bitdepth == mode_out->bitdepth) {
				size_t numbytes = loadpng_get_raw_size_lct(w, h, mode_in->colortype, mode_in->bitdepth);
				memcpy(out, in, numbytes);
				return 0;
			}
		}
		if (palettesize < palsize) palsize = palettesize;
		color_tree_init(&tree);
		for (i = 0; i != palsize; ++i) {
			const unsigned char* p = &palette[i * 4];
			error = color_tree_add(&tree, p[0], p[1], p[2], p[3], (unsigned)i);
			if (error) break;
		}
	}

	if (!error) {
		if (mode_in->bitdepth == 16 && mode_out->bitdepth == 16) {
			for (i = 0; i != numpixels; ++i) {
				unsigned short r = 0, g = 0, b = 0, a = 0;
				getPixelColorRGBA16(&r, &g, &b, &a, in, i, mode_in);
				rgba16ToPixel(out, i, mode_out, r, g, b, a);
			}
		}
		else if (mode_out->bitdepth == 8 && mode_out->colortype == LCT_RGBA) {
			getPixelColorsRGBA8(out, numpixels, in, mode_in);
		}
		else if (mode_out->bitdepth == 8 && mode_out->colortype == LCT_RGB) {
			getPixelColorsRGB8(out, numpixels, in, mode_in);
		}
		else {
			unsigned char r = 0, g = 0, b = 0, a = 0;
			for (i = 0; i != numpixels; ++i) {
				getPixelColorRGBA8(&r, &g, &b, &a, in, i, mode_in);
				error = rgba8ToPixel(out, i, mode_out, &tree, r, g, b, a);
				if (error) break;
			}
		}
	}

	if (mode_out->colortype == LCT_PALETTE) {
		color_tree_cleanup(&tree);
	}

	return error;
}
void loadpng_color_stats_init(LoadPNGColorStats* stats) {
	stats->colored = 0;
	stats->key = 0;
	stats->key_r = stats->key_g = stats->key_b = 0;
	stats->alpha = 0;
	stats->numcolors = 0;
	stats->bits = 1;
	stats->numpixels = 0;
	stats->allow_palette = 1;
	stats->allow_greyscale = 1;
}
static unsigned getValueRequiredBits(unsigned char value) {
	if (value == 0 || value == 255) return 1;
	if (value % 17 == 0) return value % 85 == 0 ? 2 : 4;
	return 8;
}
unsigned loadpng_compute_color_stats(LoadPNGColorStats* stats, const unsigned char* in, unsigned w, unsigned h, const LoadPNGColorMode* mode_in) {
	size_t i;
	ColorTree tree;
	size_t numpixels = (size_t)w * (size_t)h;
	unsigned error = 0;

	unsigned colored_done = (mode_in->colortype == LCT_GREY || mode_in->colortype == LCT_GREY_ALPHA) ? 1 : 0;
	unsigned alpha_done = (mode_in->key_defined || ((mode_in->colortype & 4) != 0) || loadpng_has_palette_alpha(mode_in)) ? 0 : 1;
	unsigned numcolors_done = 0;
	unsigned bpp = getNumColorChannels(mode_in->colortype) * mode_in->bitdepth;;
	unsigned bits_done = (stats->bits == 1 && bpp == 1) ? 1 : 0;
	unsigned sixteen = 0; /* whether the input image is 16 bit */
	unsigned maxnumcolors = 257;
	if (bpp <= 8) maxnumcolors = min(257, stats->numcolors + (1u << bpp));

	stats->numpixels += numpixels;
	if (!stats->allow_palette) numcolors_done = 1;

	color_tree_init(&tree);

	if (stats->alpha) alpha_done = 1;
	if (stats->colored) colored_done = 1;
	if (stats->bits == 16) numcolors_done = 1;
	if (stats->bits >= bpp) bits_done = 1;
	if (stats->numcolors >= maxnumcolors) numcolors_done = 1;

	if (!numcolors_done) {
		for (i = 0; i < stats->numcolors; i++) {
			const unsigned char* color = &stats->palette[i * 4];
			error = color_tree_add(&tree, color[0], color[1], color[2], color[3], i);
			if (error) goto cleanup;
		}
	}

	if (mode_in->bitdepth == 16 && !sixteen) {
		unsigned short r = 0, g = 0, b = 0, a = 0;
		for (i = 0; i != numpixels; ++i) {
			getPixelColorRGBA16(&r, &g, &b, &a, in, i, mode_in);
			if ((r & 255) != ((r >> 8) & 255) || (g & 255) != ((g >> 8) & 255) ||
				(b & 255) != ((b >> 8) & 255) || (a & 255) != ((a >> 8) & 255)) /*first and second byte differ*/ {
				stats->bits = 16;
				sixteen = 1;
				bits_done = 1;
				numcolors_done = 1; /*counting colors no longer useful, palette doesn't support 16-bit*/
				break;
			}
		}
	}

	if (sixteen) {
		unsigned short r = 0, g = 0, b = 0, a = 0;

		for (i = 0; i != numpixels; ++i) {
			getPixelColorRGBA16(&r, &g, &b, &a, in, i, mode_in);

			if (!colored_done && (r != g || r != b)) {
				stats->colored = 1;
				colored_done = 1;
			}

			if (!alpha_done) {
				unsigned matchkey = (r == stats->key_r && g == stats->key_g && b == stats->key_b);
				if (a != 65535 && (a != 0 || (stats->key && !matchkey))) {
					stats->alpha = 1;
					stats->key = 0;
					alpha_done = 1;
				}
				else if (a == 0 && !stats->alpha && !stats->key) {
					stats->key = 1;
					stats->key_r = r;
					stats->key_g = g;
					stats->key_b = b;
				}
				else if (a == 65535 && stats->key && matchkey) {
					stats->alpha = 1;
					stats->key = 0;
					alpha_done = 1;
				}
			}
			if (alpha_done && numcolors_done && colored_done && bits_done) break;
		}

		if (stats->key && !stats->alpha) {
			for (i = 0; i != numpixels; ++i) {
				getPixelColorRGBA16(&r, &g, &b, &a, in, i, mode_in);
				if (a != 0 && r == stats->key_r && g == stats->key_g && b == stats->key_b) {
					stats->alpha = 1;
					stats->key = 0;
					alpha_done = 1;
				}
			}
		}
	}
	else {
		unsigned char r = 0, g = 0, b = 0, a = 0;
		for (i = 0; i != numpixels; ++i) {
			getPixelColorRGBA8(&r, &g, &b, &a, in, i, mode_in);

			if (!bits_done && stats->bits < 8) {
				unsigned bits = getValueRequiredBits(r);
				if (bits > stats->bits) stats->bits = bits;
			}
			bits_done = (stats->bits >= bpp);

			if (!colored_done && (r != g || r != b)) {
				stats->colored = 1;
				colored_done = 1;
				if (stats->bits < 8) stats->bits = 8; /*PNG has no colored modes with less than 8-bit per channel*/
			}

			if (!alpha_done) {
				unsigned matchkey = (r == stats->key_r && g == stats->key_g && b == stats->key_b);
				if (a != 255 && (a != 0 || (stats->key && !matchkey))) {
					stats->alpha = 1;
					stats->key = 0;
					alpha_done = 1;
					if (stats->bits < 8) stats->bits = 8; /*PNG has no alphachannel modes with less than 8-bit per channel*/
				}
				else if (a == 0 && !stats->alpha && !stats->key) {
					stats->key = 1;
					stats->key_r = r;
					stats->key_g = g;
					stats->key_b = b;
				}
				else if (a == 255 && stats->key && matchkey) {
					stats->alpha = 1;
					stats->key = 0;
					alpha_done = 1;
					if (stats->bits < 8) stats->bits = 8; /*PNG has no alphachannel modes with less than 8-bit per channel*/
				}
			}

			if (!numcolors_done) {
				if (!color_tree_has(&tree, r, g, b, a)) {
					error = color_tree_add(&tree, r, g, b, a, stats->numcolors);
					if (error) goto cleanup;
					if (stats->numcolors < 256) {
						unsigned char* p = stats->palette;
						unsigned n = stats->numcolors;
						p[n * 4 + 0] = r;
						p[n * 4 + 1] = g;
						p[n * 4 + 2] = b;
						p[n * 4 + 3] = a;
					}
					++stats->numcolors;
					numcolors_done = stats->numcolors >= maxnumcolors;
				}
			}

			if (alpha_done && numcolors_done && colored_done && bits_done) break;
		}

		if (stats->key && !stats->alpha) {
			for (i = 0; i != numpixels; ++i) {
				getPixelColorRGBA8(&r, &g, &b, &a, in, i, mode_in);
				if (a != 0 && r == stats->key_r && g == stats->key_g && b == stats->key_b) {
					stats->alpha = 1;
					stats->key = 0;
					alpha_done = 1;
					if (stats->bits < 8) stats->bits = 8; /*PNG has no alphachannel modes with less than 8-bit per channel*/
				}
			}
		}
		stats->key_r += (stats->key_r << 8);
		stats->key_g += (stats->key_g << 8);
		stats->key_b += (stats->key_b << 8);
	}

cleanup:
	color_tree_cleanup(&tree);
	return error;
}
static unsigned auto_choose_color(LoadPNGColorMode* mode_out, const LoadPNGColorMode* mode_in, const LoadPNGColorStats* stats) {
	unsigned error = 0;
	unsigned palettebits;
	size_t i, n;
	size_t numpixels = stats->numpixels;
	unsigned palette_ok, gray_ok;

	unsigned alpha = stats->alpha;
	unsigned key = stats->key;
	unsigned bits = stats->bits;

	mode_out->key_defined = 0;

	if (key && numpixels <= 16) {
		alpha = 1; /*too few pixels to justify tRNS chunk overhead*/
		key = 0;
		if (bits < 8) bits = 8; /*PNG has no alphachannel modes with less than 8-bit per channel*/
	}

	gray_ok = !stats->colored;
	if (!stats->allow_greyscale) gray_ok = 0;
	if (!gray_ok && bits < 8) bits = 8;

	n = stats->numcolors;
	palettebits = n <= 2 ? 1 : (n <= 4 ? 2 : (n <= 16 ? 4 : 8));
	palette_ok = n <= 256 && bits <= 8 && n != 0; /*n==0 means likely numcolors wasn't computed*/
	if (numpixels < n * 2) palette_ok = 0; /*don't add palette overhead if image has only a few pixels*/
	if (gray_ok && !alpha && bits <= palettebits) palette_ok = 0; /*gray is less overhead*/
	if (!stats->allow_palette) palette_ok = 0;

	if (palette_ok) {
		const unsigned char* p = stats->palette;
		loadpng_palette_clear(mode_out); /*remove potential earlier palette*/
		for (i = 0; i != stats->numcolors; ++i) {
			error = loadpng_palette_add(mode_out, p[i * 4 + 0], p[i * 4 + 1], p[i * 4 + 2], p[i * 4 + 3]);
			if (error) break;
		}

		mode_out->colortype = LCT_PALETTE;
		mode_out->bitdepth = palettebits;

		if (mode_in->colortype == LCT_PALETTE && mode_in->palettesize >= mode_out->palettesize
			&& mode_in->bitdepth == mode_out->bitdepth) {
			/*If input should have same palette colors, keep original to preserve its order and prevent conversion*/
			loadpng_palette_clear(mode_out);
			loadpng_color_mode_copy(mode_out, mode_in);
		}
	}
	else /*8-bit or 16-bit per channel*/ {
		mode_out->bitdepth = bits;
		mode_out->colortype = alpha ? (gray_ok ? LCT_GREY_ALPHA : LCT_RGBA)
			: (gray_ok ? LCT_GREY : LCT_RGB);
		if (key) {
			unsigned mask = (1u << mode_out->bitdepth) - 1u; /*stats always uses 16-bit, mask converts it*/
			mode_out->key_r = stats->key_r & mask;
			mode_out->key_g = stats->key_g & mask;
			mode_out->key_b = stats->key_b & mask;
			mode_out->key_defined = 1;
		}
	}

	return error;
}
static unsigned char paethPredictor(short a, short b, short c) {
	short pa = abs(b - c);
	short pb = abs(a - c);
	short pc = abs(a + b - c - c);
	if (pb < pa) { a = b; pa = pb; }
	return (pc < pa) ? c : a;
}

static const unsigned ADAM7_IX[7] = { 0, 4, 0, 2, 0, 1, 0 }; /*x start values*/
static const unsigned ADAM7_IY[7] = { 0, 0, 4, 0, 2, 0, 1 }; /*y start values*/
static const unsigned ADAM7_DX[7] = { 8, 8, 4, 4, 2, 2, 1 }; /*x delta values*/
static const unsigned ADAM7_DY[7] = { 8, 8, 8, 4, 4, 2, 2 }; /*y delta values*/

static void Adam7_getpassvalues(unsigned passw[7], unsigned passh[7], size_t filter_passstart[8], size_t padded_passstart[8], size_t passstart[8], unsigned w, unsigned h, unsigned bpp) {
	unsigned i;
	for (i = 0; i != 7; ++i) {
		passw[i] = (w + ADAM7_DX[i] - ADAM7_IX[i] - 1) / ADAM7_DX[i];
		passh[i] = (h + ADAM7_DY[i] - ADAM7_IY[i] - 1) / ADAM7_DY[i];
		if (passw[i] == 0) passh[i] = 0;
		if (passh[i] == 0) passw[i] = 0;
	}

	filter_passstart[0] = padded_passstart[0] = passstart[0] = 0;
	for (i = 0; i != 7; ++i) {
		filter_passstart[i + 1] = filter_passstart[i]
			+ ((passw[i] && passh[i]) ? passh[i] * (1u + (passw[i] * bpp + 7u) / 8u) : 0);
		padded_passstart[i + 1] = padded_passstart[i] + passh[i] * ((passw[i] * bpp + 7u) / 8u);
		passstart[i + 1] = passstart[i] + (passh[i] * passw[i] * bpp + 7u) / 8u;
	}
}
void loadpng_decoder_settings_init(LoadPNGDecoderSettings* settings) {
	settings->ignore_crc = 0;
	settings->ignore_critical = 0;
	settings->ignore_end = 0;
	loadpng_decompress_settings_init(&settings->zlibsettings);
}
void loadpng_encoder_settings_init(LoadPNGEncoderSettings* settings) {
	loadpng_compress_settings_init(&settings->zlibsettings);
	settings->filter_palette_zero = 1;
	settings->filter_strategy = LFS_MINSUM;
	settings->force_palette = 0;
	settings->predefined_filters = 0;
}
void loadpng_state_init(LoadPNGState* state) {
	loadpng_decoder_settings_init(&state->decoder);
	loadpng_encoder_settings_init(&state->encoder);
	loadpng_color_mode_init(&state->info_raw);
	loadpng_info_init(&state->info_png);
	state->error = 1;
}
void loadpng_state_cleanup(LoadPNGState* state) {
	loadpng_palette_clear(&state->info_raw);
	loadpng_palette_clear(&(&state->info_png)->color);
}
void loadpng_state_copy(LoadPNGState* dest, const LoadPNGState* source) {
	loadpng_state_cleanup(dest);
	*dest = *source;
	loadpng_color_mode_init(&dest->info_raw);
	loadpng_info_init(&dest->info_png);
	dest->error = loadpng_color_mode_copy(&dest->info_raw, &source->info_raw); if (dest->error) return;
	dest->error = loadpng_info_copy(&dest->info_png, &source->info_png); if (dest->error) return;
}

unsigned loadpng_inspect(unsigned* w, unsigned* h, LoadPNGState* state, const unsigned char* in, size_t insize) {
	unsigned width, height;
	LoadPNGInfo* info = &state->info_png;
	if (insize == 0 || in == 0) {
		state->error = 48;
		return state->error;
	}
	if (insize < 33) {
		state->error = 27;
		return state->error;
	}

	loadpng_palette_clear(&info->color);
	loadpng_info_init(info);

	if (in[0] != 137 || in[1] != 80 || in[2] != 78 || in[3] != 71
		|| in[4] != 13 || in[5] != 10 || in[6] != 26 || in[7] != 10) {
		state->error = 28;
		return state->error;
	}
	if (readInt32(in + 8) != 13) {
		state->error = 94;
		return state->error;
	}
	if (!loadpng_chunk_type_equals(in + 8, "IHDR")) {
		state->error = 29;
		return state->error;
	}

	width = readInt32(&in[16]);
	height = readInt32(&in[20]);
	if (w) *w = width;
	if (h) *h = height;
	info->color.bitdepth = in[24];
	info->color.colortype = (LoadPNGColorType)in[25];
	info->compression_method = in[26];
	info->filter_method = in[27];
	info->interlace_method = in[28];
	if (width == 0 || height == 0) {
		state->error = 93;
		return state->error;
	}
	state->error = checkColorValidity(info->color.colortype, info->color.bitdepth);
	if (state->error) return state->error;
	if (info->compression_method != 0) {
		state->error = 32;
		return state->error;
	}
	if (info->filter_method != 0) {
		state->error = 33;
		return state->error;
	}
	if (info->interlace_method > 1) {
		state->error = 34;
		return state->error;
	}

	if (!state->decoder.ignore_crc) {
		unsigned CRC = readInt32(&in[29]);
		unsigned checksum = loadpng_crc32(&in[12], 17);
		if (CRC != checksum) {
			state->error = 57;
			return state->error;
		}
	}

	return state->error;
}
static unsigned unfilterScanline(unsigned char* recon, const unsigned char* scanline, const unsigned char* precon, size_t bytewidth, unsigned char filterType, size_t length) {

	size_t i;
	switch (filterType) {
	case 0:
		for (i = 0; i != length; ++i) recon[i] = scanline[i];
		break;
	case 1:
		for (i = 0; i != bytewidth; ++i) recon[i] = scanline[i];
		for (i = bytewidth; i < length; ++i) recon[i] = scanline[i] + recon[i - bytewidth];
		break;
	case 2:
		if (precon) {
			for (i = 0; i != length; ++i) recon[i] = scanline[i] + precon[i];
		}
		else {
			for (i = 0; i != length; ++i) recon[i] = scanline[i];
		}
		break;
	case 3:
		if (precon) {
			for (i = 0; i != bytewidth; ++i) recon[i] = scanline[i] + (precon[i] >> 1u);
			for (i = bytewidth; i < length; ++i) recon[i] = scanline[i] + ((recon[i - bytewidth] + precon[i]) >> 1u);
		}
		else {
			for (i = 0; i != bytewidth; ++i) recon[i] = scanline[i];
			for (i = bytewidth; i < length; ++i) recon[i] = scanline[i] + (recon[i - bytewidth] >> 1u);
		}
		break;
	case 4:
		if (precon) {
			for (i = 0; i != bytewidth; ++i) {
				recon[i] = (scanline[i] + precon[i]); /*paethPredictor(0, precon[i], 0) is always precon[i]*/
			}
			if (bytewidth >= 4) {
				for (; i + 3 < length; i += 4) {
					size_t j = i - bytewidth;
					unsigned char s0 = scanline[i + 0], s1 = scanline[i + 1], s2 = scanline[i + 2], s3 = scanline[i + 3];
					unsigned char r0 = recon[j + 0], r1 = recon[j + 1], r2 = recon[j + 2], r3 = recon[j + 3];
					unsigned char p0 = precon[i + 0], p1 = precon[i + 1], p2 = precon[i + 2], p3 = precon[i + 3];
					unsigned char q0 = precon[j + 0], q1 = precon[j + 1], q2 = precon[j + 2], q3 = precon[j + 3];
					recon[i + 0] = s0 + paethPredictor(r0, p0, q0);
					recon[i + 1] = s1 + paethPredictor(r1, p1, q1);
					recon[i + 2] = s2 + paethPredictor(r2, p2, q2);
					recon[i + 3] = s3 + paethPredictor(r3, p3, q3);
				}
			}
			else if (bytewidth >= 3) {
				for (; i + 2 < length; i += 3) {
					size_t j = i - bytewidth;
					unsigned char s0 = scanline[i + 0], s1 = scanline[i + 1], s2 = scanline[i + 2];
					unsigned char r0 = recon[j + 0], r1 = recon[j + 1], r2 = recon[j + 2];
					unsigned char p0 = precon[i + 0], p1 = precon[i + 1], p2 = precon[i + 2];
					unsigned char q0 = precon[j + 0], q1 = precon[j + 1], q2 = precon[j + 2];
					recon[i + 0] = s0 + paethPredictor(r0, p0, q0);
					recon[i + 1] = s1 + paethPredictor(r1, p1, q1);
					recon[i + 2] = s2 + paethPredictor(r2, p2, q2);
				}
			}
			else if (bytewidth >= 2) {
				for (; i + 1 < length; i += 2) {
					size_t j = i - bytewidth;
					unsigned char s0 = scanline[i + 0], s1 = scanline[i + 1];
					unsigned char r0 = recon[j + 0], r1 = recon[j + 1];
					unsigned char p0 = precon[i + 0], p1 = precon[i + 1];
					unsigned char q0 = precon[j + 0], q1 = precon[j + 1];
					recon[i + 0] = s0 + paethPredictor(r0, p0, q0);
					recon[i + 1] = s1 + paethPredictor(r1, p1, q1);
				}
			}

			for (; i != length; ++i) {
				recon[i] = (scanline[i] + paethPredictor(recon[i - bytewidth], precon[i], precon[i - bytewidth]));
			}
		}
		else {
			for (i = 0; i != bytewidth; ++i) {
				recon[i] = scanline[i];
			}
			for (i = bytewidth; i < length; ++i) {
				recon[i] = (scanline[i] + recon[i - bytewidth]);
			}
		}
		break;
	default: return 36; /*error: invalid filter type given*/
	}
	return 0;
}
static unsigned unfilter(unsigned char* out, const unsigned char* in, unsigned w, unsigned h, unsigned bpp) {
	unsigned y;
	unsigned char* prevline = 0;
	size_t bytewidth = (bpp + 7u) / 8u;
	size_t linebytes = loadpng_get_raw_size_idat(w, 1, bpp) - 1u;

	for (y = 0; y < h; ++y) {
		size_t outindex = linebytes * y;
		size_t inindex = (1 + linebytes) * y; /*the extra filterbyte added to each row*/
		unsigned char filterType = in[inindex];

		if (unsigned error = unfilterScanline(&out[outindex], &in[inindex + 1], prevline, bytewidth, filterType, linebytes))return error;

		prevline = &out[outindex];
	}

	return 0;
}
static void Adam7_deinterlace(unsigned char* out, const unsigned char* in, unsigned w, unsigned h, unsigned bpp) {
	unsigned passw[7], passh[7];
	size_t filter_passstart[8], padded_passstart[8], passstart[8];
	unsigned i;

	Adam7_getpassvalues(passw, passh, filter_passstart, padded_passstart, passstart, w, h, bpp);

	if (bpp >= 8) {
		for (i = 0; i != 7; ++i) {
			unsigned x, y, b;
			size_t bytewidth = bpp / 8u;
			for (y = 0; y < passh[i]; ++y)
				for (x = 0; x < passw[i]; ++x) {
					size_t pixelinstart = passstart[i] + (y * passw[i] + x) * bytewidth;
					size_t pixeloutstart = ((ADAM7_IY[i] + (size_t)y * ADAM7_DY[i]) * (size_t)w
						+ ADAM7_IX[i] + (size_t)x * ADAM7_DX[i]) * bytewidth;
					for (b = 0; b < bytewidth; ++b) {
						out[pixeloutstart + b] = in[pixelinstart + b];
					}
				}
		}
	}
	else {
		for (i = 0; i != 7; ++i) {
			unsigned x, y, b;
			unsigned ilinebits = bpp * passw[i];
			unsigned olinebits = bpp * w;
			size_t obp, ibp; /*bit pointers (for out and in buffer)*/
			for (y = 0; y < passh[i]; ++y)
				for (x = 0; x < passw[i]; ++x) {
					ibp = (8 * passstart[i]) + (y * ilinebits + x * bpp);
					obp = (ADAM7_IY[i] + (size_t)y * ADAM7_DY[i]) * olinebits + (ADAM7_IX[i] + (size_t)x * ADAM7_DX[i]) * bpp;
					for (b = 0; b < bpp; ++b) {
						unsigned char bit = readBitFromReversedStream(&ibp, in);
						setBitOfReversedStream(&obp, out, bit);
					}
				}
		}
	}
}
static void removePaddingBits(unsigned char* out, const unsigned char* in, size_t olinebits, size_t ilinebits, unsigned h) {
	unsigned y;
	size_t diff = ilinebits - olinebits;
	size_t ibp = 0, obp = 0; /*input and output bit pointers*/
	for (y = 0; y < h; ++y) {
		size_t x;
		for (x = 0; x < olinebits; ++x) {
			unsigned char bit = readBitFromReversedStream(&ibp, in);
			setBitOfReversedStream(&obp, out, bit);
		}
		ibp += diff;
	}
}
static unsigned postProcessScanlines(unsigned char* out, unsigned char* in, unsigned w, unsigned h, const LoadPNGInfo* info_png) {
	unsigned bpp = getNumColorChannels((&info_png->color)->colortype) * (&info_png->color)->bitdepth;
	if (bpp == 0) return 31; /*error: invalid colortype*/

	if (info_png->interlace_method == 0) {
		if (bpp < 8 && w * bpp != ((w * bpp + 7u) / 8u) * 8u) {
			if (unsigned error = unfilter(in, in, w, h, bpp))return error;
			removePaddingBits(out, in, w * bpp, ((w * bpp + 7u) / 8u) * 8u, h);
		}
		else if (unsigned error = unfilter(out, in, w, h, bpp))return error;
	}
	else {
		unsigned passw[7], passh[7]; size_t filter_passstart[8], padded_passstart[8], passstart[8];
		unsigned i;

		Adam7_getpassvalues(passw, passh, filter_passstart, padded_passstart, passstart, w, h, bpp);

		for (i = 0; i != 7; ++i) {
			if (unsigned error = unfilter(&in[padded_passstart[i]], &in[filter_passstart[i]], passw[i], passh[i], bpp))return error;
			if (bpp < 8) {
				removePaddingBits(&in[passstart[i]], &in[padded_passstart[i]], passw[i] * bpp,
					((passw[i] * bpp + 7u) / 8u) * 8u, passh[i]);
			}
		}

		Adam7_deinterlace(out, in, w, h, bpp);
	}

	return 0;
}
static unsigned readChunk_PLTE(LoadPNGColorMode* color, const unsigned char* data, size_t chunkLength) {
	unsigned pos = 0, i;
	color->palettesize = chunkLength / 3u;
	if (color->palettesize == 0 || color->palettesize > 256) return 38; /*error: palette too small or big*/
	loadpng_color_mode_alloc_palette(color);
	if (!color->palette && color->palettesize) {
		color->palettesize = 0;
		return 83; /*alloc fail*/
	}

	for (i = 0; i != color->palettesize; ++i) {
		color->palette[4 * i + 0] = data[pos++]; /*R*/
		color->palette[4 * i + 1] = data[pos++]; /*G*/
		color->palette[4 * i + 2] = data[pos++]; /*B*/
		color->palette[4 * i + 3] = 255; /*alpha*/
	}

	return 0; /* OK */
}
static unsigned readChunk_tRNS(LoadPNGColorMode* color, const unsigned char* data, size_t chunkLength) {
	unsigned i;
	if (color->colortype == LCT_PALETTE) {
		if (chunkLength > color->palettesize) return 39;

		for (i = 0; i != chunkLength; ++i) color->palette[4 * i + 3] = data[i];
	}
	else if (color->colortype == LCT_GREY) {
		if (chunkLength != 2) return 30;

		color->key_defined = 1;
		color->key_r = color->key_g = color->key_b = 256u * data[0] + data[1];
	}
	else if (color->colortype == LCT_RGB) {
		if (chunkLength != 6) return 41;

		color->key_defined = 1;
		color->key_r = 256u * data[0] + data[1];
		color->key_g = 256u * data[2] + data[3];
		color->key_b = 256u * data[4] + data[5];
	}
	else return 42; /*error: tRNS chunk not allowed for other color models*/

	return 0; /* OK */
}
unsigned loadpng_decode_memory(unsigned char** out, unsigned* w, unsigned* h, const unsigned char* in, size_t insize, LoadPNGColorType colortype, unsigned bitdepth) {
	LoadPNGState state;
	loadpng_state_init(&state);
	state.info_raw.colortype = colortype;
	state.info_raw.bitdepth = bitdepth; *out = 0;

	unsigned char IEND = 0;
	const unsigned char* chunk;
	unsigned char* idat; /*the data from idat chunks, zlib compressed*/
	size_t idatsize = 0;
	unsigned char* scanlines = 0;
	size_t scanlines_size = 0, expected_size = 0;
	size_t pngsize = 0;

	unsigned unknown = 0;
	*out = 0;
	*w = *h = 0;

	state.error = loadpng_inspect(w, h, &state, in, insize); /*reads header and resets other parameters in state->info_png*/
	if (state.error) return state.error;

	if (loadpng_pixel_overflow(*w, *h, &state.info_png.color, &state.info_raw)) {
		state.error = 92;
		return state.error;
	}

	idat = (unsigned char*)malloc(insize);
	if (!idat) {
		state.error = 83;
		return state.error;
	}

	chunk = &in[33]; /*first byte of the first chunk after the header*/
	while (!IEND && !state.error) {
		unsigned chunkLength;
		const unsigned char* data; /*the data in the chunk*/
		if ((size_t)((chunk - in) + 12) > insize || chunk < in) {
			if (state.decoder.ignore_end) break; /*other errors may still happen though*/
			state.error = 30;
			break;
		}
		chunkLength = readInt32(chunk);
		if (chunkLength > 2147483647) {
			if (state.decoder.ignore_end) break; /*other errors may still happen though*/
			state.error = 63;
			break;
		}

		if ((size_t)((chunk - in) + chunkLength + 12) > insize || (chunk + chunkLength + 12) < in) {
			state.error = 64;
			break;
		}

		data = chunk + 8;

		unknown = 0;
		if (loadpng_chunk_type_equals(chunk, "IDAT")) {
			SGuint newsize;
			if (safeAdd(idatsize, chunkLength, &newsize)) {
				state.error = 95;
				break;
			}
			if (newsize > insize) {
				state.error = 95;
				break;
			}
			memcpy(idat + idatsize, data, chunkLength);
			idatsize += chunkLength;
		}
		else if (loadpng_chunk_type_equals(chunk, "IEND")) {
			IEND = 1;
		}
		else if (loadpng_chunk_type_equals(chunk, "PLTE")) {
			state.error = readChunk_PLTE(&state.info_png.color, data, chunkLength);
			if (state.error) break;
		}
		else if (loadpng_chunk_type_equals(chunk, "tRNS")) {
			state.error = readChunk_tRNS(&state.info_png.color, data, chunkLength);
			if (state.error) break;
		}
		else {
			if (!state.decoder.ignore_critical && chunk[4] & 32 == 0) {
				state.error = 69;
				break;
			}

			unknown = 1;
		}

		if (!state.decoder.ignore_crc && !unknown) {
			if (loadpng_chunk_check_crc(chunk)) {
				state.error = 57;
				break;
			}
		}

		if (!IEND) chunk = loadpng_chunk_next_const(chunk, in + insize);
	}

	if (state.info_png.color.colortype == LCT_PALETTE && !state.info_png.color.palette) {
		state.error = 106; /* error: PNG file must have PLTE chunk if color type is palette */
	}

	if (!state.error) {
		if (state.info_png.interlace_method == 0) {
			size_t bpp = getNumColorChannels((&state.info_png.color)->colortype) * (&state.info_png.color)->bitdepth;
			expected_size = loadpng_get_raw_size_idat(*w, *h, bpp);
		}
		else {
			size_t bpp = getNumColorChannels((&state.info_png.color)->colortype) * (&state.info_png.color)->bitdepth;
			expected_size = 0;
			expected_size += loadpng_get_raw_size_idat((*w + 7) >> 3, (*h + 7) >> 3, bpp);
			if (*w > 4) expected_size += loadpng_get_raw_size_idat((*w + 3) >> 3, (*h + 7) >> 3, bpp);
			expected_size += loadpng_get_raw_size_idat((*w + 3) >> 2, (*h + 3) >> 3, bpp);
			if (*w > 2) expected_size += loadpng_get_raw_size_idat((*w + 1) >> 2, (*h + 3) >> 2, bpp);
			expected_size += loadpng_get_raw_size_idat((*w + 1) >> 1, (*h + 1) >> 2, bpp);
			if (*w > 1) expected_size += loadpng_get_raw_size_idat((*w + 0) >> 1, (*h + 1) >> 1, bpp);
			expected_size += loadpng_get_raw_size_idat((*w + 0), (*h + 0) >> 1, bpp);
		}

		vector<unsigned char> v(scanlines, scanlines + scanlines_size);
		state.error = loadpng_zlib_decompress(v, idat, idatsize, &state.decoder.zlibsettings);
		scanlines = (unsigned char *)malloc(v.size());
		memcpy(scanlines, v.data(), v.size());
		scanlines_size = v.size();
	}
	if (!state.error && scanlines_size != expected_size) state.error = 91; /*decompressed size doesn't match prediction*/
	free(idat);

	if (!state.error) {
		pngsize = loadpng_get_raw_size_lct(*w, *h, (&state.info_png.color)->colortype, (&state.info_png.color)->bitdepth);
		*out = (unsigned char*)malloc(pngsize);
		if (!*out) state.error = 83; /*alloc fail*/
	}
	if (!state.error) {
		memset(*out, 0, pngsize);
		state.error = postProcessScanlines(*out, scanlines, *w, *h, &state.info_png);
	}
	free(scanlines);

	if (state.error) return state.error;
	unsigned char* data = *out;
	size_t outsize;
	if (!(state.info_raw.colortype == LCT_RGB || state.info_raw.colortype == LCT_RGBA)
		&& !(state.info_raw.bitdepth == 8)) {
		return 56; /*unsupported color mode conversion*/
	}

	outsize = loadpng_get_raw_size_lct(*w, *h, (&state.info_raw)->colortype, (&state.info_raw)->bitdepth);
	*out = (unsigned char*)malloc(outsize);
	if (!(*out)) {
		state.error = 83; /*alloc fail*/
	}
	else state.error = loadpng_convert(*out, data, &state.info_raw,
		&state.info_png.color, *w, *h);
	free(data);
	loadpng_state_cleanup(&state);
	return state.error;
}

static unsigned addChunk_IHDR(vector<unsigned char> &out, unsigned w, unsigned h, LoadPNGColorType colortype, unsigned bitdepth, unsigned interlace_method) {
	unsigned char *chunk, *data;
	if (unsigned error = loadpng_chunk_init(&chunk, out, 13, "IHDR"))return error;
	data = chunk + 8;

	writeInt32(data + 0, w); /*width*/
	writeInt32(data + 4, h); /*height*/
	data[8] = (unsigned char)bitdepth; /*bit depth*/
	data[9] = (unsigned char)colortype; /*color type*/
	data[10] = 0; /*compression method*/
	data[11] = 0; /*filter method*/
	data[12] = interlace_method; /*interlace method*/

	loadpng_chunk_generate_crc(chunk);
	return 0;
}
static unsigned addChunk_PLTE(vector<unsigned char> &out, const LoadPNGColorMode* info) {
	unsigned char* chunk;
	size_t i, j = 8;

	if (unsigned error = loadpng_chunk_init(&chunk, out, info->palettesize * 3, "PLTE"))return error;

	for (i = 0; i != info->palettesize; ++i) {
		chunk[j++] = info->palette[i * 4 + 0];
		chunk[j++] = info->palette[i * 4 + 1];
		chunk[j++] = info->palette[i * 4 + 2];
	}

	loadpng_chunk_generate_crc(chunk);
	return 0;
}
static unsigned addChunk_tRNS(vector<unsigned char> &out, const LoadPNGColorMode* info) {
	unsigned char* chunk = 0;

	if (info->colortype == LCT_PALETTE) {
		size_t i, amount = info->palettesize;
		for (i = info->palettesize; i != 0; --i) {
			if (info->palette[4 * (i - 1) + 3] != 255) break;
			--amount;
		}
		if (amount) {
			if (unsigned error = loadpng_chunk_init(&chunk, out, amount, "tRNS"))return error;
			for (i = 0; i != amount; ++i) chunk[8 + i] = info->palette[4 * i + 3];
		}
	}
	else if (info->colortype == LCT_GREY) {
		if (info->key_defined) {
			if (unsigned error = loadpng_chunk_init(&chunk, out, 2, "tRNS"))return error;
			chunk[8] = (unsigned char)(info->key_r >> 8);
			chunk[9] = (unsigned char)(info->key_r & 255);
		}
	}
	else if (info->colortype == LCT_RGB) {
		if (info->key_defined) {
			if (unsigned error = loadpng_chunk_init(&chunk, out, 6, "tRNS"))return error;
			chunk[8] = (unsigned char)(info->key_r >> 8);
			chunk[9] = (unsigned char)(info->key_r & 255);
			chunk[10] = (unsigned char)(info->key_g >> 8);
			chunk[11] = (unsigned char)(info->key_g & 255);
			chunk[12] = (unsigned char)(info->key_b >> 8);
			chunk[13] = (unsigned char)(info->key_b & 255);
		}
	}

	if (chunk) loadpng_chunk_generate_crc(chunk);
	return 0;
}
static unsigned addChunk_IDAT(vector<unsigned char> &out, const unsigned char* data, size_t datasize, LoadPNGCompressSettings* zlibsettings) {
	unsigned error = 0;
	unsigned char* zlib = 0;
	size_t zlibsize = 0;

	error = loadpng_zlib_compress(&zlib, &zlibsize, data, datasize, zlibsettings);
	if (!error) {
		error = loadpng_chunk_createv(out, zlibsize, "IDAT", zlib);
	}
	free(zlib);
	return error;
}
static unsigned addChunk_IEND(vector<unsigned char> &out) {
	return loadpng_chunk_createv(out, 0, "IEND", 0);
}
static void filterScanline(unsigned char* out, const unsigned char* scanline, const unsigned char* prevline, size_t length, size_t bytewidth, unsigned char filterType) {
	size_t i;
	switch (filterType) {
	case 0: /*None*/
		for (i = 0; i != length; ++i) out[i] = scanline[i];
		break;
	case 1: /*Sub*/
		for (i = 0; i != bytewidth; ++i) out[i] = scanline[i];
		for (i = bytewidth; i < length; ++i) out[i] = scanline[i] - scanline[i - bytewidth];
		break;
	case 2: /*Up*/
		if (prevline) {
			for (i = 0; i != length; ++i) out[i] = scanline[i] - prevline[i];
		}
		else {
			for (i = 0; i != length; ++i) out[i] = scanline[i];
		}
		break;
	case 3: /*Average*/
		if (prevline) {
			for (i = 0; i != bytewidth; ++i) out[i] = scanline[i] - (prevline[i] >> 1);
			for (i = bytewidth; i < length; ++i) out[i] = scanline[i] - ((scanline[i - bytewidth] + prevline[i]) >> 1);
		}
		else {
			for (i = 0; i != bytewidth; ++i) out[i] = scanline[i];
			for (i = bytewidth; i < length; ++i) out[i] = scanline[i] - (scanline[i - bytewidth] >> 1);
		}
		break;
	case 4: /*Paeth*/
		if (prevline) {
			for (i = 0; i != bytewidth; ++i) out[i] = (scanline[i] - prevline[i]);
			for (i = bytewidth; i < length; ++i) {
				out[i] = (scanline[i] - paethPredictor(scanline[i - bytewidth], prevline[i], prevline[i - bytewidth]));
			}
		}
		else {
			for (i = 0; i != bytewidth; ++i) out[i] = scanline[i];
			for (i = bytewidth; i < length; ++i) out[i] = (scanline[i] - scanline[i - bytewidth]);
		}
		break;
	default: return; /*invalid filter type given*/
	}
}
static unsigned filter(unsigned char* out, const unsigned char* in, unsigned w, unsigned h, const LoadPNGColorMode* color, const LoadPNGEncoderSettings* settings) {
	unsigned bpp = getNumColorChannels(color->colortype) * color->bitdepth;
	size_t linebytes = loadpng_get_raw_size_idat(w, 1, bpp) - 1u;
	size_t bytewidth = (bpp + 7u) / 8u;
	const unsigned char* prevline = 0;
	unsigned x, y;
	unsigned error = 0;
	LoadPNGFilterStrategy strategy = settings->filter_strategy;
	if (settings->filter_palette_zero &&
		(color->colortype == LCT_PALETTE || color->bitdepth < 8)) strategy = LFS_ZERO;

	if (bpp == 0) return 31; /*error: invalid color type*/

	if (strategy >= LFS_ZERO && strategy <= LFS_FOUR) {
		unsigned char type = (unsigned char)strategy;
		for (y = 0; y != h; ++y) {
			size_t outindex = (1 + linebytes) * y; /*the extra filterbyte added to each row*/
			size_t inindex = linebytes * y;
			out[outindex] = type; /*filter type byte*/
			filterScanline(&out[outindex + 1], &in[inindex], prevline, linebytes, bytewidth, type);
			prevline = &in[inindex];
		}
	}
	else if (strategy == LFS_MINSUM) {
		unsigned char* attempt[5]; /*five filtering attempts, one for each filter type*/
		size_t smallest = 0;
		unsigned char type, bestType = 0;

		for (type = 0; type != 5; ++type) {
			attempt[type] = (unsigned char*)malloc(linebytes);
			if (!attempt[type]) error = 83; /*alloc fail*/
		}

		if (!error) {
			for (y = 0; y != h; ++y) {
				for (type = 0; type != 5; ++type) {
					size_t sum = 0;
					filterScanline(attempt[type], &in[y * linebytes], prevline, linebytes, bytewidth, type);
					if (type == 0) {
						for (x = 0; x != linebytes; ++x) sum += (unsigned char)(attempt[type][x]);
					}
					else {
						for (x = 0; x != linebytes; ++x) {
							unsigned char s = attempt[type][x];
							sum += s < 128 ? s : (255U - s);
						}
					}
					if (type == 0 || sum < smallest) {
						bestType = type;
						smallest = sum;
					}
				}

				prevline = &in[y * linebytes];
				out[y * (linebytes + 1)] = bestType; /*the first byte of a scanline will be the filter type*/
				for (x = 0; x != linebytes; ++x) out[y * (linebytes + 1) + 1 + x] = attempt[bestType][x];
			}
		}

		for (type = 0; type != 5; ++type) free(attempt[type]);
	}
	else if (strategy == LFS_ENTROPY) {
		unsigned char* attempt[5]; /*five filtering attempts, one for each filter type*/
		size_t bestSum = 0;
		unsigned type, bestType = 0;
		unsigned count[256];

		for (type = 0; type != 5; ++type) {
			attempt[type] = (unsigned char*)malloc(linebytes);
			if (!attempt[type]) error = 83; /*alloc fail*/
		}

		if (!error) {
			for (y = 0; y != h; ++y) {
				for (type = 0; type != 5; ++type) {
					size_t sum = 0;
					filterScanline(attempt[type], &in[y * linebytes], prevline, linebytes, bytewidth, type);
					memset(count, 0, 256 * sizeof(*count));
					for (x = 0; x != linebytes; ++x) ++count[attempt[type][x]];
					++count[type]; /*the filter type itself is part of the scanline*/
					for (x = 0; x != 256; ++x) {
						sum += count[x] * log2(count[x]);
					}
					if (type == 0 || sum > bestSum) {
						bestType = type;
						bestSum = sum;
					}
				}

				prevline = &in[y * linebytes];
				out[y * (linebytes + 1)] = bestType; /*the first byte of a scanline will be the filter type*/
				for (x = 0; x != linebytes; ++x) out[y * (linebytes + 1) + 1 + x] = attempt[bestType][x];
			}
		}

		for (type = 0; type != 5; ++type) free(attempt[type]);
	}
	else if (strategy == LFS_PREDEFINED) {
		for (y = 0; y != h; ++y) {
			size_t outindex = (1 + linebytes) * y; /*the extra filterbyte added to each row*/
			size_t inindex = linebytes * y;
			unsigned char type = settings->predefined_filters[y];
			out[outindex] = type; /*filter type byte*/
			filterScanline(&out[outindex + 1], &in[inindex], prevline, linebytes, bytewidth, type);
			prevline = &in[inindex];
		}
	}
	else if (strategy == LFS_BRUTE_FORCE) {
		size_t size[5];
		unsigned char* attempt[5]; /*five filtering attempts, one for each filter type*/
		size_t smallest = 0;
		unsigned type = 0, bestType = 0;
		unsigned char* dummy;
		LoadPNGCompressSettings zlibsettings;
		memcpy(&zlibsettings, &settings->zlibsettings, sizeof(LoadPNGCompressSettings));
		zlibsettings.btype = 1;
		for (type = 0; type != 5; ++type) {
			attempt[type] = (unsigned char*)malloc(linebytes);
			if (!attempt[type]) error = 83; /*alloc fail*/
		}
		if (!error) {
			for (y = 0; y != h; ++y) /*try the 5 filter types*/ {
				for (type = 0; type != 5; ++type) {
					unsigned testsize = (unsigned)linebytes;
					filterScanline(attempt[type], &in[y * linebytes], prevline, linebytes, bytewidth, type);
					size[type] = 0;
					dummy = 0;
					loadpng_zlib_compress(&dummy, &size[type], attempt[type], testsize, &zlibsettings);
					free(dummy);
					if (type == 0 || size[type] < smallest) {
						bestType = type;
						smallest = size[type];
					}
				}
				prevline = &in[y * linebytes];
				out[y * (linebytes + 1)] = bestType; /*the first byte of a scanline will be the filter type*/
				for (x = 0; x != linebytes; ++x) out[y * (linebytes + 1) + 1 + x] = attempt[bestType][x];
			}
		}
		for (type = 0; type != 5; ++type) free(attempt[type]);
	}
	else return 88; /* unknown filter strategy */
	return error;
}
static void addPaddingBits(unsigned char* out, const unsigned char* in, size_t olinebits, size_t ilinebits, unsigned h) {
	unsigned y;
	size_t diff = olinebits - ilinebits;
	size_t obp = 0, ibp = 0; /*bit pointers*/
	for (y = 0; y != h; ++y) {
		size_t x;
		for (x = 0; x < ilinebits; ++x) {
			unsigned char bit = readBitFromReversedStream(&ibp, in);
			setBitOfReversedStream(&obp, out, bit);
		}
		for (x = 0; x != diff; ++x) setBitOfReversedStream(&obp, out, 0);
	}
}
static void Adam7_interlace(unsigned char* out, const unsigned char* in, unsigned w, unsigned h, unsigned bpp) {
	unsigned passw[7], passh[7];
	size_t filter_passstart[8], padded_passstart[8], passstart[8];
	unsigned i;

	Adam7_getpassvalues(passw, passh, filter_passstart, padded_passstart, passstart, w, h, bpp);

	if (bpp >= 8) {
		for (i = 0; i != 7; ++i) {
			unsigned x, y, b;
			size_t bytewidth = bpp / 8u;
			for (y = 0; y < passh[i]; ++y)
				for (x = 0; x < passw[i]; ++x) {
					size_t pixelinstart = ((ADAM7_IY[i] + y * ADAM7_DY[i]) * w + ADAM7_IX[i] + x * ADAM7_DX[i]) * bytewidth;
					size_t pixeloutstart = passstart[i] + (y * passw[i] + x) * bytewidth;
					for (b = 0; b < bytewidth; ++b) {
						out[pixeloutstart + b] = in[pixelinstart + b];
					}
				}
		}
	}
	else {
		for (i = 0; i != 7; ++i) {
			unsigned x, y, b;
			unsigned ilinebits = bpp * passw[i];
			unsigned olinebits = bpp * w;
			size_t obp, ibp; /*bit pointers (for out and in buffer)*/
			for (y = 0; y < passh[i]; ++y)
				for (x = 0; x < passw[i]; ++x) {
					ibp = (ADAM7_IY[i] + y * ADAM7_DY[i]) * olinebits + (ADAM7_IX[i] + x * ADAM7_DX[i]) * bpp;
					obp = (8 * passstart[i]) + (y * ilinebits + x * bpp);
					for (b = 0; b < bpp; ++b) {
						unsigned char bit = readBitFromReversedStream(&ibp, in);
						setBitOfReversedStream(&obp, out, bit);
					}
				}
		}
	}
}
static unsigned preProcessScanlines(unsigned char** out, size_t* outsize, const unsigned char* in, unsigned w, unsigned h, const LoadPNGInfo* info_png, const LoadPNGEncoderSettings* settings) {
	unsigned bpp = getNumColorChannels((&info_png->color)->colortype) * (&info_png->color)->bitdepth;
	unsigned error = 0;

	if (info_png->interlace_method == 0) {
		*outsize = h + (h * ((w * bpp + 7u) / 8u)); /*image size plus an extra byte per scanline + possible padding bits*/
		*out = (unsigned char*)malloc(*outsize);
		if (!(*out) && (*outsize)) error = 83; /*alloc fail*/

		if (!error) {
			if (bpp < 8 && w * bpp != ((w * bpp + 7u) / 8u) * 8u) {
				unsigned char* padded = (unsigned char*)malloc(h * ((w * bpp + 7u) / 8u));
				if (!padded) error = 83; /*alloc fail*/
				if (!error) {
					addPaddingBits(padded, in, ((w * bpp + 7u) / 8u) * 8u, w * bpp, h);
					error = filter(*out, padded, w, h, &info_png->color, settings);
				}
				free(padded);
			}
			else {
				error = filter(*out, in, w, h, &info_png->color, settings);
			}
		}
	}
	else {
		unsigned passw[7], passh[7];
		size_t filter_passstart[8], padded_passstart[8], passstart[8];
		unsigned char* adam7;

		Adam7_getpassvalues(passw, passh, filter_passstart, padded_passstart, passstart, w, h, bpp);

		*outsize = filter_passstart[7]; /*image size plus an extra byte per scanline + possible padding bits*/
		*out = (unsigned char*)malloc(*outsize);
		if (!(*out)) error = 83; /*alloc fail*/

		adam7 = (unsigned char*)malloc(passstart[7]);
		if (!adam7 && passstart[7]) error = 83; /*alloc fail*/

		if (!error) {
			unsigned i;
			Adam7_interlace(adam7, in, w, h, bpp);
			for (i = 0; i != 7; ++i) {
				if (bpp < 8) {
					unsigned char* padded = (unsigned char*)malloc(padded_passstart[i + 1] - padded_passstart[i]);
					if (!padded) {
						error = 83;
						break;
					}
					addPaddingBits(padded, &adam7[passstart[i]],
						((passw[i] * bpp + 7u) / 8u) * 8u, passw[i] * bpp, passh[i]);
					error = filter(&(*out)[filter_passstart[i]], padded,
						passw[i], passh[i], &info_png->color, settings);
					free(padded);
				}
				else {
					error = filter(&(*out)[filter_passstart[i]], &adam7[padded_passstart[i]],
						passw[i], passh[i], &info_png->color, settings);
				}

				if (error) break;
			}
		}
		free(adam7);
	}
	return error;
}
unsigned loadpng_encode_memory(unsigned char** out, size_t* outsize, const unsigned char* image, unsigned w, unsigned h, LoadPNGColorType colortype, unsigned bitdepth) {
	unsigned error;
	LoadPNGState state;
	loadpng_state_init(&state);
	state.info_raw.colortype = colortype;
	state.info_raw.bitdepth = bitdepth;
	state.info_png.color.colortype = colortype;
	state.info_png.color.bitdepth = bitdepth;

	unsigned char* data = 0; /*uncompressed version of the IDAT chunk data*/
	size_t datasize = 0;
	vector<unsigned char> outv;
	LoadPNGInfo info;
	const LoadPNGInfo* info_png = &state.info_png;

	loadpng_info_init(&info);
	*out = 0;
	*outsize = 0;
	state.error = 0;
	if ((info_png->color.colortype == LCT_PALETTE || state.encoder.force_palette)
		&& (info_png->color.palettesize == 0 || info_png->color.palettesize > 256)) {
		state.error = 68; /*invalid palette size, it is only allowed to be 1-256*/
		goto cleanup;
	}
	if (state.encoder.zlibsettings.btype > 2) {
		state.error = 61; /*error: invalid btype*/
		goto cleanup;
	}
	if (info_png->interlace_method > 1) {
		state.error = 71; /*error: invalid interlace mode*/
		goto cleanup;
	}
	state.error = checkColorValidity(info_png->color.colortype, info_png->color.bitdepth);
	if (state.error) goto cleanup; /*error: invalid color type given*/
	state.error = checkColorValidity(state.info_raw.colortype, state.info_raw.bitdepth);
	if (state.error) goto cleanup; /*error: invalid color type given*/
	loadpng_info_copy(&info, &state.info_png);
	LoadPNGColorStats stats;
	loadpng_color_stats_init(&stats);
	state.error = loadpng_compute_color_stats(&stats, image, w, h, &state.info_raw);
	if (state.error) goto cleanup;
	state.error = auto_choose_color(&info.color, &state.info_raw, &stats);
	if (state.error) goto cleanup;
	if (!loadpng_color_mode_equal(&state.info_raw, &info.color)) {
		unsigned char* converted;
		size_t size = ((size_t)w * (size_t)h * (size_t)(getNumColorChannels((&info.color)->colortype) * (&info.color)->bitdepth) + 7u) / 8u;

		converted = (unsigned char*)malloc(size);
		if (!converted && size) state.error = 83; /*alloc fail*/
		if (!state.error) {
			state.error = loadpng_convert(converted, image, &info.color, &state.info_raw, w, h);
		}
		if (!state.error) {
			state.error = preProcessScanlines(&data, &datasize, converted, w, h, &info, &state.encoder);
		}
		free(converted);
		if (state.error) goto cleanup;
	}
	else {
		state.error = preProcessScanlines(&data, &datasize, image, w, h, &info, &state.encoder);
		if (state.error) goto cleanup;
	}

	outv.insert(outv.end(), { 137, 80, 78, 71, 13, 10, 26, 10 });
	if (state.error) goto cleanup;
	state.error = addChunk_IHDR(outv, w, h, info.color.colortype, info.color.bitdepth, info.interlace_method);
	if (state.error) goto cleanup;
	if (info.color.colortype == LCT_PALETTE) {
		state.error = addChunk_PLTE(outv, &info.color);
		if (state.error) goto cleanup;
	}
	if (state.encoder.force_palette && (info.color.colortype == LCT_RGB || info.color.colortype == LCT_RGBA)) {
		state.error = addChunk_PLTE(outv, &info.color);
		if (state.error) goto cleanup;
	}
	state.error = addChunk_tRNS(outv, &info.color);
	if (state.error) goto cleanup;
	state.error = addChunk_IDAT(outv, data, datasize, &state.encoder.zlibsettings);
	if (state.error) goto cleanup;
	state.error = addChunk_IEND(outv);
	if (state.error) goto cleanup;

cleanup:
	loadpng_palette_clear(&info.color);
	free(data);

	*out = (unsigned char *)malloc(outv.size());
	memcpy(*out, outv.data(), outv.size());
	*outsize = outv.size();

	error = state.error;
	loadpng_state_cleanup(&state);
	return error;
}




bitMap loadPng(SGtext filename) {
	FILE* fp;
	long size;
	fp = fopen(filename, "rb");
	if (!fp) return bitMap();
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	vector<unsigned char> file;
	file.resize(size);
	fseek(fp, 0, SEEK_SET);
	fread(file.data(), 1, size, fp);
	fclose(fp);

	vector<unsigned char> image;
	unsigned width, height;

	const unsigned char* in = file.data();
	size_t insize = file.size();
	unsigned char* buffer = 0;
	loadpng_decode_memory(&buffer, &width, &height, in, insize, LCT_RGB, 8);
	if (buffer) {
		LoadPNGColorMode info_raw;
		info_raw.colortype = LCT_RGB;
		info_raw.bitdepth = 8;
		size_t buffersize = loadpng_get_raw_size_lct(width, height, (&info_raw)->colortype, (&info_raw)->bitdepth);
		image.insert(image.end(), &buffer[0], &buffer[buffersize]);
	}
	free(buffer);

	bitMap res = bitMap();
	res.sizeX = width;
	res.sizeY = height;
	res.data = (byte *)malloc(width * height * 3 * sizeof(char));
	memcpy(res.data, image.data(), width * height * 3 * sizeof(char));
	convertColor(res);
	return res;
}
void savePng(SGtext filename, bitMap png) {
	vector<unsigned char> image(png.sizeX * png.sizeY * 3);
	convertColor(png);
	memcpy(image.data(), png.data, image.size());

	unsigned char* buffer;
	size_t buffersize;
	loadpng_encode_memory(&buffer, &buffersize, image.data(), png.sizeX, png.sizeY, LCT_RGB, 8);
	FILE* file;
	file = fopen(filename, "wb");
	if (!file) return;
	fwrite(buffer, 1, buffersize, file);
	fclose(file);
}

//JPG

bitMap loadJpg(SGtext filename) {
	return bitMap();
}
void saveJpg(SGtext filename, bitMap bmp) {

}

//Operation.

void convertColor(bitMap src) {
	char tmp;
	for (int i = 0; i < src.sizeX * src.sizeY; i++) {
		tmp = src.data[i * 3];
		src.data[i * 3] = src.data[i * 3 + 2];
		src.data[i * 3 + 2] = tmp;
	}
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
