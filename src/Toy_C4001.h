#pragma once

#include "ofMain.h"

#ifndef __DFROBOT_C4001_H__
typedef enum {
	eExitMode = 0x00,
	eSpeedMode = 0x01,
} eMode_t;

typedef enum {
	eStartSen = 0x55,
	eStopSen = 0x33,
	eResetSen = 0xCC,
	eRecoverSen = 0xAA,
	eSaveParams = 0x5C,
	eChangeMode = 0x3B,
}eSetMode_t;
#endif

// Fake Sensor
class Toy_C4001 {
public:
	Toy_C4001(std::string path = "Fake", uint8_t address = 0x00) {}
	~Toy_C4001() {}

	glm::ivec3 detectRange = { 30, 300, 150 };
	glm::ivec3 detectThres = { 100, 150, 10 };
	uint8_t triggerSensitivity = 1;
	uint8_t keepSensitivity = 1;
	uint8_t trigDelay = 0;
	uint16_t keepDelay = 4;

	void setSensor(eSetMode_t mode) {};

	bool setDelay(uint8_t trig, uint16_t keep) {
		trigDelay = trig;
		keepDelay = keep;
		return true;
	}

	bool begin() {
		return true;
	}

	bool setKeepSensitivity(uint8_t sensitivity) {
		keepSensitivity = sensitivity;
		return true;
	}

	// Centimeters
	bool setDetectionRange(uint16_t min, uint16_t max, uint16_t trig) {
		detectRange.x = min;
		detectRange.y = max;
		detectRange.z = trig;
		return true;
	}

	// Meters
	float getTargetRange() {
		return ofMap(ofNoise(ofGetElapsedTimef()), 0, 1, detectRange.x, detectRange.y)*0.01;
	}

	bool setSensorMode(eMode_t mode) {
		return true;
	}

	bool setDetectThres(uint8_t x, uint8_t y, uint8_t z) {
		detectThres.x = x;
		detectThres.y = y;
		detectThres.z = z;
		return true;
	}

	bool setTrigSensitivity(uint8_t trigSensitivity /* Normally between 0-9 */) {
		triggerSensitivity = trigSensitivity;
		return true;
	}

	// Boolean Gates
	int getTargetNumber() {
		return (ofNoise(ofGetElapsedTimef()) > 0.5) ? 1 : 0;
	}

	int motionDetection() {
		return (getTargetRange() < (detectRange.z*0.01)) ? 0 : 1;
	}

	uint32_t getTargetEnergy(void) {
		return (getTargetNumber() > 0)? ofNoise(ofGetElapsedTimef())*1000 : 0;
	}
};
