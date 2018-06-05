#include <lambos.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>

using namespace lambOS;

#define VOICES 2 // 3 will cause buffer underruns/frequency loss if compiled without -O3.

#include "math.h"
#include "osc.h"
#include "buff.h"
#include "i2c.h"
#include "command.h"
#include "envelope.h"
#include "audio.h"
#include "led.h"

void setup() {
  setup_led();  
  setup_wire();

  for (uint16_t ix = 0, f = 60; ix < VOICES; ix ++, f += 60) {
    oscs[ix].wave = osc_type::wf_saw; //silence;
    oscs[ix].set_hz(f, 0);
  }
  
  oscs[1].set_detune_hz(0b00100000);
  oscs[2].set_detune_hz(0b00110000);
  
  oscs[0].wave = 1;
  oscs[1].wave = 1;
  oscs[2].wave = 1;

  denv.set_a_time(0);
  denv.set_d_time(0b00001000);
  
  setup_audio(); 
  setup_timers();
}

uint8_t seq[] = {
12, 13, 10, 9, 
12, 13, 5, 4 
};

void soft_timer() {
  if (stime < (SRATE / 2))
    return;

  denv.trigger();
  
  stime = 0;
  
  static uint8_t iix = 0;
  uint8_t note = seq[iix >> 4];

  for (uint16_t ix = 0, f = 18 + note; ix < VOICES; ix ++) {
    oscs[ix].set_note(f + ((iix % 16 % 3) * 12));
    oscs[ix].phacc = 1 << 31;
  }
  
  iix ++;
  iix %= 128;
}

void loop() {
  for (uint8_t ix = 0; ix < 32; ix ++)
    if (! generate_audio()) break;
    
  process_commands();
  soft_timer();
}


