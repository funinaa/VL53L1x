'''
file speedMeasure.py
SEN0302 Distance Ranging Sensor
The module is connected with RaspberryPi to read the distance
This demo can get the 1D gesture(get closed and go away),then get the speed of target
Copyright    [DFRobot](http://www.dfrobot.com), 2016
Copyright    GNU Lesser General Public License
version  V1.0
date  2019-4-24
'''

import time
import sys
sys.path.append('../')
from DFRobot_VL53L1X import VL53L1X

sensor = VL53L1X(1) #VL53L1X begin
while not sensor.begin():
    time.sleep(1)
sensor.set_distance_mode_short();
sensor.set_timing_budget_in_ms(500);
sensor.set_inter_measurement_in_ms(500);
print("InterMeasurement: %.f" % sensor.get_inter_measurement_in_ms())
print("TimingBudget:     %.f" % sensor.get_timing_budget_in_ms())
time = sensor.get_inter_measurement_in_ms();
sensor.start_ranging();

dis1 = 0

def main():
    while True:
        global dis1
        while sensor.check_for_data_ready() == True:
            dis2 = dis1;
            dis1 = sensor.get_distance()
            speed = (dis1/time) - (dis2/time)
            if(abs(speed) < 2):
                if speed > 0:
                    print("Target is go away ,   speed( m/s) is %.3f" % speed)
                else:
                    print("Target is get closed ,speed( m/s) is %.3f" % speed)
                
            else:
                print("No target")
    

if __name__ == "__main__":
    main()