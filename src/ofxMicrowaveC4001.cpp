#include "ofxMicrowaveC4001.h"

ofxMicrowaveC4001::ofxMicrowaveC4001(){};
ofxMicrowaveC4001::~ofxMicrowaveC4001(){};

void ofxMicrowaveC4001::setup(const char * deviceName, uint8_t address)
{
    /*
     * The Df Robot sensor has a switch 
     * for address selection a maximum
     * of two sensors on I2C: address = 0x2A || 0x2B
     */

    MWS = new DFRobot_C4001_I2C(deviceName, address);
    MWS->begin();
    MWS->setSensorMode(eSpeedMode);
    //_DATA = MWS->getStatus();
    MWS->setDetectThres(30, 600, 10);
    MWS->setFrettingDetection(eON);
    
};

void ofxMicrowaveC4001::update()
{
    if(MWS->getTargetNumber() > 0) {
        ofLog() << "MWS: " << MWS->getTargetRange();
    }
};

DFRobot_C4001_I2C* ofxMicrowaveC4001::getSensor() {
    return MWS; // DIY
}
