#pragma once

#include "mmSensor.h"
#include "ofJson.h"

class ofxMicrowaveC4001
{
public:
	ofxMicrowaveC4001();
	~ofxMicrowaveC4001();
	void setup();
	void update();
	void clearSensors();
	std::vector<mmSensor*> &getSensors();

	ofJson getSettings();
	void setSettings(ofJson settings);

private:
	std::vector<mmSensor*> mmSensors;
};
