/*!
   file setRangeCycle.ino
   SEN0302 Distance Ranging Sensor
   The module is connected with Arduino Uno to read the distance
   This demo can set timing budget and inter measurement
   This demo output the distance between sensor and obstacles at the inter measurement that you set
   Copyright    [DFRobot](http://www.dfrobot.com), 2016
   Copyright    GNU Lesser General Public License
   version  V0.1
   date  2019-4-18
*/
#include "Arduino.h"
#include "Wire.h"
#include "DFRobot_VL53L1X.h"

DFRobot_VL53L1X sensor(&Wire);

void setup(void)
{
  Wire.begin();

  Serial.begin(9600);

  while (sensor.begin() != true){//sensor initialization
    Serial.println("Sensor init failed!");
    delay(1000);
  }
  sensor.setDistanceModeShort();
  /*The timing budget (TB) is the time the sensor needed to complete one distance measurement*/
  /*TimingBudgetInMs = eBudget_20ms  = 20ms
                       eBudget_33ms  = 33ms
                       eBudget_50ms  = 50ms
                       eBudget_100ms = 100ms
                       eBudget_200ms = 200ms
                       eBudget_500ms = 500ms
   */
  sensor.setTimingBudgetInMs(eBudget_20ms);
  /* The intermeasurement period  must be greater than or equal to the timing budget*/
  sensor.setInterMeasurementInMs(20);
  Serial.print("InterMeasurement(ms):");
  Serial.println(sensor.getInterMeasurementInMs());// Get measurement interval
  Serial.print("TimingBudget(ms):    ");
  Serial.println(sensor.getTimingBudgetInMs());//Get time overhead time
  Serial.println();
  
  sensor.startRanging();//start to ranging.
}

void loop(void)
{
  while (sensor.checkForDataReady() == true){//Polling measurement data is ready.
    int distance;
    distance = sensor.getDistance();//Get measurement distance.
    Serial.print("Distance(mm): ");
    Serial.println(distance);
  }
}