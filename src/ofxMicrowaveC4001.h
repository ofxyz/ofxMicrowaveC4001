#pragma once

#include "mmSensor.h"

class ofxMicrowaveC4001 {
public:
	ofxMicrowaveC4001();
	~ofxMicrowaveC4001();
	void setup();
	void update();
	void clearSensors();
	std::vector<mmSensor*>& getSensors();
private:
	std::vector<mmSensor*> mmSensors;
};
