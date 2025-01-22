#include "RGBActionStreaming.h"

RGBActionStreaming::RGBActionStreaming(size_t len, std::initializer_list<RGB> colors, unsigned interval, bool isUp, bool prevMove, bool currMove)
	: RGBActionBase(len, colors), interval(interval), isUp(isUp), prevMove(prevMove), currMove(currMove) {
}

int RGBActionStreaming::act(RGB *leds, int *args) {
	int &cnt = args[0];
	int &index = args[1];

	if (leds == nullptr) {
		cnt = 0;
		index = isUp ? 0 : len - 1;
		return 0;
	}
	if (cnt == 0) {
		index = isUp ? 0 : len - 1;
	}
	if (index >= len) {
		return -1;
	}

	if (cnt >= interval) {
		cnt = 0;

		if (isUp) {
			if (prevMove) {
				for (int i = len-1; i > index; --i) {
					std::swap(leds[i], leds[i-1]);
				}
			}

			if (currMove) {
				for (int i = index; i > 0; --i) {
					std::swap(leds[i], leds[i-1]);
				}
				leds[0] = initial_colors[len-index-1];
			}
			else {
				leds[index] = initial_colors[index];
			}
			
			++index;

			if (index >= len) {
				index = len;
			}
		}
		else {
			if (prevMove) {
				for (int i = 0; i < index; ++i) {
					std::swap(leds[i], leds[i+1]);
				}
			}

			if (currMove) {
				for (int i = index; i < len-1; ++i) {
					std::swap(leds[i], leds[i+1]);
				}
				leds[len-1] = initial_colors[len-index-1];
			}
			else {
				leds[index] = initial_colors[index];
			}

			--index;

			if (index < 0) {
				index = len;
			}
		}
	}
	++cnt;

	return index;
}
