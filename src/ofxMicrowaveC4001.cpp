#include "ofxMicrowaveC4001.h"

#ifdef RPI
#include "i2c.h"
#endif

ofxMicrowaveC4001::ofxMicrowaveC4001() {}
ofxMicrowaveC4001::~ofxMicrowaveC4001()
{
    clearSensors();
};

void ofxMicrowaveC4001::clearSensors()
{
    for(auto* s : mmSensors) {
        if(s != nullptr){
            delete s;
            s = nullptr;
        } 
    }
    mmSensors.clear();
}

std::vector<mmSensor*>& ofxMicrowaveC4001::getSensors() {
	return mmSensors;
}

void ofxMicrowaveC4001::setup()
{
#ifdef RPI
    /*
     * The I2C DfRobot C4001 sensor address can be set to 0x2A or 0x2B (DIP switch)
     */
    std::vector<std::pair<std::string, uint8_t>> devices = I2c::getDevices(0x2A, 0x2B);
	if (!devices.empty()) {
        ofLog(OF_LOG_NOTICE) << "Found " << devices.size() << " DfRobot C4001 sensor(s):";
        for(auto &device: devices){
            ofLog(OF_LOG_VERBOSE) << "Path: " << device.first.c_str() << " Device: " << (int)device.second;
        }
	} else {
		ofLog(OF_LOG_NOTICE) << "No DfRobot C4001 sensors found.";
        return;
	}
#endif
    
    clearSensors(); // Make sure we can run setup multiple times (Connect / Reconnect?)

#ifdef RPI
    for(auto d : devices){
        mmSensors.push_back(new mmSensor(d.first.c_str(), d.second));
    }
#else
    // Add a fake for prototyping
    mmSensors.push_back(new mmSensor());
#endif

    for(auto& mms: mmSensors)
    {
        mms->setup();
    }
};

void ofxMicrowaveC4001::update()
{
    // TODO: We should add a capacitor here
    // This will slow down fast running apps 
    for(auto& mms: mmSensors)
    {
        mms->update();
    }
}
