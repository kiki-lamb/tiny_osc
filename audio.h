volatile Buffer<uint8_t, 255> abuff;

volatile uint32_t stime = 0;

ISR(TIMER0_COMPA_vect) {
  stime++;
  if (! abuff.readable())
    LED_ON;
  else 
    OCR1A = abuff.read();
}

inline bool fill_audio_buffer() {
  if (abuff.writeable()) {
    abuff.write(VOICE.read());
    return true;
  }
  return false;
}

void setup_timers() {
  cli();

#ifdef __AVR_ATtiny85__
  PLLCSR |= _BV(PLLE) | _BV(PCKE);

  while (! (PLLCSR & _BV(PLOCK)));

  TCCR0A = _BV(WGM01);
  TCCR0B = _BV(CS01);
  OCR0A = 79; // 25 khz

  TIMSK = _BV(OCIE0A);

  TCCR1 = _BV(CS10) | _BV(COM1A0) | _BV(PWM1A); // 250khz PWM
  OCR1A = 255;
#endif

  sei();
}

void setup_audio() {
#ifdef __AVR_ATtiny85__
  DDRB  |= _BV(1);
#else
  DDRB  |= _BV(3);
#endif
  while (fill_audio_buffer());
}

