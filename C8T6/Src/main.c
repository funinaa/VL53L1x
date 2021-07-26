#include "delay.h"
#include "vl53l1.h"
#include "usart.h"

int main(void) {
	
	float t = 0.00f;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
    delay_init();
	usart_init();
	
	vl53l1_init();
	vl53l1_test();
	
	while(1) {
// 		u_printf("float:%.2f\r\n", t);
// 		t += 0.01f;
// 		delay_ms(500);
	}
}


