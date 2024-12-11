#include "mmSensor.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <regex>

#ifdef RPI
#include "i2c.h"
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
        scanForDevices();

        bool found = false;
        for (auto& d : m_Devices)
        {
            if (d.first == path && d.second == address)
            {
                m_isFake = false;
                connected = true;
                missing = false;
                m_Device = new DFRobot_C4001_I2C(path.c_str(), address);
                name = "Wave Sensor (0x" + uint8_to_hex_string(address) + ")";
                goto added;
            }
        }

        // No connected devices have been found
        m_isFake = false;
        connected = false; // Not connected
        missing = true;
        m_Device = new DFRobot_C4001_DUMMY(path.c_str(), address);
        name = "Missing Sensor (0x" + uint8_to_hex_string(address) + ")";
        goto added;
#else
        m_isFake = false;
        connected = false; // No GPIO Access
        missing = true;
        m_Device = new DFRobot_C4001_DUMMY(path.c_str(), address);
        name = "Missing Sensor (0x" + uint8_to_hex_string(address) + ")";
        goto added;
#endif
    }
    else {
        ofLog(OF_LOG_NOTICE) << "mmSensor: No path given, a toy sensor (C4001) will be initialized ...";
        m_isFake = true;
        connected = false; // Not connected
        missing = false;
        m_Device = new Toy_C4001("Toy Sensor", address);
        name = "Toy Sensor (0x" + uint8_to_hex_string(address) + ")";
        goto added;
    }

added:
    m_Location = path + "0x" + uint8_to_hex_string(address);
    // Init 
    m_bDead = false;
    m_path = path;
    m_address = address;
    targetDist = 0;
    targetCount = 0;
    targetEnergy = 0;
    // Between 0-9
    sensitivityMin = 0;
    sensitivityMax = 9;
    triggerDelay = 0;
    keepDelay = 4;
    m_uiTriggerSensitivity = 0;
    m_uiKeepSensitivity = 3;
    motionDetected = false;
    trigger = false;
    m_updateDevice = false;
    detectRange = { 40, 500, 500 };
    detectThres = { 40, 500, 0 };
    triggerRange = { 0, 300, 0 };
    m_lastUpdate = std::chrono::high_resolution_clock::now();
    m_lastSync = std::chrono::high_resolution_clock::now();
    m_lastTrigger = std::chrono::high_resolution_clock::now();
    m_fUpdateMillis = 30;
    m_fSyncMillis = 5000;
    m_fTriggerMillis = 300;
    m_ForceSync = false;
    setup();
};

mmSensor::~mmSensor()
{
    if (m_Device != nullptr) delete m_Device;
};

void mmSensor::syncNow()
{
    m_ForceSync = true;
}

bool mmSensor::connect(int tries /*= 4*/)
{
    if (m_isFake || missing) return connected;
    connected = false;

    while (!m_Device->begin() && tries-- != 0)
    {
        ofLog(OF_LOG_NOTICE) << "Waiting to connect to sensor ...";
        Sleep(1000);
    }
    if (tries == 0) {
        ofLog(OF_LOG_NOTICE) << "Could not connect to sensor (timeout) ...";
        connected = false;
    }
    else {
        ofLog(OF_LOG_NOTICE) << "Sensor connected ...";
        connected = true;
    }

    return connected;
}

bool mmSensor::setup()
{
    if (!connect()) return false;

    if (!m_isFake) {
        ofLog(OF_LOG_VERBOSE) << "setSensorMode...";
        int tries = 4;
        while (!m_Device->setSensorMode(eSpeedMode) && tries-- != 0) {
            ofLog(OF_LOG_NOTICE) << "Failed to setSensorMode, retrying ...";
            Sleep(1000);
        }
        if (tries == 0)
        {
            ofLog(OF_LOG_NOTICE) << "Failed to setSensorMode ...";
            connected = false;
            return connected;
        }
    }

    updateDelay();
    updateDetectRange();
    updateDetectThres();
    updateTrigSensitivity();
    updateKeepSensitivity();

    if (!m_isFake) {
        m_Device->setSensor(eStartSen);
        m_bSynced = true;
    }

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
    /*
    detectRange.x = ofClamp(detectRange.x, 30, 1200);
    detectRange.y = ofClamp(detectRange.y, 240, 1200);
    detectRange.z = ofClamp(detectRange.z, 240, 1200);
    */

    // Keep detect range at comfortable max
    // The DfRobot C4001 seems to work best between 40 and 1000 cm
    detectRange.x = 40;
    detectRange.y = 1000;
    detectRange.z = 1000;

    ofLog(OF_LOG_VERBOSE) << "setDetectionRange...";

    if (!m_Device->setDetectionRange(detectRange.x, detectRange.y, detectRange.z))
    {
        ofLog(OF_LOG_NOTICE) << "Failed to setDetectionRange";
        return false;
    }
    return true;
}

bool mmSensor::updateDetectThres()
{
    // Make sure the data is in range
    //detectThres.x = ofClamp(detectThres.x, 30, 1200);
    detectThres.x = 40; // Always 40 as we deal with thresholds ourselves
    detectThres.y = ofClamp(detectThres.y, 240, 1200);
    detectThres.z = 0; // Always 0 as we deal with thresholds ourselves

    ofLog(OF_LOG_VERBOSE) << "setDetectThres...";

    if (!m_Device->setDetectThres(detectThres.x, detectThres.y, detectThres.z))
    {
        ofLog(OF_LOG_NOTICE) << "Failed to setDetectThres";
        return false;
    }
    return true;
}

bool mmSensor::updateTrigSensitivity()
{
    m_uiTriggerSensitivity = ofClamp(m_uiTriggerSensitivity, 0, 9);
    
    if (!m_Device->setTrigSensitivity(m_uiTriggerSensitivity))
    {
        ofLog(OF_LOG_NOTICE) << "mmSensor: Failed to setTrigSensitivity";
        return false;
    }
    return true;
}

bool mmSensor::updateKeepSensitivity()
{
    m_uiKeepSensitivity = ofClamp(m_uiKeepSensitivity, 0, 9);

    if (!m_Device->setKeepSensitivity(m_uiKeepSensitivity))
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

    ofLog(OF_LOG_VERBOSE) << "setDelay...";
    if (!m_Device->setDelay(triggerDelay, keepDelay))
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
    to_json(settings["triggerRange"], triggerRange);

    //settings["triggerSensitivity"] = m_uiTriggerSensitivity;
    //settings["keepSensitivity"] = m_uiKeepSensitivity;
    settings["triggerDelay"] = triggerDelay;
    settings["keepDelay"] = keepDelay;
    settings["m_path"] = m_path;
    settings["m_address"] = m_address;
    settings["updateMillis"] = m_fUpdateMillis;
    settings["syncMillis"] = m_fSyncMillis;
    settings["m_fTriggerMillis"] = m_fTriggerMillis;
    settings["m_Location"] = m_Location;

    return settings;
}

void mmSensor::setSettings(ofJson settings)
{
    from_json(settings["detectRange"], detectRange);
    from_json(settings["detectThres"], detectThres);
    from_json(settings["triggerRange"], triggerRange);

    //m_uiTriggerSensitivity = settings.value("triggerSensitivity", m_uiTriggerSensitivity);
    //m_uiKeepSensitivity = settings.value("keepSensitivity", m_uiKeepSensitivity);
    triggerDelay = settings.value("triggerDelay", triggerDelay);
    keepDelay = settings.value("keepDelay", keepDelay);
    m_path = settings.value("m_path", m_path);
    m_address = settings.value("m_address", m_address);
    m_fUpdateMillis = settings.value("updateMillis", m_fUpdateMillis);
    m_fSyncMillis = settings.value("syncMillis", m_fSyncMillis);
    m_fTriggerMillis = settings.value("m_fTriggerMillis", m_fTriggerMillis);
    // a generated value i.e: hash of path and address
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
    return m_bSynced;
}

void mmSensor::updateDevice()
{
    m_updateDevice = true;
    m_bSynced = false;
}

bool mmSensor::update()
{
    if (m_Device == nullptr) {
        ofLog(OF_LOG_ERROR) << "mmSensor: Device points to NULL";
        return false;
    }

    int lastupdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_lastUpdate).count();
    if (lastupdate > m_fUpdateMillis) {
        m_lastUpdate = std::chrono::high_resolution_clock::now();
    }
    else {
        return true; // Not yet time to update, but all good.
    }

    if (!m_isFake && !connected) {
        if (!setup()) {
            //Setup will log the error
            return false;
        }
    }

    if (m_updateDevice || m_ForceSync) {
        float lastsync = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_lastSync).count();
        if (lastsync > m_fSyncMillis || m_ForceSync) {
            m_lastSync = std::chrono::high_resolution_clock::now();
            m_ForceSync = false;

            bool r5 = updateDelay();
            bool r1 = updateDetectRange();
            bool r2 = updateDetectThres();
            bool r3 = updateTrigSensitivity();
            bool r4 = updateKeepSensitivity();

            if (r1 == true && r2 == true && r3 == true && r4 == true && r5 == true) {
                m_updateDevice = false;
                m_bSynced = true;
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

    if (m_Device != nullptr) {
        // [POSSIBLY FAKE] GPIO access 
        ofLog(OF_LOG_VERBOSE) << "Getting data from C4001... ";
        targetCount = m_Device->getTargetNumber();
        if (targetCount > 0) {
            targetDist = m_Device->getTargetRange();
        }
        else {
            targetDist = 0;
        }

        /*
        bool newMotionDetected = (m_Device->motionDetection() > 0);
        
        if (newMotionDetected == true && motionDetected == false) {
            callTriggerCallbacks();
        }
        
        motionDetected = newMotionDetected;
        */
        motionDetected = (m_Device->motionDetection() > 0);

        targetEnergy = m_Device->getTargetEnergy();

        float targetCM = targetDist * 100;
        if ((targetCM > triggerRange.x) && (targetCM < triggerRange.y)) {
            if (trigger == false) {
                callTriggerCallbacks();
                trigger = true;
                m_lastTrigger = std::chrono::high_resolution_clock::now();
            }
        }
        // We use a timer to turn trigger off
        //else {
        //    trigger = false;
        //}

        ofLog(OF_LOG_VERBOSE) << "Target Count: " << (int)targetCount << " Current Distance " << targetDist;
    }

    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_lastTrigger).count() > m_fTriggerMillis) {
        trigger = false;
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

int mmSensor::scanForDevices()
{
    m_Devices.clear();

#ifdef RPI
    /*
     * The I2C DfRobot C4001 sensor address can be set to 0x2A or 0x2B (via DIP switch)
     */
    m_Devices = I2c::getDevices(0x2A, 0x2B);
#endif

    if (!m_Devices.empty()) {
        ofLog(OF_LOG_NOTICE) << "Found " << m_Devices.size() << " DfRobot C4001 sensor(s):";
        for (auto& device : m_Devices) {
            ofLog(OF_LOG_NOTICE) << "Path: " << device.first.c_str() << " Device: " << (int)device.second;
        }
    }
    else {
        ofLog(OF_LOG_NOTICE) << "No DfRobot C4001 sensors found.";
    }

    return m_Devices.size();
}

std::string& mmSensor::getName()
{
    return name;
}
