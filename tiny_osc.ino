#include <lambos.h>

using namespace lambOS;

#include "led.h"
#include "math.h"
#include "sample_type_traits.h"
#include "interfaces.h"
#include "envelope.h"
#include "osc.h"
#include "buffer.h"
#include "dsp_modules.h"
#include "voice.h"
#include "i2c.h"
#include "command.h"
#include "timers.h"
#include "audio.h"

////////////////////////////////////////////////////////////////////////////////////
// NOTE: Tiny must be be fused for PLL clock, code must be compiled with -O3 or
//       buffer underruns will occur. Use an RC filter (104 / 1000k) on the output
//       pin.
////////////////////////////////////////////////////////////////////////////////////

uint8_t seq[] = "ffrriufrbbnraamq"; // main riff from 'Sweet Dreams'.

const uint16_t interval = (SRATE / 5) * 3 >> 1;

////////////////////////////////////////////////////////////////////////////////////

void soft_timer() {
  if (stime < (interval))
    return;

  flip_led();
  
  stime = 0;

  static uint8_t iix = 0;
  
  env.trigger();

  uint8_t note = seq[iix >> 0] - 55;

  for (uint16_t ix = 0, f = note; ix < VOICES; ix ++)
    oscs[ix].set_note(f);
  
  iix ++;
  iix %= 16;
}

////////////////////////////////////////////////////////////////////////////////////

void setup() {
  setup_led();  
  setup_wire();
  setup_voice();
  setup_audio();
  setup_timers();
}

void loop() {
#ifndef __AVR_ATtiny85__
//  stime = interval;
//  delay(500);
#else
  fill_audio_buffer();
#endif
  soft_timer();
}
