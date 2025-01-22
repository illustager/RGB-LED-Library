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
	
	virtual void write_0() = 0;
	virtual void write_1() = 0;
	
	virtual void reset() = 0;
	virtual void flush() = 0;

}; // class WS2812Base
