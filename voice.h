////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define VOICES 1
#define KDIV 64

typedef Oscillator<SRATE, int8_t> osc_type;
#ifdef AMP_ENABLE
typedef Oscillator<SRATE / KDIV, int8_t> lfo_type;
lfo_type lfo;

ADEnvelope<SRATE / KDIV > env;
#endif

osc_type oscs[4];

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Amplifier : public lamb::SampleProcessor<int8_t, int8_t> {
  private:
  uint16_t ix;
#ifdef AMP_ENABLE
  uint8_t last_env;
  uint8_t last_lfo;
#endif

  public:
  virtual ~Amplifier() {};

  Amplifier(SampleSource<int8_t> * in) : 
  ix(1) 
#ifdef AMP_ENABLE
  , last_env(255), last_lfo(0) 
#endif  
  {
    connect(in);
  }

  inline virtual int8_t process(int8_t v) {
#ifdef AMP_ENABLE
    if (! ix) {
      last_env = env.read() >> 24;
      last_lfo = lfo_type::traits::to_uint8_t(lfo.read());
      last_env = Math::mul_T1U8S<8>(last_env, (128 | (last_lfo >> 1)));
    }
    
    ix++;
    ix %= KDIV;

    return Math::mul_T1U8S<8>(v, last_env); 
#else
    return v;
#endif
  }
};

void setup_voice() {
  oscs[0].set_detune_hz(0b00000000);  
  oscs[1].set_detune_hz(0b00001100);
  
  oscs[0].octave = 2;
  oscs[1].octave = 2;
  
  oscs[0].set_wave(osc_type::wf_sine);
  oscs[1].set_wave(osc_type::wf_sine);
  
  oscs[0].set_note(48);
  oscs[1].set_note(48);

#ifdef AMP_ENABLE
  env.set_a_time(512);
  env.set_d_time(0b00000111);
 
  lfo.set_hz(8, 0b00000000);
  lfo.set_wave((lfo_type::waveform)3);
 #endif
}

lamb::UnityMix mixer(&oscs[0], &oscs[1]); 
Amplifier amp(&mixer);
#ifdef AMP_ENABLE
lamb::ConvertToUnsigned<int8_t> converter(&amp);
#else
lamb::ConvertToUnsigned<int8_t> converter(&oscs[0]);
#endif
#define VOICE converter
