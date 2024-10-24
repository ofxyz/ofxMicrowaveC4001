#include "mmSensor.h"
#include <sstream>
#include <iostream>
#include <iomanip>

#ifdef RPI
#define Sleep(x) usleep(1000 * x)
#endif

static void to_json(ofJson& j, const glm::ivec3& v)
{
    j = ofJson {
        { "x", v.x},
        { "y", v.y},
        { "z", v.z}
    };
}

static void from_json(ofJson& j, glm::ivec3& v)
{
    try {
        j.at("x").get_to(v.x);
        j.at("y").get_to(v.y);
        j.at("z").get_to(v.z);
    }
    catch (const ofJson::exception& e) {
        ofLog(OF_LOG_ERROR) << e.what();
    }
}

std::string mmSensor::getLocation(){
    return m_Location;
}

mmSensor::mmSensor(std::string path /*= ""*/, uint8_t address /*= 0x00*/)
{
    if(path != "") {
#ifdef RPI
        m_isFake = false;
        device = new DFRobot_C4001_I2C(path.c_str(), address);
		toyDevice = nullptr;
        name = "Wave Sensor (0x" + uint8_to_hex_string(address) + ")";
#else
        m_isFake = true;
        device = new DFRobot_C4001_I2C(path.c_str(), address);
        toyDevice = nullptr;
        name = "Fake Sensor (0x" + uint8_to_hex_string(address) + ")";
#endif
    } else {
		ofLog(OF_LOG_WARNING) << "mmSensor: No path given, a fake sensor (C4001) will not be initialized ...";
        m_isFake = true;
        device = nullptr;
		toyDevice = new Toy_C4001();
        name = "Toy Sensor (0x" + uint8_to_hex_string(address) + ")";
    }
 
    m_Location = path + "0x" + uint8_to_hex_string(address);
    // Init 
    m_path = path;
    m_address = address;
    targetDist = 0;
	targetCount = 0;
     // Between 0-9
    triggerSensitivityMin = 0;
    triggerSensitivityMax = 9;
	triggerSensitivity = 5;
	motionDetected = false;
    m_updateDevice = false;
    detectRange = { 30, 300, 240 };
	detectThres = { 30, 150, 10 };
    m_lastUpdate = std::chrono::high_resolution_clock::now();
    m_lastSync   = std::chrono::high_resolution_clock::now();
    updateSec = 0.06; // About every 15th frame
	syncSec = 5;
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
    if (m_isFake) return true;

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

ofJson mmSensor::getSettings()
{
    ofJson settings;

    to_json(settings["detectRange"], detectRange);
    to_json(settings["detectThres"], detectThres);
    
    settings["triggerSensitivity"] = triggerSensitivity;
    settings["m_path"] = m_path;
    settings["m_address"] = m_address;
    settings["updateSec"] = updateSec;
    settings["syncSec"] = syncSec;
    settings["m_Location"] = m_Location;

    return settings;
}

void mmSensor::setSettings(ofJson settings)
{
    from_json(settings["detectRange"], detectRange);
    from_json(settings["detectThres"], detectThres);

    triggerSensitivity = settings.value("triggerSensitivity", triggerSensitivity);
    // TODO: If path or address is not the same reconnect.
    // For now only saved to ID the sensor
    // m_path = settings.value("m_path", m_path);
    // m_address = settings.value("m_address", m_address);
    updateSec = settings.value("updateSec", updateSec);
    syncSec = settings.value("syncSec", syncSec); 
    //m_Location = settings.value("m_Location", m_Location);    
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
		return true; // Not yet time to update, but all good.
    }

    if(m_updateDevice || m_ForceSync) {
        float lastsync = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - m_lastSync).count();
        if(lastsync > syncSec || m_ForceSync) {
            m_lastSync = std::chrono::high_resolution_clock::now();
            m_ForceSync = false;
            bool r1 = updateDetectRange();
            bool r2 = updateDetectThres();
            bool r3 = updateTrigSensitivity();
            if( r1 == true && r2 == true && r3 == true) {
                m_updateDevice = false;
            }
        }
    }

    // Get data from sensor
	if (toyDevice != nullptr) {
		targetCount = toyDevice->getTargetNumber();
		if (targetCount > 0) {
			targetDist = toyDevice->getTargetRange();
		}
		else {
			targetDist = 0;
		}
		motionDetected = toyDevice->motionDetection();
	}
    else if (device != nullptr) {
        // GPIO access
		ofLog(OF_LOG_VERBOSE) << "Getting data from C4001... ";
		targetCount = device->getTargetNumber();
		if (targetCount > 0) {
			targetDist = device->getTargetRange();
		}
		else {
			targetDist = 0;
		}
		motionDetected = device->motionDetection();

		ofLog(OF_LOG_VERBOSE) << "Target Count: " << (int)targetCount << " Current Distance " << targetDist;
    }

    return true;
}

bool mmSensor::isFake()
{
    return m_isFake;
}

std::string mmSensor::uint8_to_hex_string(uint8_t value)
{
    std::stringstream ss;

    ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(value);

    return ss.str();
}

std::string& mmSensor::getName()
{
    return name;
}
