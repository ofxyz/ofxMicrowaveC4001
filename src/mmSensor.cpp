#include "mmSensor.h"
#include <sstream>
#include <iostream>
#include <iomanip>

#ifdef RPI
#define Sleep(x) usleep(1000 * x)
#endif

mmSensor::mmSensor(std::string path /*= ""*/, uint8_t address /*= 0x2A*/)
{
    if(path != "") {
#ifdef RPI
        device = new DFRobot_C4001_I2C(path.c_str(), address);
        m_isFake = false;
        name = "MmWave Sensor (0x" + uint8_to_hex_string(address) +")";
#else
        m_isFake = true;
#endif
    } else {
        device = new MyToySensor();
        name = "Fake MmWave Sensor";
    }

    // Init 
    m_path = path;
    m_address = address;
    targetDist = 0;
	targetCount = 0;
	triggerSensitivity = 5; // Between 0-9
	motionDetected = false;
    detectRange = { 30, 300, 240 };
	detectThres = { 30, 150, 10 };
};

mmSensor::~mmSensor()
{
    if(device != nullptr) delete device;
};

bool mmSensor::setup()
{
    int i = 4;
    while (!device->begin() && i-- != 0)
    {
        ofLog(OF_LOG_NOTICE) << "Waiting to connect to sensor ...";
        Sleep(1000);
    }
    if(i == 0) {
        ofLog(OF_LOG_WARNING) << "Could not connect to sensor (timeout) ...";
        return false;
    }

    ofLog(OF_LOG_NOTICE) << "Sensor connected ...";

    ofLog(OF_LOG_VERBOSE) << "setSensorMode...";
    if (!device->setSensorMode(eSpeedMode))
    {
        ofLog(OF_LOG_NOTICE) << "Failed to setSensorMode";
    }

    updateDetectRange();
    updateDetectThres();
    updateTrigSensitivity();

    return true;
}

bool mmSensor::updateDetectRange()
{
    ofLog(OF_LOG_VERBOSE) << "setDetectRange...";
    if (!device->setDetectThres(detectRange.x, detectRange.y, detectRange.z))
    {
        ofLog(OF_LOG_NOTICE) << "Failed to setDetectThres";
        return false;
    }
    return true;
}

bool mmSensor::updateDetectThres()
{
    ofLog(OF_LOG_VERBOSE) << "setDetectThres...";
    if (!device->setDetectThres(detectThres.x, detectThres.y, detectThres.z))
    {
        ofLog(OF_LOG_NOTICE) << "Failed to setDetectThres";
        return false;
    }
    return true;
}

bool mmSensor::updateTrigSensitivity()
{
    ofLog(OF_LOG_VERBOSE) << "setTrigSensitivity...";
    if (!device->setTrigSensitivity(triggerSensitivity))
    {
        ofLog(OF_LOG_NOTICE) << "Failed to setTrigSensitivity";
        return false;
    }
    return true;
}

glm::ivec3& mmSensor::getDetectTRange()
{
	return detectRange;
}

glm::ivec3& mmSensor::getDetectThres()
{
	return detectThres;
}

bool mmSensor::update()
{
    // GPIO access
    ofLog(OF_LOG_VERBOSE) << "Getting data from C4001... ";
    targetCount = device->getTargetNumber();
    if(targetCount > 0) {
        targetDist = device->getTargetRange();
    } else {
        targetDist = 0;
    }
    motionDetected = device->motionDetection();

    ofLog(OF_LOG_VERBOSE) << "Target Count: " << (int)targetCount << "\n Current Distance" << targetDist;
    return true;
}

bool mmSensor::isFake()
{
    return m_isFake;
}

std::string mmSensor::uint8_to_hex_string(uint8_t value)
{
    std::stringstream ss;

    ss << std::hex << std::setw(2) << static_cast<int>(value);

    return ss.str();
}

std::string& mmSensor::getName()
{
    return name;
}
