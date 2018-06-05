class DEnvelope  {
  public:
    typedef uint32_t acc_type;
    static const acc_type maximum = ((acc_type)0) - 1;
    static const acc_type hz_phincr = maximum / SRATE;

    acc_type decay;
    acc_type decay_incr;
    
    DEnvelope() : 
      decay_incr(hz_phincr),
      decay(0) {
    }

    inline void trigger() {
      decay = maximum;
    }

    inline acc_type read() {
      return decay = (decay < decay_incr) ? 0 : decay - decay_incr;
    }
    
    inline void set_d_time (uint8_t seconds_q4n4) {
      set_d_hz(256 / seconds_q4n4);
    }

    inline void set_d_hz (uint8_t hz_q4n4) {
      decay_incr = hz_phincr * hz_q4n4 >> 4;      
    }
};

class ADEnvelope  {
  public:
    typedef uint32_t acc_type;
    static const acc_type maximum = ((acc_type)0) - 1;
    static const acc_type hz_phincr = maximum / SRATE;

    acc_type attack;
    acc_type attack_incr;
    acc_type decay;
    acc_type decay_incr;
    
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


    inline acc_type read() {
      if (attack > 0) {
        attack = attack < attack_incr ? 0 : attack - attack_incr;
        return ~attack;
      }
      else if (decay > 0) {
        return decay = (decay < decay_incr) ? 0 : decay - decay_incr;
      }
      
      return 0;
    }
    
    inline void set_d_time (uint8_t seconds_q4n4) {
      set_d_hz(seconds_q4n4 == 1 ? 255 : (256 / seconds_q4n4));
    }

    inline void set_d_hz (uint8_t hz_q4n4) {
      decay_incr = hz_phincr * hz_q4n4 >> 4;      
    }

    /* FIX THIS */
    inline void set_a_time (uint16_t seconds_q2n14) {
      set_a_hz(65536/seconds_q2n14);
    }

    inline void set_a_hz (uint16_t hz_q14n2) {
      attack_incr = hz_phincr * hz_q14n2 >> 2;
    }
};


ADEnvelope denv;

