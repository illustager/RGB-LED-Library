#include "RGBLEDControllerBase.h"

RGBLEDControllerBase::RGBLEDControllerBase(unsigned ledCount) :
	m_ledCount(ledCount),
	m_leds(new RGB[ledCount]),
	m_enabled(true)
{
	;
}

RGBLEDControllerBase::~RGBLEDControllerBase() {
	delete[] m_leds;
}

RGB *RGBLEDControllerBase::getLEDs() {
	return m_leds;
}

unsigned RGBLEDControllerBase::getLEDCount() {
	return m_ledCount;
}

uint8_t RGBLEDControllerBase::getBrightness() {
	return m_brightness;
}

void RGBLEDControllerBase::setBrightness(uint8_t brightness) {
	m_brightness = brightness;
}

void RGBLEDControllerBase::enable() {
	m_enabled = true;
	reset();
}

void RGBLEDControllerBase::disable() {
	m_enabled = false;
}
