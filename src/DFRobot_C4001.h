/*!
 * @file DFRobot_C4001.h
 * @brief Define the basic structure of the DFRobot_C4001 class, the implementation of the basic methods
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [ZhixinLiu](zhixin.liu@dfrobot.com)
 * @version V1.0
 * @date 2023-02-02
 * @url https://github.com/DFRobot/DFRobot_C4001
 */
#pragma once
#ifndef __DFROBOT_C4001_H__
#define __DFROBOT_C4001_H__
#include <stdint.h>
#include <stdio.h>
#include <string>
#ifndef WIN32
#include <unistd.h>
#include "i2c.h"
#endif
#endif

 /**
  * @struct sSensorStatus_t
  * @brief sensor status
  * @note sensor status
  */
typedef struct {
	uint8_t workStatus;
	uint8_t workMode;
	uint8_t initStatus;
}sSensorStatus_t;


/**
 * @struct sPrivateData_t
 * @brief speed mode data
 */
typedef struct {
	uint8_t number;
	float speed;
	float range;
	uint32_t energy;
}sPrivateData_t;

/**
 * @struct sResponseData_t
 * @brief response data
 */
typedef struct {
	bool status;
	float response1;
	float response2;
	float response3;
}sResponseData_t;


/**
 * @struct sPwmData_t
 * @brief config pwm data param
 */
typedef struct {
	uint8_t pwm1;
	uint8_t pwm2;
	uint8_t timer;
}sPwmData_t;

/**
 * @struct sAllData_t
 * @brief sensor return data
 */
typedef struct {
	uint8_t exist;
	sSensorStatus_t sta;
	sPrivateData_t target;
}sAllData_t;

/**
 * @enum eMode_t
 * @brief sensor work mode
 */
typedef enum {
	eExitMode = 0x00,
	eSpeedMode = 0x01,
}eMode_t;

/**
 * @enum eSwitch_t
 * @brief Micromotion detection switch
 */
typedef enum {
	eON = 0x01,
	eOFF = 0x00,
}eSwitch_t;

/**
 * @enum eSetMode_t
 * @brief Set parameters for the sensor working status
 */
typedef enum {
	eStartSen = 0x55,
	eStopSen = 0x33,
	eResetSen = 0xCC,
	eRecoverSen = 0xAA,
	eSaveParams = 0x5C,
	eChangeMode = 0x3B,
}eSetMode_t;

class DFRobot_C4001 {
public:
#define DEVICE_ADDR_0   0x2A
#define DEVICE_ADDR_1   0x2B
#define TIME_OUT        0x64     ///< time out
#define I2C_FLAG        0x01
#define UART_FLAG       0x02

#define REG_STATUS              0x00
#define REG_CTRL0               0x01
#define REG_CTRL1               0x02
#define REG_SOFT_VERSION        0x03
#define REG_RESULT_STATUS       0x10
#define REG_TRIG_SENSITIVITY    0x20
#define REG_KEEP_SENSITIVITY    0x21
#define REG_TRIG_DELAY          0x22
#define REG_KEEP_TIMEOUT_L      0x23
#define REG_KEEP_TIMEOUT_H      0x24
#define REG_E_MIN_RANGE_L       0x25
#define REG_E_MIN_RANGE_H       0x26
#define REG_E_MAX_RANGE_L       0x27
#define REG_E_MAX_RANGE_H       0x28
#define REG_E_TRIG_RANGE_L      0x29
#define REG_E_TRIG_RANGE_H      0x2A

#define REG_RESULT_OBJ_MUN      0x10
#define REG_RESULT_RANGE_L      0x11
#define REG_RESULT_RANGE_H      0x12
#define REG_RESULT_SPEED_L      0x13
#define REG_RESULT_SPEED_H      0x14
#define REG_RESULT_ENERGY_L     0x15
#define REG_RESULT_ENERGY_H     0x16
#define REG_CFAR_THR_L          0x20
#define REG_CFAR_THR_H          0x21
#define REG_T_MIN_RANGE_L       0x22
#define REG_T_MIN_RANGE_H       0x23
#define REG_T_MAX_RANGE_L       0x24
#define REG_T_MAX_RANGE_H       0x25
#define REG_MICRO_MOTION        0x26

#define START_SENSOR    "sensorStart"
#define STOP_SENSOR     "sensorStop"
#define SAVE_CONFIG     "saveConfig"
#define RECOVER_SENSOR  "resetCfg"        ///< factory data reset
#define RESET_SENSOR    "resetSystem"     ///< RESET_SENSOR
#define SPEED_MODE      "setRunApp 1"
#define EXIST_MODE      "setRunApp 0"

	DFRobot_C4001();
	virtual ~DFRobot_C4001();

	virtual bool begin(void) { return true; }
public:

	uint8_t  uartI2CFlag = 0;
	/**
	 * @fn motionDetection
	 * @brief motion Detection
	 * @return true or false
	 */
	virtual bool motionDetection(void);

	/**
	 * @fn setSensor
	 * @brief Set the Sensor object
	 * @param mode
	 * @n  eStartSen        start collect
	 * @n  eStopSen         stop collect
	 * @n  eResetSen        reset sensor
	 * @n  eRecoverSen      recover params
	 * @n  eSaveParams      save config
	 * @n  eChangeMode      chagne mode
	 */
	virtual void setSensor(eSetMode_t mode);

	/**
	 * @fn setDelay
	 * @brief Set the Delay object
	 * @param trig Trigger delay, unit 0.01s, range 0~2s (0~200)
	 * @param keep Maintain the detection timeout, unit 0.5s, range 2~1500 seconds (4~3000)
	 * @return true or false
	 */
	virtual bool setDelay(uint8_t trig, uint16_t keep);

	/**
	 * @fn getTrigDelay
	 * @brief Get the Trig Delay object
	 * @return uint8_t
	 */
	virtual uint8_t getTrigDelay(void);

	/**
	 * @fn getKeepTimerout
	 * @brief get keep timer out
	 * @return  uint16_t
	 */
	virtual uint16_t getKeepTimerout(void);

	/**
	 * @fn getTrigRange
	 * @brief Get the Trig Range object
	 * @n     The triggering distance, in cm, ranges from 2.4 to 20m (240 to 2000).
	 * @n     The actual configuration range does not exceed the maximum and minimum detection distance.
	 * @return uint16_t
	 */
	virtual uint16_t getTrigRange(void);

	/**
	 * @fn getMaxRange
	 * @brief Get the Max Range object
	 * @return  uint16_t
	 */
	virtual uint16_t getMaxRange(void);

	/**
	 * @fn getMinRange
	 * @brief Get the Min Range object
	 * @return uint16_t
	 */
	virtual uint16_t getMinRange(void);

	/**
	 * @fn setDetectionRange
	 * @brief Set the Detection Range object
	 * @param min Detection range Minimum distance, unit cm, range 0.3~20m (30~2000), not exceeding max, otherwise the function is abnormal.
	 * @param max Detection range Maximum distance, unit cm, range 2.4~20m (240~2000)
	 * @param trig The trigger distance (unit: cm) ranges from 2.4 to 20m (240 to 2000). The actual configuration range does not exceed the maximum and minimum detection distance.
	 * @return true or false
	 */
	virtual bool setDetectionRange(uint16_t min, uint16_t max, uint16_t trig);

	/**
	 * @fn setTrigSensitivity
	 * @brief Set trigger sensitivity, 0~9
	 * @param sensitivity
	 * @return true or false
	 */
	virtual bool setTrigSensitivity(uint8_t sensitivity);

	/**
	 * @fn setKeepSensitivity
	 * @brief Set the Keep Sensitivity object，0~9
	 * @param sensitivity
	 * @return true or false
	 */
	virtual bool setKeepSensitivity(uint8_t sensitivity);

	/**
	 * @fn getTrigSensitivity
	 * @brief Get the Trig Sensitivity object
	 * @return uint8_t
	 */
	virtual uint8_t getTrigSensitivity(void);

	/**
	 * @fn getKeepSensitivity
	 * @brief Get the Keep Sensitivity object
	 * @return uint8_t
	 */
	virtual uint8_t getKeepSensitivity(void);

	/**
	 * @fn getStatus
	 * @brief Get the Status object
	 * @return sSensorStatus_t
	 * @n     workStatus
	 * @n       0 stop
	 * @n       1 start
	 * @n     workMode
	 * @n       0 indicates presence detection
	 * @n       1 is speed measurement and ranging
	 * @n     initStatus
	 * @n       0 not init
	 * @n       1 init success
	 */
	virtual sSensorStatus_t getStatus(void);

	/**
	 * @fn setIoPolaity
	 * @brief Set the Io Polaity object
	 * @param value
	 * @n     0：Output low level when there is a target, output high level when there is no target
	 * @n     1: Output high level when there is a target, output low level when there is no target (default)
	 * @return true or false
	 */
	bool setIoPolaity(uint8_t value);

	/**
	 * @fn getIoPolaity
	 * @brief Get the Io Polaity object
	 * @return uint8_t The level of the signal output by the pin after the configured I/O port detects the target
	 */
	uint8_t getIoPolaity(void);

	/**
	 * @fn setPwm
	 * @brief Set the Pwm object
	 * @param pwm1 When no target is detected, the duty cycle of the output signal of the OUT pin ranges from 0 to 100
	 * @param pwm2 After the target is detected, the duty cycle of the output signal of the OUT pin ranges from 0 to 100
	 * @param timer timer The value ranges from 0 to 255, corresponding to timer x 64ms
	 * @n     For example, timer=20, it takes 20*64ms=1.28s for the duty cycle to change from pwm1 to pwm2.
	 * @return true or false
	 */
	bool setPwm(uint8_t pwm1, uint8_t pwm2, uint8_t timer);


	/**
	 * @fn getPwm
	 * @brief Get the Pwm object
	 * @return sPwmData_t
	 * @retval pwm1  When no target is detected, the duty cycle of the output signal of the OUT pin ranges from 0 to 100
	 * @retval pwm2  After the target is detected, the duty cycle of the output signal of the OUT pin ranges from 0 to 100
	 * @retval timer The value ranges from 0 to 255, corresponding to timer x 64ms
	 * @n      For example, timer=20, it takes 20*64ms=1.28s for the duty cycle to change from pwm1 to pwm2.
	 */
	sPwmData_t getPwm(void);

	/**
	 * @fn setSensorMode
	 * @brief Set the Sensor Mode object
	 * @param mode
	 * @return true or false
	 */
	virtual bool setSensorMode(eMode_t mode);

	/**
	 * @fn getTargetNumber
	 * @brief Get the Target Number object
	 * @return uint8_t
	 */
	virtual uint8_t getTargetNumber(void);

	/**
	 * @fn getTargetSpeed
	 * @brief Get the Target Speed object
	 * @return float
	 */
	virtual float getTargetSpeed(void);

	/**
	 * @fn getTargetRange
	 * @brief Get the Target Range object
	 * @return float
	 */
	virtual float getTargetRange(void);

	/**
	 * @fn getTargetEnergy
	 * @brief Get the Target Energy object
	 * @return uint32_t
	 */
	virtual uint32_t getTargetEnergy(void);

	/**
	 * @fn setDetectThres
	 * @brief Set the Detect Thres object
	 * @param min Detection range Minimum distance, unit cm, range 0.3~20m (30~2000), not exceeding max, otherwise the function is abnormal.
	 * @param max Detection range Maximum distance, unit cm, range 2.4~20m (240~2000)
	 * @param thres Target detection threshold, dimensionless unit 0.1, range 0~6553.5 (0~65535)
	 * @return true or false
	 */
	virtual bool setDetectThres(uint16_t min, uint16_t max, uint16_t thres);

	/**
	 * @fn getTMinRange
	 * @brief get speed Min Range
	 * @return uint16_t
	 */
	uint16_t getTMinRange(void);

	/**
	 * @fn getTMaxRange
	 * @brief get speed Max Range
	 * @return uint16_t
	 */
	uint16_t getTMaxRange(void);

	/**
	 * @fn getThresRange
	 * @brief Get the Thres Range object
	 * @return uint16_t
	 */
	virtual uint16_t getThresRange(void);

	/**
	 * @fn setFrettingDetection
	 * @brief Set the Fretting Detection object
	 * @param sta
	 */
	virtual void setFrettingDetection(eSwitch_t sta);

	/**
	 * @fn getFrettingDetection
	 * @brief Get the Fretting Detection object
	 * @return eSwitch_t
	 */
	virtual  eSwitch_t getFrettingDetection(void);
protected:
	sResponseData_t wRCMD(std::string cmd1, uint8_t count);
	void writeCMD(std::string cmd1, std::string cmd2, uint8_t count);
	sAllData_t anaysisData(uint8_t* data, uint8_t len);
	sResponseData_t anaysisResponse(uint8_t* data, uint8_t len, uint8_t count);
	bool sensorStop(void);
	uint8_t  _addr;
private:
	uint8_t  _M_Flag = 0;
	sPrivateData_t _buffer;
	virtual int writeReg(uint8_t reg, uint8_t* data, uint8_t len) = 0;
	virtual int16_t readReg(uint8_t reg, uint8_t* data, uint8_t len) = 0;
};

#ifndef WIN32
class DFRobot_C4001_I2C : public DFRobot_C4001 {
public:
	DFRobot_C4001_I2C(const char* deviceName, uint8_t addr = DEVICE_ADDR_0);
	~DFRobot_C4001_I2C();
	virtual bool begin(void);
protected:
	virtual int writeReg(uint8_t reg, uint8_t* data, uint8_t len);
	virtual int16_t readReg(uint8_t reg, uint8_t* data, uint8_t len);
private:
	I2c* i2c;
	std::string path; 
};
#endif
class DFRobot_C4001_DUMMY : public DFRobot_C4001 {
public:
	DFRobot_C4001_DUMMY(const char* deviceName, uint8_t addr = DEVICE_ADDR_0);
	~DFRobot_C4001_DUMMY();

protected:
	virtual int writeReg(uint8_t reg, uint8_t* data, uint8_t len);
	virtual int16_t readReg(uint8_t reg, uint8_t* data, uint8_t len);
private:
	std::string path;
};
