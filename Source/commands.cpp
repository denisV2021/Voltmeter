#include <string.h>
#include "commands.h"

Command userCommand;
Answer answer2;

Answer::Answer(){}
Command::Command(){}

void Command::setState(State state)
{
    this->state = state;    
}
void Command::setMesureType(MesureType mesure_type)
{    
    this->mesure_type = mesure_type; 
}
void Command::setChannel(Channel channel)
{
    this->channel = channel;
}

void Command::answer(Command *c)
{
    char _ok[2] = {'O', 'k'};
    char _wrong[13] = {'W', 'r', 'o', 'n', 'g', ' ', 'c', 'o', 'm', 'm', 'a', 'n', 'd'};
    if((c->state != wrong_command) && (c->mesure_type != wrong_type) && (c->channel != wrong_channel))
    {
        answer2.len = 2;
        memcpy(answer2.message, _ok, answer2.len);
    }
    else 
    {
        answer2.len = 13;
        memcpy(answer2.message, _wrong, answer2.len);
    }
    xQueueSend(userCommand.xAnswerCommandsQueue, &answer2, 0);
}

void Command::vCommandsTask( void *pvParameters )
{
    char recivedValue[16];
    
    char _start[5] = {'s', 't', 'a', 'r', 't'};
    char _stop[4] = {'s', 't', 'o', 'p'};
    char _result[6] = {'r', 'e', 's', 'u', 'l', 't'};
    char _status[6] = {'s', 't', 'a', 't', 'u', 's'};
    
    char _ch0[3] = {'c', 'h', '0'};
    char _ch1[3] = {'c', 'h', '1'};
    char _ch2[3] = {'c', 'h', '2'};
    char _ch3[3] = {'c', 'h', '3'};
    char _ch4[3] = {'c', 'h', '4'};
    
    char _avg[3] = {'a', 'v', 'g'};
    char _rms[3] = {'r', 'm', 's'};
    char _none[4] = {'n', 'o', 'n', 'e'};
    
    uint8_t firstBrake = 0;
    bool channelDefined = false;
    bool typeDefined = false;    
    
    firstBrake = 0;
    channelDefined = false;
    typeDefined = false;
    for(;;)
    {
        if(xQueueReceive(userCommand.xCommandsBufferQueue, (char *)recivedValue, 0) == pdPASS)
        { 
            if(memcmp(_result, recivedValue, 6) == 0)
            {
                userCommand.setState(userCommand.result);
                firstBrake = 6;
            }
            if(memcmp(_status, recivedValue, 6) == 0)
            {
                userCommand.setState(userCommand.status);
                firstBrake = 6;
            }
            
            if(memcmp(_start, recivedValue, 5) == 0)
            {
                userCommand.setState(userCommand.start);
                firstBrake = 5;
            }
            if(memcmp(_stop, recivedValue, 4) == 0)
            {
                userCommand.setState(userCommand.stop);
                firstBrake = 4;
            }  
            else userCommand.setState(userCommand.wrong_command);
            
            if(memcmp(_ch0, recivedValue + firstBrake + 1, 3) == 0)
            {
                userCommand.setChannel(userCommand.ch0); 
                channelDefined = true;
            }
            if(memcmp(_ch1, recivedValue + firstBrake + 1, 3) == 0)
            {
                userCommand.setChannel(userCommand.ch1); 
                channelDefined = true;
            }
            if(memcmp(_ch2, recivedValue + firstBrake + 1, 3) == 0)
            {
                userCommand.setChannel(userCommand.ch2); 
                channelDefined = true;
            }
            if(memcmp(_ch3, recivedValue + firstBrake + 1, 3) == 0)
            {
                userCommand.setChannel(userCommand.ch3); 
                channelDefined = true;
            }
            if(memcmp(_ch4, recivedValue + firstBrake + 1, 3) == 0)
            {
                userCommand.setChannel(userCommand.ch4);  
                channelDefined = true;
            }
            if(!channelDefined) userCommand.setChannel(userCommand.wrong_channel);
            
            if(memcmp(_rms, recivedValue + firstBrake + 5, 3) == 0)
            {
                userCommand.setMesureType(userCommand.rms);  
                typeDefined = true;
            }
            if(memcmp(_avg, recivedValue + firstBrake + 5, 3) == 0)
            {
                userCommand.setMesureType(userCommand.avg);   
                typeDefined = true;
            }
            if(memcmp(_none, recivedValue + firstBrake + 5, 4) == 0)
            {
                userCommand.setMesureType(userCommand.none);    
                typeDefined = true;
            }
            if(!typeDefined) userCommand.setMesureType(userCommand.wrong_type);
            userCommand.answer(&userCommand);
        }
        taskYIELD();
    }
}