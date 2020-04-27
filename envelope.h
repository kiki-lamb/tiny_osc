template <uint32_t srate, typename sample_type = uint16_t > // sample_type should be an unsigned integer type.
  class Envelope : public lamb::SampleSource<sample_type>, lamb::Triggerable {
  public:
  inline virtual ~Envelope() {};
};


template <uint32_t srate, typename sample_type = uint16_t > // sample_type should be an unsigned integer type.
class DEnvelope : public Envelope<srate, sample_type> {
  public:
    typedef sample_type acc_type;
    static const acc_type maximum = ((acc_type)0) - 1;
    static const acc_type hz_phincr = maximum / srate;

    acc_type amplitude;
    acc_type decay_incr;
    
    inline DEnvelope() : 
      amplitude(0),
      decay_incr(hz_phincr) {}

    virtual inline void trigger() {
      amplitude = maximum;
    }

    virtual inline acc_type read() {
      return amplitude = (amplitude < decay_incr) ? 0 : amplitude - decay_incr;
    }
    
    inline void set_d_time (uint8_t seconds_q4n4) {
      if (seconds_q4n4 == 0)
        decay_incr = maximum;
      else
        set_d_hz(seconds_q4n4 == 1 ? 255 : (256 / seconds_q4n4));
    }

    inline void set_d_hz (uint8_t hz_q4n4) {
      decay_incr = hz_phincr * hz_q4n4 >> 4;  
//      Serial.print("ARG = ");
//      Serial.println(hz_q4n4);
//      Serial.print("HZ_PHINCR = ");
//      Serial.println(hz_phincr);
//      Serial.print("SRATE = ");
//      Serial.println(srate);
//      Serial.print("SET DECAY_INCR = ");
//      Serial.println(decay_incr);
//      Serial.print("SET MAXIMUM = ");
//      Serial.println(maximum);
//      Serial.println();
    }

    inline virtual ~DEnvelope() {}
};

template <uint32_t srate, typename sample_type = uint32_t >
class ADEnvelope : public DEnvelope<srate, sample_type> {
  public:
    typename DEnvelope<srate>::acc_type attack;
    typename DEnvelope<srate>::acc_type attack_incr;
    
    inline virtual ~ADEnvelope() {}

    inline ADEnvelope() : 
      attack(0),
      attack_incr(DEnvelope<srate, sample_type>::maximum) {}

    virtual inline void trigger() {
      attack = DEnvelope<srate, sample_type>::maximum; // - DEnvelope<srate>::decay; // why did I add this subtract??
      DEnvelope<srate>::trigger();
    }

    virtual inline typename DEnvelope<srate, sample_type>::acc_type read() {
      if (attack > 0) {
        attack = attack < attack_incr ? 0 : attack - attack_incr;
        return ~attack;
      }
      else 
        return DEnvelope<srate, sample_type>::read();
    }
    
    inline void set_a_time (uint16_t seconds_q2n14) {
      if (seconds_q2n14 == 0)
        attack_incr = DEnvelope<srate, sample_type>::maximum;
      else
        set_a_hz(seconds_q2n14 == 1 ? 65535 : (65536/seconds_q2n14));
    }

    inline void set_a_hz (uint16_t hz_q14n2) {
      attack_incr = DEnvelope<srate, sample_type>::hz_phincr * hz_q14n2 >> 2;
    }
};
