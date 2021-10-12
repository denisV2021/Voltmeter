#ifndef COMMANDS_H
#define COMMANDS_H

#include "FreeRTOS.h"
#include "queue.h"

class Answer {    
public:
    Answer();
    char message[16];
    uint8_t len;
};

class Command {
public:
    Command();
    enum State {
        start,
        stop,
        result, 
        status, 
        wrong_command
    };    
    enum MesureType {
        rms,
        avg,
        none, 
        wrong_type
    };
    enum Channel {
        ch0,
        ch1,
        ch2,
        ch3, 
        ch4, 
        wrong_channel
    };
        
    State state;
    MesureType mesure_type;  
    Channel channel;
    void setState(State state);
    void setMesureType(MesureType mesure_type);
    void setChannel(Channel channel);
    void answer(Command *c);
    
    static void vCommandsTask( void *pvParameters );
    QueueHandle_t xCommandsBufferQueue;
    QueueHandle_t xAnswerCommandsQueue;
};

extern Command userCommand;
extern Answer answer2;

#endif