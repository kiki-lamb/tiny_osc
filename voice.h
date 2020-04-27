////////////////////////////////////////////////////////////////////////////////

#define VOICES 2
#define KDIV 64

typedef Oscillator<SRATE, int8_t> osc_type;
typedef Oscillator<SRATE, uint8_t> lfo_type;

////////////////////////////////////////////////////////////////////////////////

class Instrument :
public lamb::SampleSource<int8_t>,
  public lamb::Triggerable, public lamb::Stoppable 
{

public:
  lfo_type lfo;
  
  osc_type oscs[2];
  
  // cSlopedREnvelope<SRATE> env;
  // AREnvelope<SRATE> env;

  // SlopedEnvelope<SmoothAREnvelope, SRATE> env;
  // SlopedEnvelope<REnvelope, SRATE> env;
  SlopedEnvelope<ASREnvelope, SRATE> env;
  
  lamb::UnityMix<int8_t> mixer;

  virtual ~Instrument() {};
  
Instrument() : mixer(&oscs[0], &oscs[1])
  {
    oscs[0].octave = 3;
    oscs[1].octave = 3;
  
    oscs[0].set_wave(osc_type::wf_sine);
    oscs[1].set_wave(osc_type::wf_sine);
  
    oscs[0].set_note(60);
    oscs[1].set_note(60);

    env.set_a_hz(8 << 4);
    env.set_d_hz(8 << 4); 
  
    lfo.set_hz(16, 0b00000000);
    lfo.set_wave(lfo_type::wf_sine);
    
  }

  inline virtual void trigger() {
    if (env.amplitude == 0) {
      oscs[0].trigger();
      oscs[1].trigger();
    }

    env.trigger();
  }

  inline virtual void stop() {
    env.stop();
  }
  
  inline virtual int8_t read() {
    uint8_t env_val = env.read() >> 8; 
     
    uint8_t lfo_tmp = lfo.read();
    lfo_tmp = Math::mul_U8S<8>(env_val, 192 + (lfo_tmp>>2)); 

    int8_t mix_val = mixer.read();
    int8_t amped = mix_val;
    amped = Math::mul_U8S<8>(amped, lfo_tmp); 

    Serial.print(" ");
    Serial.print( env_val >> 1 );
    Serial.print(" ");
    Serial.print( amped );
    Serial.print(" ");
    Serial.print( lfo_tmp >> 1 );

    Serial.print(" ");
    Serial.print( 0 );
    Serial.print(" ");
    Serial.print( - (env_val >> 1 ));
    Serial.print(" ");
    Serial.print(0);
    Serial.print(" ");
    Serial.print( - (lfo_tmp >> 1 ));

    Serial.print(" ");
    Serial.print(mix_val);


    Serial.println();

    return amped;
    
//    if (! ix) {
//      last_env = env.read() >> 8;
//      last_lfo = lfo_type::traits::to_unsigned_type(lfo.read());
//      last_env = Math::mul_U8S<8>(last_env, (128 | (last_lfo >> 1)));
//    }
//    
//    ix++;
//    ix %= KDIV;
//
//    return Math::mul_U8S<8>(v, last_env); 
  }
};

////////////////////////////////////////////////////////////////////////////////

Instrument VOICE;



