#pragma once

#include "RGBActionBase.h"

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
