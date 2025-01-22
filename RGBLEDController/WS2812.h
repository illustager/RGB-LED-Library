/*
 * WS2812.h
 *
 * Copyright (c) 2024 Yang D.L.
 *
 * License: GPL-2.0-or-later
*/

#pragma once

#include <Arduino.h>
#include <SPI.h>

#include "../RGB.h"

using scrawl_func_t = RGB*(*)(const RGB*);

enum class WS2812CommunicationMode {
	SPI,
};

class WS2812 {
public:
	WS2812(	WS2812CommunicationMode mode,
			size_t num_leds,
			uint8_t pin,
			std::initializer_list<uint8_t> other_pins, 
			scrawl_func_t fscrawl = nullptr);
	~WS2812();

	WS2812(const WS2812& ws2812) = delete;
	WS2812& operator=(const WS2812& ws2812) = delete;

	void setBrightness(uint8_t brightness);
	uint8_t getBrightness();

	RGB *getLEDs();
	size_t getNumLEDs();

	void reset();
	void flush();

	void disable();
	void enable();

private:
	void begin();
	void end();

	inline void write_0();
	inline void write_1();

	inline void brighten(RGB& rgb);

	inline void send(RGB rgb);

private:
	size_t num_leds;
	uint8_t pin;
	WS2812CommunicationMode mode;

	scrawl_func_t fscrawl;

	uint8_t brightness;
	
	RGB* leds;

	uint8_t *other_pins;
	SPIClass *pSPI;

	bool abled;
};
