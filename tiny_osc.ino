#include <lambos.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>

using namespace lambOS;

#define VOICES 2

#include "math.h"
#include "osc.h"
#include "buff.h"
#include "i2c.h"
#include "command.h"
#include "audio.h"
#include "led.h"

void setup() {
  setup_led();  
  setup_oscs();
  setup_audio(); 
  setup_wire();
  
  oscs[0].wave = 3;
  oscs[1].wave = 3;
  
  oscs[0].set_hz(60, 0);
  oscs[1].set_hz(120, 0);
  
  setup_timers();
}

void soft_timer() {
  if (stime < 15000) 
    return;
  stime = 0;
  
  flip_led();
  
  static uint8_t ix = 0;
  uint8_t note = 0;
  switch(ix) {
    case 0:
      note = 0; break;
    case 1:
      note = 2; break;
    case 2:
      note = 3; break;
    case 3:
      note = 5; break;
    case 4:
      note = 7; break;
    case 5:
      note = 8; break;
    case 6:
      note = 11; break;
    case 7:
      note = 12; break;
  }

  oscs[0].set_note(48 + note); // xx);
  oscs[1].set_note(48 + note + 12); // xx);

  ix ++;
  ix %= 8;
}

void loop() {
  for (uint8_t ix = 0; ix < 32; ix ++)
    if (! generate_audio()) break;
    
  process_commands();
  soft_timer();
}


