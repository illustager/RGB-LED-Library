/*
 * RGB.cpp
 *
 * Copyright (c) 2024 Yang D.L.
 *
 * License: GPL-2.0-or-later
*/

#include "RGB.h"

// ------------------------------ RGB ------------------------------
RGB::RGB() : r(0), g(0), b(0) {}

RGB::RGB(const RGB& rgb) : r(rgb.r), g(rgb.g), b(rgb.b) {}

RGB::RGB(uint32_t rgb) : r((rgb >> 16) & 0xFF), g((rgb >> 8) & 0xFF), b(rgb & 0xFF) {}

RGB::RGB(const char *rgb) {
	*this = String(rgb);
}

RGB::RGB(const String& rgb) {
	if (rgb.length() == 7 && rgb[0] == '#') {
		r = (uint8_t)strtol(rgb.substring(1, 3).c_str(), NULL, 16);
		g = (uint8_t)strtol(rgb.substring(3, 5).c_str(), NULL, 16);
		b = (uint8_t)strtol(rgb.substring(5, 7).c_str(), NULL, 16);
	} else {
		r = g = b = 0;
	}
}

RGB::RGB(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

RGB::RGB(std::initializer_list<uint8_t> list) {
	if (list.size() == 3) {
		auto it = list.begin();
		r = *it++;
		g = *it++;
		b = *it;
	} else {
		r = g = b = 0;
	}
}

RGB& RGB::operator=(const RGB& rgb) {
	r = rgb.r;
	g = rgb.g;
	b = rgb.b;
	return *this;
}

RGB& RGB::operator=(uint32_t rgb) {
	r = (rgb >> 16) & 0xFF;
	g = (rgb >> 8) & 0xFF;
	b = rgb & 0xFF;
	return *this;
}

RGB& RGB::operator=(const char *rgb) {
	return *this = String(rgb);
}

RGB& RGB::operator=(const String& rgb) {
	if (rgb.length() == 7 && rgb[0] == '#') {
		r = (uint8_t)strtol(rgb.substring(1, 3).c_str(), NULL, 16);
		g = (uint8_t)strtol(rgb.substring(3, 5).c_str(), NULL, 16);
		b = (uint8_t)strtol(rgb.substring(5, 7).c_str(), NULL, 16);
	} else {
		r = g = b = 0;
	}
	return *this;
}

RGB& RGB::operator=(std::initializer_list<uint8_t> list) {
	if (list.size() == 3) {
		auto it = list.begin();
		r = *it++;
		g = *it++;
		b = *it;
	} else {
		r = g = b = 0;
	}
	return *this;
}

RGB RGB::operator&(const RGB& rgb) const {
	return RGB(r & rgb.r, g & rgb.g, b & rgb.b);
}

RGB RGB::operator|(const RGB& rgb) const {
	return RGB(r | rgb.r, g | rgb.g, b | rgb.b);
}

RGB RGB::operator~() const {
	return RGB(~r, ~g, ~b);
}

RGB RGB::operator+(const RGB& rgb) const {
	return RGB(r + rgb.r, g + rgb.g, b + rgb.b);
}

RGB RGB::operator-(const RGB& rgb) const {
	return RGB(r - rgb.r, g - rgb.g, b - rgb.b);
}

RGB RGB::operator*(double k) const {
	return RGB((uint8_t)(r * k), (uint8_t)(g * k), (uint8_t)(b * k));
}

const RGB RGB::White	= "#FFFFFF";
const RGB RGB::Red		= "#FF0000";
const RGB RGB::Green	= "#00FF00";
const RGB RGB::Blue		= "#0000FF";
const RGB RGB::Yellow	= "#FFFF00";
const RGB RGB::Cyan		= "#00FFFF";
const RGB RGB::Magenta	= "#FF00FF";
