#include "RGBActionFloating.h"

RGBActionFloating::RGBActionFloating(size_t len, std::initializer_list<RGB> colors, unsigned interval, bool isUp)
	: RGBActionBase(len, colors), interval(interval), isUp(isUp) {
}

int RGBActionFloating::act(RGB *leds, int *args) {
	int &cnt = args[0];
	int &index = args[1];

	if (leds == nullptr) {
		cnt = 0;
		index = isUp ? 0 : len - 2;
		return 0;
	}
	if (cnt == 0) {
		index = isUp ? 0 : len - 2;
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
	if (index >= len) {
		return -1;
	}

	if (cnt >= interval) {
		cnt = 0;

		if (isUp) {
			std::swap(leds[index], leds[index+1]);
			++index;

			if (index >= len-1) {
				index = len;
			}
		}
		else {
			std::swap(leds[index], leds[index+1]);
			--index;

			if (index < 0) {
				index = len;
			}
		}
	}
	++cnt;

	return index;
}
