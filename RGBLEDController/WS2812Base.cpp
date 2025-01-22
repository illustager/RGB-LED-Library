#include "WS2812Base.h"

WS2812Base::WS2812Base(unsigned ledCount) :
	RGBLEDControllerBase(ledCount)
{
	;
}

WS2812Base::~WS2812Base() {
	;
}

void WS2812Base::show() {
	if (!isEnabled()) {
		return;
	}

	RGB *leds = getLEDs();

	reset();
	for (unsigned i = 0; i < getLEDCount(); ++i) {
		send(leds[i]);
	}

	flush();
}

void WS2812Base::disable() {
	if (!isEnabled()) {
		return;
	}

	RGB *leds = getLEDs();
	for (unsigned i = 0; i < getLEDCount(); ++i) {
		leds[i] = RGB(0, 0, 0);
	}
	show();

	RGBLEDControllerBase::disable();
}

void WS2812Base::send(RGB rgb) {
	uint32_t value = 0x00000000;

	brighten(rgb);
	value = (((uint32_t)rgb.g << 16) | ((uint32_t)rgb.r << 8) | ((uint32_t)rgb.b));
	
	for (int i = 0; i < 24; ++i) {
		if ((value & 0x800000) != 0) {
			write_1();
		} else {
			write_0();
		}
		value <<= 1;
	}
}

void WS2812Base::brighten(RGB& rgb) {
	uint8_t brightness = getBrightness();

	rgb.r = (uint8_t)((uint16_t)rgb.r * (brightness + 1) >> 8);
	rgb.g = (uint8_t)((uint16_t)rgb.g * (brightness + 1) >> 8);
	rgb.b = (uint8_t)((uint16_t)rgb.b * (brightness + 1) >> 8);
}
