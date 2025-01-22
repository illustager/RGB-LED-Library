/*
 * RGBPlayer.cpp
 *
 * Copyright (c) 2024 Yang D.L.
 *
 * License: GPL-2.0-or-later
*/

#include "RGBPlayer.h"

// #include "log.h"

RGBPlayer::RGBPlayer(RGBLEDControllerBase *ledController, size_t start, const RGBActionSeries *actions, std::initializer_list<std::initializer_list<size_t> > orders)
	: ledController(ledController), start(start), actions(actions), state(RGBPlayerState::STOP), actionOrderSelect(0), actionOrderIndex(0) {

	this->orders.reserve(orders.size());
	for (auto &order : orders) {
		this->orders.push_back(std::vector<size_t>(order));
	}
	stop();
}

RGBPlayer::~RGBPlayer() {
	;
}

void RGBPlayer::stop() {
	state = RGBPlayerState::STOP;

	for (size_t i = 0; i < actions->size(); ++i) {
		actions->at(i)->act(nullptr, action_args);
	}

	// actionOrderSelect = 0;
	actionOrderIndex = 0;

	ledController->disable();
}

void RGBPlayer::pause() {
	state = RGBPlayerState::PAUSE;
}

void RGBPlayer::resume() {
	state = RGBPlayerState::PLAY;

	ledController->enable();
}

int RGBPlayer::play() {
	if (orders[actionOrderSelect].empty()) {
		return -1;
	}
	if (state != RGBPlayerState::PLAY) {
		return -2;
	}

	int index = 0;
	int select = 0;

	select = orders[actionOrderSelect][actionOrderIndex];
	RGBActionBase *action = actions->at(select);

	index = action->act(ledController->getLEDs() + start, action_args);

	// log_i("Action %d: %d by order %d", select, index, actionOrderIndex);

	if (index < 0) {
		action->act(nullptr, action_args);
		actionOrderIndex = (actionOrderIndex + 1) % orders[actionOrderSelect].size();
		select = orders[actionOrderSelect][actionOrderIndex];
		action = actions->at(select);
		action->act(nullptr, action_args);

		index = action->act(ledController->getLEDs() + start, action_args);
		// log_i("Action %d: %d by order %d", select, index, actionOrderIndex);
	}

	return index;
}

void RGBPlayer::show() {
	ledController->show();
}

void RGBPlayer::next() {
	actions->at(orders[actionOrderSelect][actionOrderIndex])->act(nullptr, action_args);

	actionOrderSelect = (actionOrderSelect + 1) % orders.size();
	actionOrderIndex = 0;

	actions->at(orders[actionOrderSelect][actionOrderIndex])->act(nullptr, action_args);
}

void RGBPlayer::prev() {
	actions->at(orders[actionOrderSelect][actionOrderIndex])->act(nullptr, action_args);

	actionOrderSelect = (actionOrderSelect + orders.size() - 1) % orders.size();
	actionOrderIndex = 0;

	actions->at(orders[actionOrderSelect][actionOrderIndex])->act(nullptr, action_args);
}

void RGBPlayer::setBrightness(uint8_t brightness) {
	ledController->setBrightness(brightness);
}

uint8_t RGBPlayer::getBrightness() const {
	return ledController->getBrightness();
}
