/*
 * RGBAction.h
 *
 * Copyright (c) 2024 Yang D.L.
 *
 * License: GPL-2.0-or-later
*/

#pragma once

#include "../RGBLEDController/WS2812.h"

using ScrawlFuncType = void (*)(RGB*, size_t);

enum class RGBActionType {
	Solid,
	Fading,
	Cycle,
	Floating,
	Growing,
	Streaming,
};

class RGBActionBase {
protected:
	RGBActionBase() = delete;
	RGBActionBase(RGBActionType type, size_t len, std::initializer_list<RGB> colors);
	~RGBActionBase();

public:
	virtual int act(RGB *leds, int *args) = 0;

protected:
	RGBActionType type;
	RGB *initial_colors;
	size_t len;
	bool isHeritance;
};

class RGBActionSolid : public RGBActionBase {
public:
	RGBActionSolid() = delete;
	RGBActionSolid(size_t len, std::initializer_list<RGB> colors, unsigned lengthy);
	~RGBActionSolid() = default;

	int act(RGB *leds, int *args) override;

private:
	unsigned lengthy;
};

class RGBActionFading : public RGBActionBase {
public:
	RGBActionFading() = delete;
	RGBActionFading(size_t len, std::initializer_list<RGB> colors, std::initializer_list<unsigned> section, unsigned interval);
	~RGBActionFading() = default;

	int act(RGB *leds, int *args) override;

private:
	int start;
	int end;
	int step;

	unsigned interval;
};

class RGBActionCycle : public RGBActionBase {
public:
	RGBActionCycle() = delete;
	RGBActionCycle(size_t len, std::initializer_list<RGB> colors, unsigned interval, unsigned times, bool isUp);
	~RGBActionCycle() = default;

	int act(RGB *leds, int *args) override;

private:
	unsigned interval;
	bool isUp;
	unsigned times;
};

class RGBActionFloating : public RGBActionBase {
public:
	RGBActionFloating() = delete;
	RGBActionFloating(size_t len, std::initializer_list<RGB>, unsigned interval, bool isUp);
	~RGBActionFloating() = default;

	int act(RGB *leds, int *args) override;

private:
	unsigned interval;
	bool isUp;
};

class RGBActionStreaming : public RGBActionBase {
public:
	RGBActionStreaming() = delete;
	RGBActionStreaming(size_t len, std::initializer_list<RGB>, unsigned interval, bool isUp, bool prevMove, bool currMove);
	~RGBActionStreaming() = default;

	int act(RGB *leds, int *args) override;

private:
	unsigned interval;
	bool isUp;
	bool prevMove;
	bool currMove;
};

using RGBAction = RGBActionBase;
