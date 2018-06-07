//#define NEW_BUFFER

#ifdef NEW_BUFFER
Buff256<uint8_t> abuff;
#else
declare_buff256(uint8_t, abuff);
#endif

volatile uint32_t stime = 0;

ISR(TIMER0_COMPA_vect) {
  stime++;
#ifdef NEW_BUFFER
  if (! abuff.readable())
    PORTB |= _BV(4);
  else 
    OCR1A = abuff.read();
#else
  if (! buff256_readable(abuff))
    PORTB |= _BV(4);
  else 
    OCR1A = buff256_read(abuff);
#endif
}

inline bool fill_audio_buffer() {
#ifdef NEW_BUFFER
  if (abuff.writeable()) {
    abuff.write(VOICE.read());
    return true;
  }
#else
  if (buff256_writable(abuff)) {
    buff256_write(abuff, VOICE.read());
    return true;
  }
#endif
  return false;
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

void setup_audio() {
  while (fill_audio_buffer());
}

