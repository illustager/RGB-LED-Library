#pragma once

#include "RGBActionBase.h"

class RGBActionSolid : public RGBActionBase {
public:
	RGBActionSolid() = delete;
	RGBActionSolid(size_t len, std::initializer_list<RGB> colors, unsigned lengthy);
	~RGBActionSolid() = default;

	int act(RGB *leds, int *args) override;

private:
	unsigned lengthy;
};
