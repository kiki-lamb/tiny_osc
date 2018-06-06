
template <typename output_type_> 
class SampleProvider { 
  public:
    typedef output_type_ value_type;
    typedef sample_type_traits<value_type> traits;
    virtual ~SampleProvider() {};
    virtual value_type read() = 0;
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

template <typename output_type_> 
class CustomSampleReceiver : public SampleReceiver<output_type_> {
  public:
  typedef output_type_ (*func_type)(output_type_);
  func_type func;
  CustomSampleReceiver(func_type f) : func(f) {};
  virtual ~CustomSampleReceiver() {};
  virtual output_type_ process(output_type_ v) {
    return (*func)(v);
  }
};


