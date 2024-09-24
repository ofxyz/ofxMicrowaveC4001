#pragma once

#include "DFRobot_C4001.h"
#include "ofxGPIO.h"

class ofxMicrowaveC4001 {
    public:
        ofxMicrowaveC4001();
        ~ofxMicrowaveC4001();

        void setup();
        void update();
    private:
        I2c * bus;

};

