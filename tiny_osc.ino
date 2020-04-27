//#include "liblamb/src/lamb.h"

uint16_t interval;

#include "liblamb/src/tables/kl_256_int8_t_sin.h"
#include "liblamb/src/tables/kl_256_uint8_t_sin.h"
#include "liblamb/src/ring_buffer/ring_buffer.h"
#include "liblamb/src/sample_type_traits/sample_type_traits.h"
#include "liblamb/src/sample_type_interfaces/sample_type_interfaces.h"
#include "liblamb/src/dsp/dsp.h"
#include "liblamb/src/sample_sources/gate.h"

#include "led.h"
#include "math.h"
#include "envelope.h"
#include "timers.h"
#include "osc.h"
#include "voice.h"
#include "i2c.h"
#include "sequencer.h"
#include "audio.h"

////////////////////////////////////////////////////////////////////////////////
// NOTE: Tiny must be be fused for PLL clock, code must be compiled with -O3 or
//       buffer underruns will occur. Use an RC filter (104 / 1000k) on the output
//       pin.
////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(57600);

  interval = 200;

  setup_led();  
  setup_i2c();
  setup_audio();
  // setup_timers();
}

volatile int8_t force_output = 0;

void loop() {
  // VOICE.trigger();
  
  for (uint16_t ix = 0; ix < 500; ix++) {
    switch (ix) {
    case 0:
    case 100:
    case 300:
      VOICE.trigger();
    }

    force_output = VOICE.read();
  }

  while (true);
} 
