#include "usart.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_rcc.h"
#include <string.h>

#include "task.h"
#include "commands.h"

Usart usart;


Usart::Usart()
{
    
}

void Usart::init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
       
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    USART_InitTypeDef USART_InitStructure;
        
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
          
    USART_Cmd(USART1, ENABLE);     
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    USART_ITConfig(USART1, USART_IT_ORE, ENABLE); 
}

void Usart::send(char* data, uint8_t len)
{       
    for (int i = 0; i < len; i++) {
        USART_SendData(USART1, data[i]);
        while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE)) {}
    }    
    while(!USART_GetFlagStatus(USART1, USART_FLAG_TC)) {}    
}

void Usart::vUsartTask( void *pvParameters )
{        
    char usartRecivedValue[16];    
    Answer answer;
    
    for( ;; )
    {	      
        if(xQueueReceive(usart.xUsartBufferQueue, (char *)usartRecivedValue, 0) == pdPASS)
        {            
            xQueueSend(userCommand.xCommandsBufferQueue, (char *)usartRecivedValue, 0);
        }
        if(xQueueReceive(userCommand.xAnswerCommandsQueue, &answer, 0) == pdPASS)
        {              
            usart.send(answer.message, answer.len);
        }
        taskYIELD();
    }
}

extern "C" void USART1_IRQHandler()
{
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_IDLE);
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
        if(usart.rxIndex != 0)
        {
            static portBASE_TYPE xHigherPriorityTaskWoken;
            xHigherPriorityTaskWoken = pdFALSE;
            xQueueSendFromISR(usart.xUsartBufferQueue, (char *)usart.usartRxBuffer,  &xHigherPriorityTaskWoken);
            if(xHigherPriorityTaskWoken == pdTRUE)
            {
                portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
            }
        }
        usart.rxIndex = 0;         
    }
    
    if(USART_GetITStatus(USART1, USART_IT_ORE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_ORE);
    }
    
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {         
        usart.usartRxBuffer[usart.rxIndex++] = (char)USART_ReceiveData(USART1);       
        if(usart.rxIndex == BUFFER_SIZE)
        {            
            usart.rxIndex = 0; 
            USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
        }
    }
}