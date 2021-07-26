'''
file get_distance.py
SEN0302 Distance Ranging Sensor
The module is connected with RaspberryPi to read the distance
This demo can be used to calibrate the sensor(both of offset and cross talk)
This demo output the distance between sensor and target
Copyright    [DFRobot](http://www.dfrobot.com), 2018
Copyright    GNU Lesser General Public License
version  V1.0
date  2018-12-11
'''

import time
import sys
sys.path.append('../')
from DFRobot_VL53L1X import VL53L1X

sensor = VL53L1X(1) #VL53L1X begin

while not sensor.begin():
    time.sleep(2)
act_distance = 208
#sensor.set_offset(30)           #This function apply the offset which found during calibration to the sensor
sensor.calibrate_offset(act_distance)    #This function set a certain distance to finds the offset and applies the offset
#sensor.set_x_talk(400)          #This function apply the cross talk which found during calibration to the sensor
sensor.calibrate_x_talk(act_distance)    #This function set a certain distance to finds the cross talk and applies the cross talk
print("ActDistance(mm): %.2f " % act_distance)
print(" ");
print("Offset(mm): %.2f " %sensor.get_offset())
print(" ");
print("XTalk(cps): %.2f " %sensor.get_x_talk())
print(" ");

def main():
    while True:
        sensor.start_ranging()
        time.sleep(1)
        print ("Distance   : %.2f " % sensor.get_distance())
        print (" ")
        sensor.stop_ranging()

if __name__ == "__main__":
    main()