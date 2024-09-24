#include "ofxMicrowaveC4001.h"

ofxMicrowaveC4001::ofxMicrowaveC4001(){};
ofxMicrowaveC4001::~ofxMicrowaveC4001(){};

void ofxMicrowaveC4001::setup()
{
    bus = new I2c("/dev/i2c-1");
    bus->addressSet(0x2A);
};

void ofxMicrowaveC4001::update()
{
    
};
