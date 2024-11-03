#pragma once

#include "ofMain.h"
#include "DFRobot_C4001.h"


class Toy_C4001 : public DFRobot_C4001_DUMMY {
public:
	Toy_C4001(const char* deviceName, uint8_t address = 0x00) : DFRobot_C4001_DUMMY(deviceName, address) {}
	~Toy_C4001() {}

	glm::ivec3 detectRange = { 30, 300, 150 };
	glm::ivec3 detectThres = { 100, 150, 10 };
	uint8_t triggerSensitivity = 1;
	uint8_t keepSensitivity = 1;
	uint8_t trigDelay = 0;
	uint16_t keepDelay = 4;

	virtual void setSensor(eSetMode_t mode) {};

	virtual bool setDelay(uint8_t trig, uint16_t keep) {
		trigDelay = trig;
		keepDelay = keep;
		return true;
	}

	virtual bool begin() {
		return true;
	}

	virtual bool setKeepSensitivity(uint8_t sensitivity) {
		keepSensitivity = sensitivity;
		return true;
	}

	// Centimeters
	virtual bool setDetectionRange(uint16_t min, uint16_t max, uint16_t trig) {
		detectRange.x = min;
		detectRange.y = max;
		detectRange.z = trig;
		return true;
	}

	// Meters
	virtual float getTargetRange() {
		return ofMap(ofNoise(ofGetElapsedTimef()), 0, 1, detectRange.x, detectRange.y) * 0.01;
	}

	virtual bool setSensorMode(eMode_t mode) {
		return true;
	}

	virtual bool setDetectThres(uint8_t x, uint8_t y, uint8_t z) {
		detectThres.x = x;
		detectThres.y = y;
		detectThres.z = z;
		return true;
	}

	virtual bool setTrigSensitivity(uint8_t trigSensitivity /* Normally between 0-9 */) {
		triggerSensitivity = trigSensitivity;
		return true;
	}

	// Boolean Gates
	virtual uint8_t getTargetNumber() {
		return (ofNoise(ofGetElapsedTimef()) > 0.5) ? 1 : 0;
	}

	virtual bool motionDetection() {
		return (getTargetNumber() > 0)? 1 : 0;
	}

	virtual uint32_t getTargetEnergy(void) {
		return (getTargetNumber() > 0) ? ofNoise(ofGetElapsedTimef()) * 1000 : 0;
	}
};
