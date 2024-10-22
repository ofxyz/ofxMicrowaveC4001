#pragma once

#if defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_7__)
#define RPI
#endif

#ifdef RPI
#include "ofxGPIO.h"
#include "DFRobot_C4001.h"
#endif

class mmSensor {
public:
	mmSensor(std::string path = "", uint8_t address = 0x2A);
	~mmSensor();

	bool setup();
	bool update();
	void setFake(bool f);
	bool isFake();

	bool updateDetectRange();
	bool updateDetectThres();
	bool updateTrigSensitivity();

	glm::ivec3& getDetectTRange();
	glm::ivec3& getDetectThres();

	// For prototyping purposes
	// TODO: Implement differently
	float getFakeTargetRange() {
		/* TODO: Make a better fake with some controls */
		float fakePos = ofMap(ofNoise(ofGetElapsedTimef()), 0, 1, 0.f, 12.f);
		return ( (fakePos > 2 && fakePos < 5) || fakePos > 9) ? fakePos : 0;
	}

	std::string getName();
	std::string uint8_to_hex_string(uint8_t value);

	float targetDist;
	uint8_t targetCount;
	// Trigger sensitivity between 0-9 
	uint8_t triggerSensitivity;
	bool motionDetected;
	glm::ivec3 detectRange;
	glm::ivec3 detectThres;

private:
	// Will be a fake sensor when no GPIO
	DFRobot_C4001_I2C* device;
	std::string m_path;
	uint8_t m_address;
	std::string name;
	bool m_bFake;
};
