#ifndef IIC_H
#define IIC_H

#include "sys.h"

#define VL53_SCL_GPIO_Port	GPIOB
#define VL53_SDA_GPIO_Port  GPIOB
#define VL53_SCL_Pin		GPIO_Pin_8
#define VL53_SDA_Pin		GPIO_Pin_9


#define SDA_IN()  	  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=(uint32_t)8<<4;}
#define SDA_OUT() 	  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=(uint32_t)0xf<<4;}

#define SCL_H         GPIO_SetBits(VL53_SCL_GPIO_Port, VL53_SCL_Pin)
#define SCL_L         GPIO_ResetBits(VL53_SCL_GPIO_Port, VL53_SCL_Pin)

#define SDA_H         GPIO_SetBits(VL53_SDA_GPIO_Port, VL53_SDA_Pin)
#define SDA_L         GPIO_ResetBits(VL53_SDA_GPIO_Port, VL53_SDA_Pin)

#define SDA_read      GPIO_ReadInputDataBit(VL53_SDA_GPIO_Port, VL53_SDA_Pin)

extern void    iic_init(void);
extern uint8_t IIC_ReadOneByte(uint8_t SlaveAddress,uint16_t REG_Address,uint8_t* data);
extern uint8_t IICwriteByte(uint8_t dev, uint16_t reg, uint8_t data);
extern uint8_t IICwriteBytes(uint8_t dev, uint16_t reg, uint16_t length, uint8_t* data);
extern uint8_t IICwriteBit(uint8_t dev,uint16_t reg,uint8_t bitNum,uint8_t data);
extern uint8_t IICreadBytes(uint8_t SlaveAddress,uint16_t REG_Address,uint8_t len,uint8_t *data);


#endif //!IIC_H
