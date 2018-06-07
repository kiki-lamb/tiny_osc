#include <lambos.h>

using namespace lambOS;

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
#include "audio.h"
#include "led.h"

////////////////////////////////////////////////////////////////////////////////////
// NOTE: Tiny must be be fused for PLL clock, code must be compiled with -O3 or
//       buffer underruns will occur.
////////////////////////////////////////////////////////////////////////////////////

uint8_t seq[] = "ffrriufrbbnraamq"; // main riff from 'Sweet Dreams'.

void soft_timer() {
  if (stime < (SRATE / 4))
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
  fill_audio_buffer();
  if (stime % 16 == 0) soft_timer();
}
