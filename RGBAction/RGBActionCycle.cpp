#include "RGBActionCycle.h"

RGBActionCycle::RGBActionCycle(size_t len, std::initializer_list<RGB> colors, unsigned interval, unsigned times, bool isUp)
	: RGBActionBase(len, colors), interval(interval), times(times), isUp(isUp) {
}

int RGBActionCycle::act(RGB *leds, int *args) {
	int &cnt = args[0];
	int &t = args[1];

	if (leds == nullptr) {
		cnt = 0;
		t = 0;
		return 0;
	}
	if (cnt == 0) {
		t = 0;
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
	if (t >= times) {
		return -1;
	}
	
	if (cnt >= interval) {
		if (isUp) {
			std::rotate(leds, leds + len - 1, leds + len);
		}
		else {
			std::rotate(leds, leds + 1, leds + len);
		}
		cnt = 0;
		++t;
	}
	++cnt;

	return t;
}

unsigned RGBActionCycle::argsCount() {
	return 2;
}
