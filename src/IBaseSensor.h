#pragma once

class IBaseSensor {
public:
    IBaseSensor(std::string path = "BaseClass", uint8_t address = 0x00) {}
    ~IBaseSensor() {}
};
