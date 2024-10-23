#pragma once

#include "ofMain.h"

// Toy Emulator
// Needs to be loaded after:
#ifndef __DFROBOT_C4001_H__
typedef enum {
	eExitMode = 0x00,
	eSpeedMode = 0x01,
}eMode_t;
#endif

class MyToySensor {
public:
	MyToySensor(std::string path = "long", uint8_t address = 0xBB) {
		ofLog(OF_LOG_NOTICE) << "MyToySensor: How beautiful the world is ...";
		int x = 0; while (++x < 100) ofLog(OF_LOG_VERBOSE) << " ";
	}
	~MyToySensor() {}

	glm::ivec3 detectRange = { 30, 300, 240 };

	bool begin() {
		ofLog(OF_LOG_VERBOSE) << "MyToySensor: I am alive!";
		return true;
	}

	float getTargetRange() {
		/* TODO: This is a placeholder */

		float fakePos = ofMap(ofNoise(ofGetElapsedTimef()), 0, 1, detectRange.x, detectRange.y);
		return ((fakePos > detectRange.x && fakePos < detectRange.y)) ? fakePos : 0;
	}

	bool setSensorMode(eMode_t mode) {
		ofLog(OF_LOG_VERBOSE) << "MyToySensor: setSensorMode";
		return true;
	}

	bool setDetectThres(uint8_t x, uint8_t y, uint8_t z) {
		ofLog(OF_LOG_VERBOSE) << "MyToySensor: setDetectThres";
		return true;
	}

	bool setTrigSensitivity(uint8_t triggerSensitivity /* Normally between 0-9 */) {
		return true;
	}

	int getTargetNumber() {
		// Boolean Gate
		return (ofMap(ofNoise(ofGetElapsedTimef()), 0, 1, detectRange.x, detectRange.y) > detectRange.z) ? 1 : 0;
	}

	int motionDetection() {
		return (ofMap(ofNoise(ofGetElapsedTimef()), 0, 1, detectRange.x, detectRange.y) > detectRange.z) ? 1 : 0;
	}

};
