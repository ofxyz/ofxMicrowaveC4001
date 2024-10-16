#include "ofxMicrowaveC4001.h"

ofxMicrowaveC4001::ofxMicrowaveC4001() : mmSensor(nullptr) {}
ofxMicrowaveC4001::~ofxMicrowaveC4001() {
	delete mmSensor;
};

DFRobot_C4001_I2C* ofxMicrowaveC4001::getSensor() {
	return mmSensor; // DIY
}

#ifdef __arm__

void ofxMicrowaveC4001::setup(const char * devicePath, uint8_t address)
{
    /*
     * The DfRobot sensor has a switch 
     * for address selection a maximum
     * of two sensors on I2C: address = 0x2A || 0x2B
     */

    mmSensor = new DFRobot_C4001_I2C(devicePath, address);
    mmSensor->begin();
    mmSensor->setSensorMode(eSpeedMode);
    mmSensor->setDetectThres(30, 600, 10);
    mmSensor->setFrettingDetection(eON);
};

#endif
