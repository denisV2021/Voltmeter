#include "FreeRTOS.h"
#include "task.h"
#include "stm32f0xx.h"

#include "threads_init.h"
#include "led.h"
#include "usart.h"
#include "adc.h"
#include "commands.h"


int main()
{       
    led.init();
    usart.init();
    adc.init();
    
    
    freertos_init();
    
    vTaskStartScheduler();
  while(1);
}

