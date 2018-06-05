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
  
  oscs[0].set_detune_hz(0b00100000);
  oscs[1].set_detune_hz(0b01000000);
  
  oscs[0].wave = 3;
  oscs[1].wave = 1;
  oscs[2].wave = 1;

  denv.set_a_time(128);
  denv.set_d_time(0b00000100);
  
  setup_audio(); 
  setup_timers();
}

uint8_t seq[] = {
  18, 17, 14, 15, 
  18, 17, 14, 12 
};

void soft_timer() {
  if (stime < (SRATE / 5))
    return;

  stime = 0;

  denv.trigger();
  
  static uint8_t iix = 0;
  uint8_t note = seq[iix >> 4];

  for (uint16_t ix = 0, f = 14 + note; ix < VOICES; ix ++) {
    oscs[ix].set_note(f + ((iix % 8 % 3)) * 12);
  }
  
  iix ++;
  iix %= 128;
}

void loop() {
  while (generate_audio());
  process_commands();
  soft_timer();
}


