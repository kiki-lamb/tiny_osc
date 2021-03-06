volatile Buffer<uint8_t, 255> abuff;

volatile uint32_t stime = 0;

ISR(TIMER_ISR) {
  stime++;
  if (! abuff.readable())
    LED_ON;
  else 
    PWM_PORT = abuff.read();
}

inline bool fill_audio_buffer() {
  if (abuff.writeable()) {
    abuff.write(VOICE.read());
    return true;
  }
  return false;
}

void setup_audio() {
#ifdef __AVR_ATtiny85__
  DDRB |= _BV(1);
#else
  DDRB |= _BV(3);
#endif
  while (fill_audio_buffer());
}
  
