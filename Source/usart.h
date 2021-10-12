#ifndef USART_H
#define USART_H
#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"

#define BUFFER_SIZE 16

class Usart 
{
public:  
    Usart();
    static void init();    
    static void vUsartTask( void *pvParameters );
    char usartRxBuffer[BUFFER_SIZE];
    uint8_t rxIndex = 0;
    QueueHandle_t xUsartBufferQueue;
    
    void send(char* data, uint8_t len);
private:   
    
};




extern Usart usart;

#endif