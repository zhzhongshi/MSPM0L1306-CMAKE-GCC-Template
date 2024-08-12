#include "ti_msp_dl_config.h"


#define DELAY (16000000)

int main(void)
{
	SYSCFG_DL_init(); //芯片资源初始化,由SysConfig配置软件自动生成
	while(1)
	{
		delay_cycles(DELAY);//延时0.5秒
		DL_GPIO_togglePins(GPIO_LEDS_PORT,GPIO_LEDS_USER_TEST_PIN);
		delay_cycles(DELAY);//延时0.5秒
		DL_GPIO_togglePins(GPIO_LEDS_PORT,GPIO_LEDS_USER_LED_R_PIN);		
		delay_cycles(DELAY);//延时0.5秒
		DL_GPIO_togglePins(GPIO_LEDS_PORT,GPIO_LEDS_USER_LED_G_PIN);
		// 	delay_cycles(DELAY);//延时0.5秒
		DL_GPIO_togglePins(GPIO_LEDS_PORT,GPIO_LEDS_USER_LED_B_PIN);
	}
}

