#include "RGBActionFading.h"

RGBActionFading::RGBActionFading(size_t len, std::initializer_list<RGB> colors, std::initializer_list<unsigned> section, unsigned interval)
	: RGBActionBase(len, colors), interval(interval) {
	switch (section.size()) {
		case 2:
			start = *section.begin();
			end = *(section.begin() + 1);
			step = 1;
			break;
		
		case 3:
			start = *section.begin();
			end = *(section.begin() + 1);
			step = *(section.begin() + 2);
			break;

		default:
			step = 0;
			break;
	}

	if (start > end) {
		step = -step;
	}

	end = abs(start-end)/abs(step) * step + start;
}

int RGBActionFading::act(RGB *leds, int *args) {
	int &cnt = args[0];
	int &lv = args[1];

	if (leds == nullptr || step == 0) {
		cnt = 0;
		lv = start;
		// log_i("start: %d, end: %d, step: %d", start, end, step);
		return 0;
	}
	if (cnt == 0) {
		lv = start;
		if (!isHeritance) {
			for (size_t i = 0; i < len; ++i) {
				leds[i].r = initial_colors[i].r * lv / 100. + 0.5;
				leds[i].g = initial_colors[i].g * lv / 100. + 0.5;
				leds[i].b = initial_colors[i].b * lv / 100. + 0.5;
			}
		} else {
			for (size_t i = 0; i < len; ++i) {
				leds[i].r = leds[i].r * lv / 100. + 0.5;
				leds[i].g = leds[i].g * lv / 100. + 0.5;
				leds[i].b = leds[i].b * lv / 100. + 0.5;

				initial_colors[i] = leds[i];
			}
		}
	}
	if (lv == end) {
		return -1;
	}

	if (cnt >= interval) {
		cnt = 0;
		lv += step;
	}
	++cnt;

	// log_i("cnt: %d, lv: %d", cnt, lv);
	
	for (int i = 0; i < len; ++i) {
		leds[i].r = initial_colors[i].r * lv / 100. + 0.5;
		leds[i].g = initial_colors[i].g * lv / 100. + 0.5;
		leds[i].b = initial_colors[i].b * lv / 100. + 0.5;
	}
	
	return lv;
}
