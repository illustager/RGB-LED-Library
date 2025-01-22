#pragma once

#include "RGBActionBase.h"

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
