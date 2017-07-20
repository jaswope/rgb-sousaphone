#pragma once

struct PositionInfo {
	uint8_t pos;
	uint8_t ring;
	uint8_t ring_pos;
	uint8_t theta;
	uint16_t r;
	int16_t x;
	int16_t y;
	int8_t x8;
	int8_t y8;
	int8_t r8;
};

void addGlitter();

void readPalette();

void updatePattern();

void readGlitter();

void readPattern();

void writeDisplay();

void renderDisplay();

void clearDisplay();

