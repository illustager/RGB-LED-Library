/*
 * RGBAction.cpp
 *
 * Copyright (c) 2024 Yang D.L.
 *
 * License: GPL-2.0-or-later
*/

#include "RGBActionBase.h"

// ---------------- RGBActionBase ----------------
RGBActionBase::RGBActionBase(size_t len, std::initializer_list<RGB> colors)
	: len(len)
{
	initial_colors = new RGB[len];

	if (colors.size() > 0) {
		for (size_t i = 0; i < len; ++i) {
			initial_colors[i] = *(colors.begin() + i % colors.size());
		}
		isHeritance = false;
	} 
	else {
		isHeritance = true;
	}

}

RGBActionBase::~RGBActionBase() {
	delete[] initial_colors;
}
