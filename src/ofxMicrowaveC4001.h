#pragma once

#ifdef __arm__
#include "ofxGPIO.h"
#include "DFRobot_C4001.h"
#else
#include "ofMain.h"

// Fake Sensor
class DFRobot_C4001_I2C {
public:
	float getTargetRange() {
		// Not connected
		return 0;
	}
};
#endif

class ofxMicrowaveC4001 {
public:
	ofxMicrowaveC4001();
	~ofxMicrowaveC4001();
#ifdef __arm__
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
	float getFakeRange() {
		/* TODO: Make a better fake with some controls */
		float fakePos = ofMap(ofNoise(ofGetElapsedTimef()), 0, 1, 0.f, 12.f);
		return ( (fakePos > 2 && fakePos < 5) || fakePos > 9) ? fakePos : 0;
	}

private:
	DFRobot_C4001_I2C* mmSensor;
};
