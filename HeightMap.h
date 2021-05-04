#pragma once
#include "bitmap.h"
#include <FL/fl_ask.H>
#include <vector>
#include <iostream>
#include <FL/glu.h>
#include "modelerdraw.h"

#define COLOR_GREY		    0.88f, 0.95f, 0.95

class HeightMap {
public:
	HeightMap();
	HeightMap(const char* fileName);
	void render();

private:
	int width;
	int height;
	unsigned char* bitmap;
	GLuint textureID;

	std::vector<std::vector<float> > heights;

	float size = 0.02;
	float h = 2;
};