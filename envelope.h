#include "liblamb/src/tables/kl_256_uint8_t_qsin.h"

// Sample types are expected to be unsigned. Signed types have not been considered
// and so may behave unpredictably.

////////////////////////////////////////////////////////////////////////////////
// Envelope
////////////////////////////////////////////////////////////////////////////////

template <uint32_t srate, typename sample_type = uint16_t >
  class Envelope : public lamb::SampleSource<sample_type>, public lamb::Triggerable {
  public:
  inline virtual ~Envelope() {};
};

////////////////////////////////////////////////////////////////////////////////
// REnvelope
////////////////////////////////////////////////////////////////////////////////

template <uint32_t srate, typename sample_type = uint16_t >
class REnvelope : public Envelope<srate, sample_type> {
  public:
    typedef sample_type acc_type;
    static const acc_type maximum = ((acc_type)0) - 1;
    static const acc_type hz_phincr = maximum / srate;

    acc_type amplitude;
    acc_type release_phincr;
    
    inline REnvelope() : 
      amplitude(0),
      release_phincr(hz_phincr) {}

    virtual inline void trigger() {
      amplitude = maximum;
    }
  
  // v Test this? I forget if it works.
  inline void set_r_time (uint8_t seconds_q4n4) {
    if (seconds_q4n4 == 0)
        release_phincr = maximum;
      else
        set_r_hz(seconds_q4n4 == 1 ? 255 : (256 / seconds_q4n4));
    }

    inline void set_r_hz (uint8_t hz_q4n4) {
      release_phincr = hz_phincr * hz_q4n4 >> 4;  
    }

    inline virtual ~REnvelope() {}


    virtual inline acc_type read() {
      amplitude = (amplitude < release_phincr) ? 0 : amplitude - release_phincr;
      
//      Serial.print("Env reads: ");
//      Serial.println(amplitude);

      return amplitude;
    }    
};

////////////////////////////////////////////////////////////////////////////////
// SREnvelope
////////////////////////////////////////////////////////////////////////////////

template <uint32_t srate, typename sample_type = uint16_t >
  class SREnvelope : public REnvelope<srate, sample_type>, public lamb::Stoppable {
  public:
    bool gate;
  
    inline SREnvelope() : gate(false){}

    inline virtual ~SREnvelope() {}
  
    inline virtual void trigger() {
      gate = true;
      REnvelope<srate, sample_type>::trigger();
    };

    inline virtual void stop() {
      gate = false;
    };

  inline virtual sample_type read() {
    if (gate)
      return REnvelope<srate, sample_type>::maximum;
    else
      return REnvelope<srate, sample_type>::read();
  }
};

////////////////////////////////////////////////////////////////////////////////
// AREnvelope
////////////////////////////////////////////////////////////////////////////////

template <uint32_t srate, typename sample_type = uint16_t >
class AREnvelope : public REnvelope<srate, sample_type> {
  public:
    typename REnvelope<srate>::acc_type attack_phacc;
    typename REnvelope<srate>::acc_type attack_phincr;
    
    inline virtual ~AREnvelope() {}

    inline AREnvelope() : 
      attack_phacc(0),
      attack_phincr(REnvelope<srate, sample_type>::maximum) {}

    virtual inline void trigger() {
      attack_phacc = REnvelope<srate, sample_type>::maximum;
    }

    virtual inline typename REnvelope<srate, sample_type>::acc_type read() {
      if (attack_phacc > 0) {
        if (attack_phacc < attack_phincr) {
          attack_phacc = 0;
          REnvelope<srate>::trigger();          
        }
        else
          attack_phacc -= attack_phincr;

        REnvelope<srate, sample_type>::amplitude = ~attack_phacc;
        return REnvelope<srate, sample_type>::amplitude;
      }
      else 
        return REnvelope<srate, sample_type>::read();
    }
    
    inline void set_a_time (uint16_t seconds_q2n14) {
      if (seconds_q2n14 == 0)
        attack_phincr = REnvelope<srate, sample_type>::maximum;
      else
        set_a_hz(seconds_q2n14 == 1 ? 65535 : (65536/seconds_q2n14));
    }

    inline void set_a_hz (uint16_t hz_q14n2) {
      attack_phincr = REnvelope<srate, sample_type>::hz_phincr * hz_q14n2 >> 2;
    }
};

////////////////////////////////////////////////////////////////////////////////
// SmoothAREnvelope
////////////////////////////////////////////////////////////////////////////////

template <uint32_t srate, typename sample_type = uint16_t >
class SmoothAREnvelope : public AREnvelope<srate, sample_type> {
  public:
    inline virtual ~SmoothAREnvelope() {}

    inline SmoothAREnvelope() {}

    virtual inline void trigger() {
      if (REnvelope<srate, sample_type>::amplitude > 0)
        AREnvelope<srate, sample_type>::attack_phacc = ~REnvelope<srate, sample_type>::amplitude;
      else
        AREnvelope<srate, sample_type>::attack_phacc = REnvelope<srate, sample_type>::maximum;
      
      // ^ - REnvelope<srate>::decay; // why did I add this subtract??
      REnvelope<srate, sample_type>::trigger();
    }
};

////////////////////////////////////////////////////////////////////////////////
// ASREnvelope
////////////////////////////////////////////////////////////////////////////////

template <uint32_t srate, typename sample_type = uint16_t >
  class ASREnvelope : public SmoothAREnvelope<srate, sample_type>, public lamb::Stoppable {
  public:
    bool gate;
  
    inline ASREnvelope() : gate(false){}

    inline virtual ~ASREnvelope() {}
  
    inline virtual void trigger() {
      gate = true;
      SmoothAREnvelope<srate, sample_type>::trigger();
    };

    inline virtual void stop() {
      gate = false;
    };

  inline virtual sample_type read() {
    if (SmoothAREnvelope<srate, sample_type>::attack_phacc > 0) {
      
      return SmoothAREnvelope<srate, sample_type>::read();
    }
    else if (gate) {
      return REnvelope<srate, sample_type>::maximum;
    }
    else {
      return SmoothAREnvelope<srate, sample_type>::read();
    }

    return 0;
  }
};

////////////////////////////////////////////////////////////////////////////////
// ADSREnvelope
////////////////////////////////////////////////////////////////////////////////

template <uint32_t srate, typename sample_type = uint16_t >
  class ADSREnvelope : public ASREnvelope<srate, sample_type>
{
  typename REnvelope<srate, sample_type>::acc_type decay_phincr;
  typename REnvelope<srate, sample_type>::acc_type decay_phacc;
  
private:
  sample_type _sustain_level;
public:
  inline void set_sustain_level(sample_type value) {
    _sustain_level = value;    
    _sustain_level = 16000;
  }

  inline void set_d_hz (uint8_t hz_q4n4) {
//    Serial.print("Need to move from ");
//    Serial.print(REnvelope<srate, sample_type>::maximum);
//    Serial.print(" to ");
//    Serial.print(_sustain_level);
//    Serial.print(" (");
    Serial.print( REnvelope<srate, sample_type>::maximum - _sustain_level );
//    Serial.print(") in ");
//    Serial.print(hz_q4n4 >> 4);
//    Serial.print(" and ");
//    Serial.print(hz_q4n4 & 0b1111);
//    Serial.println("/16ths hz.");
    Serial.print(" ");
    Serial.print(srate);
    Serial.println();
    decay_phincr = REnvelope<srate, sample_type>::hz_phincr * hz_q4n4 >> 4;
  }

  inline sample_type sustain_level() {
    // somehow set a phincr based on a decay hz?
    return _sustain_level;
  }

  inline ADSREnvelope() {
    set_sustain_level(REnvelope<srate, sample_type>::maximum);    
  }

  inline virtual ~ADSREnvelope() {}

  inline virtual void trigger() {
    decay_phacc = REnvelope<srate, sample_type>::maximum;
    
    ASREnvelope<srate, sample_type>::trigger();
  }

  inline virtual void stop() {
    ASREnvelope<srate, sample_type>::stop();
  };

  inline virtual sample_type read() {
    if (SmoothAREnvelope<srate, sample_type>::attack_phacc > 0) {
      Serial.print("1 ");

      return SmoothAREnvelope<srate, sample_type>::read();
    }
    else if (decay_phacc > 0) {
      Serial.print("2");

      if (decay_phacc < decay_phincr) {
        decay_phacc = 0;
      }
      else
        decay_phacc -= decay_phincr;

      REnvelope<srate, sample_type>::amplitude = decay_phacc;
      
      return REnvelope<srate, sample_type>::amplitude;
    }
    else if (ASREnvelope<srate, sample_type>::gate) {
      Serial.print("3 ");

      return _sustain_level;
    }
    else {
      Serial.print("4 ");

      return SmoothAREnvelope<srate, sample_type>::read();
    }

    return 0;
  }
};

////////////////////////////////////////////////////////////////////////////////
// SlopedEnvelope
////////////////////////////////////////////////////////////////////////////////

template <
  template<uint32_t, typename> typename template_t,
  uint32_t srate,
  typename sample_type = uint16_t
>
  class SlopedEnvelope : public template_t<srate, sample_type> {
public:
  inline SlopedEnvelope() {}
  inline virtual ~SlopedEnvelope() {}

  inline typename template_t<srate, sample_type>::acc_type read() {
    typename template_t<srate, sample_type>::acc_type tmp =
      template_t<srate, sample_type>::read();

    typename template_t<srate, sample_type>::acc_type tmp2 = ~pgm_read_byte(
      lamb::Tables::qsin256_uint8_t::data +
      lamb::Tables::qsin256_uint8_t::length -
      (tmp >> 8)-
      1
    );
    
    /* Serial.print("SlopeEnv reads: "); */
    /* Serial.println(tmp2); */
    
    return tmp2 << 8;
  } 
};

