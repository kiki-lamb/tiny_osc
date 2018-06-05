#include <lambos.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>

using namespace lambOS;

#define VOICES 3 // 3 will cause buffer underruns/frequency loss if compiled without -O3.

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
    oscs[1].set_detune_hz(0b00001001);
    oscs[2].set_detune_hz(0b00010101);
    oscs[2].wave = 3;
    oscs[2].octave = 2;
  setup_audio(); 
  setup_timers();
}

 uint8_t seq[] = {
 0,
 12,
 24,
 0,
 12,
 24,
 0,
 12,

 24,
 0,
 12,
 24,
 0,
 12,
 24,
 0,
 
 27,
 3,
 15,
 27,
 3,
 15,
 27,
 3,

 1,
 13,
 25,
 1,  
 13,
 25,
 1,  
 13,
};

void soft_timer() {
  if (stime < 65535) 
    return;

  denv.trigger();
      
  stime = 0;
  
  static uint8_t ix = 0;
  uint8_t note = seq[ix];

  for (uint16_t ix = 0, f = 36 + note; ix < VOICES; ix ++) 
    oscs[ix].set_note(f);
  
  ix ++;
  ix %= 32;
}

void loop() {
  for (uint8_t ix = 0; ix < 32; ix ++)
    if (! generate_audio()) break;
    
  process_commands();
  soft_timer();
}


