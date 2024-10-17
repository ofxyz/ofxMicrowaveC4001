#include "ofxMicrowaveC4001.h"

#define sleep(x) usleep(1000 * x)

ofxMicrowaveC4001::ofxMicrowaveC4001() : mmSensor(nullptr) {}
ofxMicrowaveC4001::~ofxMicrowaveC4001() {
	if(mmSensor != nullptr) delete mmSensor;
};

DFRobot_C4001_I2C* ofxMicrowaveC4001::getSensor() {
	return mmSensor; // DIY
}

#ifdef RPI

void ofxMicrowaveC4001::setup(const char * devicePath, uint8_t address)
{

    /*
     * The Df Robot sensor has a switch 
     * for address selection a maximum
     * of two sensors on I2C: address = 0x2A || 0x2B
     */

    mmSensor = new DFRobot_C4001_I2C(devicePath, address);
    while (!mmSensor->begin()){
        ofLog() << "Waiting to connect to sensor ...";
        sleep(1000);
    }

    ofLog() << "Sensor connected ...";

    sleep(1000);
    ofLog() << "setSensorMode...";

    if (!mmSensor->setSensorMode(eSpeedMode))
    {
        ofLog() << "Failed to setSensorMode";
    }
    
    sleep(1000);
    ofLog() << "setDetectThres...";

    if (!mmSensor->setDetectThres(30, 500, 120))
    {
        ofLog() << "Failed to setDetectThres";
    }
    
    sleep(1000);
    ofLog() << "setFrettingDetection...";

    mmSensor->setFrettingDetection(eON);
    sleep(1000);
};

#endif
