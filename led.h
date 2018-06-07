#ifdef __AVR_ATtiny85__
#define LED_ON  PORTB |= _BV(4)
#define LED_OFF PORTB &= ~_BV(4)
#else
#define LED_ON  PORTC |= _BV(0)
#define LED_OFF PORTC &= ~_BV(0)
#endif


inline void flip_led() {
    static bool f = false;

    f = !f;

    if (f)
      LED_ON;
    else
      LED_OFF;
}

void setup_led() {
#ifdef __AVR_ATtiny85__
  DDRB  |= _BV(4);
  PORTB &= ~ _BV(4);
#else
  DDRC  |= _BV(0);
  PORTC &= ~ _BV(0);
#endif
}
 
