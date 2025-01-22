/*
 * RGBAction.cpp
 *
 * Copyright (c) 2024 Yang D.L.
 *
 * License: GPL-2.0-or-later
*/

#include "RGBAction.h"

// #include "log.h"

// ---------------- RGBActionBase ----------------
RGBActionBase::RGBActionBase(RGBActionType type, size_t len, std::initializer_list<RGB> colors)
	: type(type), len(len) {
	initial_colors = new RGB[len];

	if (colors.size() > 0) {
		for (size_t i = 0; i < len; ++i) {
			initial_colors[i] = *(colors.begin() + i % colors.size());
		}
		isHeritance = false;
	} 
	else {
		isHeritance = true;
	}

}

RGBActionBase::~RGBActionBase() {
	delete[] initial_colors;
}

// ---------------- RGBActionSolid ----------------
RGBActionSolid::RGBActionSolid(size_t len, std::initializer_list<RGB> colors, unsigned lengthy)
	: RGBActionBase(RGBActionType::Solid, len, colors), lengthy(lengthy) {
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

// ---------------- RGBActionFading ----------------
RGBActionFading::RGBActionFading(size_t len, std::initializer_list<RGB> colors, std::initializer_list<unsigned> section, unsigned interval)
	: RGBActionBase(RGBActionType::Fading, len, colors), interval(interval) {
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

// ---------------- RGBActionCycle ----------------
RGBActionCycle::RGBActionCycle(size_t len, std::initializer_list<RGB> colors, unsigned interval, unsigned times, bool isUp)
	: RGBActionBase(RGBActionType::Cycle, len, colors), interval(interval), times(times), isUp(isUp) {
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

// ---------------- RGBActionFloating ----------------
RGBActionFloating::RGBActionFloating(size_t len, std::initializer_list<RGB> colors, unsigned interval, bool isUp)
	: RGBActionBase(RGBActionType::Floating, len, colors), interval(interval), isUp(isUp) {
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

// ---------------- RGBActionGrowing ----------------
RGBActionStreaming::RGBActionStreaming(size_t len, std::initializer_list<RGB> colors, unsigned interval, bool isUp, bool prevMove, bool currMove)
	: RGBActionBase(RGBActionType::Growing, len, colors), interval(interval), isUp(isUp), prevMove(prevMove), currMove(currMove) {
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
