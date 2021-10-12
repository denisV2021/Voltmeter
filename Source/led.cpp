#include "led.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "FreeRTOS.h"
#include "task.h"

Led led;

Led::Led()
{
    
}

void Led::init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
       
}

void Led::vLedTask( void *pvParameters )
{    
    for( ;; )
    {	
        GPIO_SetBits(GPIOC, GPIO_Pin_8);
        vTaskDelay(100);
        GPIO_ResetBits(GPIOC, GPIO_Pin_8);
        vTaskDelay(100);
        taskYIELD();
    }
}
