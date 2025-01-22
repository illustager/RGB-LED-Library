/*
 * RGBPlayer.h
 *
 * Copyright (c) 2024 Yang D.L.
 *
 * License: GPL-2.0-or-later
*/

#pragma once

#include "RGBLEDController/RGBLEDControllerBase.h"
#include "RGBAction/RGBAction.h"

using RGBActionSeries = std::vector<RGBActionBase*>;

enum class RGBPlayerState {
	STOP,
	PAUSE,
	PLAY,
};

class RGBPlayer {
public:
	RGBPlayer() = delete;
	RGBPlayer( RGBLEDControllerBase *ledController,
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

	void show();

	void setBrightness(uint8_t brightness);
	uint8_t getBrightness() const;

private:
	RGBLEDControllerBase *ledController;
	size_t start;
	const RGBActionSeries *actions;
	std::vector<std::vector<size_t> > orders;

	RGBPlayerState state;
	size_t actionOrderSelect;
	size_t actionOrderIndex;

	int action_args[3];
};
