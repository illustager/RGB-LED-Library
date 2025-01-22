#include "RGBActionSolid.h"

RGBActionSolid::RGBActionSolid(size_t len, std::initializer_list<RGB> colors, unsigned lengthy)
	: RGBActionBase(len, colors), lengthy(lengthy) {
}

int RGBActionSolid::act(RGB *leds, int *args) {
	int &cnt = args[0];

	if (leds == nullptr) {
		cnt = 0;
		return 0;
	}
	if (cnt >= lengthy) {
		return -1;
	}
	if (cnt == 0) {
		if (!isHeritance) {
			for (size_t i = 0; i < len; ++i) {
				leds[i] = initial_colors[i];
			}			
		} else {
			for (size_t i = 0; i < len; ++i) {
				initial_colors[i] = leds[i];
			}
		}
	}

	return ++cnt;
}
