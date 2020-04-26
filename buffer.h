template <typename T> 
class Buffer256 {
  private: 
  uint8_t write_ix;
  uint8_t read_ix;
  uint8_t count;
  volatile T buff[256];
  
  public:
  Buffer256() : read_ix(0), write_ix(0), count(0) {}
  ~Buffer256() {}

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

template <typename T, uint8_t SIZE> 
class Buffer {
  private: 
   uint8_t read_ix;
   uint8_t write_ix;
   uint8_t count;
   T buff[SIZE];
  
  public:
  Buffer() : read_ix(0), write_ix(0), count(0) {}
  ~Buffer() {}

  inline void write(T t) volatile {
    buff[write_ix] = t;
    count++;
    write_ix++;
    write_ix %= SIZE;
  }

  inline T read() volatile {
    T tmp = buff[read_ix];
    count--;
    read_ix++;
    read_ix %= SIZE;
    
    return tmp; 
  }

  inline bool writeable() volatile {
    return count < SIZE;
  }

  inline bool readable() volatile {
    return count;
  }
};
