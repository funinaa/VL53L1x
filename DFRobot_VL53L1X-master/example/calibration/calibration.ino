/*!
   file calibration.ino
   SEN0302 Distance Ranging Sensor
   The module is connected with Arduino Uno to read the distance
   This demo can be used to calibrate the sensor(both of offset and cross talk)
   This demo output the distance between sensor and target
   Copyright    [DFRobot](http://www.dfrobot.com), 2016
   Copyright    GNU Lesser General Public License
   version  V0.1
   date  2019-4-22
*/
#include "Arduino.h"
#include "Wire.h"
#include "DFRobot_VL53L1X.h" 

DFRobot_VL53L1X sensor(&Wire);
int16_t offset = 0;

void setup(void)
{
  Wire.begin();
  Serial.begin(9600);
  while (sensor.begin() != true){//sensor initialization
    Serial.println("Sensor init failed!");
    delay(1000);
  }

  offset = sensor.getOffset();//get sensor's current offset value
  Serial.print("Offset(mm): ");
  Serial.println(offset);
  Serial.println("offset = Actual Distance - Distance");// offset = targetDistance - sensorDistance
  Serial.println("Please input enter to enter calibration mode!");
  Serial.println();
}

void loop(void)
{
  sensor.startRanging();//start to ranging
  uint16_t distance = sensor.getDistance();//Get measurement distanc.
  sensor.update();//Waiting for calibration
  sensor.stopRanging();//stop to ranging
  Serial.print("Distance(mm): ");
  Serial.println(distance);
  delay(1000);
}