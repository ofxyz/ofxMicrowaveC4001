#pragma once

#include "Toy_C4001.h"

class DFRobot_C4001_I2C : public Toy_C4001 {
public:
	DFRobot_C4001_I2C(std::string path = "long", uint8_t address = 0x00) : Toy_C4001(path, address) {}
	~DFRobot_C4001_I2C() {}
};
