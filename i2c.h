// #define USE_WIRE 1

#include "liblamb/src/light_buffer/light_buffer.h"

#ifdef USE_WIRE
  #ifdef __AVR_ATtiny85__
    #include <TinyWireS.h>
    #define Wire TinyWireS
    #define I2C_READ Wire.receive
  #else
    #include <Wire.h>
    #define I2C_READ Wire.read
  #endif
  
#define SLAVE_ADDRESS 0x60

declare_light_buffer(uint8_t, 8, cbuff);

void on_receive_i2c(int bytesReceived) {
  for (uint8_t ix = 0; ix < bytesReceived; ix++)
    light_buffer_write(cbuff, I2C_READ());
}
#endif

void setup_i2c() {
#ifdef USE_WIRE
  Wire.begin( SLAVE_ADDRESS );
  Wire.onReceive( on_receive_i2c );
#endif  
}

#ifdef USE_WIRE
  #include "command.h"
#endif
