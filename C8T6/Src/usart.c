#include "usart.h"
#include "sys.h"
#include "stdarg.h"
#include "string.h"
#include "stdio.h"


#define USE_USART_DMA_TX
#define USE_USART_IRQ_RX

#define BUF_LEN		64
static uint8_t rx_buf[BUF_LEN] = {0};
static uint8_t tx_buf[BUF_LEN] = {0};

#ifdef USE_USART_DMA_TX
static void dma_send(uint16_t len);
#endif

static void clk_init(void) {
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

static void gpio_init(void) {
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_DeInit(GPIOA);
	
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}


static void dma_init(void) {
	
	DMA_InitTypeDef DMA_InitStruct;
	
#ifdef USE_USART_DMA_TX	
	DMA_DeInit(DMA1_Channel4);
	////USART1 DMA_tx config
	DMA_InitStruct.DMA_DIR 				= DMA_DIR_PeripheralDST; //memory to Peripheral
	DMA_InitStruct.DMA_BufferSize 		= BUF_LEN;
	
	DMA_InitStruct.DMA_PeripheralInc 	= DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc 		= DMA_MemoryInc_Enable;
	
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize 	= DMA_MemoryDataSize_Byte;
	
	DMA_InitStruct.DMA_Mode				= DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority			= DMA_Priority_Medium;
	DMA_InitStruct.DMA_M2M				= DMA_M2M_Disable;
	
	DMA_InitStruct.DMA_MemoryBaseAddr     = (uint32_t)&tx_buf;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
	DMA_Init(DMA1_Channel4, &DMA_InitStruct);
	
	DMA_Cmd(DMA1_Channel4, DISABLE);
#endif	
	////USART1 DMA_rx config
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStruct.DMA_DIR 				= DMA_DIR_PeripheralSRC; //Peripheral to memory
	DMA_InitStruct.DMA_BufferSize 		= BUF_LEN;
	
	DMA_InitStruct.DMA_PeripheralInc 	= DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc 		= DMA_MemoryInc_Enable;
	
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize 	= DMA_MemoryDataSize_Byte;
	
	DMA_InitStruct.DMA_Mode				= DMA_Mode_Circular;
	DMA_InitStruct.DMA_Priority			= DMA_Priority_High;
	DMA_InitStruct.DMA_M2M				= DMA_M2M_Disable;
	
	DMA_InitStruct.DMA_MemoryBaseAddr     = (uint32_t)&rx_buf;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
	DMA_Init(DMA1_Channel5, &DMA_InitStruct);
	
	DMA_Cmd(DMA1_Channel5, ENABLE);
	
}

#ifdef USE_USART_IRQ_RX
static void nvic_init(void) {
	
	NVIC_InitTypeDef NVIC_InitStruct;	
	
	NVIC_InitStruct.NVIC_IRQChannel	    = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 1;
	NVIC_Init(&NVIC_InitStruct);
}
#endif

void usart_init(void) {
	
	USART_InitTypeDef USART_InitStruct;
	
	clk_init();
	gpio_init();
	dma_init();
	
#ifdef USE_USART_IRQ_RX
	nvic_init();
#endif
	USART_DeInit(USART1);
	
	USART_InitStruct.USART_BaudRate				= 115200;
	USART_InitStruct.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode					= USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity				= USART_Parity_No;
	USART_InitStruct.USART_StopBits				= USART_StopBits_1;
	USART_InitStruct.USART_WordLength			= USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);
	
	USART_ClearFlag(USART1, USART_FLAG_IDLE | USART_FLAG_RXNE);
	
	USART_DMACmd(USART1, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);
}


extern void calibration(char* cmdStr);
void USART1_IRQHandler(void) {
	
	uint8_t rx = 0x00;
	
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) {
		rx = USART1->SR;
		rx = USART1->DR;
		rx += 0;
		
		DMA_Cmd(DMA1_Channel5, DISABLE);
		DMA_ClearFlag(DMA1_FLAG_GL5 | DMA1_FLAG_TC5 |  DMA1_FLAG_HT5 | DMA1_FLAG_TE5);
		uint16_t not_accepted = DMA_GetCurrDataCounter(DMA1_Channel5);
		
//		u_printf("num:%d:%s\r\n", BUF_LEN - not_accepted, rx_buf);
//		for (not_accepted = 0; not_accepted < BUF_LEN; not_accepted++)  
//			rx_buf[not_accepted] = 0;
		calibration((char*)rx_buf);
		memset(rx_buf, 0, sizeof(rx_buf)*(BUF_LEN - not_accepted));
		
		DMA_SetCurrDataCounter(DMA1_Channel5, BUF_LEN);
		DMA_Cmd(DMA1_Channel5, ENABLE);
	}
	
}

#ifdef USE_USART_DMA_TX

static void dma_send(uint16_t len) {
	
	
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel4, len);
	DMA_Cmd(DMA1_Channel4, ENABLE);
}
#endif

void u_printf(char* fmt, ...) {
	
	va_list va;
	
	va_start(va, fmt); //打开参数列表
	vsprintf((char*)tx_buf, fmt, va); //将参数列表中的数据按格式输入到发送缓冲区
	va_end(va);  //关闭参数列表
	
	uint16_t len = strlen((char*)tx_buf);
	
#ifdef USE_USART_DMA_TX
	dma_send(len);
#else
	uint16_t i = 0;
	for (i = 0; i < len; ++i) {
		while ((USART1->SR & 0x40) == RESET);
		USART1->DR = tx_buf[i];
	}
#endif
}


