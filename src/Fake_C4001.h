#pragma once

#include "ofMain.h"

#ifndef __DFROBOT_C4001_H__
typedef enum {
	eExitMode = 0x00,
	eSpeedMode = 0x01,
}eMode_t;
#endif

class DFRobot_C4001_I2C {
public:
	DFRobot_C4001_I2C(std::string path = "long", uint8_t address = 0xBB) {
		ofLog(OF_LOG_NOTICE) << "Fake_C4001: How beautiful the world is ...";
		int x = 0; while (++x < 100) ofLog(OF_LOG_VERBOSE) << " ";
	}
	~DFRobot_C4001_I2C() {}

	glm::ivec3 detectRange = { 30, 300, 240 };
	uint8_t triggerSensitivity = 1;

	bool begin() {
		ofLog(OF_LOG_VERBOSE) << "Fake_C4001: I am alive!";
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
		detectRange.x = x;
		detectRange.y = y;
		detectRange.z = z;
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

};
