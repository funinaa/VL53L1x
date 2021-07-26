'''
file get_distance.py
SEN0302 Distance Ranging Sensor
The module is connected with RaspberryPi to read the distance
This demo can set distance mode(short for 2m, long for 4m)
This demo can set Threshold and get distance inside the threshold
Copyright    [DFRobot](http://www.dfrobot.com), 2016
Copyright    GNU Lesser General Public License
version  V1.0
date  2019-4-18
'''

import time
import sys
import threading
from gpio import GPIO
sys.path.append('../')
from DFRobot_VL53L1X import VL53L1X



sensor = VL53L1X(1) #VL53L1X begin

while not sensor.begin():
    time.sleep(1)

#sensor.setInterruptPolarityHigh()
sensor.set_distance_mode_short()
sensor.set_distance_threshold(300, 400, 1)#ThreshLow, ThreshHigh, Window(0 = below, 1 = above, 2 = out and 3 = in)
print("ThresholdWindow    :%.f" % sensor.get_distance_threshold_window())#0 = below, 1 = above, 2 = out and 3 = in
print("ThresholdLow(mm)   :%.f" % sensor.get_distance_threshold_low())
print("ThresholdHigh(mm)  :%.f" % sensor.get_distance_threshold_high())
#sensor.check_for_data_ready()
#sensor.set_distance_mode_long()
sensor.start_ranging()
time.sleep(0.1)
'''
while True:
    GPIO_OUT_PIN = 21
    pinOut = GPIO(GPIO_OUT_PIN, GPIO.OUT)
    pinOut.setOut(GPIO.LOW)
    time.sleep(1)
    pinOut.setOut(GPIO.HIGH)
    time.sleep(1)
'''
IO1 = 21
IO1Lock = threading.Lock()
IO1Flag = False

def IO1CallBack():
  global IO1Lock, IO1Flag
  IO1Lock.acquire() # wait key A lock release
  IO1Flag = True
  IO1Lock.release()
 
io1 = GPIO(IO1, GPIO.IN)
io1.setInterrupt(GPIO.RISING, IO1CallBack)


def main():
    global IO1Lock, IO1Flag
    while True:
        sensor.clear_interrupt()
        while IO1Flag:
            IO1Lock.acquire() # wait io1 release
            IO1Flag = False
            IO1Lock.release()
            print ("Distance(mm)   :%.f" % sensor.get_distance())
            time.sleep(0.1)
        time.sleep(0.1)

if __name__ == "__main__":
    main()
