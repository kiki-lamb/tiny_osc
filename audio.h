volatile Buffer<uint8_t, 255> abuff;

volatile uint32_t stime = 0;

ISR(TIMER_ISR) {
#ifdef STOP
  static uint16_t stop_ix = 0;
  if (stop_ix >= STOP)
    return;
  stop_ix +=1;
#endif

  stime++;
  
  if (! abuff.readable())
    {}
  else {
    uint8_t tmp = abuff.read(); 
    PWM_PORT = tmp;
    Serial.print(255);
    Serial.print(" ");
    Serial.print(stime);
    Serial.print(" ");
    Serial.print(interval);
    Serial.print(" ");
    Serial.print(128);
    Serial.print(" ");
    Serial.println(tmp);
  }
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
  
