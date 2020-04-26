//#include "liblamb/src/lamb.h"

uint16_t interval;

#define STOP 500

#include "liblamb/src/tables/kl_256_int8_t_sin.h"
#include "liblamb/src/tables/kl_256_uint8_t_sin.h"
#include "liblamb/src/tables/kl_256_uint8_t_qsin.h"
#include "liblamb/src/ring_buffer/ring_buffer.h"
#include "liblamb/src/sample_type_traits/sample_type_traits.h"
#include "liblamb/src/sample_type_interfaces/sample_type_interfaces.h"
#include "liblamb/src/dsp/dsp.h"

#include "led.h"
#include "math.h"
#include "envelope.h"
#include "timers.h"
#include "osc.h"
#include "voice.h"
#include "i2c.h"
#include "sequencer.h"
#include "audio.h"

////////////////////////////////////////////////////////////////////////////////////
// NOTE: Tiny must be be fused for PLL clock, code must be compiled with -O3 or
//       buffer underruns will occur. Use an RC filter (104 / 1000k) on the output
//       pin.
////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(57600);

  interval = 200;

  setup_led();  
  setup_i2c();
  setup_voice();
  setup_audio();
//  setup_timers();
}


void loop() {
  env.set_d_hz(2 << 4);
      
  for (uint16_t ix = 0; ix < 500; ix++) {
    if ((ix % 250) == 0) {
      env.trigger();
      oscs[0].trigger();
      oscs[1].trigger();
    }

    uint8_t tmp = env.read() >> 8; 
     
//    Serial.print("Step ");
//    Serial.print(ix);
    int8_t voice = VOICE.read() - 128;  
    uint8_t env_val = ~pgm_read_byte(lamb::Tables::qsin256_uint8_t::data+lamb::Tables::qsin256_uint8_t::length-tmp-1);

    uint8_t lfo_tmp = lfo.read();
    lfo_tmp = Math::mul_T1U8S<8>(env_val, 192 + (lfo_tmp>>2)); 
    
    int8_t amped = voice; 
    amped = Math::mul_T1U8S<8>(amped, lfo_tmp); 
//    amped = Math::mul_T1U8S<8>(amped, env_val); 
    
    Serial.print(" ");
    Serial.print(tmp >> 1);
    Serial.print(" ");
    Serial.print( env_val >> 1 );
    Serial.print(" ");
    Serial.print( amped );
    Serial.print(" ");
    Serial.print( lfo_tmp >> 1 );

    Serial.print(" ");
    Serial.print(- (tmp >> 1));
    Serial.print(" ");
    Serial.print( - (env_val >> 1 ));
    Serial.print(" ");
    Serial.print(0);
    Serial.print(" ");
    Serial.print( - (lfo_tmp >> 1 ));

//    Serial.print(" ");
//    Serial.print(voice);
    Serial.println();
        
  }

  while (true);
} 
