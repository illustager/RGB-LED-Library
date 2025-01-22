/*
 * RGBPlayer.h
 *
 * Copyright (c) 2024 Yang D.L.
 *
 * License: GPL-2.0-or-later
*/

#pragma once

#include "RGBLEDController/WS2812.h"
#include "RGBAction/RGBAction.h"

using RGBActionSeries = std::vector<RGBAction*>;

enum class RGBPlayerState {
	STOP,
	PAUSE,
	PLAY,
};

class RGBPlayer {
public:
	RGBPlayer() = delete;
	RGBPlayer( WS2812 *ws2812,
	           size_t start,
			   const RGBActionSeries *actions,
			   std::initializer_list<std::initializer_list<size_t> > orders );
	~RGBPlayer();

	void stop();
	void pause();
	void resume();

	void next();
	void prev();

	int play();

	void flush();

	void setBrightness(uint8_t brightness);
	uint8_t getBrightness() const;

private:
	WS2812 *ws2812;
	size_t start;
	const RGBActionSeries *actions;
	std::vector<std::vector<size_t> > orders;

	RGBPlayerState state;
	size_t actionOrderSelect;
	size_t actionOrderIndex;

	int action_args[3];
};
