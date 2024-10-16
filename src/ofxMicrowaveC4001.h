#pragma once

#ifdef __arm__
#include "ofxGPIO.h"
#include "DFRobot_C4001.h"
#else
#include "ofMain.h"
// Toy Sensor
class DFRobot_C4001_I2C {
public:
	float getTargetRange(bool fakeData = false) {
		if (fakeData) {
			float fakePos = ofMap(ofNoise(ofGetElapsedTimef()), 0, 1, 0.f, 5.f);
			return (fakePos > 1 && fakePos < 4) ? fakePos : 0;
		}
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
	void setup(const char* deviceName, uint8_t address);
#else
	void setup(const char* deviceName, uint8_t address) {
		// Returns a fake sensor
		ofLog(OF_LOG_WARNING) << "ofxMicrowaveC4001: No access to GPIO. Implementing a fake sensor ...";
		mmSensor = new DFRobot_C4001_I2C();
	};
#endif

	// Toy Sensor
	DFRobot_C4001_I2C* getSensor();

private:
	DFRobot_C4001_I2C* mmSensor;
};
