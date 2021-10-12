#include "FreeRTOS.h"
#include "task.h"
#include "stm32f0xx.h"

volatile int cc = 0;


void vLCDTask( void *pvParameters )
{    
    for( ;; )
    {		       
        cc = xTaskGetTickCount();
        taskYIELD();
    }
}


int main()
{    
    SystemInit();    
    xTaskCreate( vLCDTask, "LCD", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
    vTaskStartScheduler();
  while(1);
}

