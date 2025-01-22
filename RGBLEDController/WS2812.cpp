/*
 * WS2812.cpp
 *
 * Copyright (c) 2024 Yang D.L.
 *
 * License: GPL-2.0-or-later
*/

#include "WS2812.h"

// #include "log.h"

// ------------------------------ WS2812 ------------------------------
WS2812::WS2812(WS2812CommunicationMode mode, size_t num_leds, uint8_t pin, std::initializer_list<uint8_t> other_pins, scrawl_func_t fscrawl)
	: mode(mode), num_leds(num_leds), pin(pin), brightness(255), abled(true), fscrawl(fscrawl) {
	leds = new RGB[num_leds];

	this->other_pins = new uint8_t[other_pins.size()];
	auto it = other_pins.begin();
	for (size_t i = 0; i < other_pins.size(); ++i) {
		this->other_pins[i] = *it++;
	}

	begin();
}

WS2812::~WS2812() {
	end();

	delete[] leds;
	delete[] other_pins;

	switch (mode) {
	case WS2812CommunicationMode::SPI:
		delete pSPI;
		break;

	default:
		break;
	}
}

void WS2812::setBrightness(uint8_t brightness) {
	this->brightness = brightness;
}

uint8_t WS2812::getBrightness() {
	return brightness;
}

RGB* WS2812::getLEDs() {
	return leds;
}

size_t WS2812::getNumLEDs() {
	return num_leds;
}

void WS2812::flush() {
	if (!abled) {
		return;
	}
	
	RGB *buf = leds;
	if (fscrawl != nullptr) {
		buf = fscrawl(leds);
	}

	// noInterrupts();
	reset();
	for (register int i = 0; i < num_leds; ++i) {
		send(buf[i]);
	}
	// interrupts();
}

void WS2812::begin() {
	switch (mode) {
	case WS2812CommunicationMode::SPI:
		pSPI = new SPIClass(pin, other_pins[0], other_pins[1]);
		pSPI->begin();
		pSPI->beginTransaction(SPISettings(8e6, MSBFIRST, SPI_MODE1));
		break;
	
	default:
		break;
	}

	enable();
}

void WS2812::end() {
	switch (mode) {
	case WS2812CommunicationMode::SPI:
		pSPI->endTransaction();
		pSPI->end();
		delete pSPI;
		break;
	
	default:
		break;
	}

	disable();
}

inline void WS2812::write_0() {
	switch (mode) {
	case WS2812CommunicationMode::SPI:
		pSPI->transfer(0xC0);
		break;
	
	default:
		break;
	}
}

inline void WS2812::write_1() {
	switch (mode) {
	case WS2812CommunicationMode::SPI:
		pSPI->transfer(0xF8);
		asm volatile("nop");
		asm volatile("nop");
		break;
	
	default:
		break;
	}
}

void WS2812::reset() {
	delayMicroseconds(300);
}

inline void WS2812::brighten(RGB& rgb) {
	rgb.r = (uint8_t)((uint16_t)rgb.r * brightness / 255);
	rgb.g = (uint8_t)((uint16_t)rgb.g * brightness / 255);
	rgb.b = (uint8_t)((uint16_t)rgb.b * brightness / 255);
}

inline void WS2812::send(RGB rgb) {
	uint32_t value = 0x00000000;
	uint8_t buf[24];

	brighten(rgb);
	value = (((uint32_t)rgb.g << 16) | ((uint32_t)rgb.r << 8) | ((uint32_t)rgb.b));
	
	for (int i = 0; i < 24; ++i) {
		if ((value & 0x800000) != 0) {
			buf[i] = 0xF8;
		} else {
			buf[i] = 0xC0;
		}
		value <<= 1;
	}

	switch (mode) {
	case WS2812CommunicationMode::SPI:
		pSPI->transfer(buf, (size_t)24, true);
		break;

	default:
		break;
	}
}

void WS2812::disable() {
	if (!abled) {
		return;
	}

	for (size_t i = 0; i < num_leds; ++i) {
		leds[i] = RGB(0, 0, 0);
	}
	flush();

	abled = false;
}

void WS2812::enable() {
	abled = true;
}
