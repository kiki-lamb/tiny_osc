////////////////////////////////////////////////////////////////////////////////

template <typename output_type_> 
class SampleSource { 
  public:
    typedef output_type_ output_type;
    typedef sample_type_traits<output_type> traits;
    virtual ~SampleSource() {};
    virtual output_type read() = 0;
};

////////////////////////////////////////////////////////////////////////////////

template <typename output_type_>
class CustomSampleSource : public SampleSource<output_type_> {
  public:
  typedef output_type_ (*func_type)();
  func_type func;
  CustomSampleSource(func_type f) : func(f) {};
  virtual ~CustomSampleSource() {};  
  inline virtual output_type_ read() {
    return (*func)();
  }
};

////////////////////////////////////////////////////////////////////////////////

template <typename input_type_>
class SampleSink {
  public:
    typedef input_type_ input_type;
  
    SampleSource<input_type> * source;
      
    virtual ~SampleSink() {};
  
    virtual inline void connect(SampleSource<input_type> * source_ = NULL) {
      source = source_;
    }

    virtual inline bool sink() = 0;

    protected:
      inline input_type_ read() {
        return source->read();
      }
};

////////////////////////////////////////////////////////////////////////////////

template <typename input_type_>
class CustomSampleSink : public SampleSink<input_type_> {
  public:
    ~CustomSampleSink() {};
    typedef bool (*func_type)(input_type_);  
    func_type func;
    CustomSampleSink(func_type f, SampleSource<input_type_> * source = NULL) : func(f) {
      connect(source);
    }
    virtual inline bool sink() {
      return (*func)(SampleSink<input_type_>::read());
    }
};

////////////////////////////////////////////////////////////////////////////////

template <typename input_type_, typename output_type_ = input_type_ > 
class SampleProcessor : public SampleSource<output_type_> { 
  public:
    typedef input_type_ input_type;
    typedef output_type_ output_type;
  
    SampleSource<input_type> * source;
      
    virtual ~SampleProcessor() {};

    virtual inline void connect(SampleSource<input_type> * source_ = NULL) {
      source = source_;
    }
    
    inline virtual output_type read() {
      return process(source->read());
    }
    
    virtual output_type process(input_type) = 0;
};

////////////////////////////////////////////////////////////////////////////////

template <typename input_type_, typename output_type_ = input_type_ > 
class CustomSampleProcessor : public SampleProcessor<input_type_, output_type_> {
  public:
  typedef output_type_ (*func_type)(input_type_);
  
  func_type func;
  
  CustomSampleProcessor(func_type f, SampleSource<input_type_> * source = NULL) : func(f) {
    connect(source);
  };
  
  virtual ~CustomSampleProcessor() {};
  
  inline virtual output_type_ process(input_type_ v) {
    return (*func)(v);
  }
};


