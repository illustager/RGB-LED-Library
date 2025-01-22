/*
 * RGB.h
 *
 * Copyright (c) 2024 Yang D.L.
 *
 * License: GPL-2.0-or-later
*/

#pragma once

#include <Arduino.h>

class RGB {
public:
	uint8_t r, g, b;

	~RGB() = default;

	RGB();
	RGB(const RGB& rgb);
	RGB(uint32_t rgb);
	RGB(const char *rgb);
	RGB(const String& rgb);
	RGB(uint8_t r, uint8_t g, uint8_t b);
	RGB(std::initializer_list<uint8_t> list);

	RGB& operator=(const RGB& rgb);
	RGB& operator=(uint32_t rgb);
	RGB& operator=(const char *rgb);
	RGB& operator=(const String& rgb);
	RGB& operator=(std::initializer_list<uint8_t> list);

	RGB operator&(const RGB& rgb) const;
	RGB operator|(const RGB& rgb) const;
	RGB operator~() const;

	RGB operator+(const RGB& rgh) const;
	RGB operator-(const RGB& rgh) const;
	RGB operator*(double k) const;

	static const RGB White;
	static const RGB Red;
	static const RGB Green;
	static const RGB Blue;
	static const RGB Yellow;
	static const RGB Cyan;
	static const RGB Magenta;
};
