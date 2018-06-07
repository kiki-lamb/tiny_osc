////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define VOICES 2

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UnityMix : public SampleSource<int8_t> {
  public:
  CustomSampleProcessor<int8_t> input1;
  CustomSampleProcessor<int8_t> input2;

  virtual ~UnityMix() {}; 
  
  UnityMix(SampleSource<int8_t> * in1, SampleSource<int8_t> * in2) : 
    input1(&sample_type_traits<int8_t>::id), 
    input2(&sample_type_traits<int8_t>::id) {
      input1.connect(in1);
      input2.connect(in2);
  }

  inline virtual int8_t read() {
    return (input1.read() >> 1) + (input2.read() >> 1);
  }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Amplifier : public SampleProcessor<int8_t, int8_t> {
  public:
  virtual ~Amplifier() {};
  Amplifier(SampleSource<int8_t> * in) {
    connect(in);
  }
  inline virtual int8_t process(int8_t in) {
    return in;
  }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef Oscillator<SRATE, int8_t> osc_type;
typedef Oscillator<SRATE / 32, int8_t> lfo_type;

ADEnvelope<SRATE / 32> env;

osc_type oscs[VOICES];
lfo_type lfo;

inline int8_t read_voice();

//CustomSampleSource<int8_t> source(&read_voice);

UnityMix mixer(&oscs[0], &oscs[1]);
Amplifier amp(&mixer);
CustomSampleProcessor<int8_t, uint8_t> converter(&sample_type_traits<int8_t>::to_uint8_t, &amp);

inline int8_t read_voice() {
  static uint8_t ix = 0;
  static uint8_t last_env = 255;
  static uint8_t last_lfo = 0;

  if (! (ix++ & 0b11111)) {
//    last_env = env.read() >> 24;
//    last_lfo = lfo_type::traits::to_uint8_t(lfo.read());
//    last_env = last_env * (128 | (last_lfo >> 1)) >> 8;
  }
 
   return 
    mul_T1U8S<8>( 
      mixer.read(), 
      last_env
    );
}

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

#define VOICE converter
