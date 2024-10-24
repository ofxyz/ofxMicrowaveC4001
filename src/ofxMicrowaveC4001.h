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
	int scanAdd();
	void addToySensor();
	std::vector<mmSensor*> &getSensors();

	ofJson getSettings();
	void setSettings(ofJson settings);

private:
	int scanForDevices();
	int addDevices();
	std::vector<std::pair<std::string, uint8_t>> devices;
	std::vector<mmSensor*> mmSensors;
};
