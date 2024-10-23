#pragma once

#include <chrono>

#if defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_7__)
#define RPI
#include "ofxGPIO.h"
#include "DFRobot_C4001.h"
#else

extern class DFRobot_C4001_I2C;

#endif

#include "MyToySensor.h"


class mmSensor {
public:
	mmSensor(std::string path = "", uint8_t address = 0x00);
	~mmSensor();

	bool setup();
	bool update();
	bool isFake();
	bool isInSync();
	void syncNow();

	void updateDevice();
	bool updateDetectRange();
	bool updateDetectThres();
	bool updateTrigSensitivity();

	glm::ivec3& getDetectTRange();
	glm::ivec3& getDetectThres();
	std::string& getName();
	std::string uint8_to_hex_string(uint8_t value);

	float targetDist;
	uint8_t targetCount;
	// Between 0-9 
	uint8_t triggerSensitivity;
	bool motionDetected;
	glm::ivec3 detectRange;
	glm::ivec3 detectThres;
	std::string m_path;
	uint8_t m_address;
	std::string name;
	// How often we speak to the device
	float updateSec = 0.5;
	float syncSec = 5;

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_lastUpdate;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_lastSync;
	bool m_ForceSync;
	bool m_isFake;
	bool m_updateDevice;
#ifdef RPI
	DFRobot_C4001_I2C* device;
#endif
	MyToySensor* fakeSensor;

};
