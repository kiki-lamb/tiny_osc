
template <typename output_type_> 
class SampleProvider { 
  public:
    typedef output_type_ output_type;
    typedef sample_type_traits<output_type> traits;
    virtual ~SampleProvider() {};
    virtual output_type read() = 0;
};

template <typename output_type_>
class CustomSampleProvider : public SampleProvider<output_type_> {
  public:
  typedef output_type_ (*func_type)();
  func_type func;
  CustomSampleProvider(func_type f) : func(f) {};
  virtual ~CustomSampleProvider() {};  
  virtual output_type_ read() {
    return (*func)();
  }
};

template <typename input_type_, typename output_type_ = input_type_ > 
class SampleProcessor : public SampleProvider<output_type_> { 
  public:
    typedef input_type_ input_type;
    typedef output_type_ output_type;
  
    SampleProvider<output_type> * source;
      
    virtual ~SampleProcessor() {};

    void connect(SampleProvider<input_type> * source_ = NULL) {
      source = source_;
    }
    virtual output_type read() {
      return process(source->read());
    }
    virtual output_type process(input_type) = 0;
};

template <typename input_type_, typename output_type_ = input_type_ > 
class CustomSampleProcessor : public SampleProcessor<input_type_, output_type_> {
  public:
  typedef output_type_ (*func_type)(input_type_);
  func_type func;
  CustomSampleProcessor(func_type f) : func(f) {};
  virtual ~CustomSampleProcessor() {};
  virtual output_type_ process(input_type_ v) {
    return (*func)(v);
  }
};


