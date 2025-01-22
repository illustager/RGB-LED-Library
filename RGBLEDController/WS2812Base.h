#pragma once

#include "RGBLEDControllerBase.h"

class WS2812Base : public RGBLEDControllerBase {
public:
	void show()  override;
	void disable() override;

protected:
	WS2812Base(unsigned ledCount);
	~WS2812Base();

	WS2812Base(const WS2812Base& ws2812) = delete;
	WS2812Base& operator=(const WS2812Base& ws2812) = delete;

protected:
	void send(RGB rgb);
	void brighten(RGB& rgb);
	void reset();
	
	virtual void write_bits(bool *bits, size_t count) = 0;
	virtual void delay_us(uint32_t us) = 0;
}; // class WS2812Base
