//volatile Buffer<uint8_t, 255> abuff;
volatile lamb::RingBuffer<uint8_t, 255> abuff;

ISR(TIMER_ISR) {
  if (! abuff.readable())
    {}
  else {
    #ifdef STOP
      static uint16_t stop_ix = 0;
      if (stop_ix >= STOP)
        return;
     stop_ix +=1;
    #endif
    
    sequencer_time++;
    uint8_t tmp = abuff.read(); 
    PWM_PORT = tmp;
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
//  while (fill_audio_buffer());
}
  
