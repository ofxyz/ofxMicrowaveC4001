#include "mmSensor.h"
#include <sstream>
#include <iostream>
#include <iomanip>

#ifdef RPI
#define Sleep(x) usleep(1000 * x)
#endif

static void to_json(ofJson& j, const glm::ivec3& v)
{
	j = ofJson{
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

std::string mmSensor::getLocation() {
	return m_Location;
}

mmSensor::mmSensor(std::string path /*= ""*/, uint8_t address /*= 0x00*/)
{
	if (path != "") {
#ifdef RPI
		m_isFake = false;
		device = new DFRobot_C4001_I2C(path.c_str(), address);
		name = "Wave Sensor (0x" + uint8_to_hex_string(address) + ")";
#else
		m_isFake = true;
		device = new DFRobot_C4001_DUMMY(path.c_str(), address);
		name = "Fake Sensor (0x" + uint8_to_hex_string(address) + ")";
#endif
	}
	else {
		ofLog(OF_LOG_NOTICE) << "mmSensor: No path given, a fake sensor (C4001) will be initialized ...";
		m_isFake = true;
		device = new Toy_C4001("Toy Sensor", address);
		name = "Fake Sensor (0x" + uint8_to_hex_string(address) + ")";
	}

	m_Location = path + "0x" + uint8_to_hex_string(address);
	// Init 
	dead = false;
	m_path = path;
	m_address = address;
	targetDist = 0;
	targetCount = 0;
	targetEnergy = 0;
	// Between 0-9
	sensitivityMin = 0; // Should be define
	sensitivityMax = 9; // Should be define
	triggerDelay = 0;
	keepDelay = 4;
	triggerSensitivity = 3;
	keepSensitivity = 3;
	motionDetected = false;
	m_updateDevice = false;
	detectRange = { 30, 300, 240 };
	detectThres = { 30, 150, 10 };
	m_lastUpdate = std::chrono::high_resolution_clock::now();
	m_lastSync = std::chrono::high_resolution_clock::now();
	updateMillis = 15;
	syncMillis = 5000;
	zoom = 1;
	m_ForceSync = false;
};

mmSensor::~mmSensor()
{
	if (device != nullptr) delete device;
};

void mmSensor::syncNow()
{
	m_ForceSync = true;
}

bool mmSensor::connect(int tries /*= 4*/)
{
	if (m_isFake) return true;

	while (!device->begin() && tries-- != 0)
	{
		ofLog(OF_LOG_NOTICE) << "Waiting to connect to sensor ...";
		Sleep(1000);
	}
	if (tries == 0) {
		ofLog(OF_LOG_WARNING) << "Could not connect to sensor (timeout) ...";
		return false;
	}

	ofLog(OF_LOG_NOTICE) << "Sensor connected ...";
	return true;
}

bool mmSensor::setup()
{
	if (!connect()) return false;

	if (!m_isFake) {
		ofLog(OF_LOG_VERBOSE) << "setSensorMode...";
		if (!device->setSensorMode(eSpeedMode))
		{
			ofLog(OF_LOG_NOTICE) << "Failed to setSensorMode";
		}
	}

    updateDelay();
    updateDetectRange();
    updateDetectThres();
    updateTrigSensitivity();
    updateKeepSensitivity();
    if (m_isFake == false)
        device->setSensor(eStartSen);
    return true;
}

bool mmSensor::setup(void (*callbackPtr)(void*), void* pOwner)
{

	if (setup()) {
		addTriggerCallback(callbackPtr, pOwner);
		return true;
	}

	return false;
}

bool mmSensor::updateDetectRange()
{
	// Make sure the data is in range
	detectRange.x = ofClamp(detectRange.x, 30, 2000);
	detectRange.y = ofClamp(detectRange.y, 240, 2000);
	detectRange.z = ofClamp(detectRange.z, 240, 2000);

    if (m_isFake) return true;
    ofLog(OF_LOG_VERBOSE) << "setDetectionRange...";

    if (!device->setDetectionRange(detectRange.x, detectRange.y, detectRange.z))
    {
        ofLog(OF_LOG_NOTICE) << "Failed to setDetectionRange";
        return false;
    }
    return true;
}

bool mmSensor::updateDetectThres()
{
	// Make sure the data is in range
    detectThres.x = ofClamp(detectThres.x, 30, 2000);
    detectThres.y = ofClamp(detectThres.y, 240, 2000);
    detectThres.z = ofClamp(detectThres.z, 0, 65535);

    if (m_isFake) return true;

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
    triggerSensitivity = ofClamp(triggerSensitivity, 0, 9);

    if (m_isFake) return true;
    
    if (!device->setTrigSensitivity(triggerSensitivity))
	{
		ofLog(OF_LOG_NOTICE) << "mmSensor: Failed to setTrigSensitivity";
        return false;
    }
    return true;
}

bool mmSensor::updateKeepSensitivity()
{
    keepSensitivity = ofClamp(keepSensitivity, 0, 9);

    if (m_isFake) return true;
    if (!device->setKeepSensitivity(keepSensitivity))
    {
        ofLog(OF_LOG_NOTICE) << "Failed to setKeepSensitivity";
        return false;
    }
    return true;
}

bool mmSensor::updateDelay()
{
	triggerDelay = ofClamp(triggerDelay, 0, 200);
    keepDelay = ofClamp(keepDelay, 4, 3000);

    if (m_isFake) return true;
    ofLog(OF_LOG_VERBOSE) << "setDelay...";
    if (!device->setDelay(triggerDelay, keepDelay))
    {
        ofLog(OF_LOG_NOTICE) << "Failed to setDelay";
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
	settings["keepSensitivity"] = keepSensitivity;
	settings["triggerDelay"] = triggerDelay;
	settings["keepDelay"] = keepDelay;
	settings["m_path"] = m_path;
	settings["m_address"] = m_address;
	settings["updateMillis"] = updateMillis;
	settings["syncMillis"] = syncMillis;
	settings["m_Location"] = m_Location;

	return settings;
}

void mmSensor::setSettings(ofJson settings)
{
	from_json(settings["detectRange"], detectRange);
	from_json(settings["detectThres"], detectThres);

	triggerSensitivity = settings.value("triggerSensitivity", triggerSensitivity);
	keepSensitivity = settings.value("keepSensitivity", keepSensitivity);
	triggerDelay = settings.value("triggerDelay", triggerDelay);
	keepDelay = settings.value("keepDelay", keepDelay);
	// TODO: If path or address is not the same reconnect.
	// For now only saved to ID the sensor
	// m_path = settings.value("m_path", m_path);
	// m_address = settings.value("m_address", m_address);
	updateMillis = settings.value("updateMillis", updateMillis);
	syncMillis = settings.value("syncMillis", syncMillis);
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

bool mmSensor::isInSync()
{
	return !m_updateDevice;
}

void mmSensor::updateDevice()
{
	m_updateDevice = true;
}

bool mmSensor::update()
{
	if (device == nullptr) {
		ofLog(OF_LOG_ERROR) << "mmSensor: Device points to NULL";
		return false;
	}

	float lastupdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_lastUpdate).count();
	if (lastupdate > updateMillis) {
		m_lastUpdate = std::chrono::high_resolution_clock::now();
	}
	else {
		return true; // Not yet time to update, but all good.
	}

	if (m_updateDevice || m_ForceSync) {
		float lastsync = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_lastSync).count();
		if (lastsync > syncMillis || m_ForceSync) {
			m_lastSync = std::chrono::high_resolution_clock::now();
			m_ForceSync = false;
			bool r5 = updateDelay();
			bool r1 = updateDetectRange();
			bool r2 = updateDetectThres();
			bool r3 = updateTrigSensitivity();
			bool r4 = updateKeepSensitivity();

			if (r1 == true && r2 == true && r3 == true && r4 == true && r5 == true) {
				m_updateDevice = false;
			}

			// Make sure we are back on
			/*
			sSensorStatus_t data;
			data = device->getStatus();
			if (data.workMode != eStartSen)
			{
				device->setSensor(eStartSen);
			}
			*/
		}
	}

	bool newMotionDetected = false;

	if (device != nullptr) {
		// [POSSIBLY FAKE] GPIO access 
		ofLog(OF_LOG_VERBOSE) << "Getting data from C4001... ";
		targetCount = device->getTargetNumber();
		if (targetCount > 0) {
			targetDist = device->getTargetRange();
		}
		else {
			targetDist = 0;
		}
		newMotionDetected = (device->motionDetection() > 0);
		if (newMotionDetected == true && motionDetected == false) {
			callTriggerCallbacks();
		}
		motionDetected = newMotionDetected;
		targetEnergy = device->getTargetEnergy();

		ofLog(OF_LOG_VERBOSE) << "Target Count: " << (int)targetCount << " Current Distance " << targetDist;
	}

	return true;
}

void mmSensor::addTriggerCallback(void (*funcPtr)(void*), void* pOwner)
{
	m_vTriggerCallbacks.push_back(std::make_pair(pOwner, funcPtr));
}

void mmSensor::callTriggerCallbacks()
{
	for (auto& tcb : m_vTriggerCallbacks) {
		tcb.second(tcb.first);
	}
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
