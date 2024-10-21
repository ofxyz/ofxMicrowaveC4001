#pragma once

#include <stdio.h>

#if defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_7__)
#define RPI
#endif

#ifdef RPI
#include "ofxGPIO.h"
#include "DFRobot_C4001.h"
#else
#include "ofMain.h"

// Fake Sensor
class DFRobot_C4001_I2C {
public:
	DFRobot_C4001_I2C(){};
	virtual ~DFRobot_C4001_I2C(){};
	float getTargetRange() {
		// Not connected
		ofLog() << "Sensor not connected...";
		return 0;
	}
};
#endif

class ofxMicrowaveC4001 {
public:
	ofxMicrowaveC4001();
	virtual ~ofxMicrowaveC4001();

#ifdef RPI
	void setup(const char* devicePath, uint8_t address);
	bool isFake() { return false; };
#else
	void setup(const char* devicePath, uint8_t address) {
		// Returns a fake sensor
		ofLog(OF_LOG_WARNING) << "ofxMicrowaveC4001: No access to GPIO. Implementing a fake sensor ...";
		mmSensor = new DFRobot_C4001_I2C();
	};
	bool isFake() { return true; };
#endif

	// Returns fake sensor without GPIO
	DFRobot_C4001_I2C* getSensor();

	// For prototyping purposes
	float getFakeTargetRange() {
		/* TODO: Make a better fake with some controls */
		float fakePos = ofMap(ofNoise(ofGetElapsedTimef()), 0, 1, 0.f, 12.f);
		return ( (fakePos > 2 && fakePos < 5) || fakePos > 9) ? fakePos : 0;
	}

private:
	DFRobot_C4001_I2C* mmSensor;
	bool useFakeData = false;

	//@param x = min Detection range Minimum distance, unit cm, range 0.3~20m(30~2000), not exceeding max, otherwise the function is abnormal.
	//@param y = max Detection range Maximum distance, unit cm, range 2.4~20m(240~2000)
	//@param z = Target detection threshold, dimensionless unit 0.1, range 0~6553.5 (0~65535) 
	glm::ivec3 detectThres = { 30, 500, 120 };
};
