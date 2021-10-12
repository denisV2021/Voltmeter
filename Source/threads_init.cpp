#include "threads_init.h"
#include "FreeRTOS.h"
#include "queue.h"

#include "usart.h"
#include "led.h"
#include "adc.h"
#include "commands.h"

void freertos_init()
{
    xTaskCreate(led.vLedTask, "Led", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL); 
    
    xTaskCreate(adc.vAdcTask, "Adc", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);    
    
    xTaskCreate(usart.vUsartTask, "Usart", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);     
    usart.xUsartBufferQueue = xQueueCreate(1, (UBaseType_t) 16 * sizeof(char));
    
    xTaskCreate(userCommand.vCommandsTask, "Usart", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    userCommand.xCommandsBufferQueue = xQueueCreate(1, (UBaseType_t) 16 * sizeof(char));
    userCommand.xAnswerCommandsQueue = xQueueCreate(1, (UBaseType_t) 1 * sizeof(Answer));
    
}