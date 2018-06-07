////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define VOICES 2

typedef Oscillator<SRATE, int8_t> osc_type;
typedef Oscillator<SRATE / 32, int8_t> lfo_type;

ADEnvelope<SRATE / 32> env;

osc_type oscs[VOICES];
lfo_type lfo;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Amplifier : public SampleProcessor<int8_t, int8_t> {
  private:
  uint16_t ix;
  uint8_t last_env;
  uint8_t last_lfo;
  
  public:
  virtual ~Amplifier() {};
  Amplifier(SampleSource<int8_t> * in) : 
  ix(1), last_env(255), last_lfo(0) {
    connect(in);
  }
  
  inline virtual int8_t process(int8_t v) {
//    if (! ix) {
//      last_env = env.read() >> 24;
//      last_lfo = lfo_type::traits::to_uint8_t(lfo.read());
//      last_env = last_env * (128 | (last_lfo >> 1)) >> 8;
//    }
    
//    ix++;
//    ix %= 256;
return v;
    return mul_T1U8S<8>(v, last_env);
  }
};

void setup_voice() {
  lfo.set_hz(10, 0b00000000);
  lfo.set_wave(3);
 
  oscs[0].set_detune_hz(0b00000000);  
  oscs[1].set_detune_hz(0b00001100);
  
  oscs[0].octave = 0;
  oscs[1].octave = 1;
  
  oscs[0].set_wave(osc_type::wf_saw);
  oscs[1].set_wave(osc_type::wf_saw);
  
  oscs[0].set_note(48);
  oscs[1].set_note(48);
  
  env.set_a_time(1024);
  env.set_d_time(0b00000100);
}

UnityMix mixer(&oscs[0], &oscs[1]); 
Amplifier amp(&mixer);
//Amplifier nothing(&oscs[0]);
ConvertToUnsigned<int8_t> converter(&amp);

#define VOICE converter
