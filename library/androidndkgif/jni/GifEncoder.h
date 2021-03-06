#pragma once

#include <stdio.h>
#include <stdint.h>

enum COLOR {
	RED = 0,
	GREEN,
	BLUE,
	COLOR_MAX
};

#define GET_COLOR(color, colorIdx) (((color) >> ((colorIdx) << 3)) & 0xFF)
#define ABS(v) (0 > (v) ? -(v) : (v))
#define ABS_DIFF(a, b) ((a) > (b) ? (a) - (b) : (b) - (a))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

struct Cube {
	static const int COLOR_RANGE = 256;

	uint32_t cMin[COLOR_MAX];
	uint32_t cMax[COLOR_MAX];
	uint32_t colorHistogramFromIndex;
	uint32_t colorHistogramToIndex;
	uint32_t color[COLOR_MAX];
};

struct EncodeRect {
	int32_t x;
	int32_t y;
	int32_t width;
	int32_t height;
};

class GifEncoder {
private:
	static const int32_t MAX_STACK_SIZE = 4096;
	static const int32_t BYTE_NUM = 256;

	uint16_t width;
	uint16_t height;
	int32_t frameNum;
	bool useDither;
	uint32_t* lastPixels;
	uint32_t* lastColorReducedPixels;
	uint32_t lastRootColor;
	FILE* fp;

	void removeSamePixels(uint8_t* src1, uint8_t* src2, EncodeRect* rect);
	void qsortColorHistogram(uint32_t* imageColorHistogram, int32_t maxColor, uint32_t from, uint32_t to);
	void updateColorHistogram(Cube* nextCube, Cube* maxCube, int32_t maxColor, uint32_t* imageColorHistogram);
	void computeColorTable(uint32_t* pixels, Cube* cubes);
	void reduceColor(Cube* cubes, uint32_t cubeNum, uint32_t* pixels);

	void writeHeader();
	bool writeLSD();
	bool writeContents(Cube* cubes, uint8_t* pixels, uint16_t delay, const EncodeRect& encodingRect);
	bool writeNetscapeExt();
	bool writeGraphicControlExt(uint16_t delay);
	bool writeFrame(Cube* cubes, uint8_t* pixels, const EncodeRect& encodingRect);
	bool writeLCT(int32_t colorNum, Cube* cubes);
	bool writeBitmapData(uint8_t* pixels, const EncodeRect& encodingRect);
public:
	GifEncoder();

	bool init(uint16_t width, uint16_t height, const char* fileName);
	void release();
	void setDither(bool useDither);
	uint16_t getWidth();
	uint16_t getHeight();

	void encodeFrame(uint32_t* pixels, int32_t delayMs);
};