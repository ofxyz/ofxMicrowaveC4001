#pragma once

#include <chrono>

#if defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_7__)
#define RPI
#include "ofxGPIO.h"
#endif

#include "DFRobot_C4001.h"
#include "Toy_C4001.h"
#include "ofJson.h"

class mmSensor {
public:
    mmSensor(std::string path = "", uint8_t address = 0x00);
    ~mmSensor();

    bool setup();
    bool setup(void (*callbackPtr)(void*), void* pOwner);
    bool connect(int tries = 4);
    bool update();

    void addTriggerCallback(void (*funcPtr)(void*), void* pOwner);
    void callTriggerCallbacks();

    bool isFake();
    bool isInSync();
    void syncNow();

    void updateDevice();
    bool updateDetectRange();
    bool updateDetectThres();
    bool updateTrigSensitivity();
    bool updateKeepSensitivity();
    bool updateDelay();

    ofJson getSettings();
    void setSettings(ofJson settings);

    std::string getLocation();

    glm::ivec3& getDetectTRange();
    glm::ivec3& getDetectThres();
    std::string& getName();
    std::string uint8_to_hex_string(uint8_t value);

    bool connected;
    bool missing;
    bool motionDetected;
    bool trigger;
    glm::ivec3 triggerRange;
    float targetDist;
    uint8_t targetCount;
    uint32_t targetEnergy;

    // Both keep and trigger sensitivity
    uint8_t sensitivityMin;
    uint8_t sensitivityMax;
    uint8_t triggerDelay;
    uint16_t keepDelay;

    std::string name;

    // Saved...
    // TODO: Will be better in a struct
    std::string m_Location; // Will be used as ID
    //unit cm, range 0.3~20m (30~2000)

    // Units : cm
    // @param x: min Detection range 0.3~20m(30~2000), not exceeding max
    // @param y: max Detection range 2.4~20m(240~2000)
    // @param z: Trigger distance, from 2.4 to 20m (240 to 2000) 
    glm::ivec3 detectRange;
    // Units : cm
    // @param x: min Detection range 0.3~20m (30~2000), not exceeding max
    // @param y: max Detection range 2.4~20m (240~2000)
    // @param z: Target detection threshold, dimensionless unit 0.1, range 0~6553.5 (0~65535)
    glm::ivec3 detectThres;

    uint8_t m_uiTriggerSensitivity;
    uint8_t m_uiKeepSensitivity;
    uint8_t m_address;
    std::string m_path;

    // How often we speak to the device
    int m_fUpdateMillis;
    int m_fSyncMillis;
    int m_fTriggerMillis;
    bool m_bDead;
    bool m_bSynced;

private:
    int scanForDevices();
    std::vector<std::pair<std::string, uint8_t>> m_Devices;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_lastUpdate;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_lastSync;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_lastTrigger;
    bool m_ForceSync;
    bool m_isFake;
    bool m_updateDevice;

    DFRobot_C4001* m_Device;

    std::vector<std::pair<void*, void (*)(void*)>> m_vTriggerCallbacks;
};
