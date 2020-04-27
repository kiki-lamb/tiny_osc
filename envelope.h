////////////////////////////////////////////////////////////////////////////////
// Envelope
////////////////////////////////////////////////////////////////////////////////

// sample_type should be an unsigned integer type.
template <uint32_t srate, typename sample_type = uint16_t >
  class Envelope : public lamb::SampleSource<sample_type>, lamb::Triggerable {
  public:
  inline virtual ~Envelope() {};
};

////////////////////////////////////////////////////////////////////////////////
// REnvelope
////////////////////////////////////////////////////////////////////////////////

// sample_type should be an unsigned integer type.
template <uint32_t srate, typename sample_type = uint16_t >
class REnvelope : public Envelope<srate, sample_type> {
  public:
    typedef sample_type acc_type;
    static const acc_type maximum = ((acc_type)0) - 1;
    static const acc_type hz_phincr = maximum / srate;

    acc_type amplitude;
    acc_type decay_incr;
    
    inline REnvelope() : 
      amplitude(0),
      decay_incr(hz_phincr) {}

    virtual inline void trigger() {
      amplitude = maximum;
    }

    virtual inline acc_type read() {
      return amplitude = (amplitude < decay_incr) ? 0 : amplitude - decay_incr;
    }
    
  // v Test this? I forget if it works.
  inline void set_d_time (uint8_t seconds_q4n4) {
    if (seconds_q4n4 == 0)
        decay_incr = maximum;
      else
        set_d_hz(seconds_q4n4 == 1 ? 255 : (256 / seconds_q4n4));
    }

    inline void set_d_hz (uint8_t hz_q4n4) {
      decay_incr = hz_phincr * hz_q4n4 >> 4;  
    }

    inline virtual ~REnvelope() {}
};

////////////////////////////////////////////////////////////////////////////////
// AREnvelope
////////////////////////////////////////////////////////////////////////////////

template <uint32_t srate, typename sample_type = uint32_t >
class AREnvelope : public REnvelope<srate, sample_type> {
  public:
    typename REnvelope<srate>::acc_type attack;
    typename REnvelope<srate>::acc_type attack_incr;
    
    inline virtual ~AREnvelope() {}

    inline AREnvelope() : 
      attack(0),
      attack_incr(REnvelope<srate, sample_type>::maximum) {}

    virtual inline void trigger() {
      attack = REnvelope<srate, sample_type>::maximum;
      // ^ - REnvelope<srate>::decay; // why did I add this subtract??
      REnvelope<srate>::trigger();
    }

    virtual inline typename REnvelope<srate, sample_type>::acc_type read() {
      if (attack > 0) {
        attack = attack < attack_incr ? 0 : attack - attack_incr;
        return ~attack;
      }
      else 
        return REnvelope<srate, sample_type>::read();
    }
    
    inline void set_a_time (uint16_t seconds_q2n14) {
      if (seconds_q2n14 == 0)
        attack_incr = REnvelope<srate, sample_type>::maximum;
      else
        set_a_hz(seconds_q2n14 == 1 ? 65535 : (65536/seconds_q2n14));
    }

    inline void set_a_hz (uint16_t hz_q14n2) {
      attack_incr = REnvelope<srate, sample_type>::hz_phincr * hz_q14n2 >> 2;
    }
};
