#ifndef VL53L1_H
#define VL53L1_H

#include <stdint.h>
#include <stdbool.h>

#define SOFT_RESET                                            0x0000
#define VL53L1_I2C_SLAVE__DEVICE_ADDRESS                      0x0001
#define VL53L1_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND          0x0008
#define ALGO__CROSSTALK_COMPENSATION_PLANE_OFFSET_KCPS        0x0016
#define ALGO__CROSSTALK_COMPENSATION_X_PLANE_GRADIENT_KCPS    0x0018
#define ALGO__CROSSTALK_COMPENSATION_Y_PLANE_GRADIENT_KCPS    0x001A
#define ALGO__PART_TO_PART_RANGE_OFFSET_MM                    0x001E
#define MM_CONFIG__INNER_OFFSET_MM                            0x0020
#define MM_CONFIG__OUTER_OFFSET_MM                            0x0022
#define GPIO_HV_MUX__CTRL                                     0x0030
#define GPIO__TIO_HV_STATUS                                   0x0031
#define SYSTEM__INTERRUPT_CONFIG_GPIO                         0x0046
#define PHASECAL_CONFIG__TIMEOUT_MACROP                       0x004B
#define RANGE_CONFIG__TIMEOUT_MACROP_A_HI                     0x005E
#define RANGE_CONFIG__VCSEL_PERIOD_A                          0x0060
#define RANGE_CONFIG__VCSEL_PERIOD_B                          0x0063
#define RANGE_CONFIG__TIMEOUT_MACROP_B_HI                     0x0061
#define RANGE_CONFIG__TIMEOUT_MACROP_B_LO                     0x0062
#define RANGE_CONFIG__SIGMA_THRESH                            0x0064
#define RANGE_CONFIG__MIN_COUNT_RATE_RTN_LIMIT_MCPS           0x0066
#define RANGE_CONFIG__VALID_PHASE_HIGH                        0x0069
#define VL53L1_SYSTEM__INTERMEASUREMENT_PERIOD                0x006C
#define SYSTEM__THRESH_HIGH                                   0x0072
#define SYSTEM__THRESH_LOW                                    0x0074
#define SD_CONFIG__WOI_SD0                                    0x0078
#define SD_CONFIG__INITIAL_PHASE_SD0                          0x007A
#define ROI_CONFIG__USER_ROI_CENTRE_SPAD                      0x007F
#define ROI_CONFIG__USER_ROI_REQUESTED_GLOBAL_XY_SIZE         0x0080
#define SYSTEM__SEQUENCE_CONFIG                               0x0081
#define VL53L1_SYSTEM__GROUPED_PARAMETER_HOLD                 0x0082
#define SYSTEM__INTERRUPT_CLEAR                               0x0086
#define SYSTEM__MODE_START                                    0x0087
#define VL53L1_RESULT__RANGE_STATUS                           0x0089
#define VL53L1_RESULT__DSS_ACTUAL_EFFECTIVE_SPADS_SD0         0x008C
#define RESULT__AMBIENT_COUNT_RATE_MCPS_SD                    0x0090
#define VL53L1_RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0                  0x0096
#define VL53L1_RESULT__PEAK_SIGNAL_COUNT_RATE_CROSSTALK_CORRECTED_MCPS_SD0     0x0098
#define VL53L1_RESULT__OSC_CALIBRATE_VAL                      0x00DE
#define VL53L1_FIRMWARE__SYSTEM_STATUS                        0x00E5
#define VL53L1_IDENTIFICATION__MODEL_ID                       0x010F
#define VL53L1_ROI_CONFIG__MODE_ROI_CENTRE_SPAD               0x013E


#define VL53L1X_DEFAULT_DEVICE_ADDRESS                        0x29
#define CMDRECVBUFSIZE                                        20


enum eVL53L1X_Status{
  eVL53L1X_ok,
  eVL53L1X_InitError,
  eVL53L1X_WriteRegError,
  eVL53L1X_ReadRegError
};

typedef enum{
  eVL53L1X_Below = 0,
  eVL53L1X_Above = 1,
  eVL53L1X_Out = 2,
  eVL53L1X_In = 3
}eWindows;

typedef enum {
  eBudget_15ms = 15,//该选项仅能在短距离测距条件下使用
  eBudget_20ms = 20,//该选项仅能在短距离测距条件下使用
  eBudget_33ms = 33,//所有距离模式的最小时间
  eBudget_50ms = 50,
  eBudget_100ms = 100,
  eBudget_200ms = 200,
  eBudget_500ms = 500
}eTimingBudget;


eTimingBudget getTimingBudgetInMs(void);//get time overhead
void setDistanceMode(uint16_t DM);// Set distance mode,long: 0~4m,short: 0~1.3m
void setDistanceModeLong(void);//Set it to long distance mode 0~4m
void setDistanceModeShort(void);//Set it to short distance mode 0~1.3m
void setXTalk(uint16_t XtalkValue);//Set the crosstalk calibration value
void setOffset(int16_t OffsetValue);//This function programs the offset correction in mm.
int16_t getOffset(void);//This function returns the programmed offset correction value in mm. 
int8_t  calibrateOffset(uint16_t targetDistInMm);//Offset calibration
int8_t  calibrateXTalk(uint16_t targetDistInMm);//crosstalk calibration 
uint8_t getDistanceMode(void);//Get the current ranging mode 1:short mode  2:long mode
void    setTimingBudgetInMs(eTimingBudget timingBudget);//Set time overhead

bool vl53l1_init(void);
uint16_t getDistance(void); //Get the distance value
void     calibration(char* cmdStr);//enter calibration and waiting for calibration
uint16_t vl53l1_readdistance(void);

void vl53l1_test(void);

#endif //!VL53L1_H
