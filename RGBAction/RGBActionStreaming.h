#pragma once

#include "RGBActionBase.h"

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
