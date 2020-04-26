//#define USE_WIRE 1
#include "liblamb/src/light_buffer/light_buffer.h"

#ifdef USE_WIRE
#include <TinyWireS.h>
#define SLAVE_ADDRESS 0x60

declare_light_buffer(uint8_t, 8, cbuff);

void on_receive(int bytesReceived) {
  for (uint8_t ix = 0; ix < bytesReceived; ix++)
    light_buffer_write(cbuff, TinyWireS.receive());
}
#endif

void setup_wire() {
#ifdef USE_WIRE
  TinyWireS.begin( SLAVE_ADDRESS );
  TinyWireS.onReceive( on_receive );
#endif  
}
