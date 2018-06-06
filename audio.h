#define BUFFER_AUDIO

#ifdef BUFFER_AUDIO
  declare_buff256(uint8_t, abuff);
#endif

volatile uint32_t stime = 0;

uint8_t generate_audio();

ISR(TIMER0_COMPA_vect) {
  stime++;
#ifdef BUFFER_AUDIO
  if (! buff256_readable(abuff))
    PORTB |= _BV(4);
  else 
    OCR1A = buff256_read(abuff);
#else
  OCR1A =  generate_audio();
#endif
}


inline bool fill_buffer() {
#ifdef BUFFER_AUDIO
  if (buff256_writable(abuff)) {
    buff256_write(abuff, generate_audio());
    return true;
  }
  return false;
#else
  return false;
#endif
}

void setup_timers() {
  cli();

  PLLCSR |= _BV(PLLE) | _BV(PCKE);

  while (! (PLLCSR & _BV(PLOCK)));

  TCCR0A = _BV(WGM01);
  TCCR0B = _BV(CS01);
  OCR0A = 60; // 33.33 khz 

  TIMSK = _BV(OCIE0A);

  TCCR1 = _BV(CS10) | _BV(COM1A0) | _BV(PWM1A); // 250khz PWM
  OCR1A = 255;

  sei();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

typedef Oscillator<SRATE, int8_t> osc_type;
typedef Oscillator<SRATE / 32, int8_t> lfo_type;

osc_type oscs[VOICES];
lfo_type lfo;

//Attenuator att;

inline uint8_t generate_audio() {
  static uint8_t ix = 0;
  static uint8_t last_env = 0;
  static uint8_t last_lfo = 0;
  
  if (! (ix++ & 0b11111)) {
    last_env = env.read() >> 24;
    last_lfo = lfo_type::traits::to_uint8_t(lfo.read());
    last_env = last_env * (128 | (last_lfo >> 1)) >> 8;
  }

  return osc_type::traits::to_uint8_t( 
    mul_T1U8S<8>(
        osc_type::play_mixed<VOICES>(oscs),
        //att.read(), //oscs[0].read(),
        last_env // amp
      )
  );
}

void setup_audio() {
  lfo.set_hz(8, 0b00000000);
  lfo.set_wave(3);
 
  oscs[0].set_detune_hz(0b00000000);  
  oscs[1].set_detune_hz(0b00001100);
  //oscs[2].set_detune_hz(0b00011000);
  
  oscs[0].octave = osc_type::wf_saw;
  oscs[1].octave = osc_type::wf_saw;
  //oscs[2].octave = osc_type::wf_saw;
  
  oscs[0].set_wave(1);
  oscs[1].set_wave(1);
  //oscs[2].set_wave(1);

  oscs[0].set_note(48);
  oscs[1].set_note(48);
  //oscs[2].set_note(48);

  env.set_a_time(1024);
  env.set_d_time(0b00001000);
  
//  att.connect(&(oscs[0]));

#ifdef BUFFER_AUDIO
  while (fill_buffer());
#endif
}

