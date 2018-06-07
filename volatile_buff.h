template <typename T> 
class VolatileBuff256 {
  private: 
  volatile uint8_t write_ix;
  volatile uint8_t read_ix;
  volatile uint8_t count;
  volatile T buff[256];
  
  public:
  VolatileBuff256() : read_ix(0), write_ix(0), count(0) {}
  ~VolatileBuff256() {}

  inline void write(T t) {
    buff[write_ix] = t;
    count++;
    write_ix++;
  }

  inline T read() {
    T tmp = buff[read_ix];
    count--;
    read_ix++;
    return tmp; 
  }

  inline bool writeable() {
    return count < 255;
  }

  inline bool readable() {
    return count;
  }
};

template <uint8_t SIZE, typename T> 
class VolatileBuff {
  private: 
   uint8_t write_ix;
   uint8_t read_ix;
   uint8_t count;
   T buff[SIZE];
  
  public:
  VolatileBuff() : read_ix(0), write_ix(0), count(0) {}
  ~VolatileBuff() {}

  inline void write(T t) {
    buff[write_ix] = t;
    count++;
    write_ix++;
    write_ix %= SIZE;
  }

  inline T read() {
    T tmp = buff[read_ix];
    count--;
    read_ix++;
    read_ix %= SIZE;
    
    return tmp; 
  }

  inline bool writeable() {
    return count < SIZE;
  }

  inline bool readable() {
    return count;
  }
};

