#pragma once

#include "DFRobot_C4001.h"
#include "ofxGPIO.h"

class ofxMicrowaveC4001 {
    public:
        ofxMicrowaveC4001();
        ~ofxMicrowaveC4001();

        void setup(const char * deviceName, uint8_t address);
        void update();
        DFRobot_C4001_I2C* getSensor();

    private:
        DFRobot_C4001_I2C* MWS;
};
