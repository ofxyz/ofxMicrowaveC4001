#include "ofxMicrowaveC4001.h"

ofxMicrowaveC4001::ofxMicrowaveC4001() {};
ofxMicrowaveC4001::~ofxMicrowaveC4001() {};

DFRobot_C4001_I2C* ofxMicrowaveC4001::getSensor() {
	return mmSensor; // DIY
}

#ifdef __arm__

void ofxMicrowaveC4001::setup(const char * deviceName, uint8_t address)
{
    /*
     * The Df Robot sensor has a switch 
     * for address selection a maximum
     * of two sensors on I2C: address = 0x2A || 0x2B
     */

    mmSensor = new DFRobot_C4001_I2C(deviceName, address);
    mmSensor->begin();
    mmSensor->setSensorMode(eSpeedMode);
    mmSensor->setDetectThres(30, 600, 10);
    mmSensor->setFrettingDetection(eON);
    
};

void ofxMicrowaveC4001::update()
{
    if(mmSensor->getTargetNumber() > 0) {
        ofLog() << "mmSensor: " << mmSensor->getTargetRange();
    }
};

#endif
