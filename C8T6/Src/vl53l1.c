#include "vl53l1.h"
#include "iic.h"
#include "delay.h"
#include "stdlib.h"
#include "string.h"

#ifdef FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#endif 


//#define   USE_USART_DEBUG

//static enum eVL53L1X_Status lastOperateStatus;
static uint8_t   addr = 0x52;//0x29 or 0x52

static void startRanging(void);	//Start ranging
static void stopRanging(void);		//Stop ranging
static bool checkForDataReady(void);  //Check if the data is ready, return true, not return false
static void clearInterrupt(void);//clear interrupt
static uint8_t getInterruptPolarity(void);//Get the interrupt polarity

//static uint16_t getSignalRate(void);//get signal rate
static void writeByteData(uint16_t index, uint8_t data);//write data for 1 bytes
static void writeWordData(uint16_t index, uint16_t data);//write data for 2 bytes
//static void writeWordData32(uint16_t index, uint32_t data);//write data for 4 bytes
static void readByteData(uint16_t index, uint8_t *data);// read a byte data
static void readWordData(uint16_t index, uint16_t *data);//Read data for a bytes
//static void readWordData32(uint16_t index, uint32_t *data);//Read data for 4 bytes
static void i2CWrite(uint16_t reg, uint8_t *pBuf, uint16_t len);//IIC writes len bytes of data
static void i2CRead(uint16_t reg, uint8_t *pBuf, uint16_t len);//IIC reads len bytes of data

extern void u_printf(char* fmt, ...);

const uint8_t VL51L1X_DEFAULT_CONFIGURATION[] = {
0x00, /* 0x2d : set bit 2 and 5 to 1 for fast plus mode (1MHz I2C), else don't touch */
0x00, /* 0x2e : bit 0 if I2C pulled up at 1.8V, else set bit 0 to 1 (pull up at AVDD) */
0x00, /* 0x2f : bit 0 if GPIO pulled up at 1.8V, else set bit 0 to 1 (pull up at AVDD) */
0x01, /* 0x30 : set bit 4 to 0 for active high interrupt and 1 for active low (bits 3:0 must be 0x1), use SetInterruptPolarity() */
0x02, /* 0x31 : bit 1 = interrupt depending on the polarity, use CheckForDataReady() */
0x00, /* 0x32 : not user-modifiable */
0x02, /* 0x33 : not user-modifiable */
0x08, /* 0x34 : not user-modifiable */
0x00, /* 0x35 : not user-modifiable */
0x08, /* 0x36 : not user-modifiable */
0x10, /* 0x37 : not user-modifiable */
0x01, /* 0x38 : not user-modifiable */
0x01, /* 0x39 : not user-modifiable */
0x00, /* 0x3a : not user-modifiable */
0x00, /* 0x3b : not user-modifiable */
0x00, /* 0x3c : not user-modifiable */
0x00, /* 0x3d : not user-modifiable */
0xff, /* 0x3e : not user-modifiable */
0x00, /* 0x3f : not user-modifiable */
0x0F, /* 0x40 : not user-modifiable */
0x00, /* 0x41 : not user-modifiable */
0x00, /* 0x42 : not user-modifiable */
0x00, /* 0x43 : not user-modifiable */
0x00, /* 0x44 : not user-modifiable */
0x00, /* 0x45 : not user-modifiable */
0x20, /* 0x46 : interrupt configuration 0->level low detection, 1-> level high, 2-> Out of window, 3->In window, 0x20-> New sample ready , TBC */
0x0b, /* 0x47 : not user-modifiable */
0x00, /* 0x48 : not user-modifiable */
0x00, /* 0x49 : not user-modifiable */
0x02, /* 0x4a : not user-modifiable */
0x0a, /* 0x4b : not user-modifiable */
0x21, /* 0x4c : not user-modifiable */
0x00, /* 0x4d : not user-modifiable */
0x00, /* 0x4e : not user-modifiable */
0x05, /* 0x4f : not user-modifiable */
0x00, /* 0x50 : not user-modifiable */
0x00, /* 0x51 : not user-modifiable */
0x00, /* 0x52 : not user-modifiable */
0x00, /* 0x53 : not user-modifiable */
0xc8, /* 0x54 : not user-modifiable */
0x00, /* 0x55 : not user-modifiable */
0x00, /* 0x56 : not user-modifiable */
0x38, /* 0x57 : not user-modifiable */
0xff, /* 0x58 : not user-modifiable */
0x01, /* 0x59 : not user-modifiable */
0x00, /* 0x5a : not user-modifiable */
0x08, /* 0x5b : not user-modifiable */
0x00, /* 0x5c : not user-modifiable */
0x00, /* 0x5d : not user-modifiable */
0x01, /* 0x5e : not user-modifiable */
0xdb, /* 0x5f : not user-modifiable */
0x0f, /* 0x60 : not user-modifiable */
0x01, /* 0x61 : not user-modifiable */
0xf1, /* 0x62 : not user-modifiable */
0x0d, /* 0x63 : not user-modifiable */
0x01, /* 0x64 : Sigma threshold MSB (mm in 14.2 format for MSB+LSB), use SetSigmaThreshold(), default value 90 mm  */
0x68, /* 0x65 : Sigma threshold LSB */
0x00, /* 0x66 : Min count Rate MSB (MCPS in 9.7 format for MSB+LSB), use SetSignalThreshold() */
0x80, /* 0x67 : Min count Rate LSB */
0x08, /* 0x68 : not user-modifiable */
0xb8, /* 0x69 : not user-modifiable */
0x00, /* 0x6a : not user-modifiable */
0x00, /* 0x6b : not user-modifiable */
0x00, /* 0x6c : interMeasurement period MSB, 32 bits register, use SetIntermeasurementInMs() */
0x00, /* 0x6d : interMeasurement period */
0x0f, /* 0x6e : interMeasurement period */
0x89, /* 0x6f : interMeasurement period LSB */
0x00, /* 0x70 : not user-modifiable */
0x00, /* 0x71 : not user-modifiable */
0x00, /* 0x72 : distance threshold high MSB (in mm, MSB+LSB), use SetD:tanceThreshold() */
0x00, /* 0x73 : distance threshold high LSB */
0x00, /* 0x74 : distance threshold low MSB ( in mm, MSB+LSB), use SetD:tanceThreshold() */
0x00, /* 0x75 : distance threshold low LSB */
0x00, /* 0x76 : not user-modifiable */
0x01, /* 0x77 : not user-modifiable */
0x0f, /* 0x78 : not user-modifiable */
0x0d, /* 0x79 : not user-modifiable */
0x0e, /* 0x7a : not user-modifiable */
0x0e, /* 0x7b : not user-modifiable */
0x00, /* 0x7c : not user-modifiable */
0x00, /* 0x7d : not user-modifiable */
0x02, /* 0x7e : not user-modifiable */
0xc7, /* 0x7f : ROI center, use SetROI() */
0xff, /* 0x80 : XY ROI (X=Width, Y=Height), use SetROI() */
0x9B, /* 0x81 : not user-modifiable */
0x00, /* 0x82 : not user-modifiable */
0x00, /* 0x83 : not user-modifiable */
0x00, /* 0x84 : not user-modifiable */
0x01, /* 0x85 : not user-modifiable */
0x00, /* 0x86 : clear interrupt, use ClearInterrupt() */
0x00  /* 0x87 : start ranging, use StartRanging() or StopRanging(), If you want an automatic start after VL53L1X_init() call, put 0x40 in location 0x87 */
};


uint16_t vl53l1_readdistance(void) {
	
	uint16_t distance = 0;
#ifdef FreeRTOS
	taskENTER_CRITICAL();
#endif
	startRanging();
	distance = getDistance();
	stopRanging();
#ifdef FreeRTOS
	taskEXIT_CRITICAL();
#endif
	return distance;
}

void vl53l1_test(void) {
	
	uint16_t distance = 0;
	
	setDistanceModeShort();
	setTimingBudgetInMs(eBudget_15ms);
	while (1) {		
		startRanging();
		distance = getDistance();
		stopRanging();
		u_printf("distance:%dmm  getoffset:%d\r\n", distance, getOffset());
		delay_ms(100); 
	}
}


bool vl53l1_init(void) {
	
//	lastOperateStatus = eVL53L1X_InitError;
	uint8_t Addr = 0x00, tmp = 0;
	
	iic_init();
	
	for (Addr = 0x2D; Addr <= 0x87; Addr++){
		writeByteData(Addr, VL51L1X_DEFAULT_CONFIGURATION[Addr - 0x2D]);
	}
	startRanging();
	while(tmp==0){
		tmp = checkForDataReady();
#ifdef USE_USART_DEBUG
		u_printf("No VL53L1...\r\n");
#endif
		delay_ms(100);
	}
	tmp  = 0;
	clearInterrupt();
	stopRanging();
	writeByteData(VL53L1_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND, 0x09); /* two bounds VHV */
	writeByteData(0x0B, 0);
	
	setDistanceMode(1);	//ÉèÖÃ¶Ì¾àÀë²â¾à
	setTimingBudgetInMs(eBudget_100ms); //ÉèÖÃ²ÉÑùÆµÂÊ10HZ
//	lastOperateStatus = eVL53L1X_ok;
	
	return true;
}


int8_t calibrateOffset(uint16_t targetDistInMm)
{
	int16_t offset = getOffset();
	uint8_t  tmp;
	int i = 0;
	int16_t AverageDistance = 0;
	uint16_t distance;

	writeWordData(ALGO__PART_TO_PART_RANGE_OFFSET_MM, 0x0);
	writeWordData(MM_CONFIG__INNER_OFFSET_MM, 0x0);
	writeWordData(MM_CONFIG__OUTER_OFFSET_MM, 0x0);
	startRanging();    /* Enable VL53L1X sensor */
	for (i = 0; i < 50; i++) {
		while (tmp == 0){
			tmp = checkForDataReady();
#ifdef USE_USART_DEBUG
			u_printf("%d\r\n", tmp);
#endif 
			delay_ms(500);
		}
		distance = getDistance();
		clearInterrupt();
		AverageDistance = AverageDistance + distance;
	}
	stopRanging();
	AverageDistance = AverageDistance / 50;
	offset = targetDistInMm - AverageDistance;
	writeWordData(ALGO__PART_TO_PART_RANGE_OFFSET_MM, offset*4);
	return offset;
}

void setOffset(int16_t OffsetValue)
{
	int16_t Temp;
	Temp = (OffsetValue*4);
	writeWordData(ALGO__PART_TO_PART_RANGE_OFFSET_MM, (uint16_t)Temp);
	writeWordData(MM_CONFIG__INNER_OFFSET_MM, 0x0);
	writeWordData(MM_CONFIG__OUTER_OFFSET_MM, 0x0);
}


void setXTalk(uint16_t XtalkValue)
{
  writeWordData(ALGO__CROSSTALK_COMPENSATION_X_PLANE_GRADIENT_KCPS, 0x0000);
  writeWordData(ALGO__CROSSTALK_COMPENSATION_Y_PLANE_GRADIENT_KCPS, 0x0000);
  writeWordData(ALGO__CROSSTALK_COMPENSATION_PLANE_OFFSET_KCPS, (XtalkValue<<9)/1000);
}

int16_t getOffset()
{
	int16_t offset;
	uint16_t Temp;

	readWordData(ALGO__PART_TO_PART_RANGE_OFFSET_MM, &Temp);
	if(Temp & 0x1000){
		Temp |= 0xE000;
	}
	offset = (int16_t)(Temp);
	return offset/4;
}

uint16_t getDistance(void) { //Get the distance value
	
	uint16_t tmp;
	
	readWordData(VL53L1_RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0, &tmp);
	clearInterrupt();
	if(tmp > 4000){
		tmp = 65535;
	}
	
	return tmp;
}


uint8_t getDistanceMode() {
	
	uint8_t TempDM;
	uint16_t DM;

	readByteData(PHASECAL_CONFIG__TIMEOUT_MACROP, &TempDM);
	if (TempDM == 0x14)
		DM=1;
	if(TempDM == 0x0A)
		DM=2;
	return DM;
}


void calibration(char* cmdStr) {
	
	
	char* off_str = NULL, *dis_str = NULL;
	int16_t offset = 0, distance = 0;
	
	if (cmdStr == NULL) return;
	
	off_str = strstr(cmdStr, "offset:");
	dis_str = strstr(cmdStr, "distance:");
	
	off_str += strlen("offset:");
	dis_str += strlen("distance:");
	
	offset = atoi(off_str);
	distance = atoi(dis_str);
	
	setOffset(offset);
	calibrateOffset(distance);

#ifdef USE_USART_DEBUG
	u_printf("calibration successful!\r\n");
#endif
	cmdStr = NULL;
}

eTimingBudget getTimingBudgetInMs(void) {
	
	uint16_t Temp;
  eTimingBudget pTimingBudget;

  readWordData(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, &Temp);
  switch (Temp) {
      case 0x0051 :
      case 0x001E :
          pTimingBudget = eBudget_20ms;
          break;
      case 0x00D6 :
      case 0x0060 :
          pTimingBudget = eBudget_33ms;
          break;
      case 0x1AE :
      case 0x00AD :
          pTimingBudget = eBudget_50ms;
          break;
      case 0x02E1 :
      case 0x01CC :
          pTimingBudget = eBudget_100ms;
          break;
      case 0x03E1 :
      case 0x02D9 :
          pTimingBudget = eBudget_200ms;
          break;
      case 0x0591 :
      case 0x048F :
          pTimingBudget = eBudget_500ms;
          break;
      default:
          pTimingBudget = eBudget_20ms;
          break;
  }
  return pTimingBudget;
}

void setDistanceMode(uint16_t DM) {
	
	eTimingBudget TB;

	TB = getTimingBudgetInMs();
	switch (DM) { /* Short DistanceMode */
		case 1:
			writeByteData(PHASECAL_CONFIG__TIMEOUT_MACROP, 0x14);
			writeByteData(RANGE_CONFIG__VCSEL_PERIOD_A, 0x07);
			writeByteData(RANGE_CONFIG__VCSEL_PERIOD_B, 0x05);
			writeByteData(RANGE_CONFIG__VALID_PHASE_HIGH, 0x38);
			writeWordData(SD_CONFIG__WOI_SD0, 0x0705);
			writeWordData(SD_CONFIG__INITIAL_PHASE_SD0, 0x0606);
		break;
		case 2:
			writeByteData(PHASECAL_CONFIG__TIMEOUT_MACROP, 0x0A);
			writeByteData(RANGE_CONFIG__VCSEL_PERIOD_A, 0x0F);
			writeByteData(RANGE_CONFIG__VCSEL_PERIOD_B, 0x0D);
			writeByteData(RANGE_CONFIG__VALID_PHASE_HIGH, 0xB8);
			writeWordData(SD_CONFIG__WOI_SD0, 0x0F0D);
			writeWordData(SD_CONFIG__INITIAL_PHASE_SD0, 0x0E0E);
		break;
		default:
		break;
	}
	setTimingBudgetInMs(TB);
}

void setDistanceModeLong() {//Set it to long distance mode 0~4m
	
	setDistanceMode(2);
}
void setDistanceModeShort() {//Set it to short distance mode 0~1.3m
	
	setDistanceMode(1);
}

void setTimingBudgetInMs(eTimingBudget timingBudget) {
	
	uint16_t DM;

  DM = getDistanceMode();
  if (DM == 0)
      return;
  else if (DM == 1) {/* Short DistanceMode */
      switch (timingBudget) {
      case 15: /* only available in short distance mode */
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, 0x01D);
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, 0x0027);
          break;
      case 20:
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, 0x0051);
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, 0x006E);
          break;
      case 33:
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, 0x00D6);
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, 0x006E);
          break;
      case 50:
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, 0x1AE);
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, 0x01E8);
          break;
      case 100:
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, 0x02E1);
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, 0x0388);
          break;
      case 200:
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, 0x03E1);
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, 0x0496);
          break;
      case 500:
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, 0x0591);
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, 0x05C1);
          break;
      default:
          break;
      }
  } else {
      switch (timingBudget) {
      case 20:
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, 0x001E);
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, 0x0022);
          break;
      case 33:
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, 0x0060);
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, 0x006E);
          break;
      case 50:
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, 0x00AD);
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, 0x00C6);
          break;
      case 100:
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, 0x01CC);
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, 0x01EA);
          break;
      case 200:
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, 0x02D9);
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, 0x02F8);
          break;
      case 500:
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_A_HI, 0x048F);
          writeWordData(RANGE_CONFIG__TIMEOUT_MACROP_B_HI, 0x04A4);
          break;
      default:
          break;
      }
  }
}


void clearInterrupt(void) {//clear interrupt
	
	writeByteData(SYSTEM__INTERRUPT_CLEAR, 0x01);
}

uint8_t getInterruptPolarity(void) { //Get the interrupt polarity
	
	uint8_t Temp;
	uint8_t pInterruptPolarity;

	readByteData(GPIO_HV_MUX__CTRL, &Temp);
	Temp = Temp & 0x10;
	pInterruptPolarity = !(Temp>>4);
	
	return pInterruptPolarity;
}

bool checkForDataReady(void) {	//Check if the data is ready, return true, not return false
	
	uint8_t Temp;
	uint8_t IntPol;
//	uint8_t isDataReady;

	IntPol = getInterruptPolarity();
	readByteData(GPIO__TIO_HV_STATUS, &Temp);
	delay_ms(1);
	
	if ((Temp & 1) == IntPol)
		return  1;
	else
		return  0;
}

void startRanging(void) {	//Start ranging
	
	writeByteData(SYSTEM__MODE_START, 0x40);
}

void stopRanging(void) {		//Stop ranging
	
	writeByteData(SYSTEM__MODE_START, 0x00);
}

//static uint16_t getSignalRate(void) {
//	
//	uint16_t tmp;

//	readWordData(VL53L1_RESULT__DSS_ACTUAL_EFFECTIVE_SPADS_SD0, &tmp);
//	tmp = tmp * 8;
//	return tmp;
//}


static void writeByteData(uint16_t index, uint8_t data) {
	
	i2CWrite(index, &data, 1);
}

static void writeWordData(uint16_t index, uint16_t data) {
	
	uint8_t buffer[2];

	buffer[0] = data >> 8;
	buffer[1] = data & 0x00FF;
	i2CWrite(index, (uint8_t *)buffer, 2);
}

//static void writeWordData32(uint16_t index, uint32_t data){
//	
//	uint8_t buffer[4];

//	buffer[0] = (data >> 24) & 0xFF;
//	buffer[1] = (data >> 16) & 0xFF;
//	buffer[2] = (data >> 8) & 0xFF;
//	buffer[3] = (data >>  0) & 0xFF;
//	i2CWrite(index, (uint8_t *)buffer, 4);
//}

static void readByteData(uint16_t index, uint8_t *data) {
	
	i2CRead(index, data, 1);
}

static void readWordData(uint16_t index, uint16_t *data){
	
	uint8_t buffer[2] = {0,0};

	i2CRead(index, buffer, 2);
	*data = (buffer[0] << 8) + buffer[1];
}

//static void readWordData32(uint16_t index, uint32_t *data){
//	
//	uint8_t buffer[4] = {0,0,0,0};

//	i2CRead(index, buffer, 4);
//	*data = (buffer[0] << 24) + (buffer[1] << 16) + (buffer[2] << 8) + buffer[3];
//}

static void i2CWrite(uint16_t reg, uint8_t *pBuf, uint16_t len) {
	
//	lastOperateStatus = eVL53L1X_WriteRegError;
	
	IICwriteBytes(addr, reg, len, pBuf);
//    lastOperateStatus = eVL53L1X_ok;
}

static void i2CRead(uint16_t reg, uint8_t *pBuf, uint16_t len) {
	
//	lastOperateStatus = eVL53L1X_ReadRegError;
	
    IICreadBytes(addr, reg, len, pBuf);
//    lastOperateStatus = eVL53L1X_ok;
}
