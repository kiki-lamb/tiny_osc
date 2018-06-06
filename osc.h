#define SRATE 40000
#define VOICES 2 // 3 // 3 will cause buffer underruns/frequency loss if compiled without -O3.

#ifndef __AVR__
#define pgm_read_dword(x) (*(x))
#define pgm_read_byte(x) (*(x))
#endif
const uint32_t notes[] 
#ifdef __AVR__
PROGMEM 
#endif
= {
  561833, 595241, 630636, 668136, 707865, 749957,
  794551, 841798, 891854, 944886, 1001072, 1060599,
  1123666, 1190482, 1261272, 1336271, 1415730, 1499913,
  1589103, 1683596, 1783708, 1889773, 2002144, 2121198,
  2247331, 2380964, 2522544, 2672542, 2831460, 2999827,
  3178206, 3367192, 3567416, 3779545, 4004289, 4242396,
  4494662, 4761929, 5045088, 5345084, 5662919, 5999654,
  6356412, 6734384, 7134831, 7559090, 8008577, 8484792,
  8989324, 9523857, 10090175, 10690168, 11325838, 11999307,
  12712823, 13468767, 14269662, 15118180, 16017154, 16969584,
  17978648, 19047714, 20180350, 21380336, 22651676, 23998614,
  25425646, 26937534, 28539324, 30236360, 32034308, 33939168,
  35957296, 38095428, 40360700, 42760672, 45303352, 47997228,
  50851292, 53875068, 57078648, 60472720, 64068616, 67878336,
  71914592, 76190856, 80721400, 85521344, 90606704, 95994456,
  101702584, 107750136, 114157296, 120945440, 128137232, 135756672,
  143829184, 152381712, 161442800, 171042688, 181213408, 191988912,
  203405168, 215500272, 228314592, 241890880, 256274464, 271513344,
  287658368, 304763424, 322885600, 342085376, 362426816, 383977824,
  406810336, 431000544, 456629184, 483781760, 512548928, 543026688,
  575316736, 609526848, 645771200, 684170752, 724853632, 767955648,
  813620672, 862001088, 1536518824,
};

template <uint32_t srate, typename sample_type>
class Oscillator : public SampleProvider<sample_type> { 
  public:
  static const uint32_t hz_phincr = UINT32_MAX/srate;
    
  enum waveform { wf_silence, wf_saw, wf_square, wf_sine }; 

  virtual ~Oscillator() {}
  
  int8_t octave;
  uint8_t amp;
  
  private:
  waveform wave;
  
  public:
  uint32_t phacc;
  uint32_t phincr;
  uint32_t detune_phincr;
  mutable uint8_t last_sine_msb;
  mutable sample_type last_sine_sample;
 
  Oscillator () : 
    amp(255),
    wave(wf_saw), 
    octave(0), 
    phacc(rand()), 
    phincr(0), 
    detune_phincr(0),
    last_sine_msb(0),
    last_sine_sample(0)
    {
      set_wave(wave);
    }

  inline void set_hz(uint16_t hz_q16n0, uint8_t hz_q0n8 = 0) {
    phincr = (hz_phincr * hz_q16n0) + (hz_phincr * hz_q0n8 >> 8);
  }

  inline void set_note(uint8_t note) {
    phincr = (pgm_read_dword(notes + note) << octave) + detune_phincr; 
  }  

  inline void set_detune_hz(uint16_t hz_q4n4) {
    detune_phincr = hz_phincr*hz_q4n4 >> 4;
  }
  
  template <uint8_t voices> 
  static inline sample_type play_mixed(Oscillator* os) { // converts out to unsigned!       
    if (voices == 1) 
      return os[0].read();
    else if (voices == 2) 
      return (os[0].read() >> 1) + (os[1].read() >> 1);
    else if (voices == 3)
      return (os[0].read() >> 2) + (os[1].read() >> 2) + (os[2].read() >> 2);
    else if (voices == 4)
      return (os[0].read() >> 2) + (os[1].read() >> 2) + (os[2].read() >> 2) + (os[3].read() >> 2);

//    typename traits::mix_type mix = 0;
//    static const uint8_t amp = voices == 1 ? 255 : (UINT8_MAX+1)/voices;
//
//    for (uint8_t v = 0; v < voices; v++)
//      mix += os[v].read();
//  
//    return mul_T1U8S<amp>(mix);
  }

  inline sample_type render_silence() const { 
    return SampleProvider<sample_type>::traits::silence;
  }
  
  inline sample_type render_saw() const {
    return (phacc >> 24) + SampleProvider<sample_type>::traits::minimum;
  }

  inline sample_type render_square() const {
      return phacc & (1 << 31) ? SampleProvider<sample_type>::traits::maximum : SampleProvider<sample_type>::traits::minimum;  
  }

  inline sample_type render_sine() const {
    uint8_t tmp = phacc >> 24;
    
    if (tmp != last_sine_msb) {
      last_sine_msb = tmp;
      last_sine_sample = pgm_read_byte(SampleProvider<sample_type>::traits::sine_table+last_sine_msb);
    }

    return last_sine_sample;
  }
      
  inline void set_wave(waveform wf) {
    wave = wf;
   }
 
  virtual inline sample_type read() {
   phacc += phincr;

   switch (wave) {
    case 0:
      return render_silence();
    case 1:
      return render_saw();
    case 2:
      return render_square();
    case 3: 
      return render_sine();
    }
  }
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef Oscillator<SRATE, int8_t> osc_type;
typedef Oscillator<SRATE / 32, int8_t> lfo_type;

osc_type oscs[VOICES];
lfo_type lfo;

