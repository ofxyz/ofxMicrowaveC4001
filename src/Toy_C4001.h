#pragma once

#include "ofMain.h"

// Fake Sensor
class Toy_C4001 {
public:
	Toy_C4001(std::string path = "long", uint8_t address = 0x00) {}
	~Toy_C4001() {}

	typedef enum {
		eExitMode = 0x00,
		eSpeedMode = 0x01,
	} eMode_t;

	glm::ivec3 detectRange = { 30, 300, 240 };

	bool begin() {
		ofLog(OF_LOG_VERBOSE) << "Toy_C4001: I am alive!";
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
		return true;
	}

	bool setTrigSensitivity(uint8_t triggerSensitivity /* Normally between 0-9 */) {
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
