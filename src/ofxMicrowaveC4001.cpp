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

std::vector<mmSensor*>& ofxMicrowaveC4001::getSensors()
{
	return mmSensors;
}

ofJson ofxMicrowaveC4001::getSettings()
{
    ofJson settings;

    for(mmSensor* s : mmSensors) {
        settings.push_back(s->getSettings());
    }

    return settings;
}

void ofxMicrowaveC4001::setSettings(ofJson settings)
{
    for(ofJson j : settings) {
        std::string location = "";
        location = j.value("m_Location", location);
        if(location != ""){
            bool found = false;
            for(mmSensor* s : mmSensors){
                if(s->getLocation() == location) {
                    found = true;
                    s->setSettings(j);
                    continue;
                }
            }
            if(!found) {
                mmSensors.push_back(new mmSensor(j.value("m_path", ""), j.value("m_address", 0)));
                mmSensors[mmSensors.size()-1]->setSettings(j);
            }
        } 
    }
}

int ofxMicrowaveC4001::scanAdd()
{
    return (scanForDevices() > 0)? addDevices() : 0;
}

int ofxMicrowaveC4001::addDevices()
{
    int addedCount = 0;

#ifdef RPI
    for(auto d : devices) {
        bool exist = false;
        for(mmSensor* s : mmSensors){
            if(s->m_path == d.first && s->m_address == d.second){
                exist = true;
                continue;
            }
        }
        if(!exist){
            mmSensors.push_back(new mmSensor(d.first.c_str(), d.second));
            mmSensors[mmSensors.size()-1]->setup();
            addedCount++;
        }
    }
#endif

    return addedCount;
}

void ofxMicrowaveC4001::setup()
{
    if(scanAdd() == 0) addToySensor();
};

void ofxMicrowaveC4001::update()
{
    mmSensors.erase(std::remove_if(mmSensors.begin(), mmSensors.end(), [](const mmSensor* sensor)
    {
        if(sensor->dead){
            delete sensor;
            sensor = nullptr;
            return true;
        } else {
            return false;
        }
    }), mmSensors.end());

    for(auto& s: mmSensors)
    {
        s->update();
    }
}

void ofxMicrowaveC4001::addToySensor()
{
    mmSensors.push_back(new mmSensor());
    mmSensors[mmSensors.size()-1]->setup();
}

int ofxMicrowaveC4001::scanForDevices()
{
    devices.clear();

#ifdef RPI
    /*
     * The I2C DfRobot C4001 sensor address can be set to 0x2A or 0x2B (via DIP switch)
     */
    devices = I2c::getDevices(0x2A, 0x2B);
#endif

    if (!devices.empty()) {
        ofLog(OF_LOG_NOTICE) << "Found " << devices.size() << " DfRobot C4001 sensor(s):";
        for(auto &device: devices){
            ofLog(OF_LOG_NOTICE) << "Path: " << device.first.c_str() << " Device: " << (int)device.second;
        }
	} else {
		ofLog(OF_LOG_NOTICE) << "No DfRobot C4001 sensors found.";
	}

    return devices.size();
}
