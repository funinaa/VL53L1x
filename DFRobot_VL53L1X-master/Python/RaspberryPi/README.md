# VL53L1X

The VL53L1X is a state-of-the-art, Time-of-Flight (ToF), laser-ranging sensor<br>
It is the fastest miniature ToF sensor on the market with accurate ranging up to 4 m and fast ranging frequency up to 50 Hz<br>
Housed in a miniature and reflowable package, it integrates a SPAD receiving array, a 940 nm invisible Class1 laser emitter, physical infrared filters<br>
and optics to achieve the best ranging performance in various ambient lighting conditions with a range of cover window options<br>
Unlike conventional IR sensors, the VL53L1X uses ST’s latest generation ToF technology which allows absolute distance measurement whatever the target color and reflectance<br>

## DFRobot_VL53L1X Library for RaspberryPi
---------------------------------------------------------
Provide a Raspberry Pi library for the VL53L1X modules.

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
This is a library for the RaspBerryPi, which can help RaspBerryPi to communicate with DFRobot_VL53L1X laser range sensor.<br>
The library makes it easy to configure the sensor via I2C and read range data from the sensor.<br>

## Feature
1. Serial input via I2C-bus
2. Max sampling rate given by I2C-bus speed
3. Operating voltage range from 2.6 to 3.5V

## Installation

This Sensor should work with VL53L1X on RaspberryPi.<br>
Run the program:
```
$> python VL53L1X.py

```

## Methods

```Python

'''
@brief Init The VL53L1X

@param smbus Bus = 1

'''
VL53L1X(1)

'''
@brief Init sensor and check the data is ready

@return true if VL53L1X init succeed
@return false if VL53L1X init failed
'''
def begin(self)

'''
@brief This function starts the ranging distance operation which is continuous
'''
def startRanging(self)

'''
@brief This function stops the ranging
'''
def stopRanging(self)

'''
@brief This function programs the timing budget in ms
       The predefined values are 20, 50, 100, 200, and 500
'''
def setTimingBudgetInMs(self, timingBudget)

'''
@brief This function returns the current timing budget in ms

@return timing budget
'''
def getTimingBudgetInMs(self)

'''
@brief Set long distance mode
       Long mode can range up to 4 m in the dark with a timing budget of 200 ms
'''
def setDistanceModeLong(self)

'''
@brief Set short distance mode
       Short mode maximum distance is limited to 2.2m

'''
def setDistanceModeShort(self)

'''
@brief Gets the distance mode

@return distance mode(2 for long or 1 for short)
'''
def getDistanceMode(self)

'''
@brief This function programs the intermeasurement period (IMP) in ms

@param InterMeasurement must be greater than or equal to the timing budget
'''
def setInterMeasurementInMs(self, interMeasurement)

'''
@brief This function returns the intermeasurement period in ms

@return The intermeasurement period in ms
'''
def getInterMeasurementInMs(self)

'''
@brief This function returns the distance measured by the sensor in mm

@return The detailed distance
'''
def getDistance(self) 

'''
@brief The function applies the offset value in millimeters to the sensor

@param The detailed offset value(depends on sensor works)
       OffsetValue = targetDistance - sensorDistance
'''
def setOffset(self, OffsetValue);

'''
@brief Set Xtalk calibration

@param The user may use this function to apply, to the sensor
       The offset found during calibration which is stored in the host system
       XtalkValue = 512*(SignalRate*(1-(Distance/targetDistance)))/SpadNb
'''
def setXTalk(self, XtalkValue);

'''
@brief Set distance threshold

@param ThreshLow(in mm) is the threshold under which the device raises an interrupt if window = 0
       ThreshHigh(in mm) is the threshold above which the device raises an interrupt if window = 1
       IntOnNoTarget is the window detection mode where 0 = below, 1 = above, 2 = out, and 3 = in
'''
def setDistanceThreshold(self, ThreshLow, ThreshHigh, Window)

'''
@brief This function returns the window detection mode where 0 = below, 1 = above, 2 = out and 3 = in

@return The threshold window:0 = below, 1 = above, 2 = out, and 3 = in
'''
def getDistanceThresholdWindow(self);

'''
@brief This function returns the low threshold in mm

@return The distance threshold low
'''
def getDistanceThresholdLow(self)

'''
@brief This function returns the high threshold in mm

@return The distance threshold high
'''
def getDistanceThresholdHigh(self)

'''
@brief This function finds the offset and applies the offset
       The offset correction value must be stored in the host system

@param The target distance in mm
'''
def calibrateOffset(self, targetDistInMm)

'''
@brief This function finds the crosstalk compensation value and applies the correction

@return The target distance in mm
'''
def calibrateXTalk(self, targetDistInMm)

'''
@brief 1D gesture
'''
def gesture(self)


```


## Credits

Â·author [luoyufeng yufeng.luo@dfrobot.com]
