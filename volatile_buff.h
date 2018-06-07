template <typename T> 
class VolatileBuff256 {
  private: 
  volatile uint8_t write_ix;
  volatile uint8_t read_ix;
  volatile uint8_t count;
  volatile T buff[256];
  
  public:
  Buff256() : read_ix(0), write_ix(0), count(0) {}
  ~Buff256() {}

  inline void write(T t) {
    buff[count++, write_ix++] = t;
  }

  inline T read() {
    return buff[count--, read_ix++];
  }

  inline bool writeable() {
    return count < 255;
  }

  inline bool readable() {
    return count;
  }
};

