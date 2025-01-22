#pragma once

#include "RGBActionBase.h"

class RGBActionCycle : public RGBActionBase {
public:
	RGBActionCycle() = delete;
	RGBActionCycle(size_t len, std::initializer_list<RGB> colors, unsigned interval, unsigned times, bool isUp);
	~RGBActionCycle() = default;

	int act(RGB *leds, int *args) override;
	unsigned argsCount() override;

private:
	unsigned interval;
	bool isUp;
	unsigned times;
};
