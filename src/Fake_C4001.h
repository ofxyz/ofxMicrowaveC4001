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

class DFRobot_C4001_I2C {
public:
	DFRobot_C4001_I2C(std::string path = "long", uint8_t address = 0xBB) {
		ofLog(OF_LOG_NOTICE) << "Fake_C4001: How beautiful the world is ...";
		int x = 0; while (++x < 100) ofLog(OF_LOG_VERBOSE) << " ";
	}
	~DFRobot_C4001_I2C() {}

	glm::ivec3 detectRange = { 30, 300, 240 };
	glm::ivec3 detectThres = { 30, 150, 10 };
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
		ofLog(OF_LOG_VERBOSE) << "Fake_C4001: I am alive!";
		return true;
	}

	bool setKeepSensitivity(uint8_t sensitivity) {
		keepSensitivity = sensitivity;
		return true;
	}

	bool setDetectionRange(uint16_t min, uint16_t max, uint16_t trig) {
		detectRange.x = min;
		detectRange.y = max;
		detectRange.z = trig;
		return true;
	}

	float getTargetRange() {
		/* TODO Better: This is a placeholder */
		float fakePos = ofMap(ofNoise(ofGetElapsedTimef()), 0, 1, detectRange.x, detectRange.y);
		return ((fakePos > detectRange.x && fakePos < detectRange.y)) ? fakePos*0.01 : 0;
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
		return (ofMap(ofNoise(ofGetElapsedTimef()), 0, 1, detectRange.x, detectRange.y) < detectRange.z) ? 1 : 0;
	}

	int motionDetection() {
		return (ofMap(ofNoise(ofGetElapsedTimef()), 0, 1, detectRange.x, detectRange.y) < detectRange.z) ? 1 : 0;
	}

	uint32_t getTargetEnergy(void) {
		return 0; // TODO: Implement this
	}
};
