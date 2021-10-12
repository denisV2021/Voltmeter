#ifndef LED_H
#define LED_H

class Led {
public:

  enum State {
    idle,
    measure
  };
  
  Led();
  static void init();    
  static void vLedTask( void *pvParameters );
};

extern Led led;

#endif