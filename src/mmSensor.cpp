#include "mmSensor.h"
#include <sstream>
#include <iostream>
#include <iomanip>

#ifdef RPI
#define Sleep(x) usleep(1000 * x)
#endif

mmSensor::mmSensor(std::string path /*= ""*/, uint8_t address /*= 0x00*/)
{
    if(path != "") {
#ifdef RPI
        m_isFake = false;
        fakeSensor = nullptr;
        device = new DFRobot_C4001_I2C(path.c_str(), address);
        name = "MmWave Sensor (0x" + uint8_to_hex_string(address) +")";
#else
        m_isFake = true;
        device = nullptr;
        fakeSensor = new MyToySensor("Fake "+path).c_str(), address);
#endif
    } else {
        m_isFake = true;
        device = nullptr;
        fakeSensor = new MyToySensor();
        name = "Fake MmWave Sensor";
    }

    // Init 
    m_path = path;
    m_address = address;
    targetDist = 0;
	targetCount = 0;
	triggerSensitivity = 5; // Between 0-9
	motionDetected = false;
    m_updateDevice = false;
    detectRange = { 30, 300, 240 };
	detectThres = { 30, 150, 10 };
    m_lastUpdate = std::chrono::high_resolution_clock::now();
    m_lastSync   = std::chrono::high_resolution_clock::now();
    m_ForceSync = false;
};

mmSensor::~mmSensor()
{
    if(device != nullptr) delete device;
};

void mmSensor::syncNow(){
    m_ForceSync = true;
}

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

bool mmSensor::isInSync(){
    return !m_updateDevice;
}

void mmSensor::updateDevice(){
    m_updateDevice = true;
}

bool mmSensor::update()
{
    float lastupdate = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - m_lastUpdate).count();
    if(lastupdate > updateSec) {
        m_lastUpdate = std::chrono::high_resolution_clock::now();
    } else {
        // All good ...
        return true;
    }

    float lastsync = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - m_lastSync).count();
    if( (m_updateDevice && lastsync > syncSec) || m_ForceSync) {
        m_lastSync = std::chrono::high_resolution_clock::now();
        m_ForceSync = false;
        bool r1 = updateDetectRange();
        bool r2 = updateDetectThres();
        bool r3 = updateTrigSensitivity();
        if( r1 == true && r2 == true && r3 == true) {
            m_updateDevice = false;
        }
    }

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
