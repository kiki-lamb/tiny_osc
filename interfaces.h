
template <typename output_type_> 
class SampleProvider { 
  public:
    typedef output_type_ value_type;
    typedef sample_type_traits<value_type> traits;
    virtual ~SampleProvider() {};
    virtual value_type read() = 0;
};

template <typename output_type_> 
class SampleReceiver : public SampleProvider<output_type_> { 
  public:
    typedef output_type_ value_type;
  
  private:
    SampleProvider<value_type> * source;
  
  public:
    typedef sample_type_traits<value_type> traits;
    virtual ~SampleReceiver() {};
    void connect(SampleProvider<value_type> * source_ = NULL) {
      source = source_;
    }
    virtual value_type read() {
      return process(source->read());
    }
    virtual value_type process(value_type) = 0;
};


class Attenuator : public SampleReceiver<int8_t> {
  public:
  virtual ~Attenuator() {}
  Attenuator() {}

  virtual value_type process(SampleReceiver<int8_t>::value_type value) {
    return value;
  }
};

