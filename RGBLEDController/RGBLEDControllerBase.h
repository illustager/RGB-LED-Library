#pragma once

#include "../RGB.h"

class RGBLEDControllerBase {
public:
	RGB			*getLEDs();
	unsigned	getLEDCount();

	uint8_t		 getBrightness();
	virtual void setBrightness(uint8_t brightness);

	virtual void enable();
	virtual void disable();

	virtual void reset() = 0;
	virtual void show()  = 0;

protected:
	RGBLEDControllerBase(unsigned ledCount);
	~RGBLEDControllerBase();

	RGBLEDControllerBase(const RGBLEDControllerBase& ledController) = delete;
	RGBLEDControllerBase& operator=(const RGBLEDControllerBase& ledController) = delete;

protected:
	RGB		  	*m_leds;
	unsigned	m_ledCount;
	bool		m_enabled;
	uint8_t		m_brightness;
}; // class RGBLEDControllerBase
