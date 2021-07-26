'''
file setRangeCycle.py
SEN0302 Distance Ranging Sensor
The module is connected with RaspberryPi to read the distance

This demo can set timing budget and inter measurement
This demo output the distance between sensor and obstacles at the inter measurement that you set

Copyright    [DFRobot](http://www.dfrobot.com), 2016
Copyright    GNU Lesser General Public License
version  V1.0
date  2019-4-18
'''

import time
import sys
sys.path.append('../')
from DFRobot_VL53L1X import VL53L1X

sensor = VL53L1X(1) #VL53L1X begin

while not sensor.begin():
    time.sleep(1)

sensor.set_distance_mode_short()
'''
The timing budget (TB) is the time the sensor needed to complete one distance measurement.
TimingBudgetInMs = 20,33,50,100,200,500 ms
'''
sensor.set_timing_budget_in_ms(200)
'''
The intermeasurement period  must be greater than or equal to the timing budget
'''
sensor.set_inter_measurement_in_ms(200)
print("InterMeasurement(ms): %.f" % sensor.get_inter_measurement_in_ms())
print("TimingBudget(ms)    : %.f" % sensor.get_timing_budget_in_ms())
print(" ")
sensor.start_ranging()

def main():
    while True:
        while sensor.check_for_data_ready() == True:
            print ("Distance   : %.f" % sensor.get_distance())
            print (" ")

if __name__ == "__main__":
    main()