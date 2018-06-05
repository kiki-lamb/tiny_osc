template <uint32_t srate>
class DEnvelope {
  public:
    typedef uint32_t acc_type;
    static const acc_type maximum = ((acc_type)0) - 1;
    static const acc_type hz_phincr = maximum / srate;

    acc_type decay;
    acc_type decay_incr;
    
    DEnvelope() : 
      decay_incr(hz_phincr),
      decay(0) {
    }

    virtual inline void trigger() {
      decay = maximum;
    }

    virtual inline acc_type read() {
      return decay = (decay < decay_incr) ? 0 : decay - decay_incr;
    }
    
    inline void set_d_time (uint8_t seconds_q4n4) {
      if (seconds_q4n4 == 0)
        decay_incr = maximum;
      else
        set_d_hz(seconds_q4n4 == 1 ? 255 : (256 / seconds_q4n4));
    }

    inline void set_d_hz (uint8_t hz_q4n4) {
      decay_incr = hz_phincr * hz_q4n4 >> 4;      
    }

    virtual ~DEnvelope() {}
};

template <uint32_t srate>
class ADEnvelope : public DEnvelope<srate> {
  public:
    typename DEnvelope<srate>::acc_type attack;
    typename DEnvelope<srate>::acc_type attack_incr;
    
    virtual ~ADEnvelope() {}

    ADEnvelope() : 
      attack_incr(DEnvelope<srate>::maximum),
      attack(0){}

    virtual inline void trigger() {
      attack = DEnvelope<srate>::maximum - DEnvelope<srate>::decay;
      DEnvelope<srate>::trigger();
    }

    virtual inline typename DEnvelope<srate>::acc_type read() {
      if (attack > 0) {
        attack = attack < attack_incr ? 0 : attack - attack_incr;
        return ~attack;
      }
      else 
        return DEnvelope<srate>::read();
    }
    
    inline void set_a_time (uint16_t seconds_q2n14) {
      if (seconds_q2n14 == 0)
        attack_incr = DEnvelope<srate>::maximum;
      else
        set_a_hz(seconds_q2n14 == 1 ? 65535 : (65536/seconds_q2n14));
    }

    inline void set_a_hz (uint16_t hz_q14n2) {
      attack_incr = DEnvelope<srate>::hz_phincr * hz_q14n2 >> 2;
    }
};

ADEnvelope<SRATE / 32> denv;

