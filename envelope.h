class DEnvelope  {
  public:
    static const uint32_t hz_phincr = UINT32_MAX/SRATE;

    uint32_t decay;
    uint32_t decay_incr;
    
    DEnvelope() : 
      decay_incr(1),
      decay(0) {
    }

    inline void trigger() {
      decay = UINT32_MAX;
    }

    inline uint32_t read() {
      return decay = (decay < decay_incr) ? 0 : decay - decay_incr;
    }
    
    inline void set_time (uint8_t seconds_q4n4) {
      set_hz(256 / seconds_q4n4);
    }

    inline void set_hz (uint8_t hz_q4n4) {
      decay_incr = hz_phincr * hz_q4n4 >> 4;      
    }
};

class ADEnvelope  {
  public:
    static const uint32_t hz_phincr = UINT32_MAX/SRATE;

    uint32_t attack;
    uint32_t attack_incr;
    uint32_t decay;
    uint32_t decay_incr;
    
    ADEnvelope() : 
      attack_incr(UINT32_MAX),
      decay_incr(1),
      attack(0),
      decay(0) {;
      decay = UINT32_MAX;
    }

    inline void trigger() {
      attack = UINT32_MAX;
      decay = UINT32_MAX;
    }


    inline uint32_t read() {
      if (attack > 0) {
        attack = attack < attack_incr ? 0 : attack - attack_incr;
        return UINT32_MAX - attack;
      }
      else if (decay > 0) {
        return decay = (decay < decay_incr) ? 0 : decay - decay_incr;
      }
      
      return 0;
    }
    
    inline void set_d_time (uint8_t seconds_q4n4) {
      set_d_hz(256 / seconds_q4n4);
    }

    inline void set_d_hz (uint8_t hz_q4n4) {
      decay_incr = hz_phincr * hz_q4n4 >> 4;      
    }

    inline void set_a_time (uint8_t seconds_q4n4) {
      set_a_hz(256 / seconds_q4n4);
    }

    inline void set_a_hz (uint8_t hz_q4n4) {
      attack_incr = hz_phincr * hz_q4n4 >> 4;
    }
};


ADEnvelope denv;

