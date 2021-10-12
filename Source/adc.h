#ifndef ADC_H
#define ADC_H
#include <stdint.h>


class Adc {
public:  
    Adc();
    static void init();    
    static void vAdcTask( void *pvParameters );
    
private:
    
   
};

extern Adc adc;
#endif