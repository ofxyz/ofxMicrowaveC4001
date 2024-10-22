#pragma once

#if defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_7__)
#define RPI
#endif

#include "MyToySensor.h"

#ifdef RPI
#include "ofxGPIO.h"
#include "DFRobot_C4001.h"
#else

extern class DFRobot_C4001_I2C;

#endif

class mmSensor {
public:
	mmSensor(std::string path = "", uint8_t address = 0x2A);
	~mmSensor();

	bool setup();
	bool update();
	bool isFake();

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

private:
	bool m_isFake;
#ifdef RPI
	DFRobot_C4001_I2C* device;
#else
	MyToySensor* device;
#endif

};
