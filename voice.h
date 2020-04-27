////////////////////////////////////////////////////////////////////////////////

#define VOICES 2
#define KDIV 64
#define VOICE instr

typedef Oscillator<SRATE, int8_t> osc_type;
typedef Oscillator<SRATE, uint8_t> lfo_type;

lfo_type lfo;

osc_type oscs[2];

////////////////////////////////////////////////////////////////////////////////

class Instrument :
  public lamb::SampleProcessor<int8_t, int8_t>,
  public lamb::Triggerable
{
  // private:
  // uint16_t ix;
  // uint8_t last_env;
  // uint8_t last_lfo;

  public:
  DEnvelope<SRATE> env;

  virtual ~Instrument() {};

  Instrument(SampleSource<int8_t> * in) : 
  ix(1) 
  , last_env(255), last_lfo(0) 
  {
    connect(in);
  }

  inline void trigger() {
    env.trigger();
    oscs[0].trigger();
    oscs[1].trigger();
  }
  
  inline virtual int8_t process(int8_t voice) {
    uint8_t raw_env_val = env.read() >> 8; 
     
    uint8_t env_val = ~pgm_read_byte(
      lamb::Tables::qsin256_uint8_t::data +
      lamb::Tables::qsin256_uint8_t::length -
      raw_env_val -
      1
    );

    uint8_t lfo_tmp = lfo.read();
    lfo_tmp = Math::mul_U8S<8>(env_val, 192 + (lfo_tmp>>2)); 
    
    int8_t amped = voice; 
    amped = Math::mul_U8S<8>(amped, lfo_tmp); 

    Serial.print(" ");
    Serial.print(raw_env_val >> 1);
    Serial.print(" ");
    Serial.print( env_val >> 1 );
    Serial.print(" ");
    Serial.print( amped );
    Serial.print(" ");
    Serial.print( lfo_tmp >> 1 );

    Serial.print(" ");
    Serial.print(- (raw_env_val >> 1));
    Serial.print(" ");
    Serial.print( - (env_val >> 1 ));
    Serial.print(" ");
    Serial.print(0);
    Serial.print(" ");
    Serial.print( - (lfo_tmp >> 1 ));
    Serial.print(" ");
    Serial.print(0);
    Serial.print(" ");
    Serial.print(voice);

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

lamb::UnityMix<int8_t> mixer(&oscs[0], &oscs[1]); 
Instrument instr(&mixer);
// lamb::ConvertToUnsigned<int8_t> converter(&oscs[0]);

////////////////////////////////////////////////////////////////////////////////

void setup_voice() {
  oscs[0].set_detune_hz(0b00000000);  
  oscs[1].set_detune_hz(0b00000000);
  
  oscs[0].octave = 2;
  oscs[1].octave = 2;
  
  oscs[0].set_wave(osc_type::wf_sine);
  oscs[1].set_wave(osc_type::wf_sine);
  
  oscs[0].set_note(60);
  oscs[1].set_note(60);

  instr.env.set_d_hz(2 << 4);
  
  lfo.set_hz(8, 0b00000000);
  lfo.set_wave(lfo_type::wf_sine);
}

////////////////////////////////////////////////////////////////////////////////

#define VOICE instr
