#define BUFFER_AUDIO

#ifdef BUFFER_AUDIO
  declare_buff256(uint8_t, abuff);
#endif

volatile uint32_t stime = 0;

int8_t lastOutput;
int8_t lastOutput2;
int8_t lastOutput3;

int8_t doLPF(int8_t input) {
    int8_t distanceToGo = input - lastOutput;
    lastOutput += distanceToGo >> 1; // Lower / higher number here will lower / raise the cutoff frequency
    distanceToGo = lastOutput - lastOutput2;
    lastOutput2 += distanceToGo >> 1; 
    distanceToGo = lastOutput2 - lastOutput3;
    lastOutput3 += distanceToGo >> 1; 
    return lastOutput3;
}

inline uint8_t generate_sample() {
  static uint8_t ix = 0;
  static uint8_t last_env = 0;
  static uint8_t last_lfo = 0;
  
  if (! (ix++ & 0b11111)) {
    last_env = denv.read() >> 24;
    last_lfo = lfo_type::traits::to_uint8_t(lfo.read());
    last_env = last_env * (128 | (last_lfo >> 1)) >> 8;
  }

  return osc_type::traits::to_uint8_t(
    doLPF(mul_T1U8S<8>(
        lpf.read ( osc_type::play_mixed<VOICES>(oscs), last_lfo ),
        last_env // amp
      ))
  );
}

ISR(TIMER0_COMPA_vect) {
  stime++;
#ifdef BUFFER_AUDIO
  if (! buff256_readable(abuff))
    PORTB |= _BV(4);
  else 
    OCR1A = buff256_read(abuff);
#else
  OCR1A =  generate_sample();
#endif
}

inline bool generate_audio() {
#ifdef BUFFER_AUDIO
  if (buff256_writable(abuff)) {
    buff256_write(abuff, generate_sample());
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

//  TCCR0A = _BV(WGM01); // 50khz
//  TCCR0B = _BV(CS01) | _BV(CS00);
//  OCR0A = 4;

  TCCR0A = _BV(WGM01);
  TCCR0B = _BV(CS01);
  OCR0A = 60; // 33.33 khz 

  TIMSK = _BV(OCIE0A);

  TCCR1 = _BV(CS10) | _BV(COM1A0) | _BV(PWM1A); // 250khz PWM
  OCR1A = 255;

  sei();
}

void setup_audio() {
#ifdef BUFFER_AUDIO
  while (generate_audio());
#endif
}

