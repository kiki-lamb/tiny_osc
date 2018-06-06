#include <lambos.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>

using namespace lambOS;

#define VOICES 2 // 3 // 3 will cause buffer underruns/frequency loss if compiled without -O3.

#include "math.h"
#include "sample_type_traits.h"
#include "interfaces.h"
#include "dsp.h"
#include "osc.h"
#include "envelope.h"
#include "buff.h"
#include "i2c.h"
#include "command.h"
#include "lpf.h"
#include "audio.h"
#include "led.h"

void setup() {
  setup_led();  
  setup_wire();

  lfo.set_hz(10, 0b00000000);
  lfo.set_wave(3);
 
  oscs[0].set_detune_hz(0b00000000);  
  oscs[1].set_detune_hz(0b00001100);
  oscs[2].set_detune_hz(0b00011000);
  
  oscs[0].octave = 0;
  oscs[1].octave = 1;
  oscs[2].octave = 2;
  
  oscs[0].set_wave(1);
  oscs[1].set_wave(1);
  oscs[2].set_wave(1);

  oscs[0].set_note(48);
  oscs[1].set_note(48);
  oscs[2].set_note(48);

  env.set_a_time(1024);
  env.set_d_time(0b00000100);
  
//  setup_audio(); 
  setup_timers();
}

uint8_t seq[] = "ffrriufrbbnraamq";
//  12, 12, 24, 24, 
//  15, 27, 12, 24, 
//  8,  8,  20, 24,
//  7, 7, 19, 24  
//};

void soft_timer() {
  if (stime < (SRATE / 5))
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

void loop() {
  while (generate_audio());
  process_commands();
  soft_timer();
}
