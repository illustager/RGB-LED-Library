/*
 * RGBAction.h
 *
 * Copyright (c) 2024 Yang D.L.
 *
 * License: GPL-2.0-or-later
*/

#pragma once

#include "../RGB.h"

class RGBActionBase {
protected:
	RGBActionBase() = delete;
	RGBActionBase(size_t len, std::initializer_list<RGB> colors);
	~RGBActionBase();

public:
	virtual int act(RGB *leds, int *args) = 0;
	virtual unsigned argsCount() = 0;

protected:
	RGB *initial_colors;
	size_t len;
	bool isHeritance;
};
