#pragma once

#include <chrono>

#if defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_7__)
#define RPI
#include "ofxGPIO.h"
#include "DFRobot_C4001.h"
#else
#include "Fake_C4001.h"
#endif

#include "Toy_C4001.h"
#include "ofJson.h"

class mmSensor {
public:
	mmSensor(std::string path = "", uint8_t address = 0x00);
	~mmSensor();

	bool setup();
	bool connect(int tries = 4);
	bool update();
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

	bool motionDetected;
	float targetDist;
	uint8_t targetCount;
	uint32_t targetEnergy;

	// Both keep and trigger sensitivity
	uint8_t sensitivityMin;
	uint8_t sensitivityMax;
	uint8_t triggerDelay;
	uint16_t keepDelay;
	float zoom;
	std::string name;

	// Saved...
	// TODO: Will be better in a struct
	std::string m_Location; // Will be used as ID
	glm::ivec3 detectRange;
	glm::ivec3 detectThres;
	uint8_t triggerSensitivity;
	uint8_t keepSensitivity;
	std::string m_path;
	uint8_t m_address;
	// How often we speak to the device
	float updateMillis;
	float syncMillis;
	bool dead;
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_lastUpdate;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_lastSync;
	bool m_ForceSync;
	bool m_isFake;
	bool m_updateDevice;

	DFRobot_C4001_I2C* device;
	Toy_C4001* toyDevice;

};
