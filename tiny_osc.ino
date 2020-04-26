//#include <lambos.h>
//using namespace lambOS;

uint16_t interval;

#define STOP 1500

#include "D:/Code/liblamb/src/tables/kl_256_int8_t_sin.h"
#include "D:/Code/liblamb/src/tables/kl_256_uint8_t_sin.h"

#include "led.h"
#include "math.h"
#include "sample_type_traits.h"
#include "interfaces.h"
#include "envelope.h"
#include "timers.h"
#include "osc.h"
#include "buffer.h"
#include "dsp_modules.h"
#include "voice.h"
#include "i2c.h"
#include "command.h"
#include "audio.h"

////////////////////////////////////////////////////////////////////////////////////
// NOTE: Tiny must be be fused for PLL clock, code must be compiled with -O3 or
//       buffer underruns will occur. Use an RC filter (104 / 1000k) on the output
//       pin.
////////////////////////////////////////////////////////////////////////////////////

uint8_t seq[] = "ffrriufrbbnraamq"; // main riff from 'Sweet Dreams'.

////////////////////////////////////////////////////////////////////////////////////

void soft_timer() {
  if (stime < (interval))
    return;

  stime = 0;

  flip_led();
  
  static uint8_t iix = 0;

#ifdef AMP_ENABLE  
  env.trigger();
#endif

  uint8_t note = seq[iix >> 0] - 55;

  for (uint16_t ix = 0, f = note; ix < VOICES; ix ++)
    oscs[ix].set_note(f);
  
  iix ++;
  iix %= 16;
  
}

////////////////////////////////////////////////////////////////////////////////////

void setup() {
  interval = 240;

  setup_led();  
  setup_wire();
  setup_voice();
  setup_audio();
  setup_timers();
  Serial.begin(57600);
}

void loop() {
  fill_audio_buffer();
  soft_timer();
}
