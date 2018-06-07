#include <lambos.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>

using namespace lambOS;


#include "math.h"
#include "sample_type_traits.h"
#include "interfaces.h"
#include "dsp.h"
#include "envelope.h"
#include "osc.h"
#include "buff.h"
#include "lpf.h"
#include "voice.h"
#include "i2c.h"
#include "command.h"
#include "audio.h"
#include "led.h"

////////////////////////////////////////////////////////////////////////////////////

uint8_t seq[] = "ffrriufrbbnraamq";
//  12, 12, 24, 24, 
//  15, 27, 12, 24, 
//  8,  8,  20, 24,
//  7, 7, 19, 24  
//};

void soft_timer() {
  if (stime < (SRATE / 4))
    return;

  stime = 0;

  static uint8_t iix = 0;
  
  env.trigger();

  uint8_t note = seq[iix >> 0] - 60;

  for (uint16_t ix = 0, f = note; ix < VOICES; ix ++) {
    oscs[ix].set_note(f);
    //oscs[ix].phacc = 1 << 30;
  }
  
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
  while (fill_audio_buffer());
  process_commands();
  soft_timer();
}
