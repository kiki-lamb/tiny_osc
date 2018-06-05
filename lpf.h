
class LowpassFilter {
  public:
  int8_t last;
  int8_t momentum;

  LowpassFilter() : last(0), momentum(0) {
    }
    
  inline int8_t read(int8_t input, uint8_t f) {
    return input;
    int8_t delta = input - last;
    return last += delta * f / 256; // Lower / higher number here will lower / raise the cutoff frequency
  }
};

LowpassFilter lpf;

