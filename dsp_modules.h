////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename input, typename output = input>
class Identity : public SampleProcessor<input, output> {
  public:
  inline virtual ~Identity() {};
  inline Identity(SampleSource<input> * in = NULL) {
    connect(in);
  }
  inline __attribute((alwaysinline)) virtual output process(input v) {
    return v;
  }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UnityMix : public SampleSource<int8_t> {
  public:
  Identity<int8_t> input1;
  Identity<int8_t> input2;

  inline virtual ~UnityMix() {}; 
  
  inline UnityMix(SampleSource<int8_t> * in1, SampleSource<int8_t> * in2) {
      input1.connect(in1);
      input2.connect(in2);
  }

  inline virtual int8_t read() {
    uint16_t tmp;
    tmp += input1.read();
    tmp += input2.read();
    return tmp / 2;
  }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename input>
class ConvertToUnsigned : public SampleProcessor<input, typename sample_type_traits<input>::unsigned_type> {
  public:
  inline virtual ~ConvertToUnsigned() {};
  
  inline ConvertToUnsigned(SampleSource<input> * in) {
    connect(in);
  }
  
  inline virtual uint8_t process(input v) {
    return sample_type_traits<input>::to_uint8_t(v);
  }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename input>
class ConvertToSigned : public SampleProcessor<input, typename sample_type_traits<input>::signed_type> {
  public:
  inline virtual ~ConvertToSigned() {};
  
  inline ConvertToSigned(SampleSource<input> * in) {
    connect(in);
  }
  
  inline virtual int8_t process(input v) {
    return sample_type_traits<input>::to_int8_t(v);
  }
};

