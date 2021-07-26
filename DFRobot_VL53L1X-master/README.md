# VL53L1X


The VL53L1X is a state-of-the-art, Time-of-Flight (ToF), laser-ranging sensor<br>
It is the fastest miniature ToF sensor on the market with accurate ranging up to 4 m and fast ranging frequency up to 50 Hz<br>
Housed in a miniature and reflowable package, it integrates a SPAD receiving array, a 940 nm invisible Class1 laser emitter, physical infrared filters<br>
and optics to achieve the best ranging performance in various ambient lighting conditions with a range of cover window options<br>
Unlike conventional IR sensors, the VL53L1X uses ST’s latest generation ToF technology which allows absolute distance measurement whatever the target color and reflectance<br>

## DFRobot_VL53L1X Library for Arduino
---------------------------------------------------------
Provide an Arduino library for the VL53L1X modules.

## Table of Contents

* [Summary](#summary)
* [Feature](#feature)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [Credits](#credits)
<snippet>
<content>

## Summary
This is a library for the Arduino IDE, which can help Arduino to communicate with DFRobot_VL53L1X laser range sensor.<br>
The library makes it easy to configure the sensor via I2C and read range data from the sensor.<br>

## Feature
1. Serial input via I2C-bus
2. Max sampling rate given by I2C-bus speed
3. Operating voltage range from 2.6 to 3.5V

## Installation

Download the library ZIP file and unzip it to the Arduino folder of the library.<br>

## Methods

```C++

#include "DFRobot_VL53L1X.h"

/*
 * @brief init Wire
 */
DFRobot_VL53L1X(TwoWire *pWire);

/* 
 * @brief Init sensor and check the data is ready
 *
 * @return true if VL53L1X init succeed
 * @return false if VL53L1X init failed
 */
bool begin();

/*
 * @brief This function starts the ranging distance operation which is continuous
 */
void startRanging();

/*
 * @brief This function stops the ranging
 *
 */
void stopRanging();

/*
 * @brief This function programs the timing budget in ms
 *        The predefined values are 20, 50, 100, 200, and 500
 *
 * @param eTimingBudget eBudget_20ms = 20
 *                      eBudget_33ms = 33
 *                      eBudget_50ms = 50
 *                      eBudget_100ms = 100
 *                      eBudget_200ms = 200
 *                      eBudget_500ms = 500
 */
void setTimingBudgetInMs(eTimingBudget timingBudget);

/*
 * @brief This function returns the current timing budget in ms.
 *
 * @return timing budget
 */
eTimingBudget getTimingBudgetInMs();

/*
 * @brief Set long distance mode
 *        Long mode can range up to 4 m in the dark with a timing budget of 200 ms
 */
void setDistanceModeLong();

/*
 * @brief Set short distance mode
 *        Short mode maximum distance is limited to 2.2m
 */
void setDistanceModeShort();

/*
 * @brief This function programs the intermeasurement period (IMP) in ms
 *
 * @param InterMeasurement must be greater than or equal to the timing budget rang: 0~1693
 */
void setInterMeasurementInMs(uint16_t interMeasurement);

/*
 * @brief This function returns the intermeasurement period in ms
 *
 * @return The intermeasurement period in ms,rang:0~1693
 */
uint16_t getInterMeasurementInMs();

/*
 * @brief This function returns the distance measured by the sensor in mm
 *
 * @return The detailed distance
 */
uint16_t getDistance(); 


/*
 * @brief The function applies the offset value in millimeters to the sensor
 *
 * @param The user may use this function to apply, to the sensor
 *        The offset found during calibration which is stored in the host system
 *        OffsetValue = targetDistance - sensorDistance
 *        range: -1024~1023
 *
 */
void setOffset(int16_t OffsetValue);

/*
 * @brief The function returns the current offset correction applied to the sensor
 *
 * @return The detailed offset value, range: -1024~1023
 */
int16_t getOffset();

/*
 * @brief The function applies the crosstalk value, in cps, to the sensor
 *
 * @param The user may use this function to apply, to the sensor
 *        The crosstalk correction found during calibration and stored in the host system
 *        XtalkValue = 512*(SignalRate*(1-(Distance/targetDistance)))/SpadNb
 *
 */
void setXTalk(uint16_t XtalkValue);

/*
 * @brief The function returns the current crosstalk value programmed in the sensor
 *        Zero means there is no crosstalk compensation. The unit is cps (counts per second).
 *
 * @return The detailed Xtalk value
 */
uint16_t getXTalk(); 

/*
 * @brief Set distance threshold
 *
 * @param ThreshLow(in mm) is the threshold under which the device raises an interrupt if window = 0
 *        ThreshHigh(in mm) is the threshold above which the device raises an interrupt if window = 1
 *        IntOnNoTarget is the window detection mode where 0 = eVL53L1X_Below, 1 = eVL53L1X_Above, 2 = eVL53L1X_Out, and 3 = eVL53L1X_In
 */
void setDistanceThreshold(uint16_t ThreshLow, uint16_t ThreshHigh, eWindows Window);

/*
 * @brief This function returns the window detection mode where 0 = below, 1 = above, 2 = out and 3 = in
 *
 * @return The threshold window:0 = eVL53L1X_Below, 1 = eVL53L1X_Above, 2 = eVL53L1X_Out, and 3 = eVL53L1X_In
 */
eWindows getDistanceThresholdWindow();

/*
 * @brief This function returns the low threshold in mm
 *
 * @return The distance low threshold 
 */
uint16_t getDistanceThresholdLow();

/*
 * @brief This function returns the high threshold in mm
 *
 * @return The distance high threshold
 */
uint16_t getDistanceThresholdHigh(); 

/*
 * @brief This function finds the offset and applies the offset
 *        The offset correction value must be stored in the host system
 *
 * @param The target distance in mm
 */
void calibrateOffset(uint16_t targetDistInMm);

/*
 * @brief This function finds the crosstalk compensation value and applies the correction
 *
 * @return The target distance in mm
 */
void calibrateXTalk(uint16_t targetDistInMm);


/*
 * @brief 1D eVL53L1X_ok            for ok 
 *           eVL53L1X_InitError     for init error
 *           eVL53L1X_WriteRegError for I2C write error
 *           eVL53L1X_ReadRegError  for I2C read error
 */
eVL53L1X_Status lastOperateStatus;
```



## Compatibility

MCU                | Work Well | Work Wrong | Untested  | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino uno |       √      |             |            | 
RaspberryPi |       √      |             |            | 
Firebeetle  |       √      |             |            | 

## Credits

·author [luoyufeng yufeng.luo@dfrobot.com]
