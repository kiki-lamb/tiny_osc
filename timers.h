#ifdef __AVR_ATtiny85__
#define TIMER_ISR TIMER0_COMPA_vect
#define PWM_PORT OCR1A
#else
#define TIMER_ISR TIMER1_COMPA_vect
#define PWM_PORT OCR2A
#endif

inline void setup_timers() {
  cli();

#ifdef __AVR_ATtiny85__
  PLLCSR |= _BV(PLLE) | _BV(PCKE);

//  OSCCAL = 0xFF;

  while (! (PLLCSR & _BV(PLOCK)));

  TCCR0A = _BV(WGM01);
  TCCR0B = _BV(CS01);
  OCR0A = 79; // 25 khz
  #define SRATE 25000 // (F_CPU/((OCR0A+1)*8)

  TIMSK = _BV(OCIE0A);
  TCCR1 = _BV(CS10) | _BV(COM1A0) | _BV(PWM1A); // 250khz PWM

  OCR1A = 255;
#else 
  TIMSK0 = 0;
  
  OSCCAL = 0xFF;

//  TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
//  TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));
//  TCCR1B = (TCCR1B & ~(_BV(CS11) | _BV(CS10))) | _BV(CS11) ;  // Prescaler 64
//  OCR1A = 79;   // 25000Hz Samplefreq (16000000/((79+1)*8))

  // 1000 Hz (16000000/((249+1)*64))
  OCR1A = 249;
  // CTC
  TCCR1B |= (1 << WGM12);
  // Prescaler 64
  TCCR1B |= (1 << CS11) | (1 << CS10);
  // Output Compare Match A Interrupt Enable
  TIMSK1 |= (1 << OCIE1A);
  TIMSK1 |= _BV(OCIE1A);
  
  #define SRATE 500
  
  ASSR &= ~(_BV(EXCLK) | _BV(AS2));

  TCCR2A |= _BV(WGM21) | _BV(WGM20);  // Set fast PWM mode  (p.157)
  TCCR2B &= ~_BV(WGM22);
  TCCR2A = (TCCR2A | _BV(COM2A1)) & ~_BV(COM2A0);  // Do non-inverting PWM on pin OC2A (p.155)
  TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));
  TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);  // No prescaler (p.158)

  OCR2A = 128;
#endif

  sei();
}
