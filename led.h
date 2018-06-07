void flash_led() {
    for (uint8_t ix = 0; ix < 3; ix++) {
    PORTB |= _BV(4);
    delay(100);
    PORTB &= ~_BV(4); 
    delay(100);
    PORTB |= _BV(4);
    delay(50);
    PORTB &= ~_BV(4); 
    delay(50);
  }
}

inline void flip_led() {
    static bool f = false;
    
    f = !f;

    if (f)
      PORTB |= _BV(4);
    else
      PORTB &= ~_BV(4);  
}

void setup_led() {
  DDRB  |= _BV(4) | _BV(1);
  PORTB &= ~ _BV(4);
}

